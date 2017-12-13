/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
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
#define SOFA_COMPONENT_MAPPING_MapperQuadraticTetrahedronSetTopology_CPP
#include <MapperQuadraticTetrahedronSetTopology.inl>

#include <sofa/defaulttype/VecTypes.h>

#include <sofa/core/ObjectFactory.h>

namespace sofa
{

namespace component
{

namespace mapping
{

using namespace sofa::defaulttype;


// Register in the Factory
int MapperQuadraticTetrahedronSetTopologyClass = core::RegisterObject("Better mapping")
.add< MapperQuadraticTetrahedronSetTopology< Vec3dTypes, Vec3dTypes > >()
//.add< MapperQuadraticTetrahedronSetTopology< Vec3dTypes, ExtVec3fTypes > >()
//.add< MapperQuadraticTetrahedronSetTopology< Vec3fTypes, Vec3fTypes > >()
//.add< MapperQuadraticTetrahedronSetTopology< Vec3fTypes, ExtVec3fTypes > >()

//#ifndef SOFA_FLOAT
//#ifndef SOFA_DOUBLE
//.add< MapperQuadraticTetrahedronSetTopology< Vec3fTypes, Vec3dTypes > >()
//.add< MapperQuadraticTetrahedronSetTopology< Vec3dTypes, Vec3fTypes > >()
//#endif
//#endif
;


template class SOFA_BASE_MECHANICS_API MapperQuadraticTetrahedronSetTopology< Vec3dTypes, Vec3dTypes >;
//template class SOFA_BASE_MECHANICS_API MapperQuadraticTetrahedronSetTopology< Vec3dTypes, ExtVec3fTypes >;
//template class SOFA_BASE_MECHANICS_API MapperQuadraticTetrahedronSetTopology< Vec3fTypes, Vec3fTypes >;
//template class SOFA_BASE_MECHANICS_API MapperQuadraticTetrahedronSetTopology< Vec3fTypes, ExtVec3fTypes >;


//#endif

SOFA_DECL_CLASS(MapperQuadraticTetrahedronSetTopology)

} // namespace mapping

} // namespace component

} // namespace sofa
