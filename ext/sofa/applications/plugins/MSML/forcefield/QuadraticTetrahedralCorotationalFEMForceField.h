/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, QuadraticTetrahedralCorotationalFEMForceField
  Author:    Stefan Suwelack (nip and tuck by Mario Laugisch)

=========================================================================*/
#ifndef SOFA_COMPONENT_FORCEFIELD_QuadraticTetrahedralCorotationalFEMForceField_H
#define SOFA_COMPONENT_FORCEFIELD_QuadraticTetrahedralCorotationalFEMForceField_H

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <sofa/core/behavior/ForceField.h>
#include "RotationForceField.h"
//#include <sofa/component/topology/TetrahedronData.h>
//TODO include the new head file of Quadratic Tetrahedon data
#include <QuadraticTetrahedronData.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>

#include <QuadraticBaseMeshTopology.h>
#include "sofa/helper/AdvancedTimer.h"

#include <QString>
#include <QFile>

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define NUMBER_OF_ELEMENT_NODES_TET10 10
#define NUMBER_OF_INTEGRATION_POINTS_TET10 4
#define STIFFNESS_MATRIX_SIZE_TET10 (3*NUMBER_OF_ELEMENT_NODES_TET10)
//---------------------------------------------------------------------------
// QuadraticTetrahedralCorotationalFEMForceField
//---------------------------------------------------------------------------
namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;
using sofa::helper::vector;
using namespace sofa::component::topology;

//---------------------------------------------------------------------------
// This class is used to do some internal evaluations, either replacing or extending the class, used for CUDA!
//---------------------------------------------------------------------------
template< class DataTypes >
class QuadraticTetrahedralCorotationalFEMForceFieldInternalData
{
};

/** Compute Finite Element forces based on tetrahedral elements.
*/
template<class DataTypes>
class QuadraticTetrahedralCorotationalFEMForceField : public core::behavior::ForceField<DataTypes>, public sofa::core::behavior::RotationForceField
{
public:
	// standard class typedefs
	SOFA_CLASS(SOFA_TEMPLATE(QuadraticTetrahedralCorotationalFEMForceField, DataTypes), SOFA_TEMPLATE(core::behavior::ForceField, DataTypes));

//---------------------------------------------------------------------------
// Public typedefs
//---------------------------------------------------------------------------
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::VecReal VecReal;
    typedef VecCoord Vector;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef typename sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra QuadraticTetraType;

    typedef core::objectmodel::Data<VecDeriv>    DataVecDeriv;

    //---------------------------------------------------------------------------
    // Public methods
    //---------------------------------------------------------------------------

    // constructor
    QuadraticTetrahedralCorotationalFEMForceField();
//    : m_updateStiffnessMatrix(core::objectmodel::BaseObject::initData(&m_updateStiffnessMatrix,true,"updateStiffnessMatrix",""))
//    , m_assembling(core::objectmodel::BaseObject::initData(&m_assembling,true,"computeGlobalMatrix",""))
//    {
//    	this->m_PoissonRatio = 0.49;//365;//49;
//    	this->m_YoungModulus = 2061;//3000;
//    	this->m_ShearModulus = this->m_YoungModulus/(2*(1+this->m_PoissonRatio));
//    	this->m_LamesFirstParameter = (this->m_YoungModulus*this->m_PoissonRatio)/((1+this->m_PoissonRatio)*(1-2*this->m_PoissonRatio));
//       // this->addAlias(&_assembling, "assembling");
//        //_poissonRatio.setWidget("poissonRatio");
//    	//JacobiFile.setFileName("/home/mediassi/MediAssistData/Temp/JacobiMatrixDeterminant.csv");
//    }

    // destructor
    ~QuadraticTetrahedralCorotationalFEMForceField();

	//---------------------------------------------------------------------------
	// Main functions
	//---------------------------------------------------------------------------
	virtual void init();
    virtual void reinit();
    virtual void addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);
    virtual void addDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor);
    virtual void addKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset);

    void applyRotation(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, sofa::core::MultiVecDerivId dxId, sofa::core::MultiVecDerivId dfId, bool invert );

    virtual double getPotentialEnergy(const VecCoord& x) const;
	virtual void handleTopologyChange();

	//former main functions to switch, could also be the new ones while testing!
	virtual void AddForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	virtual void AddDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor);
	virtual void AddKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset);



    // member access
	void setPoissonRatio(Real val) {
		this->m_PoissonRatio.setValue( val); }
