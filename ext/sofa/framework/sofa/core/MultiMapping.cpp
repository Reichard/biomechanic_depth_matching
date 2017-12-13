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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define SOFA_CORE_MULTIMAPPING_CPP
#include <sofa/core/MultiMapping.inl>
#include <sofa/defaulttype/Vec3Types.h>
#include <sofa/defaulttype/RigidTypes.h>


namespace sofa
{

namespace core
{

using namespace sofa::defaulttype;
using namespace core::behavior;

#ifndef SOFA_FLOAT
template class SOFA_CORE_API MultiMapping< Vec1dTypes, Vec1dTypes >;
template class SOFA_CORE_API MultiMapping< Vec2dTypes, Vec1dTypes >;
template class SOFA_CORE_API MultiMapping< Vec3dTypes, Vec3dTypes >;
template class SOFA_CORE_API MultiMapping< Vec3dTypes, Vec1dTypes >;
template class SOFA_CORE_API MultiMapping< Vec6dTypes, Vec1dTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Vec1dTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Vec3dTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Vec6dTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Rigid3dTypes >;
#endif

#ifndef SOFA_DOUBLE
template class SOFA_CORE_API MultiMapping< Vec1fTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Vec2fTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Vec3fTypes, Vec3fTypes >;
template class SOFA_CORE_API MultiMapping< Vec3fTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Vec6fTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Vec3fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Vec6fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Rigid3fTypes >;
#endif

#ifndef SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_CORE_API MultiMapping< Vec1dTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Vec1fTypes, Vec1dTypes > ;
template class SOFA_CORE_API MultiMapping< Vec3dTypes, Vec3fTypes >;
template class SOFA_CORE_API MultiMapping< Vec3fTypes, Vec3dTypes > ;
template class SOFA_CORE_API MultiMapping< Vec3dTypes, Vec1fTypes >;
template class SOFA_CORE_API MultiMapping< Vec3fTypes, Vec1dTypes > ;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Vec3fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Vec3dTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3dTypes, Vec6fTypes >;
template class SOFA_CORE_API MultiMapping< Rigid3fTypes, Vec6dTypes >;
#endif
#endif

} // namespace core

} // namespace sofa
