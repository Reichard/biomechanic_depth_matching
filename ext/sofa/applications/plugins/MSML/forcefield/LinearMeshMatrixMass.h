/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, LinearMeshMatrixMass
  Author:    Stefan Suwelack

=========================================================================*/

#ifndef __LinearMeshMatrixMass_h
#define __LinearMeshMatrixMass_h

// ****************************************************************************
// Includes
// ****************************************************************************
#include <QString>
#include <QFile>

//#include <sofa/component/topology/TetrahedronData.h>
#include <sofa/core/behavior/Mass.h>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/helper/vector.h>

#include <sofa/core/behavior/ForceField.h>


// ****************************************************************************
// Defines
// ****************************************************************************




// ****************************************************************************
// LinearMeshMatrixMass
// ****************************************************************************


namespace sofa
{

namespace component
{

namespace mass
{

using namespace sofa::defaulttype;
using sofa::helper::vector;
using namespace sofa::core::topology;

//---------------------------------------------------------------------------
// This class is used to do some internal evaluations, either replacing or extending the class, used for CUDA!
//---------------------------------------------------------------------------
template< class DataTypes, class TMassType >
class LinearMeshMatrixMassInternalData
{
};

/** \class LinearMeshMatrixMass
 * \brief This class does some really fancy stuff
 *
 * \ingroup Examples
 */
template <class DataTypes, class TMassType>
class LinearMeshMatrixMass : public core::behavior::Mass<DataTypes>
{
public:

	//const members
	static const unsigned int numberOfIntegrationPoints = 1;

	// constructor
	LinearMeshMatrixMass();

	// destructor
	~LinearMeshMatrixMass();

	// standard class typedefs
    SOFA_CLASS(SOFA_TEMPLATE2(LinearMeshMatrixMass,DataTypes,TMassType), SOFA_TEMPLATE(core::behavior::Mass,DataTypes));


	typedef core::behavior::Mass<DataTypes> Inherited;
	typedef typename DataTypes::VecCoord                    VecCoord;
	typedef typename DataTypes::VecDeriv                    VecDeriv;
	typedef typename DataTypes::Coord                       Coord;
	typedef typename DataTypes::Deriv                       Deriv;
	typedef typename DataTypes::Real                        Real;
	typedef TMassType                                       MassType;
	typedef helper::vector<Deriv> GravityForceVectorType;
	typedef Mat<4,4,Real> ElementMassMatrixType;
	typedef helper::vector<ElementMassMatrixType> ElementMassMatrixVectorType;
	typedef typename sofa::core::topology::BaseMeshTopology::Tetra LinearTetraType;

	typedef helper::vector<Real> DiagonalForceVectorType;

	// public methods
	void clear();
	virtual void reinit();
	virtual void init();

	// -- Mass interface
	void addMDx(VecDeriv& f, const VecDeriv& dx, double factor = 1.0);
	void accFromF(VecDeriv& a, const VecDeriv& f); // This function can't be used as it use M^-1
	void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);
	double getKineticEnergy(const VecDeriv& v)const {return 0;} ;  ///< vMv/2 using dof->getV()
	virtual double getPotentialEnergy(const sofa::core::MechanicalParams* mparams) const;
	void addGravityToV(double dt/*, defaulttype::BaseVector& v*/);
	bool isDiagonal(){return false;}
	void addMToMatrix(defaulttype::BaseMatrix * mat, double mFact, unsigned int &offset); /// Add Mass contribution to global Matrix assembling
	double getElementMass(unsigned int index) const;
	void getElementMass(unsigned int index, defaulttype::BaseMatrix *m) const;
	virtual void handleTopologyChange();// handle topological changes


	// member access
	Real getMassDensity() const {
	  return m_massDensity.getValue();
	}

	void setMassDensity(Real m) {
	  m_massDensity.setValue(m);
	}



protected:

	//---------------------------------------------------------------------------
	// Handling internal data for evaluations, i.e. with CUDA.
	//---------------------------------------------------------------------------
	LinearMeshMatrixMassInternalData<DataTypes, TMassType> *InternalData;
	friend class LinearMeshMatrixMassInternalData<DataTypes, TMassType>;

	//protected typedefs


	// protected methods


	// protected attributes
	Data< Real >         m_massDensity;
	Data<bool> m_isDiagonal;
	sofa::core::topology::BaseMeshTopology* m_Topology;


private:
	//private typedefs

	// private methods
	void AssembleElementMatrices();
	void BuildElementMatrix(ElementMassMatrixType & elementMatrix, unsigned int elementID);
	void AssembleGravityForceVector();
	void AssembleDiagonalForceVector();
	void BuildElementGravityForceVector(helper::fixed_array<Deriv,4>  & elementGravityVector, unsigned int elementID);
	void InitializeShapeFunctionDerivatives();
	void InitializeShapeFunctions();
	void InitializeIntegrationPointsAndWeights();
	void InitializeElementData();
	double GetJacobianDeterminant(helper::fixed_array<Coord,4> coords, int indexOfIntegrationPoint);
	//double ShapeFunctionDerivativeValue(double x1, double x2, double x3, int p);
	//double ShapeFunctionValue(double x1, double x2, double x3, int p);
	void WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename);
	void WriteElementMatrixToCVSFile(Mat<12,12,Real>  mat,char* filename, int elementNumber);
	void WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber);


	// private attributes
	Data<ElementMassMatrixVectorType> m_ElementMassMatrixVector;
	Data<GravityForceVectorType> m_GravityForceVector;
	helper::fixed_array< Mat<4,3,Real>, numberOfIntegrationPoints> m_ShapeFunctionDerivatives;
	helper::vector<helper::fixed_array<Real,4> > m_ShapeFunctions;

	DiagonalForceVectorType m_DiagonalForceVector;

	double m_IntPoints[3*numberOfIntegrationPoints];
	double m_IntWeights[numberOfIntegrationPoints];

	double m_PotentialEnergy;




};

} // end namespace mass
} //end namespace component
} //end namespace sofa


#endif /* __LinearMeshMatrixMass_h */