//		Real poissonRatio = val;
//		Real youngModulus = this->m_YoungModulus.getValue();
//		this->m_ShearModulus = youngModulus/(2*(1+poissonRatio));
//		this->m_LamesFirstParameter = (youngModulus*poissonRatio)/((1+poissonRatio)*(1-2*poissonRatio));}

	void setYoungModulus(Real val) {
//		this->m_YoungModulus.setValue( val);
//		Real poissonRatio = this->m_PoissonRatio.getValue();
//		Real youngModulus = val;
//		this->m_ShearModulus = youngModulus/(2*(1+poissonRatio));
//		this->m_LamesFirstParameter = (youngModulus*poissonRatio)/((1+poissonRatio)*(1-2*poissonRatio));}
        VecReal newY;
        newY.resize(1);
        newY[0] = val;
        m_YoungModulus.setValue(newY);
	}

	void setUpdateStiffnessMatrix(bool val) { this->m_updateStiffnessMatrix.setValue(val); }

	void setComputeGlobalMatrix(bool val) { this->m_assembling.setValue(val); }

//----------------------------------------------------------------------------------------------------------------
// Public member variables
//----------------------------------------------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//const members
	//---------------------------------------------------------------------------
	static const unsigned int numberOfIntegrationPoints = NUMBER_OF_INTEGRATION_POINTS_TET10;
	static const unsigned int nodesPerElement = NUMBER_OF_ELEMENT_NODES_TET10;



protected:
	//---------------------------------------------------------------------------
	// Handling internal data for evaluations, i.e. with CUDA.
	//---------------------------------------------------------------------------
    QuadraticTetrahedralCorotationalFEMForceFieldInternalData<DataTypes> *InternalData;
	friend class QuadraticTetrahedralCorotationalFEMForceFieldInternalData<DataTypes>;

	//----------------------------------------------------------------------------------------------------------------
	// protected typedefs
	//----------------------------------------------------------------------------------------------------------------

	/// @name Per element (tetrahedron) data
    /// @{

    /// Displacement vector (deformation of the 4 corners of a tetrahedron
    typedef VecNoInit<30, Real> DisplacementType;
    /// Strain-displacement matrix
    typedef Mat<30, 6, Real> StrainDisplacementType;
    /// Element Stiffness matrix ( = RJKJtRt  with K the Material stiffness matrix, J the strain-displacement matrix, and R the transformation matrix if any )
    typedef Mat<30, 30, Real> ElementStiffnessMatrixType;
    typedef helper::vector<ElementStiffnessMatrixType> ElementStiffnessMatrixVectorType;

    /*
     * Code bei Beibei
     */
    typedef Mat<3,3,Real> RotationMatrixType;
    typedef helper::fixed_array<Coord,10> RotatedInitialElementType;
    typedef Mat<3,3,Real> PrecomputedStiffnessBlockType;
    typedef Mat<10,10,PrecomputedStiffnessBlockType> PrecomputedStiffnessBlockMatrixType;

	//----------------------------------------------------------------------------------------------------------------
    // Typedefs like in Cuda-part of this class!
	//----------------------------------------------------------------------------------------------------------------
	typedef Coord ElementCoordsType[nodesPerElement];						// The coords of the element!
	typedef Coord ElementSimpleCoordsType[4];
	typedef Real ShapeFunctionDerivativesGlobalType[nodesPerElement][3];
	typedef Real SimpleShapeFunctionDerivativesGlobalType[4][3];

	//---------------------------------------------------------------------------
	// The element struct containing some data
	//---------------------------------------------------------------------------
    typedef struct{
    	//helper::fixed_array<Coord,10> rotatedInitialElement;
    	ElementStiffnessMatrixType ElementStiffnessMatrix;
    	helper::fixed_array<PrecomputedStiffnessBlockMatrixType,4> precomputedElementStiffnessBlockMatrixArray;
    	helper::fixed_array<RotationMatrixType,4> elementRotationMatrixArray;
    	helper::fixed_array<RotationMatrixType,4> elementRotationMatrixTransposedArray;
    	helper::fixed_array<Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real>,4>  shapeFunctionDerivativesGlobal;
    	helper::fixed_array<Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real>,NUMBER_OF_ELEMENT_NODES_TET10>  shapeFunctionDerivativesGlobalPerNode;
    	helper::fixed_array<Mat<4,3,Real>,1> simpleShapeFunctionDerivativesGlobal;
    	double volume;
    } QuadTetrahedronInformation;

	//---------------------------------------------------------------------------
	// Build the full system matrix
	//---------------------------------------------------------------------------
   /// @name Full system matrix assembly support
   /// @{

       typedef std::pair<int,Real> Col_Value;
       typedef vector< Col_Value > CompressedValue;
       typedef vector< CompressedValue > CompressedMatrix;
       typedef unsigned int Index;

       CompressedMatrix m_stiffnesses;
   /// @}

   	//---------------------------------------------------------------------------
    //	Vectors we need for AddForce and AddDForce
   	//---------------------------------------------------------------------------
    const VecCoord *m_X0;

	//---------------------------------------------------------------------------
	// How many nodes and elements
	//---------------------------------------------------------------------------
	unsigned int m_nElements; //How many elements
	unsigned int m_nNodes;	// How many nodes

	//---------------------------------------------------------------------------
	//	Protected member variables
	//---------------------------------------------------------------------------
    helper::vector<QuadTetrahedronInformation> m_QuadTetrahedronInfos;
	helper::vector<RotationMatrixType> m_MeanRotationMatricesPerNode;
    sofa::core::topology::QuadraticBaseMeshTopology* m_Topology;

    // member variables
