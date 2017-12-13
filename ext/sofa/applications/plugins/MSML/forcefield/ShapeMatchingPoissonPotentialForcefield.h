/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefield
  Author:    Tino Milschewski

=========================================================================*/

#ifndef __ShapeMatchingPoissonPotentialForcefield_h
#define __ShapeMatchingPoissonPotentialForcefield_h

#include <sofa/core/behavior/ForceField.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Mat.h>

#include "vtkStructuredGrid.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkCellLocator.h"

#include "MeshSampler.h"

namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::helper;
using namespace sofa::defaulttype;
using namespace sofa::core::topology;

template<class DataTypes>
class ShapeMatchingPoissonPotentialForcefield : public core::behavior::ForceField<DataTypes>
{
public:
	// standard class typedefs
	SOFA_CLASS(SOFA_TEMPLATE(ShapeMatchingPoissonPotentialForcefield, DataTypes), SOFA_TEMPLATE(core::behavior::ForceField, DataTypes));

	static const unsigned int numberOfSurfaceIntegrationPoints = 1;

	double m_IntegrationPoints[3*numberOfSurfaceIntegrationPoints];
	double m_IntegrationWeights[numberOfSurfaceIntegrationPoints];

	typedef typename DataTypes::VecCoord VecCoord;
	typedef typename DataTypes::VecDeriv VecDeriv;
	typedef typename Topology::Triangle Triangle;
	typedef typename Topology::Tetra Tetrahedron;
	typedef typename BaseMeshTopology::TetrahedraAroundTriangle TetrahedraAroundTriangle;
	typedef typename BaseMeshTopology::TrianglesInTetrahedron TrianglesInTetrahedron;

	typedef typename DataTypes::Coord Coord;
	typedef typename DataTypes::Deriv Deriv;
	typedef typename Coord::value_type Real;

	typedef Mat<12, 12, Real> ElementStiffnessMatrixType;
	typedef Mat<4, 3, Real> ElementShapeFunctionDerivativesType;
	typedef Vec<numberOfSurfaceIntegrationPoints, double> SurfaceTriangleSamplingDistanceType;

	typedef struct
	{
		int tetrahedronIndex;
		vector<int> surfaceTriangleLocalIndexes;
		vector<int> surfaceTriangleGlobalIndexes;
		vector<double> surfaceAreasX0Doubled;
		ElementStiffnessMatrixType stiffnessMatrix;
		Vec<numberOfSurfaceIntegrationPoints, ElementShapeFunctionDerivativesType> globalShapeFunctionDerivatives;
		vector<SurfaceTriangleSamplingDistanceType> surfaceTriangleSamplingDistances;
		bool isActive;
		vector<bool> surfaceTriangleActiveFlag;
	} TetrahedronOnSurfaceElementInformation;

	ShapeMatchingPoissonPotentialForcefield();
	virtual ~ShapeMatchingPoissonPotentialForcefield();

	virtual void init();
//	virtual void cleanup();
	virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	virtual void addDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor) {}
	virtual void addKToMatrix(BaseMatrix *mat, SReal kFactor, unsigned int &offset);
	virtual double getPotentialEnergy(const sofa::core::MechanicalParams* mparams) const;
	virtual void setTotalElasticEnergy(double totalElasticEnergy){m_TotalElasticEnergy = totalElasticEnergy;}
	virtual void setTotalKineticEnergy(double totalKineticEnergy){m_TotalKineticEnergy = totalKineticEnergy;}

