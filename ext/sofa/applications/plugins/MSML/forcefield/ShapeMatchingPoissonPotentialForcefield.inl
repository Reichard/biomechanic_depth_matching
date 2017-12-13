/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefield
  Author:    Tino Milschewski

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "ShapeMatchingPoissonPotentialForcefield.h"

#include "vtkStructuredGridReader.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkSTLReader.h"
#include "vtkCell.h"
#include "vtkPointData.h"
#include "vtkPolyDataNormals.h"

#include "UnstructuredTetrahedralMeshSampler.h"
#include "StructuredHexahedralMeshSampler.h"
#include "CollisionDetectionMeshSampler.h"
#include "PoissonPotentialForcefieldUtil.h"

// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
#include "vtkUnstructuredGridWriter.h"
#include "vtkPolyDataWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkPointData.h"
#include "vtkCellData.h"
#include "vtkCellArray.h"
#include <cmath>

#include "local_ntoa.h"

#include "VisitorCollection.h"

#define PI 3.14159265

namespace sofa
{

namespace component
{

namespace forcefield
{

template <class DataTypes>
	ShapeMatchingPoissonPotentialForcefield<DataTypes>::ShapeMatchingPoissonPotentialForcefield()
	: m_ChargeDensity(initData(&m_ChargeDensity, 1.0,"charge","")),
	  m_PrecomputedPotentialFieldFilename(initData(&m_PrecomputedPotentialFieldFilename,std::string("unnamed"),"precomputedPotentialFieldFilename","name of the precomputed VTK structured grid")),
	  m_SamplingDistance(initData(&m_SamplingDistance, 1.0, "samplingDistance", "Sampling distance for computing gradient and hessian")),
	  m_AdaptChargeDensity(initData(& m_AdaptChargeDensity, false, "adaptCharge", "Activate charge adaptivity")),
	  m_AdaptSamplingDistance(initData(&m_AdaptSamplingDistance, 0, "adaptSamplingDistance", "Activate sampling adaptivity")),
	  m_NormalDifferenceThreshold(initData(&m_NormalDifferenceThreshold, 20.0, "normalDifferenceThreshold", "NormalDifferenceThreshold")),
	  m_DistanceThreshold(initData(&m_DistanceThreshold, 20.0, "distanceThreshold", "distanceThreshold")),
	  m_DebugPath(initData(&m_DebugPath,std::string("unnamed"),"debugPath","Path for debug output")),
	  m_AdaptActiveTriangles(initData(& m_AdaptActiveTriangles, false, "adaptActiveTriangles", "Adapt acitve triangles for parital surface problems")),
	  m_ChargeFactorMultiplier(initData(&m_ChargeFactorMultiplier, 1.0, "chargeFactorMultiplier", "Charge Factor Multiplier"))
	  {

	  }

template <class DataTypes>
	ShapeMatchingPoissonPotentialForcefield<DataTypes>::~ShapeMatchingPoissonPotentialForcefield()
	{

	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::init()
	{
		this->core::behavior::ForceField<DataTypes>::init();
		m_UndeformedMeshTopology = (BaseMeshTopology*) this->getContext()->getMeshTopology();

		m_DeformedSurface = vtkSmartPointer<vtkPolyData>::New();

		if(m_PrecomputedPotentialFieldFilename.getValueString().find(".stl") != std::string::npos)
		{
			vtkSTLReader * stlReader = vtkSTLReader::New();
			stlReader->SetFileName(m_PrecomputedPotentialFieldFilename.getValue().c_str());

			m_MeshSampler = new CollisionDetectionMeshSampler();
			m_MeshSampler->loadFile(m_PrecomputedPotentialFieldFilename.getValue().c_str());


			//create normals
			vtkSmartPointer<vtkPolyDataNormals> meshNormals = vtkSmartPointer<vtkPolyDataNormals>::New();
			meshNormals->SetInputConnection(stlReader->GetOutputPort());
			meshNormals->SplittingOff();
			meshNormals->ConsistencyOn();
			meshNormals->ComputeCellNormalsOn();
			meshNormals->ComputePointNormalsOff();
			meshNormals->Update();



			m_DeformedSurface->DeepCopy(meshNormals->GetOutput());

			//build kdtree
			  m_CellLocator =  vtkSmartPointer<vtkCellLocator>::New();
			  m_CellLocator->SetDataSet(m_DeformedSurface);
			  m_CellLocator->BuildLocator();


			stlReader->Delete();
		}
		else
		{
			vtkDataReader * dataReader = vtkDataReader::New();
			dataReader->SetFileName(m_PrecomputedPotentialFieldFilename.getValue().c_str());

			if(dataReader->IsFileStructuredGrid())
			{
				m_MeshSampler = new StructuredHexahedralMeshSampler();
				m_MeshSampler->loadFile(m_PrecomputedPotentialFieldFilename.getValue().c_str());
			} else
			{
				m_MeshSampler = new UnstructuredTetrahedralMeshSampler();
				m_MeshSampler->loadFile(m_PrecomputedPotentialFieldFilename.getValue().c_str());
			}

			dataReader->Delete();
		}

		InitializeSurfaceIntegrationPointsAndWeights();
		InitializeElementInformation();

		m_CurrentChargeDensity = m_ChargeDensity.getValue();
		m_PotentialEnergy = 1.0;
		m_TotalElasticEnergy = 1.0;
		m_TotalKineticEnergy = 1.0;
		m_SamplingDistanceGradient = 0.005;

		m_FrameCounter = 0;
		m_FrameStart = 118;
		m_FrameEnd = 122;

		if(m_DebugPath.getValue() != "unnamed")
		{
			std::cout<<"Debug output is switched on, path is "<<m_DebugPath.getValue()<<"\n";
			m_DebugOutput = true;
		}
		else
			m_DebugOutput = false;

		if(m_AdaptActiveTriangles.getValue())
			std::cout<<"Adaptive triangle activation is used\n";
	}

//template <class DataTypes>
//	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::cleanup()
//	{
//		delete meshSampler;
//	}

template <class DataTypes>
	int ShapeMatchingPoissonPotentialForcefield<DataTypes>::findTetrahedronInTetrahedraOnSurfaceElementInformation(int tetrahedronIndex)
	{
		int listIndex = -1;

		for(unsigned int i = 0; i < m_tetrahedraOnSurfaceElementInformation.size(); i++)
		{
			if(m_tetrahedraOnSurfaceElementInformation[i].tetrahedronIndex == tetrahedronIndex)
			{
				listIndex = i;
				break;
			}
		}

		return listIndex;
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::InitializeElementInformation()
	{
		TetrahedraAroundTriangle tetrahedraAroundTriangle;
		TrianglesInTetrahedron trianglesInTetrahedron;
		Tetrahedron tetrahedron;
		Triangle triangle;
		int localTriangleIndex = -1;
		TetrahedronOnSurfaceElementInformation elementInformation;
		const VecCoord *X0=this->mstate->getX0();
		Mat<3, 4, Real> tetrahedronBasis;
		Mat<4, 3, Real> globalShapeFunctionDerivatives;
		Vec<3, Real> localSample;
		SurfaceTriangleSamplingDistanceType surfaceTriangleSamplingDistance;

		surfaceTriangleSamplingDistance.fill(m_SamplingDistance.getValue());

		for(int triangleId = 0; triangleId < m_UndeformedMeshTopology->getNbTriangles(); triangleId++)
		{
			tetrahedraAroundTriangle = m_UndeformedMeshTopology->getTetrahedraAroundTriangle(triangleId);

			if(tetrahedraAroundTriangle.size() == 1)
			{
				tetrahedron = m_UndeformedMeshTopology->getTetrahedron(tetrahedraAroundTriangle[0]);
				triangle = m_UndeformedMeshTopology->getTriangle(triangleId);

				trianglesInTetrahedron = m_UndeformedMeshTopology->getTrianglesInTetrahedron(tetrahedraAroundTriangle[0]);
				localTriangleIndex = m_UndeformedMeshTopology->getTriangleIndexInTetrahedron(trianglesInTetrahedron, triangleId);

				int listIndex = findTetrahedronInTetrahedraOnSurfaceElementInformation(tetrahedraAroundTriangle[0]);
				if(listIndex != -1)
				{
					m_tetrahedraOnSurfaceElementInformation[listIndex].surfaceTriangleLocalIndexes.push_back(localTriangleIndex);
					m_tetrahedraOnSurfaceElementInformation[listIndex].surfaceTriangleGlobalIndexes.push_back(triangleId);
					m_tetrahedraOnSurfaceElementInformation[listIndex].surfaceTriangleActiveFlag.push_back(1);
					m_tetrahedraOnSurfaceElementInformation[listIndex].surfaceAreasX0Doubled.push_back(GetTriangleSurfaceAreaDoubled(triangle, X0));
					m_tetrahedraOnSurfaceElementInformation[listIndex].surfaceTriangleSamplingDistances.push_back(surfaceTriangleSamplingDistance);
				} else
				{
					ClearElementInformation(elementInformation);
					GetTetrahedronBasis(tetrahedronBasis, tetrahedron, X0);

					for(unsigned int surfaceIntegrationPointIndex = 0; surfaceIntegrationPointIndex < numberOfSurfaceIntegrationPoints; surfaceIntegrationPointIndex++)
					{
						GetSurfaceTriangleSample(localSample, surfaceIntegrationPointIndex, localTriangleIndex);
						GetGlobalTetrahedronShapeFunctionDerivatives(globalShapeFunctionDerivatives, tetrahedronBasis, localSample[0], localSample[1], localSample[2]);
						elementInformation.globalShapeFunctionDerivatives[surfaceIntegrationPointIndex] =  globalShapeFunctionDerivatives;
					}

					elementInformation.tetrahedronIndex = tetrahedraAroundTriangle[0];
					elementInformation.surfaceTriangleLocalIndexes.push_back(localTriangleIndex);
					elementInformation.surfaceTriangleGlobalIndexes.push_back(triangleId);
					elementInformation.surfaceTriangleActiveFlag.push_back(1);
					elementInformation.surfaceAreasX0Doubled.push_back(GetTriangleSurfaceAreaDoubled(triangle, X0));
					elementInformation.surfaceTriangleSamplingDistances.push_back(surfaceTriangleSamplingDistance);

					m_tetrahedraOnSurfaceElementInformation.push_back(elementInformation);
				}
			}
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v)
	{
		if(m_AdaptChargeDensity.getValue())
		{
//			double currentChargeFactor = (m_TotalElasticEnergy) / (m_PotentialEnergy+m_TotalKineticEnergy);
			double currentChargeFactor = (m_TotalElasticEnergy) / (m_PotentialEnergy);

//			if(currentChargeFactor > 2)
//				currentChargeFactor = 2;

			std::cout << "CurrentChargeFactor: " << currentChargeFactor << '\n';
			m_CurrentChargeDensity = m_ChargeFactorMultiplier.getValue() * currentChargeFactor* m_CurrentChargeDensity;

			if(m_CurrentChargeDensity < m_ChargeDensity.getValue() )
				m_CurrentChargeDensity = m_ChargeDensity.getValue();
		}
		else
			m_CurrentChargeDensity = m_ChargeDensity.getValue();

		//forward to landmark ff
		sofa::core::ExecParams* paramsExec = sofa::core::ExecParams::defaultInstance();
		sofa::simulation::SetChargeFactorVisitor theVisitor(paramsExec, m_CurrentChargeDensity);
		theVisitor.execute(this->getContext());


		int surfaceTetrahedron;
		int q, j, p, i;
		const VecCoord *X0=this->mstate->getX0();

		m_FrameCounter++;

		if(m_AdaptActiveTriangles.getValue())
			MarkActiveSurfaceTriangles(&x);

		for(surfaceTetrahedron = 0; surfaceTetrahedron < m_tetrahedraOnSurfaceElementInformation.size(); surfaceTetrahedron++)
		{
			TetrahedronOnSurfaceElementInformation * elementInformation = &m_tetrahedraOnSurfaceElementInformation[surfaceTetrahedron];
			Vec<12, Real> elementForce;
			elementForce.clear();
			Tetrahedron tetrahedron = m_UndeformedMeshTopology->getTetrahedron(elementInformation->tetrahedronIndex);

			if(elementInformation->isActive)
			{
				for(int surfaceTriangle = 0; surfaceTriangle < elementInformation->surfaceTriangleLocalIndexes.size(); surfaceTriangle++)
				{
					if(elementInformation->surfaceTriangleActiveFlag[surfaceTriangle])
					{
						for(int surfaceIntegrationPointIndex = 0; surfaceIntegrationPointIndex < numberOfSurfaceIntegrationPoints; surfaceIntegrationPointIndex++)
						{
							Vec<3, Real> localSample;
							Mat<3, 4, Real> tetrahedronBasis;
							Vec<4, Real> tetrahedronShapeFunctionValues;
							Vec<3, Real> globalTetrahedronSample;
							Vec<3, Real> potentialGradient;

							GetSurfaceTriangleSample(localSample, surfaceIntegrationPointIndex, elementInformation->surfaceTriangleLocalIndexes[surfaceTriangle]);
							GetTetrahedronBasis(tetrahedronBasis, tetrahedron, &x);
							GetTetrahedronShapeFunctionValues(tetrahedronShapeFunctionValues, localSample[0], localSample[1], localSample[2]);

							globalTetrahedronSample = tetrahedronBasis * tetrahedronShapeFunctionValues;
							Triangle triangle = m_UndeformedMeshTopology->getTriangle(elementInformation->surfaceTriangleGlobalIndexes[surfaceTriangle]);
							GetPotentialGradientInternal(potentialGradient, globalTetrahedronSample, triangle, &x);

							// compute element force
							for(q = 0; q < 4; q++)
							{
								for(i = 0; i < 3; i++)
								{
									elementForce[3*q+i] += m_CurrentChargeDensity
												* potentialGradient[i] * tetrahedronShapeFunctionValues[q]
												* elementInformation->surfaceAreasX0Doubled[surfaceTriangle]
												/** (1.0/(double)elementInformation->surfaceTriangleLocalIndexes.size())*/
												* m_IntegrationWeights[surfaceIntegrationPointIndex];
								}
							}
						}
					}
				}

				for(q = 0; q < 4; q++)
				{
					for(i = 0; i < 3; i++)
					{
						f[tetrahedron[q]][i] += -elementForce[3*q+i];
					}
				}
			}
		}

		for(surfaceTetrahedron = 0; surfaceTetrahedron < m_tetrahedraOnSurfaceElementInformation.size(); surfaceTetrahedron++)
		{
			TetrahedronOnSurfaceElementInformation * elementInformation = &m_tetrahedraOnSurfaceElementInformation[surfaceTetrahedron];
			elementInformation->stiffnessMatrix.clear();

			if(elementInformation->isActive)
			{
				for(int surfaceTriangle = 0; surfaceTriangle < elementInformation->surfaceTriangleLocalIndexes.size(); surfaceTriangle++)
				{
					if(elementInformation->surfaceTriangleActiveFlag[surfaceTriangle])
					{
						for(int surfaceIntegrationPointIndex = 0; surfaceIntegrationPointIndex < numberOfSurfaceIntegrationPoints; surfaceIntegrationPointIndex++)
						{
							Vec<3, Real> localSample;
							Mat<3, 4, Real> tetrahedronBasis;
							Vec<4, Real> tetrahedronShapeFunctionValues;
							Tetrahedron tetrahedron = m_UndeformedMeshTopology->getTetrahedron(elementInformation->tetrahedronIndex);
							Vec<3, Real> globalTetrahedronSample;
							Mat<3, 3, Real> potentialHessian;

							GetSurfaceTriangleSample(localSample, surfaceIntegrationPointIndex, elementInformation->surfaceTriangleLocalIndexes[surfaceTriangle]);
							GetTetrahedronBasis(tetrahedronBasis, tetrahedron, &x);
							GetTetrahedronShapeFunctionValues(tetrahedronShapeFunctionValues, localSample[0], localSample[1], localSample[2]);

							globalTetrahedronSample = tetrahedronBasis * tetrahedronShapeFunctionValues;
							Triangle triangle = m_UndeformedMeshTopology->getTriangle(elementInformation->surfaceTriangleGlobalIndexes[surfaceTriangle]);
							double & distance = elementInformation->surfaceTriangleSamplingDistances[surfaceTriangle][surfaceIntegrationPointIndex];
							GetPotentialHessianInternal(potentialHessian, globalTetrahedronSample, triangle, distance, &x);

		//					std::cout << "Adaptive Distance: " << elementInformation->surfaceTriangleSamplingDistances[surfaceTriangle][surfaceIntegrationPointIndex] << '\n';

							// compute element stiffness matrices
							for(q = 0; q < 4; q++)
							{
								for(j = 0; j < 3; j++)
								{
									for(p = 0; p < 4; p++)
									{
										for(i = 0; i < 3; i++)
										{
											elementInformation->stiffnessMatrix[3*q+j][3*p+i] += m_CurrentChargeDensity * tetrahedronShapeFunctionValues[q]
												* potentialHessian[j][i] * tetrahedronShapeFunctionValues[p]
												* elementInformation->surfaceAreasX0Doubled[surfaceTriangle]
												/** (1.0/(double)elementInformation->surfaceTriangleLocalIndexes.size())*/
												* m_IntegrationWeights[surfaceIntegrationPointIndex];
										}
									}
								}
							}
						}
					}
				}
			}
		}

		 //sum force to get energy

		 m_PotentialEnergy = 0;
		 unsigned int nNodes = m_UndeformedMeshTopology->getNbPoints();

		for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
		{
			m_PotentialEnergy += f[iterNode].norm();
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::addKToMatrix(BaseMatrix *mat, SReal kFactor, unsigned int &offset)
	{
		kFactor = kFactor*(-1);

		TetrahedronOnSurfaceElementInformation elementInformation;
		Tetrahedron tetrahedron;
		int posRow, posCol = 0;

		for(int surfaceTetrahedron = 0; surfaceTetrahedron < m_tetrahedraOnSurfaceElementInformation.size(); surfaceTetrahedron++)
		{
			if(m_tetrahedraOnSurfaceElementInformation[surfaceTetrahedron].isActive)
			{
				elementInformation = m_tetrahedraOnSurfaceElementInformation[surfaceTetrahedron];
				tetrahedron = m_UndeformedMeshTopology->getTetrahedron(elementInformation.tetrahedronIndex);

				for(int q = 0; q < 4; q++)
				{
					for(int j = 0; j < 3; j++)
					{
						posRow = 3 * tetrahedron[q] + j + offset;

						for(int p = 0; p < 4; p++)
						{
							for(int i = 0; i < 3; i++)
							{
								posCol = 3 * tetrahedron[p] + i + offset;

								mat->add(posRow, posCol, elementInformation.stiffnessMatrix[3*q+j][3*p+i] * kFactor);
							}
						}
					}
				}
			}
		}
	}

template <class DataTypes>
	double ShapeMatchingPoissonPotentialForcefield<DataTypes>::getPotentialEnergy(const sofa::core::MechanicalParams* mparams) const
	   {
			return m_PotentialEnergy;
	   }

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialGradientInternal(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Triangle triangle, const VecCoord * x)
	{
		double samplingDistanceMultiplicator = 1.0;

//		GetPotentialGradientCentralDifferencesInNormalDirection(potentialGradient, globalPoint, samplingDistanceMultiplicator, triangle, x);

//		GetPotentialGradientCentralDifferences(blub1, globalPoint, samplingDistanceMultiplicator);
		GetPotentialGradientFEM(potentialGradient, globalPoint);
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialHessianInternal(Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Triangle triangle, double & samplingDistance, const VecCoord * x)
	{
		if(m_AdaptSamplingDistance.getValue() == 1)
		{
			// keine Besserung/ eher schlechtere Ergebnisse  TODO

			Mat<3, 3, Real> currentPotentialHessian;
			double currentSamplingDistance = samplingDistance;
			bool hessianOK = false;
			bool adaptionOK = false;
			bool hessianFound = false;

			hessianFound = GetPotentialHessianCentralDifferencesInNormalDirectionCD(hessianOK, currentPotentialHessian, globalPoint, currentSamplingDistance, triangle, x);
			if(!hessianFound)
			{
				return;
			}

			if(hessianOK)
			{
				do
				{
					potentialHessian = currentPotentialHessian;
					samplingDistance = currentSamplingDistance;

					currentSamplingDistance = currentSamplingDistance * 1.0 / 2.0;
					hessianFound = GetPotentialHessianCentralDifferencesInNormalDirectionCD(adaptionOK, currentPotentialHessian, globalPoint, currentSamplingDistance, triangle, x);
				} while(hessianFound && adaptionOK);
			}
			else
			{
				while(hessianFound && !adaptionOK)
				{
					currentSamplingDistance = currentSamplingDistance * 2.0;
					if(currentSamplingDistance > 1000.0)
					{
						std::cout << "CurrentSamplingDistance: " << currentSamplingDistance << '\n';
					}
					hessianFound = GetPotentialHessianCentralDifferencesInNormalDirectionCD(adaptionOK ,currentPotentialHessian, globalPoint, currentSamplingDistance, triangle, x);
				}

				if(hessianFound)
				{
					potentialHessian = currentPotentialHessian;
					samplingDistance = currentSamplingDistance;
				}
			}
		}
		else
		{
			bool hessianOK = false;

			GetPotentialHessianCentralDifferencesInNormalDirection(hessianOK, potentialHessian, globalPoint, samplingDistance, triangle, x);
//			GetPotentialHessianCentralDifferences(hessianOK, potentialHessian, globalPoint, samplingDistance);
		}
	}

//template <class DataTypes>
//	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::HasCellId(vtkIdType cellId, vector<vtkIdType> * cellIds)
//	{
//		if(cellId == -1)
//		{
//			return false;
//		}
//
//		for(int i = 0; i < cellIds->size(); i++)
//		{
//			if(cellId == (*cellIds)[i])
//			{
//				return true;
//			}
//		}
//
//		return false;
//	}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialHessianCentralDifferencesInNormalDirection(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance, Triangle triangle, const VecCoord * x)
	{
		double interpolatedValue = 0.0f;
		double sample = 0.0;
		Coord normal;
		vtkIdType currentCellId;

		// not used TODO
		hessianOK = true;

		GetTriangleUnnormalizedNormal(normal, triangle, x);
		normal.normalize();
		Vec<3, Real> normalVec(normal[0], normal[1], normal[2]);

		bool hessianFound = true;

		if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint + distance * normalVec))
		{
			hessianFound = false;
		}
		interpolatedValue = sample;

		if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint))
		{
			hessianFound = false;
		}
		interpolatedValue -= 2.0 * sample;

		if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint - distance * normalVec))
		{
			hessianFound = false;
		}
		interpolatedValue += sample;

		interpolatedValue /= distance * distance;

		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				potentialHessian[i][j] = normalVec[i] * normalVec[j] * interpolatedValue;
			}
		}

