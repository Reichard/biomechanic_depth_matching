/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, LinearTetrahedralCorotationalFEMForceField
  Author:    Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "LinearTetrahedralCorotationalFEMForceField.inl"
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/component/container/MechanicalObject.h>
#include <sofa/core/ObjectFactory.h>



namespace sofa
{

namespace component
{

namespace forcefield
{

using namespace sofa::defaulttype;


SOFA_DECL_CLASS(LinearTetrahedralCorotationalFEMForceField)

// Register in the Factory
int LinearTetrahedralCorotationalFEMForceFieldClass = core::RegisterObject("Corotational FEM Linear Tetrahedral finite elements")
#ifndef SOFA_FLOAT
.add< LinearTetrahedralCorotationalFEMForceField<Vec3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
.add< LinearTetrahedralCorotationalFEMForceField<Vec3fTypes> >()
#endif
;

#ifndef SOFA_FLOAT
template class LinearTetrahedralCorotationalFEMForceField<Vec3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class LinearTetrahedralCorotationalFEMForceField<Vec3fTypes>;
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa
