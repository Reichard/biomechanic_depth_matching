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



// File automatically generated by "generateTypedef"


#ifndef SOFA_TYPEDEF__Miscellaneous_float_H
#define SOFA_TYPEDEF__Miscellaneous_float_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>




#include <SofaValidation/DevAngleCollisionMonitor.h>
#include <SofaValidation/DevTensionMonitor.h>
#include <SofaValidation/EvalPointsDistance.h>
#include <SofaValidation/EvalSurfaceDistance.h>
#include <SofaValidation/ExtraMonitor.h>
#include <SofaBaseMechanics/MappedObject.h>
#include <SofaValidation/Monitor.h>
#include <SofaSphFluid/SpatialGridContainer.h>



//---------------------------------------------------------------------------------------------
//Typedef for DevAngleCollisionMonitor
typedef sofa::component::misc::DevAngleCollisionMonitor<sofa::defaulttype::StdRigidTypes<3, float> > DevAngleCollisionMonitorRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for DevTensionMonitor
typedef sofa::component::misc::DevTensionMonitor<sofa::defaulttype::StdRigidTypes<3, float> > DevTensionMonitorRigid3f;



//---------------------------------------------------------------------------------------------
//Typedef for EvalPointsDistance
typedef sofa::component::misc::EvalPointsDistance<sofa::defaulttype::StdRigidTypes<3, float> > EvalPointsDistanceRigid3f;
typedef sofa::component::misc::EvalPointsDistance<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > EvalPointsDistance3f;



//---------------------------------------------------------------------------------------------
//Typedef for EvalSurfaceDistance
typedef sofa::component::misc::EvalSurfaceDistance<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > EvalSurfaceDistance3f;



//---------------------------------------------------------------------------------------------
//Typedef for ExtraMonitor
typedef sofa::component::misc::ExtraMonitor<sofa::defaulttype::StdRigidTypes<3, float> > ExtraMonitorRigid3f;
typedef sofa::component::misc::ExtraMonitor<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > ExtraMonitor3f;
typedef sofa::component::misc::ExtraMonitor<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > ExtraMonitor6f;



//---------------------------------------------------------------------------------------------
//Typedef for MappedObject
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdRigidTypes<2, float> > MappedObjectRigid2f;
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdRigidTypes<3, float> > MappedObjectRigid3f;
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > MappedObject1f;
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > MappedObject2f;
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > MappedObject3f;
typedef sofa::component::container::MappedObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > MappedObject6f;



//---------------------------------------------------------------------------------------------
//Typedef for Monitor
typedef sofa::component::misc::Monitor<sofa::defaulttype::StdRigidTypes<3, float> > MonitorRigid3f;
typedef sofa::component::misc::Monitor<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > Monitor3f;
typedef sofa::component::misc::Monitor<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > Monitor6f;



//---------------------------------------------------------------------------------------------
//Typedef for SpatialGridContainer
typedef sofa::component::container::SpatialGridContainer<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > SpatialGridContainer3f;





#ifdef SOFA_FLOAT
typedef DevAngleCollisionMonitorRigid3f DevAngleCollisionMonitorRigid3;
typedef DevTensionMonitorRigid3f DevTensionMonitorRigid3;
typedef EvalPointsDistanceRigid3f EvalPointsDistanceRigid3;
typedef EvalPointsDistance3f EvalPointsDistance3;
typedef EvalSurfaceDistance3f EvalSurfaceDistance3;
typedef ExtraMonitorRigid3f ExtraMonitorRigid3;
typedef ExtraMonitor3f ExtraMonitor3;
typedef ExtraMonitor6f ExtraMonitor6;
typedef MappedObjectRigid2f MappedObjectRigid2;
typedef MappedObjectRigid3f MappedObjectRigid3;
typedef MappedObject1f MappedObject1;
typedef MappedObject2f MappedObject2;
typedef MappedObject3f MappedObject3;
typedef MappedObject6f MappedObject6;
typedef MonitorRigid3f MonitorRigid3;
typedef Monitor3f Monitor3;
typedef Monitor6f Monitor6;
typedef SpatialGridContainer3f SpatialGridContainer3;
#endif

#endif