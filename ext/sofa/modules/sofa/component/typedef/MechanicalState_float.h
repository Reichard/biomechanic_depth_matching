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


#ifndef SOFA_TYPEDEF_MechanicalState_float_H
#define SOFA_TYPEDEF_MechanicalState_float_H

//Default files containing the declaration of the vector type
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/Mat.h>




#include <SofaBaseMechanics/MechanicalObject.h>



//---------------------------------------------------------------------------------------------
//Typedef for MechanicalObject
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdRigidTypes<2, float> > MechanicalObjectRigid2f;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdRigidTypes<3, float> > MechanicalObjectRigid3f;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<1, float>, sofa::defaulttype::Vec<1, float>, float> > MechanicalObject1f;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<2, float>, sofa::defaulttype::Vec<2, float>, float> > MechanicalObject2f;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, float>, sofa::defaulttype::Vec<3, float>, float> > MechanicalObject3f;
typedef sofa::component::container::MechanicalObject<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<6, float>, sofa::defaulttype::Vec<6, float>, float> > MechanicalObject6f;





#ifdef SOFA_FLOAT
typedef MechanicalObjectRigid2f MechanicalObjectRigid2;
typedef MechanicalObjectRigid3f MechanicalObjectRigid3;
typedef MechanicalObject1f MechanicalObject1;
typedef MechanicalObject2f MechanicalObject2;
typedef MechanicalObject3f MechanicalObject3;
typedef MechanicalObject6f MechanicalObject6;
#endif

#endif