//    Real m_PoissonRatio;
//    Real m_YoungModulus;
//    Real m_ShearModulus;
//    Real m_LamesFirstParameter;
    Data<bool> m_updateStiffnessMatrix;
    Data<bool> m_assembling;
    double m_potentialEnergy;

    Data<Real> m_PoissonRatio;
    //Data<Real> m_YoungModulus;
    Data<VecReal > m_YoungModulus;

    Data<bool> m_UseSimpleRotations;

    Data<bool> m_LinearMode;


  private:
	//---------------------------------------------------------------------------
	//	private typedefs
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	// private methods
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	//	old functions to initialize all the matrices and vectors we need
	//---------------------------------------------------------------------------
    void AssembleElementMatrices();
	void BuildElementMatrix(ElementStiffnessMatrixType & elementMatrix, unsigned int elementID);
	void BuildCoRotationalElementMatrix(const VecCoord& x, unsigned int elementID);
	void BuildCoRotationalElementMatrix(unsigned int elementID, double kFactor);
	void OldBuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlock,helper::fixed_array<Coord,10> coords, unsigned int integrationPointIndex);
	void computeForce(VecDeriv& df, const VecDeriv& dx, unsigned int elementID);
	void OldInitializeShapeFunctionDerivatives(unsigned int numberOfIntegrationPoints);
	void OldInitializeShapeFunctions(unsigned int numberOfIntegrationPoints);
	void OldInitializeIntegrationPointsAndWeights(unsigned int numberOfIntegrationPoints);
	double OldGetJacobian(Mat<3,3,Real> &jacobianMatrix, helper::fixed_array<Coord,10> coords, int indexOfIntegrationPoint);

	//---------------------------------------------------------------------------
	//	functions to initialize all the matrices and vectors we need !
	//---------------------------------------------------------------------------
	void InitializePrecomputedStiffnessBlocks();
	inline void InitializePrecomputedStiffnessBlockForElement(unsigned int elementID);
	inline void BuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlockMatrix,ElementCoordsType coords, unsigned int integrationPointIndex);
	void InitializeShapeFunctionDerivatives();
	void InitializeShapeFunctions();
	void InitializeIntegrationPointsAndWeights();
	void InitializeElementData();
	inline void GetJacobian(Mat<3,3,Real> & jacobianMatrix, ElementCoordsType coords, int indexOfIntegrationPoint);
	void GetNodalJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfNodalPoint);
	inline void GetSimpleJacobian( Mat<3,3,Real> &jacobianMatrix, ElementSimpleCoordsType coords);
	double GetElementVolume(unsigned int elementID, ElementCoordsType coords);


	void WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename);
	void WriteElementMatrixToCVSFile(Mat<30,30,Real>  mat,char* filename, int elementNumber);
	void WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber);

	//---------------------------------------------------------------------------
	// private attributes
	//---------------------------------------------------------------------------
	Data<ElementStiffnessMatrixVectorType> m_ElementStiffnessMatrixVector;
	Data<ElementStiffnessMatrixVectorType> m_PrecomputedElementStiffnessMatrixBlock;
	helper::vector<Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real> > m_ShapeFunctionDerivatives;
	helper::vector<helper::fixed_array<Real,10> > m_ShapeFunctions;
	Mat<10,3,Real> ShapeFunctionDerivative;
	Mat<4,3,Real> m_SimpleShapeFunctionDerivative;

	helper::fixed_array< Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real>, NUMBER_OF_ELEMENT_NODES_TET10> m_ShapeFunctionDerivativesPerNode;
	double m_NodalLocalCoordinates[3*NUMBER_OF_ELEMENT_NODES_TET10];

	double m_IntPoints[33];
	double m_IntWeights[11];

	int counter;

};

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif
