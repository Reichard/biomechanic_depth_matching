/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingElectricPotentialForcefield
  Author:    Stefan Suwelack

=========================================================================*/

#ifndef __ShapeMatchingElectricPotentialForcefield_h
#define __ShapeMatchingElectricPotentialForcefield_h

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <QString>
#include <QFile>

//#include <sofa/component/topology/TetrahedronData.h>
#include <sofa/core/behavior/ForceField.h>
#include "RotationForceField.h"
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/helper/vector.h>

#include <sofa/component/topology/RegularGridTopology.h>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define NUMBER_OF_ELEMENT_NODES 4
#define NUMBER_OF_SURFACE_INTEGRATION_POINTS 1
//---------------------------------------------------------------------------
// ShapeMatchingElectricPotentialForcefield
//---------------------------------------------------------------------------
namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;
using sofa::helper::vector;
using namespace sofa::core::topology;
//using namespace sofa::component::topology;

//---------------------------------------------------------------------------
// This class is used to do some internal evaluations, either replacing or extending the class, used for CUDA!
//---------------------------------------------------------------------------
template< class DataTypes >
class ShapeMatchingElectricPotentialForcefieldInternalData
{
};

/** \class ShapeMatchingElectricPotentialForcefield
 * \brief This class does some really fancy stuff
 *
 * \ingroup Examples
 */
template<class DataTypes>
class ShapeMatchingElectricPotentialForcefield : public core::behavior::ForceField<DataTypes>, public sofa::core::behavior::RotationForceField
{
public:
	//const members
	static const unsigned int numberOfIntegrationPoints = 1;

	// constructor
	ShapeMatchingElectricPotentialForcefield();

	// destructor
	~ShapeMatchingElectricPotentialForcefield();

	// standard class typedefs
	SOFA_CLASS(SOFA_TEMPLATE(ShapeMatchingElectricPotentialForcefield, DataTypes), SOFA_TEMPLATE(core::behavior::ForceField, DataTypes));

	typedef typename DataTypes::VecCoord VecCoord;
	typedef typename DataTypes::VecDeriv VecDeriv;
	typedef typename DataTypes::VecReal VecReal;
	typedef VecCoord Vector;
	typedef typename DataTypes::Coord Coord;
	typedef typename DataTypes::Deriv Deriv;
	typedef typename Coord::value_type Real;
	typedef typename sofa::core::topology::BaseMeshTopology::Tetra LinearTetraType;
	typedef typename sofa::core::topology::BaseMeshTopology::Triangle LinearTriangleType;
	typedef typename sofa::core::topology::BaseMeshTopology::Hexa LinearHexaType;

	 typedef core::objectmodel::Data<VecDeriv>    DataVecDeriv;

	// public methods
	virtual void init();
	virtual void reinit();
	virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	virtual void addDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor);
	virtual double getPotentialEnergy(const VecCoord& xData) const;
	virtual void addKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset);
	virtual void handleTopologyChange();

	void applyRotation(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, sofa::core::MultiVecDerivId dxId, sofa::core::MultiVecDerivId dfId, bool invert );

	//void setUpdateStiffnessMatrix(bool val) { this->m_updateStiffnessMatrix.setValue(val); }

	//void setComputeGlobalMatrix(bool val) { this->m_assembling.setValue(val); }

protected:
	//---------------------------------------------------------------------------
	// Handling internal data for evaluations, i.e. with CUDA.
	//---------------------------------------------------------------------------
	ShapeMatchingElectricPotentialForcefieldInternalData<DataTypes> *InternalData;
    friend class ShapeMatchingElectricPotentialForcefieldInternalData<DataTypes>;

	//protected typedefs
	// Displacement vector (deformation of the 4 corners of a tetrahedron
	typedef VecNoInit<12, Real> DisplacementType;
	/// Strain-displacement matrix
	typedef Mat<12, 6, Real> StrainDisplacementType;
	/// Element Stiffness matrix ( = RJKJtRt  with K the Material stiffness matrix, J the strain-displacement matrix, and R the transformation matrix if any )
	typedef Mat<12, 12, Real> ElementStiffnessMatrixType;
	typedef helper::vector<ElementStiffnessMatrixType> ElementStiffnessMatrixVectorType;
	typedef Mat<3,3,Real> RotationMatrixType;
	typedef Mat<3,3,Real> PrecomputedStiffnessBlockType;
	typedef Mat<4,4,PrecomputedStiffnessBlockType> PrecomputedStiffnessBlockMatrixType;
	typedef helper::fixed_array<Coord,4> ElementCoordsType;

	typedef struct{
	ElementStiffnessMatrixType ElementStiffnessMatrix;
	ElementStiffnessMatrixType ElementStiffnessMatrixLinear;
	helper::fixed_array<PrecomputedStiffnessBlockMatrixType,numberOfIntegrationPoints> precomputedElementStiffnessBlock;
	helper::fixed_array<RotationMatrixType,numberOfIntegrationPoints> elementRotationMatrixArray;
	helper::fixed_array<RotationMatrixType,numberOfIntegrationPoints> elementRotationMatrixTransposedArray;
	helper::fixed_array<Mat<4,3,Real>,numberOfIntegrationPoints>  shapeFunctionDerivativesGlobal;
	helper::fixed_array<Mat<4,3,Real>,NUMBER_OF_ELEMENT_NODES>  shapeFunctionDerivativesGlobalPerNode;
	double volume;
	//surface based values for shape matching
	helper::fixed_array< Mat<4,3,Real>,4*NUMBER_OF_SURFACE_INTEGRATION_POINTS>  shapeFunctionDerivativesGlobalSurface;
	//helper::fixed_array< helper::fixed_array<Real,4>, NUMBER_OF_SURFACE_INTEGRATION_POINTS>  shapeFunctionValuesSurface;
	} LinearTetrahedronInformation;

	// protected member variables
	helper::vector<LinearTetrahedronInformation> m_LinearTetrahedronInfos;
	helper::vector<RotationMatrixType> m_MeanRotationMatricesPerNode;
	sofa::core::topology::BaseMeshTopology* m_Topology;

	// protected methods


	// protected attributes
