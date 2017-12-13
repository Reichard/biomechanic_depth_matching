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
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "MyProjectiveConstraintSet.inl"

#include <sofa/core/ObjectFactory.h>


namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{

using namespace sofa::defaulttype;


SOFA_DECL_CLASS(MyProjectiveConstraintSet)

int MyProjectiveConstraintSetClass = core::RegisterObject("just an example of templated component")
#ifndef SOFA_FLOAT
    .add< MyProjectiveConstraintSet<Vec3dTypes> >()
    .add< MyProjectiveConstraintSet<Vec1dTypes> >()
    .add< MyProjectiveConstraintSet<Rigid3dTypes> >()
#endif
#ifndef SOFA_DOUBLE
    .add< MyProjectiveConstraintSet<Vec3fTypes> >()
    .add< MyProjectiveConstraintSet<Rigid3fTypes> >()
#endif
    ;

#ifndef SOFA_FLOAT
template class MyProjectiveConstraintSet<Rigid3dTypes>;
template class MyProjectiveConstraintSet<Vec3dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class MyProjectiveConstraintSet<Rigid3fTypes>;
template class MyProjectiveConstraintSet<Vec3fTypes>;
#endif


} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa
