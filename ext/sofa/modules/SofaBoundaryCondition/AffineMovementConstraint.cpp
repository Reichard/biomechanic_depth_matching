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
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_AFFINEMOVEMENTCONSTRAINT_CPP

#include <SofaBoundaryCondition/AffineMovementConstraint.inl>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>

#include <sofa/simulation/common/Node.h>

namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

//declaration of the class, for the factory
SOFA_DECL_CLASS(AffineMovementConstraint)


int AffineMovementConstraintClass = core::RegisterObject("bilinear constraint")
#ifndef SOFA_FLOAT
        .add< AffineMovementConstraint<defaulttype::Vec3dTypes> >()
        .add< AffineMovementConstraint<defaulttype::Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
        .add< AffineMovementConstraint<defaulttype::Vec3fTypes> >()
        .add< AffineMovementConstraint<defaulttype::Rigid3fTypes> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_BOUNDARY_CONDITION_API AffineMovementConstraint<defaulttype::Vec3dTypes>;
template class SOFA_BOUNDARY_CONDITION_API AffineMovementConstraint<defaulttype::Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_BOUNDARY_CONDITION_API AffineMovementConstraint<defaulttype::Vec3fTypes>;
template class SOFA_BOUNDARY_CONDITION_API AffineMovementConstraint<defaulttype::Rigid3fTypes>;
#endif

} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa
