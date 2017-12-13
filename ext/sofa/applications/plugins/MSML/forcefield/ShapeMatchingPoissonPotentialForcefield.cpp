/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefield
  Author:    Tino Milschewski

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "ShapeMatchingPoissonPotentialForcefield.inl"
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


SOFA_DECL_CLASS(ShapeMatchingPoissonPotentialForcefield)

// Register in the Factory
int ShapeMatchingPoissonPotentialForcefieldClass = core::RegisterObject("ShapeMatchingPoissonPotentialForcefield")
.add< ShapeMatchingPoissonPotentialForcefield<Vec3dTypes> >()

;


template class ShapeMatchingPoissonPotentialForcefield<Vec3dTypes>;



} // namespace forcefield

} // namespace component

} // namespace sofa
