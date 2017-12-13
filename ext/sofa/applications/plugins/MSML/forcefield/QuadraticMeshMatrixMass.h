/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_MASS_QuadraticMeshMatrixMass_H
#define SOFA_COMPONENT_MASS_QuadraticMeshMatrixMass_H



//#define SOFA_HAVE_GLEW

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/behavior/Mass.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/core/objectmodel/Event.h>
//#include <sofa/component/topology/PointData.h>
//#include <sofa/component/topology/EdgeData.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>

#include "QuadraticMeshTopology.h"
#include "QuadraticBaseMeshTopology.h"


namespace sofa
{

namespace component
{

namespace mass
{

using namespace sofa::component::topology;

//---------------------------------------------------------------------------
// This class is used to do some internal evaluations, either replacing or extending the class, used for CUDA!
//---------------------------------------------------------------------------
template< class DataTypes, class TMassType >
class QuadraticMeshMatrixMassInternalData
{
};


template <class DataTypes, class TMassType>
class QuadraticMeshMatrixMass : public core::behavior::Mass<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE2(QuadraticMeshMatrixMass,DataTypes,TMassType), SOFA_TEMPLATE(core::behavior::Mass,DataTypes));

    typedef core::behavior::Mass<DataTypes> Inherited;
    typedef typename DataTypes::VecCoord                    VecCoord;
   typedef typename DataTypes::VecDeriv                    VecDeriv;
   typedef typename DataTypes::Coord                       Coord;
   typedef typename DataTypes::Deriv                       Deriv;
   typedef typename DataTypes::Real                        Real;
   typedef TMassType                                       MassType;
   typedef helper::vector<Deriv> GravityForceVectorType;
   typedef Mat<10,10,Real> ElementMassMatrixType;
   typedef helper::vector<ElementMassMatrixType> ElementMassMatrixVectorType;
   typedef helper::vector<Real> DiagonalForceVectorType;
    
   typedef typename sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra QuadraticTetraType;
    /// the mass density used to compute the mass from a mesh topology and geometry
    Data< Real >         m_massDensity;
    Data<bool> m_isDiagonal;

    /// to display the center of gravity of the system
    //Data< bool >         showCenterOfGravity;
    //Data< float >        showAxisSize;


public:

	sofa::core::topology::QuadraticBaseMeshTopology* m_Topology;
	

	QuadraticMeshMatrixMass();
	
   ~QuadraticMeshMatrixMass(){};

	void clear();

	virtual void reinit();
	virtual void init();
        


    
	Real getMassDensity() const {
	  return m_massDensity.getValue();
	}
    
	void setMassDensity(Real m) {
	  m_massDensity.setValue(m);
	}


	// handle topological changes
	virtual void handleTopologyChange();

    
	// -- Mass interface
	void addMDx(VecDeriv& f, const VecDeriv& dx, double factor = 1.0);

	void accFromF(VecDeriv& a, const VecDeriv& f); // This function can't be used as it use M^-1

	void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);

        double getKineticEnergy(const VecDeriv& v)const {return 0;} ;  ///< vMv/2 using dof->getV()

        double getPotentialEnergy(const VecCoord& x) const;   ///< Mgx potential in a uniform gravity field, null at origin

	void addGravityToV(double dt/*, defaulttype::BaseVector& v*/);

   bool isDiagonal(){return false;}

        

	/// Add Mass contribution to global Matrix assembling
	void addMToMatrix(defaulttype::BaseMatrix * mat, double mFact, unsigned int &offset);

	double getElementMass(unsigned int index) const;
	void getElementMass(unsigned int index, defaulttype::BaseMatrix *m) const;




protected:

	//---------------------------------------------------------------------------
	// Handling internal data for evaluations, i.e. with CUDA.
	//---------------------------------------------------------------------------
	QuadraticMeshMatrixMassInternalData<DataTypes, TMassType> *InternalData;
	friend class QuadraticMeshMatrixMassInternalData<DataTypes, TMassType>;

	void AssembleElementMatrices();
	void BuildElementMatrix(ElementMassMatrixType & elementMatrix, unsigned int elementID);
	void AssembleGravityForceVector();
	void AssembleDiagonalForceVector();
	void BuildElementGravityForceVector(helper::fixed_array<Deriv,10>  & elementGravityVector, unsigned int elementID);
	void InitializeShapeFunctionDerivatives(unsigned int numberOfIntegrationPoints);
	void InitializeShapeFunctions(unsigned int numberOfIntegrationPoints);
	void InitializeIntegrationPointsAndWeights(int numberOfIntegrationPoints);
	double GetJacobianDeterminant(helper::fixed_array<Coord,10> coords, int indexOfIntegrationPoint);
	//double ShapeFunctionDerivativeValue(double x1, double x2, double x3, int p);
	//double ShapeFunctionValue(double x1, double x2, double x3, int p);
	void WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename);
	void WriteElementMatrixToCVSFile(Mat<10,10,Real>  mat,char* filename, int elementNumber);
	void WriteElementVectorToMatFile(helper::fixed_array<Deriv,10>& vec,char* filename,int elementNumber);
	void WriteVectorToMatFile(GravityForceVectorType& vec,char* filename);

	Data<ElementMassMatrixVectorType> m_ElementMassMatrixVector;
	Data<GravityForceVectorType> m_GravityForceVector;
	helper::vector<Mat<10,3,Real> > m_ShapeFunctionDerivatives;
	helper::vector<helper::fixed_array<Real,10> > m_ShapeFunctions;
	unsigned int numberOfIntegrationPoints;

	DiagonalForceVectorType m_DiagonalForceVector;

	double m_IntPoints[33];
	double m_IntWeights[11];


};


} // namespace mass

} // namespace component

} // namespace sofa

#endif