protected:

	void InitializeSurfaceIntegrationPointsAndWeights();
	inline void GetTetrahedronShapeFunctionValues(Vec<4,Real> & values, Real r, Real s,Real t);
	inline void GetLocalTetrahedronShapeFunctionDerivatives(Mat<4,3,Real> & values, Real r, Real s,Real t);
	void GetJacobian(Mat<3,3, Real> & jacobian, Mat<3, 4, Real> tetrahedronBasis, Real r, Real s, Real t);

	void GetTetrahedronBasis(Mat<3, 4, Real> &basis, Tetrahedron tetrahedron, const VecCoord * x);
	void InitializeElementInformation();
	double GetTriangleSurfaceAreaDoubled(Triangle triangle, const VecCoord * x);
	void GetTriangleUnnormalizedNormal(Coord & normal ,Triangle triangle, const VecCoord *x);
	void GetGlobalTetrahedronShapeFunctionDerivatives(Mat<4, 3, Real> & globalShapeFunctionDerivatives, Mat<3, 4, Real> tetrahedronBasis, Real r, Real s, Real t);

	void ClearElementInformation(TetrahedronOnSurfaceElementInformation & elementInformation);

	void GetPotentialGradientInternal(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Triangle triangle, const VecCoord * x);
	bool GetPotentialGradientCentralDifferences(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Real samplingDistanceMultiplicator);
	bool GetPotentialGradientCentralDifferencesInNormalDirection(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint, Real samplingDistanceMultiplicator, Triangle triangle, const VecCoord * x);
	bool GetPotentialGradientFEM(Vec<3, Real> & potentialGradient, Vec<3, Real> globalPoint);

	void GetPotentialHessianInternal(Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Triangle triangle, double & samplingDistance, const VecCoord * x);
	bool GetPotentialHessianCentralDifferences(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance);
	bool GetPotentialHessianCentralDifferencesInNormalDirection(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance,Triangle triangle, const VecCoord * x);
	bool GetPotentialHessianCentralDifferencesInNormalDirectionCD(bool & hessianOK, Mat<3, 3, Real> & potentialHessian, Vec<3, Real> globalPoint, Real distance,Triangle triangle, const VecCoord * x);

	void GetSurfaceTriangleSample(Vec<3, Real> & localSample, int surfaceIntegrationPointIndex, int surfaceTriangleLocalIndex);
	int findTetrahedronInTetrahedraOnSurfaceElementInformation(int tetrahedronIndex);

	void MarkActiveSurfaceTriangles(const VecCoord * x);

	void WriteMatrixToCSVFile(defaulttype::BaseMatrix * mat,char* filename);
	void WriteForcesToVTKFile(VecDeriv& f, const VecCoord& x, char * filename);

	void GetTriangleBasis(Mat<3, 3, double> &basis, Triangle triangle, const VecCoord * x);
	double GetSurfaceJacobianDeterminant(Mat<3, 3, double> triangleBasis, double r, double s);

	bool HasCellId(vtkIdType cellId, vector<vtkIdType> * cellIds);

	vector<TetrahedronOnSurfaceElementInformation> m_tetrahedraOnSurfaceElementInformation;
	BaseMeshTopology* m_UndeformedMeshTopology;
	MeshSampler * m_MeshSampler;

	double m_PotentialEnergy;
	double m_TotalElasticEnergy;
	double m_TotalKineticEnergy;
	double m_CurrentChargeDensity;

	int m_FrameCounter;
	int m_FrameStart;
	int m_FrameEnd;

	double m_SamplingDistanceGradient;
	bool m_DebugOutput;

	vtkSmartPointer<vtkPolyData> m_DeformedSurface;
	 vtkSmartPointer<vtkCellLocator> m_CellLocator;



	// xml configuration variables
	Data<Real> m_ChargeDensity;
	Data<std::string> m_PrecomputedPotentialFieldFilename;
	Data<Real> m_SamplingDistance;
	Data<bool> m_AdaptChargeDensity;
	Data<int> m_AdaptSamplingDistance;
	Data<Real> m_ChargeFactorMultiplier;

	Data<std::string> m_DebugPath;
	Data<bool> m_AdaptActiveTriangles;
	Data<Real> m_NormalDifferenceThreshold;
	Data<Real> m_DistanceThreshold;

};

}

}

}
#endif