		if(!hessianFound)
		{
			potentialHessian.clear();
		}

		return hessianFound;
	}

template <class DataTypes>
bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialHessianCentralDifferencesInNormalDirectionCD(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance, Triangle triangle, const VecCoord * x)
{
	double interpolatedValue = 0.0f;
	double sample = 0.0;
	Coord normal;

	hessianOK = true;

	GetTriangleUnnormalizedNormal(normal, triangle, x);
	normal.normalize();
	Vec<3, Real> normalVec(normal[0], normal[1], normal[2]);

	bool hessianFound = true;
	Vector3 collisionPointToSample1, collisionPointToSample2, collisionPointToSample3;

	if(!m_MeshSampler->getSampleCD(sample, collisionPointToSample1, globalPoint + distance * normalVec))
	{
		hessianFound = false;
	}
	interpolatedValue = sample;

	if(!m_MeshSampler->getSampleCD(sample, collisionPointToSample2, globalPoint))
	{
		hessianFound = false;
	}
	interpolatedValue -= 2.0 * sample;

	if(!m_MeshSampler->getSampleCD(sample, collisionPointToSample3, globalPoint - distance * normalVec))
	{
		hessianFound = false;
	}
	interpolatedValue += sample;

	double dot = collisionPointToSample1 * collisionPointToSample3;
	if(dot > 0.0)
	{
		hessianOK = false;
	}
	else
	{
		hessianOK = true;
	}

	interpolatedValue /= distance * distance;

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			potentialHessian[i][j] = normalVec[i] * normalVec[j] * interpolatedValue;
		}
	}

	if(!hessianFound)
	{
		potentialHessian.clear();
	}

	return hessianFound;
}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialHessianCentralDifferences(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance)
	{
		Vec<3, Real> e1, e2, e3;
		double dx, dy, dz;
		vtkIdType currentCellId;

		// not used TODO
		hessianOK = true;

		e1.set(1.0, 0.0, 0.0);
		e2.set(0.0, 1.0, 0.0);
		e3.set(0.0, 0.0, 1.0);

		dx = distance;
		dy = distance;
		dz = distance;

		//
		// compute hessian
		//

		Vec<3, Vec<3, Real> > e;
		e.set(e1, e2, e3);
		Vec<3, Real> d;
		d.set(dx, dy, dz);

		double interpolatedValue = 0.0f;
		double sample = 0.0;

		bool hessianFound = true;

		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint + d[i] * e[i] + d[j] * e[j]))
				{
					hessianFound = false;
				}
				interpolatedValue = sample;

				if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint + d[i] * e[i] - d[j] * e[j]))
				{
					hessianFound = false;
				}
				interpolatedValue -= sample;

				if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint - d[i] * e[i] - d[j] * e[j]))
				{
					hessianFound = false;
				}
				interpolatedValue += sample;

				if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint - d[i] * e[i] + d[j] * e[j]))
				{
					hessianFound = false;
				}
				interpolatedValue -= sample;

				interpolatedValue /= 4.0 * d[i] * d[j];

				potentialHessian[i][j] = interpolatedValue;
			}
		}

		if(!hessianFound)
		{
			potentialHessian.clear();
		}

		return hessianFound;
	}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialGradientFEM(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint)
	{
		if(m_MeshSampler->isValidMesh())
		{
			return m_MeshSampler->getPotentialGradientFEM(potentialGradient, globalPoint);
		}
		else
		{
			GetPotentialGradientCentralDifferences(potentialGradient, globalPoint, 1.0);
			return true;
		}
	}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialGradientCentralDifferencesInNormalDirection(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Real samplingDistanceMultiplicator, Triangle triangle, const VecCoord * x)
	{
		double sample = 0.0;
		double interpolatedValue = 0.0f;
		Coord normal;

		vtkIdType currentCellId;

		GetTriangleUnnormalizedNormal(normal, triangle, x);
		normal.normalize();
		Vec<3, Real> normalVec(normal[0], normal[1], normal[2]);

		double distance = m_SamplingDistanceGradient* samplingDistanceMultiplicator;

		bool abort = false;

		if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint + distance * normalVec))
		{
			abort = true;
		}
		interpolatedValue = sample;

		if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint - distance * normalVec))
		{
			abort = true;
		}
		interpolatedValue -= sample;

		interpolatedValue /= distance * 2.0;
		potentialGradient = interpolatedValue * normalVec;

		if(abort)
		{
			potentialGradient.clear();
		}

		return abort;
	}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetPotentialGradientCentralDifferences(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Real samplingDistanceMultiplicator)
	{
		double bounds[6];
		Vec<3, Real> e1, e2, e3;
		double dx, dy, dz;
		vtkIdType currentCellId;

		e1.set(1.0, 0.0, 0.0);
		e2.set(0.0, 1.0, 0.0);
		e3.set(0.0, 0.0, 1.0);

		dx = m_SamplingDistanceGradient * samplingDistanceMultiplicator;
		dy = m_SamplingDistanceGradient * samplingDistanceMultiplicator;
		dz = m_SamplingDistanceGradient * samplingDistanceMultiplicator;

		//
		// Compute gradient
		//

		Vec<3, Vec<3, Real> > e;
		e.set(e1, e2, e3);
		Vec<3, Real> d;
		d.set(dx, dy, dz);

		double sample = 0.0;
		double interpolatedValue = 0.0f;

		bool abort = false;

		for(int i = 0; i < 3; i++)
		{
			if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint + d[i] * e[i]))
			{
				abort = true;
			}
			interpolatedValue = sample;

			if(!m_MeshSampler->getSample(sample, currentCellId, globalPoint - d[i] * e[i]))
			{
				abort = true;
			}
			interpolatedValue -= sample;

			interpolatedValue /= 2.0 * d[i];
			potentialGradient[i] = interpolatedValue;
		}

		if(abort)
		{
			potentialGradient.clear();
		}

		return abort;
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetSurfaceTriangleSample(Vec<3, Real> & localSample, int surfaceIntegrationPointIndex, int surfaceTriangleLocalIndex)
	{
		Real r = m_IntegrationPoints[3 * surfaceIntegrationPointIndex];
		Real s = m_IntegrationPoints[3 * surfaceIntegrationPointIndex + 1];
		Real t = m_IntegrationPoints[3 * surfaceIntegrationPointIndex + 2];

		switch(surfaceTriangleLocalIndex)
		{
			case 0:
				localSample[0] = 0.0;
				localSample[1] = s;
				localSample[2] = t;
				break;

			case 1:
				localSample[0] = r;
				localSample[1] = 0.0;
				localSample[2] = t;
				break;

			case 2:
				localSample[0] = r;
				localSample[1] = s;
				localSample[2] = t;
				break;

			case 3:
				localSample[0] = r;
				localSample[1] = s;
				localSample[2] = 0.0;
				break;
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetTetrahedronBasis(Mat<3, 4, Real> &basis, Tetrahedron tetrahedron, const VecCoord * x)
	{
		for(int i = 0; i < basis.nbLines; i++)
		{
			for(int j = 0; j < basis.nbCols; j++)
			{
				basis[i][j] = (*x)[tetrahedron[j]][i];
			}
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetTriangleUnnormalizedNormal(Coord & normal, Triangle triangle, const VecCoord *x)
	{
		Coord vector1 = (*x)[triangle[1]] - (*x)[triangle[0]];
		Coord vector2 = (*x)[triangle[2]] - (*x)[triangle[0]];

		normal = vector1.cross(vector2);
	}

template <class DataTypes>
	double ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetTriangleSurfaceAreaDoubled(Triangle triangle, const VecCoord * x)
	{
		Coord normal;
		GetTriangleUnnormalizedNormal(normal, triangle, x);
		return normal.norm();
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::InitializeSurfaceIntegrationPointsAndWeights()
	{
		switch(numberOfSurfaceIntegrationPoints)
		{
			case 1:
				m_IntegrationPoints[0] = 1.0/3.0;
				m_IntegrationPoints[1] = 1.0/3.0;
				m_IntegrationPoints[2] = 1.0/3.0;
				m_IntegrationWeights[0] = 0.5;
				break;
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetGlobalTetrahedronShapeFunctionDerivatives(Mat<4, 3, Real> & globalShapeFunctionDerivatives, Mat<3, 4, Real> tetrahedronBasis, Real r, Real s, Real t)
	{
		Mat<4, 3, Real> localShapeFunctionDerivatives;
		Mat<3,3, Real> jacobian;
		Mat<3, 3, Real> invertedJacobian;

		GetLocalTetrahedronShapeFunctionDerivatives(localShapeFunctionDerivatives, r, s, t);
		GetJacobian(jacobian, tetrahedronBasis, r, s, t);
		invertMatrix(invertedJacobian, jacobian);

		globalShapeFunctionDerivatives = localShapeFunctionDerivatives * invertedJacobian.transposed();
	}

template <class DataTypes>
	   void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetTetrahedronShapeFunctionValues(Vec<4,Real> & values, Real r, Real s,Real t)
	   {
			values[0] = r;
			values[1] = s;
			values[2] = 1 - r - s - t;
			values[3] = t;
	   }

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetLocalTetrahedronShapeFunctionDerivatives(Mat<4,3,Real> & values, Real r, Real s,Real t)
	{
		// dN1/dr, dN1/ds, dN1/dt
		values[0][0] = 1;
		values[0][1] = 0;
		values[0][2] = 0;

		// dN2/dr, dN2/ds, dN2/dt
		values[1][0] = 0;
		values[1][1] = 1;
		values[1][2] = 0;

		values[2][0] = -1;
		values[2][1] = -1;
		values[2][2] = -1;

		values[3][0] = 0;
		values[3][1] = 0;
		values[3][2] = 1;
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetJacobian(Mat<3,3, Real> & jacobian, Mat<3, 4, Real> tetrahedronBasis, Real r, Real s, Real t)
	{
		Mat<4,3,Real> localShapeFunctionDerivativeValues;
		GetLocalTetrahedronShapeFunctionDerivatives(localShapeFunctionDerivativeValues, r, s, t);

		jacobian = tetrahedronBasis * localShapeFunctionDerivativeValues;
		jacobian.transpose();
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::ClearElementInformation(TetrahedronOnSurfaceElementInformation & elementInformation)
	{
		elementInformation.tetrahedronIndex = -1;
		elementInformation.surfaceTriangleLocalIndexes.clear();
		elementInformation.surfaceTriangleGlobalIndexes.clear();
		elementInformation.surfaceAreasX0Doubled.clear();
		elementInformation.stiffnessMatrix.clear();
		elementInformation.globalShapeFunctionDerivatives.clear();
		elementInformation.surfaceTriangleSamplingDistances.clear();
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetTriangleBasis(Mat<3, 3, double> &basis, Triangle triangle, const VecCoord * x)
	{
		for(int i = 0; i < basis.nbLines; i++)
		{
			for(int j = 0; j < basis.nbCols; j++)
			{
				basis[i][j] = (*x)[triangle[j]][i];
			}
		}
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::MarkActiveSurfaceTriangles(const VecCoord * x)
	{
		//if debug: initialize vtk datastructure
		vtkSmartPointer<vtkPolyData> deformedMesh = vtkSmartPointer<vtkPolyData>::New();

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();

		vtkSmartPointer<vtkFloatArray> normalsMesh = vtkSmartPointer<vtkFloatArray>::New();
		vtkSmartPointer<vtkFloatArray> normalsDeformedSurface = vtkSmartPointer<vtkFloatArray>::New();
		vtkSmartPointer<vtkFloatArray> activeFlags = vtkSmartPointer<vtkFloatArray>::New();

		vtkSmartPointer<vtkIntArray> mappedTri = vtkSmartPointer<vtkIntArray>::New();
		vtkSmartPointer<vtkFloatArray> activeFlagsNormal2 = vtkSmartPointer<vtkFloatArray>::New();
		vtkSmartPointer<vtkFloatArray> activeFlagsNormal1 = vtkSmartPointer<vtkFloatArray>::New();
		vtkSmartPointer<vtkFloatArray> activeFlagsDistance = vtkSmartPointer<vtkFloatArray>::New();

		if(m_DebugOutput)
		{
			normalsMesh->SetName("Normals");
			normalsMesh->SetNumberOfComponents(3);

			normalsDeformedSurface->SetName("NormalsDeformed");
			normalsDeformedSurface->SetNumberOfComponents(3);

			activeFlags->SetName("ActiveFlags");
			activeFlags->SetNumberOfComponents(1);

			activeFlagsNormal2->SetName("ActiveFlagsNormal2");
			activeFlagsNormal2->SetNumberOfComponents(1);

			activeFlagsNormal1->SetName("ActiveFlagsNormal1");
			activeFlagsNormal1->SetNumberOfComponents(1);

			activeFlagsDistance->SetName("ActiveFlagdistance");
			activeFlagsDistance->SetNumberOfComponents(1);

			mappedTri->SetName("MappedTriId");
			mappedTri->SetNumberOfComponents(1);
		}

		//loop over all surface tetras
		for( int surfaceTetrahedron = 0; surfaceTetrahedron < m_tetrahedraOnSurfaceElementInformation.size(); surfaceTetrahedron++)
		{
			TetrahedronOnSurfaceElementInformation * elementInformation = &m_tetrahedraOnSurfaceElementInformation[surfaceTetrahedron];
			bool tetActiveFlag = 0;

			//loop over all active surfaces triangles
			for(int surfaceTriangle = 0; surfaceTriangle < elementInformation->surfaceTriangleLocalIndexes.size(); surfaceTriangle++)
			{
				//get surface triangle
				int globalTriId = elementInformation->surfaceTriangleGlobalIndexes[surfaceTriangle];
				Triangle triangle = m_UndeformedMeshTopology->getTriangle(globalTriId);
				Coord currentPoint1 = (*x)[triangle[0]];
				Coord currentPoint2 = (*x)[triangle[1]];
				Coord currentPoint3 = (*x)[triangle[2]];

				Coord midpoint = (currentPoint1+currentPoint2+currentPoint3)/3;


				// Find distance and tri Id in deformed mesh
				double distance;
				Coord collisionPointToGlobalPoint;
				Coord globalPoint;
				globalPoint[0] = midpoint[0];
				globalPoint[1] = midpoint[1];
				globalPoint[2] = midpoint[2];


				this->m_MeshSampler->getSampleCD(distance, collisionPointToGlobalPoint,  globalPoint);
				Coord collisionPoint = globalPoint-collisionPointToGlobalPoint;



				// calculate normals on computational mesh
				Deriv normal;
				Deriv vec1 = currentPoint2 - currentPoint1;
				Deriv vec2 = currentPoint3 - currentPoint1;
				normal = vec2.cross(vec1);
				normal.normalize();

				// calculate normals on deformed mesh (or just read it from the point data)
				double collisionPointTemp[3];
				collisionPointTemp[0] = collisionPoint[0];
				collisionPointTemp[1] = collisionPoint[1];
				collisionPointTemp[2] = collisionPoint[2];

				vtkIdType triId = this->m_CellLocator->FindCell(collisionPointTemp);

				Real tempAngle;
				Deriv cellDeformedNormal;
				Real degreeAngle;
				bool activeNormal1 = false;

				if(triId != -1)
				{

					this->m_DeformedSurface->GetCellData()->GetVectors("Normals")->GetTuple(triId, cellDeformedNormal.ptr() );

					// compare normals and set activeness of surface
					tempAngle = cellDeformedNormal*normal;
					tempAngle = tempAngle / (normal.norm()*cellDeformedNormal.norm());
					degreeAngle = acos(tempAngle) * 180.0 / PI;

					if(degreeAngle<m_NormalDifferenceThreshold.getValue())
					{
						activeNormal1 = true;
					}

				}


				//second normal criterion
				Deriv normal2;
				normal = vec2.cross(vec1);
				normal.normalize();
				bool active2 = false;

				tempAngle = collisionPointToGlobalPoint*normal;
				tempAngle = tempAngle / (collisionPointToGlobalPoint.norm()*normal.norm());
				degreeAngle = acos(tempAngle) * 180.0 / PI;

				if((degreeAngle<m_NormalDifferenceThreshold.getValue())||(degreeAngle>(180-m_NormalDifferenceThreshold.getValue())) )
				{
					active2 = true;
				}


				//distance criterion
				bool activeDistance = false;

				if(distance<this->m_DistanceThreshold.getValue())
					activeDistance = true;


				//create active flag for surface and tetra
				if(activeDistance && activeNormal1)
				{
					elementInformation->surfaceTriangleActiveFlag[surfaceTriangle] = 1;
					tetActiveFlag = 1;
				}
				else
				{
					elementInformation->surfaceTriangleActiveFlag[surfaceTriangle] = 0;
				}





				// if debug: write normal data and active data to vkt datastructure
				if(this->m_DebugOutput)
				{
					vtkIdType currentTriangle[3];
					currentTriangle[0] = points->InsertNextPoint(currentPoint1[0],currentPoint1[1],currentPoint1[2]);
					currentTriangle[1]= points->InsertNextPoint(currentPoint2[0],currentPoint2[1],currentPoint2[2]);
					currentTriangle[2] = points->InsertNextPoint(currentPoint3[0],currentPoint3[1],currentPoint3[2]);
					cellArray->InsertNextCell(3, currentTriangle);

					normalsMesh->InsertNextTuple3(normal[0],normal[1],normal[2]);
					normalsDeformedSurface->InsertNextTuple3(cellDeformedNormal[0],cellDeformedNormal[1],cellDeformedNormal[2]);
					activeFlags->InsertNextTuple1((float)elementInformation->surfaceTriangleActiveFlag[surfaceTriangle]);

					mappedTri->InsertNextTuple1(triId);
					activeFlagsNormal2->InsertNextTuple1((float)active2);
					activeFlagsNormal1->InsertNextTuple1((float)activeNormal1);
					activeFlagsDistance->InsertNextTuple1((float)activeDistance);


				}
			}

			//set activeness of tetra
			elementInformation->isActive = tetActiveFlag;

		}

		//if debug: write datastructure to disk
		if(this->m_DebugOutput)
		{
//			std::cout<<"Writing debug output\n";
			deformedMesh->SetPoints(points);
			deformedMesh->SetPolys(cellArray);

			deformedMesh->GetCellData()->SetActiveScalars("ActiveFlags");
			deformedMesh->GetCellData()->SetScalars(activeFlags);

			deformedMesh->GetCellData()->SetActiveScalars("MappedTri");
			deformedMesh->GetCellData()->SetScalars(mappedTri);
			deformedMesh->GetCellData()->SetActiveScalars("ActiveFlagsNormal2");
			deformedMesh->GetCellData()->SetScalars(activeFlagsNormal2);
			deformedMesh->GetCellData()->SetActiveScalars("ActiveFlagsNormal1");
			deformedMesh->GetCellData()->SetScalars(activeFlagsNormal1);
			deformedMesh->GetCellData()->SetActiveScalars("ActiveFlagdistance");
			deformedMesh->GetCellData()->SetScalars(activeFlagsDistance);

			deformedMesh->GetCellData()->SetActiveVectors("NormalsDeformed");
			deformedMesh->GetCellData()->SetVectors(normalsDeformedSurface);
			deformedMesh->GetCellData()->SetActiveVectors("Normals");
			deformedMesh->GetCellData()->SetVectors(normalsMesh);

			vtkSmartPointer<vtkPolyDataWriter> myWriter = vtkSmartPointer<vtkPolyDataWriter>::New();
			std::string debugFilename(m_DebugPath.getValue());
			debugFilename += "_";
			debugFilename += local_ntoa(this->m_FrameCounter);
			debugFilename += ".vtk";

			std::cout<<"Writing debug output to"<< debugFilename<<"\n";
			myWriter->SetFileName( debugFilename.c_str() );
			myWriter->SetInputData( deformedMesh );
			myWriter->Update();
			myWriter->Write();
		}



	}



template <class DataTypes>
	double ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetSurfaceJacobianDeterminant(Mat<3, 3, double> triangleBasis, double r, double s)
	{
		Mat<3,2, double> jacobian;
		Mat<3,2,double> localShapeFunctionDerivativeValues;
		PoissonPotentialForcefieldUtil::GetLocalTriangleShapeFunctionDerivatives(localShapeFunctionDerivativeValues, r, s);

		jacobian = triangleBasis * localShapeFunctionDerivativeValues;

		Mat<2,2> mat = jacobian.transposed() * jacobian;
		double det = determinant(mat);
		det = fabs(det);
		return det;
	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::WriteMatrixToCSVFile(defaulttype::BaseMatrix * mat,char* filename)
	{
		//open file and generate textstream
		QFile file(filename);

	    if (!file.open(QFile::WriteOnly | QFile::Truncate))
			 return ;

		QTextStream out(&file);

		//matrix line by line
		unsigned int rowSize = mat->rowSize();
		unsigned int columnSize = mat->colSize();

		for(unsigned int m=0;m<rowSize;m++)
		{
			for(unsigned int n=0;n<columnSize;n++)
			{
				 out << mat->element(m,n);
				 if(n!=(columnSize-1))
					 out<<",";
			}
			out<<"\n";
		 }

	 }

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefield<DataTypes>::WriteForcesToVTKFile(VecDeriv& f, const VecCoord& x, char * filename)
	{
		vtkUnstructuredGrid *sgrid = vtkUnstructuredGrid::New();

		int nPoints = m_UndeformedMeshTopology->getNbPoints();
		Real currentVertex[3];
		float currentForce[3];
		vtkIdType currentTetrahedron[4];

		vtkFloatArray *forces = vtkFloatArray::New();
		forces->SetNumberOfComponents(3);
		forces->SetNumberOfTuples(nPoints);

		vtkPoints *points = vtkPoints::New();
		points->Allocate(nPoints);

		vtkCellArray* cellArray = vtkCellArray::New();

		sofa::core::topology::BaseMeshTopology::SeqTetrahedra tetrahedra = m_UndeformedMeshTopology->getTetrahedra();

		for(int i = 0;i < tetrahedra.size(); i++)
		{
			Tetrahedron tetrahedron = tetrahedra[i];

			for(int index = 0; index < 4; index++)
			{
				int pointIndex = tetrahedron[index];

				currentTetrahedron[index] = pointIndex;

				currentVertex[0]= x[pointIndex][0];
				currentVertex[1]= x[pointIndex][1];
				currentVertex[2]= x[pointIndex][2];

				currentForce[0] = f[pointIndex][0];
				currentForce[1] = f[pointIndex][1];
				currentForce[2] = f[pointIndex][2];

				points->InsertPoint(pointIndex, currentVertex);
				forces->SetTupleValue(pointIndex, currentForce);

			}
			cellArray->InsertNextCell(4, currentTetrahedron);
		}

		sgrid->SetPoints(points);
		sgrid->GetPointData()->SetScalars(forces);
		sgrid->SetCells(VTK_TETRA, cellArray);

		vtkUnstructuredGridWriter *myWriter = vtkUnstructuredGridWriter::New();
		myWriter->SetFileName( filename );
		myWriter->SetInputData( sgrid );
		myWriter->Update();
		myWriter->Delete();

		forces->Delete();
		points->Delete();
		cellArray->Delete();
		sgrid->Delete();
	}
}

}

}
