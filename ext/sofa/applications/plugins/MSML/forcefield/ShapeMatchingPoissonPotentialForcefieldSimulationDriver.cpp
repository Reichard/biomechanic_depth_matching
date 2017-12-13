/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefieldSimulationDriver
  Author:    Tino Milschewski

=========================================================================*/

#include "ShapeMatchingPoissonPotentialForcefieldSimulationDriver.inl"
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/component/container/MechanicalObject.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace collision
{

SOFA_DECL_CLASS(ShapeMatchingPoissonPotentialForcefieldSimulationDriver)

// Register in the Factory
int ShapeMatchingPoissonPotentialForcefieldSimulationDriverClass = core::RegisterObject("SimulationDriver for ShapeMatchingPoissonPotentialForcefield")
.add< ShapeMatchingPoissonPotentialForcefieldSimulationDriver<Vec3dTypes> >();

template class ShapeMatchingPoissonPotentialForcefieldSimulationDriver<Vec3dTypes>;

}

}

}
