/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, LinearTetrahedralCorotationalFEMForceField
  Author:    Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#define SOFA_COMPONENT_MASS_LinearMeshMatrixMass_CPP
#include "LinearMeshMatrixMass.inl"
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/gl/Axis.h>



namespace sofa
{

namespace component
{

namespace mass
{

using namespace sofa::defaulttype;

  SOFA_DECL_CLASS(LinearMeshMatrixMass)

  // Register in the Factory
  int LinearMeshMatrixMassClass = core::RegisterObject("LinearMass: Define a specific mass for each particle")
#ifndef SOFA_FLOAT
    .add< LinearMeshMatrixMass<Vec3dTypes,double> >()
    .add< LinearMeshMatrixMass<Vec2dTypes,double> >()
    .add< LinearMeshMatrixMass<Vec1dTypes,double> >()
#endif
#ifndef SOFA_DOUBLE
    .add< LinearMeshMatrixMass<Vec3fTypes,float> >()
    .add< LinearMeshMatrixMass<Vec2fTypes,float> >()
    .add< LinearMeshMatrixMass<Vec1fTypes,float> >()
#endif
    ;

//#ifndef SOFA_FLOAT
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec3dTypes,double>;
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec2dTypes,double>;
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec1dTypes,double>;
//#endif
//#ifndef SOFA_DOUBLE
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec3fTypes,float>;
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec2fTypes,float>;
//  template class SOFA_COMPONENT_MASS_API LinearMeshMatrixMass<Vec1fTypes,float>;
//#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

