/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingElectricPotentialForcefield
  Author:    Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "ShapeMatchingElectricPotentialForcefield.inl"
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


SOFA_DECL_CLASS(ShapeMatchingElectricPotentialForcefield)

// Register in the Factory
int ShapeMatchingElectricPotentialForcefieldClass = core::RegisterObject("Corotational FEM Linear Tetrahedral finite elements")
.add< ShapeMatchingElectricPotentialForcefield<Vec3dTypes> >()

;


template class ShapeMatchingElectricPotentialForcefield<Vec3dTypes>;



} // namespace forcefield

} // namespace component

} // namespace sofa
