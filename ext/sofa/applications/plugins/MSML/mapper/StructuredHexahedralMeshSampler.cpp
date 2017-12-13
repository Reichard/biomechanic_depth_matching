/*
 * StructuredHexahedralMeshSampler.cpp
 *
 *  Created on: Feb 20, 2013
 *      Author: milschew
 */

#include "StructuredHexahedralMeshSampler.h"
#include "vtkCell.h"
#include "vtkGenericCell.h"
#include "vtkPointData.h"

StructuredHexahedralMeshSampler::StructuredHexahedralMeshSampler(void)
{
	reader = NULL;
	grid = NULL;
}

StructuredHexahedralMeshSampler::~StructuredHexahedralMeshSampler()
{
	cleanUp();
}

void StructuredHexahedralMeshSampler::cleanUp(void)
{
	if(reader != NULL)
	{
		reader->Delete();
		reader = NULL;
	}
}

void StructuredHexahedralMeshSampler::loadFile(const char * filePath)
{
	cleanUp();

	reader = vtkStructuredGridReader::New();
	reader->SetFileName(filePath);
	reader->Update();
	grid = reader->GetOutput();
}

bool StructuredHexahedralMeshSampler::getSample(double & sample, vtkIdType & cellId,Vec<3, double> globalPoint)
{
	Vec<8, double> localHexaShapeFunctionValues;
	vtkCell * hexahedronCell = NULL;
	int tempSubId = 0;
	double tempWeights[8];
	double localSample[3];
	Vec<8, double> potentialValues;

	// FIXME localSample really r, s, t values??? --> NO!
	cellId = grid->FindCell(globalPoint.ptr(), NULL, 0, 1e-6, tempSubId,localSample, tempWeights);

	if(cellId == -1)
	{
		sample = 0.0;
		return false;
	}

	hexahedronCell = grid->GetCell(cellId);

	if(hexahedronCell == NULL)
	{
		sample = 0.0;
		return false;
	}

	vtkIdList * pointIds = hexahedronCell->GetPointIds();

	for(int i = 0; i < 8; i++)
	{
		potentialValues[i] = grid->GetPointData()->GetScalars()->GetTuple1(pointIds->GetId(i));
	}

	GetHexahedronShapeFunctionValues(localHexaShapeFunctionValues, localSample[0], localSample[1], localSample[2]);
	sample = potentialValues * localHexaShapeFunctionValues;

	return true;
}


bool StructuredHexahedralMeshSampler::getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint)
{
	// TODO implement

	potentialGradient.clear();
	return false;

//		vtkCell * hexahedronCell = 0;
//		double localHexahedronSample[3];
//		Vec<8, Real> potentialValues;
//		Mat<8, 3, Real> localHexahedronShapeFunctionDerivatives;
//		Mat<3, 3, Real> invertedHexahedronJacobian;
//		Mat<8, 3, Real> globalHexahedronShapeFunctionDerivatives;
//
//		hexahedronCell = GetCellPotentialValuesAndLocalSample(potentialValues, localHexahedronSample, globalPoint);
//
//		if(hexahedronCell == 0)
//		{
//			std::cout << "No hexahedron found for global point " << globalPoint << ". Setting gradient to 0 0 0" << '\n';
//			// TODO add boolean return value to function
//			potentialGradient.set(0.0, 0.0, 0.0);
//			return;
//		}
//
//		/*
//		 * Quicker computation of gradient [dPhi(x) / dx]
//		 * local cosy r, s, t not translated/rotated to global cosy x, y, z
//		 */
//		double bounds[6];
//		hexahedronCell->GetBounds(bounds);
//
//		invertedHexahedronJacobian[0][0] = fabs(1.0/(bounds[1]-bounds[0])); // 1/dx
//		invertedHexahedronJacobian[1][1] = fabs(1.0/(bounds[3]-bounds[2])); // 1/dy
//		invertedHexahedronJacobian[2][2] = fabs(1.0/(bounds[5]-bounds[4])); // 1/dz
//
//		GetLocalHexahedronShapeFunctionDerivatives(localHexahedronShapeFunctionDerivatives, localHexahedronSample[0], localHexahedronSample[1], localHexahedronSample[2]);
//		globalHexahedronShapeFunctionDerivatives = localHexahedronShapeFunctionDerivatives * invertedHexahedronJacobian;
//
//		potentialGradient = globalHexahedronShapeFunctionDerivatives.transposed() * potentialValues;
//
//		/*
//		 * Formal computation of gradient [dPhi(x) / dx]
//		 */
//		Mat<3, 8, Real> hexahedronBasis;
//		Mat<3, 3, Real> hexahedronJacobian;
//
//		for(int i = 0; i < 8; i++)
//		{
//			double * point = m_PreComputedPotentialFieldGrid->GetPoint(hexahedronCell->GetPointIds()->GetId(i));
//			hexahedronBasis[0][i] = point[0];
//			hexahedronBasis[1][i] = point[1];
//			hexahedronBasis[2][i] = point[2];
//		}
//
//		GetLocalHexahedronShapeFunctionDerivatives(localHexahedronShapeFunctionDerivatives, localHexahedronSample[0], localHexahedronSample[1], localHexahedronSample[2]);
//		hexahedronJacobian = hexahedronBasis * localHexahedronShapeFunctionDerivatives;
//		hexahedronJacobian.transpose();
//		invertMatrix(invertedHexahedronJacobian, hexahedronJacobian);
//		globalHexahedronShapeFunctionDerivatives = localHexahedronShapeFunctionDerivatives * invertedHexahedronJacobian.transposed();
//
//		// TODO mit Determinante der Jacobi-Matrix gewichten? oder nur bei integralen?
//		potentialGradient = globalHexahedronShapeFunctionDerivatives.transposed() * potentialValues;
}