//	Data<bool> m_updateStiffnessMatrix;
//	Data<bool> m_assembling;
	Data<Real> m_Charge;
	Data<std::string> m_DeformedMeshFilename;
	Data<std::string> m_PrecomputedPotentialFieldFilename;
	Data<bool> m_UsePrecomputedPotentialField;
	Data<bool> m_SavePrecomputedPotentialField;
	Data<int> m_Resolution;



//	Data<bool> m_LinearMode;

private:
	//private typedefs

	// private methods
	void InitializePrecomputedStiffnessBlocks();
	void InitializePrecomputedStiffnessBlocks(unsigned int elementID);
	void BuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlock,ElementCoordsType coords, unsigned int integrationPointIndex);
	void InitializeShapeFunctionDerivatives();
	void InitializeShapeFunctions();
	void InitializeIntegrationPointsAndWeights();
	void InitializeElementData();
	void GetJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfIntegrationPoint);
	void GetNodalJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfNodalPoint);
	void GetSurfaceJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int surfaceIndex, int indexOfIntegrationPoint);
	double GetElementVolume(unsigned int elementID, ElementCoordsType coords);

	void InitializeIntegrationPointsAndWeights2D();

	inline void GetHexaShapeFunctionValues(Vec<8,Real> & values, Real r, Real s,Real t);
	inline void GetHexaShapeFunctionDerivativeValues(Mat<3,8,Real> & values,Real r, Real s,Real t);

	void WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename);
	void WriteElementMatrixToCVSFile(Mat<12,12,Real>  mat,char* filename, int elementNumber);
	void WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber);

	// private attributes
	Data<ElementStiffnessMatrixVectorType> m_ElementStiffnessMatrixVector;
	Data<ElementStiffnessMatrixVectorType> m_PrecomputedElementStiffnessMatrixBlock;
	helper::vector<helper::fixed_array<Real,4> > m_ShapeFunctions;
	helper::fixed_array< Mat<4,3,Real>, numberOfIntegrationPoints> m_ShapeFunctionDerivatives;

	helper::fixed_array< helper::fixed_array< Vec<4,Real>, NUMBER_OF_SURFACE_INTEGRATION_POINTS>, 4> m_ShapeFunctionsSurface;
	helper::fixed_array< helper::fixed_array< Mat<4,3,Real>, NUMBER_OF_SURFACE_INTEGRATION_POINTS>, 4>  m_ShapeFunctionDerivativesSurface;

	helper::fixed_array< Mat<4,3,Real>, NUMBER_OF_ELEMENT_NODES> m_ShapeFunctionDerivativesPerNode;
	double m_NodalLocalCoordinates[3*NUMBER_OF_ELEMENT_NODES];

	double m_IntPoints[3*numberOfIntegrationPoints];
	double m_IntWeights[numberOfIntegrationPoints];

	Mat<4,NUMBER_OF_SURFACE_INTEGRATION_POINTS,Coord> m_IntPoints2D;
	Mat<4,NUMBER_OF_SURFACE_INTEGRATION_POINTS,Real> m_IntWeights2D;

	int counter;
	Real dx,dy,dz;

	sofa::component::topology::RegularGridTopology::SPtr m_PotentialFieldTopology;
	helper::vector<Real > m_PotentialFieldValues;
	helper::vector<Real > m_SurfaceTrianglesGlobalIndices;
	helper::vector<Real> m_SurfaceTrianglesLocalIndices;
	helper::vector<Real> m_SurfaceTrianglesVolume; //we actually store 2*volume (factor 0.5 is in intWeights2D)

};

} // end namespace forcefield
} //end namespace component
} //end namespace sofa


#endif /* __ShapeMatchingElectricPotentialForcefield_h */
