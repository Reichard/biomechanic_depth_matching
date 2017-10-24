/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
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
#ifndef SOFA_COMPONENT_FORCEFIELD_HEXAHEDRONANDMASSFEMFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_HEXAHEDRONANDMASSFEMFORCEFIELD_H
#include "config.h"


#include "HexahedronFEMForceField.h"
#include <sofa/core/behavior/Mass.h>
#include <sofa/helper/gl/template.h>
namespace sofa
{

namespace component
{

namespace forcefield
{

/** Compute Finite Element forces based on hexahedral elements including continuum mass matrices
*/
template<class DataTypes>
class HexahedronFEMForceFieldAndMass : virtual public core::behavior::Mass<DataTypes>, virtual public HexahedronFEMForceField<DataTypes>
{
public:
    SOFA_CLASS2(SOFA_TEMPLATE(HexahedronFEMForceFieldAndMass,DataTypes), SOFA_TEMPLATE(sofa::core::behavior::Mass,DataTypes), SOFA_TEMPLATE(HexahedronFEMForceField,DataTypes));

    typedef HexahedronFEMForceField<DataTypes> HexahedronFEMForceFieldT;
    typedef sofa::core::behavior::Mass<DataTypes> MassT;

    typedef typename DataTypes::Real        Real        ;
    typedef typename DataTypes::Coord       Coord       ;
    typedef typename DataTypes::Deriv       Deriv       ;
    typedef typename DataTypes::VecCoord    VecCoord    ;
    typedef typename DataTypes::VecDeriv    VecDeriv    ;
    typedef typename DataTypes::VecReal     VecReal     ;
    typedef VecCoord Vector;

    typedef core::objectmodel::Data<VecDeriv> DataVecDeriv;
    typedef core::objectmodel::Data<VecCoord> DataVecCoord;

    typedef typename HexahedronFEMForceFieldT::Mat33 Mat33;
    typedef typename HexahedronFEMForceFieldT::Displacement Displacement;
    typedef typename HexahedronFEMForceFieldT::VecElement VecElement;
    typedef typename HexahedronFEMForceFieldT::VecElementStiffness VecElementMass;
    typedef typename HexahedronFEMForceFieldT::ElementStiffness ElementMass;
    typedef helper::vector<Real> MassVector;

protected:
    HexahedronFEMForceFieldAndMass();
public:

    virtual void init( );
    virtual void reinit( );

    virtual void computeElementMasses( ); ///< compute the mass matrices
    virtual void computeElementMass( ElementMass &Mass, const helper::fixed_array<Coord,8> &nodes, const int elementIndice, SReal stiffnessFactor=1.0); ///< compute the mass matrix of an element
    Real integrateMass( int signx, int signy, int signz, Real l0, Real l1, Real l2 );

    virtual std::string getTemplateName() const;

    // -- Mass interface
    virtual  void addMDx(const core::MechanicalParams* mparams, DataVecDeriv& f, const DataVecDeriv& dx, SReal factor);

    virtual void addMToMatrix(const core::MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix);

    using HexahedronFEMForceFieldT::addKToMatrix;
    using core::behavior::Mass<DataTypes>::addKToMatrix;
    void addKToMatrix(const core::MechanicalParams* mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix)
    {
        HexahedronFEMForceFieldT::addKToMatrix(mparams, matrix);
    }

    virtual  void accFromF(const core::MechanicalParams* mparams, DataVecDeriv& a, const DataVecDeriv& f);

    virtual  void addForce(const core::MechanicalParams* mparams, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v);

    virtual SReal getKineticEnergy(const core::MechanicalParams*, const DataVecDeriv& /*v*/ ) const  ///< vMv/2 using dof->getV()
    {
        serr << "HexahedronFEMForceFieldAndMass::getKineticEnergy() not implemented" << sendl;
        return 0.0;
    }

    virtual SReal getPotentialEnergy(const core::MechanicalParams* /*mparams*/, const DataVecCoord&  /* x */) const
    {
        serr << "HexahedronFEMForceFieldAndMass::getPotentialEnergy() not implemented" << sendl;
        return 0.0;
    }

    virtual SReal getPotentialEnergy(const core::MechanicalParams* /*mparams*/) const
    {
        serr << "HexahedronFEMForceFieldAndMass::getPotentialEnergy() not implemented" << sendl;
        return 0.0;
    }

    virtual void addDForce(const core::MechanicalParams* mparams, DataVecDeriv& df, const DataVecDeriv& dx);

    virtual void addGravityToV(const core::MechanicalParams* mparams, DataVecDeriv& d_v);

    SReal getElementMass(unsigned int index) const;
    // visual model

    virtual void draw(const core::visual::VisualParams* vparams);

    virtual void initTextures() { }

    virtual void update() { }



    void setDensity(Real d) {_density.setValue( d );}
    Real getDensity() {return _density.getValue();}



protected :

    Data<VecElementMass> _elementMasses; ///< mass matrices per element
    Data<Real> _density;
    Data<bool> _lumpedMass;

    MassVector _particleMasses; ///< masses per particle in order to compute gravity
    helper::vector<Coord> _lumpedMasses; ///< masses per particle computed by lumping mass matrices


};

#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_FORCEFIELD_HEXAHEDRONFEMFORCEFIELDANDMASS_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_SIMPLE_FEM_API HexahedronFEMForceFieldAndMass< defaulttype::Vec3dTypes >;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_SIMPLE_FEM_API HexahedronFEMForceFieldAndMass< defaulttype::Vec3fTypes >;
#endif
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_FORCEFIELD_HEXAHEDRONANDMASSFEMFORCEFIELD_H