//template <class DataTypes>
//	vtkCell * ShapeMatchingPoissonPotentialForcefield<DataTypes>::GetCellPotentialValuesAndLocalSample(Vec<8, Real> & potentialValues, double localSample[3], Vec<3, double> globalPoint)
//	{
//		vtkCell * hexahedronCell = 0;
//		int tempSubId = 0;
//		double tempWeights[3];
////		double center[3];
//
//		std::cout << m_PreComputedPotentialFieldGrid->GetNumberOfCells() << '\n';
//		hexahedronCell = m_PreComputedPotentialFieldGrid->FindAndGetCell(globalPoint.ptr(), NULL, 0, 0.1, tempSubId,localSample, tempWeights);
//
//		if(hexahedronCell == 0)
//		{
////			m_PreComputedPotentialFieldGrid->GetCenter(center);
////			Vec<3, Real> test(center[0] - globalPoint[0], center[1] - globalPoint[1], center[2] - globalPoint[2]);
////			double norm = test.norm();
////			potentialValues.fill(norm);
//
//			potentialValues.clear();
//			localSample[0] = 0.5;
//			localSample[1] = 0.5;
//			localSample[2] = 0.5;
//			return NULL;
//		}
//
//		vtkIdList * pointIds = hexahedronCell->GetPointIds();
//
//		for(int i = 0; i < 8; i++)
//		{
//			potentialValues[i] = m_PreComputedPotentialFieldGrid->GetPointData()->GetScalars()->GetTuple1(pointIds->GetId(i));
//		}
//
//		return hexahedronCell;
//	}

void StructuredHexahedralMeshSampler::getSmallestCellBoundingBox(double bounds[6])
{
	grid->GetCell(0)->GetBounds(bounds);
}

bool StructuredHexahedralMeshSampler::getCellBoundingBox(double bounds[6], Vector3 globalPoint)
{
	// TODO implement
	return false;
}

void StructuredHexahedralMeshSampler::GetHexahedronShapeFunctionValues(Vec<8, double> & values, double r, double s, double t)
{
	/*
	 * Order of points in hexahedron (bottom face)
	 * 3--2
	 * |  |
	 * 0--1
	 */

	values[0] = (1.0 - r) * (1.0 - s) * (1.0 - t);
	values[1] = (r) * (1.0 - s) * (1.0 - t);
	values[2] = (r) * (s) * (1.0 - t);
	values[3] = (1.0 - r) * (s) * (1.0 - t);
	values[4] = (1.0 - r) * (1.0 - s) * (t);
	values[5] = (r) * (1.0 - s) * (t);
	values[6] = (r) * (s) * (t);
	values[7] = (1.0 - r) * (s) * (t);
}
