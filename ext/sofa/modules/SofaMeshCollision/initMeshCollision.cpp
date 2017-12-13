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
#include <sofa/helper/system/config.h>
#include <SofaMeshCollision/initMeshCollision.h>
#include <SofaMeshCollision/MeshNewProximityIntersection.h>

using namespace sofa::defaulttype;
using namespace sofa::core::collision;
using namespace sofa::component::collision;

namespace sofa
{

namespace component
{


void initMeshCollision()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }

    core::collision::IntersectorCreator<collision::NewProximityIntersection, collision::MeshNewProximityIntersection>* pMeshNewProximityIntersectors = new core::collision::IntersectorCreator<collision::NewProximityIntersection, collision::MeshNewProximityIntersection>("Mesh");
    (void) pMeshNewProximityIntersectors;
}

SOFA_LINK_CLASS(BarycentricPenalityContact)
SOFA_LINK_CLASS(BarycentricContactMapper)
SOFA_LINK_CLASS(IdentityContactMapper)
SOFA_LINK_CLASS(SubsetContactMapper)
SOFA_LINK_CLASS(MeshDiscreteIntersection)
SOFA_LINK_CLASS(MeshMinProximityIntersection)
SOFA_LINK_CLASS(MeshNewProximityIntersection)
SOFA_LINK_CLASS(Point)
SOFA_LINK_CLASS(Line)
SOFA_LINK_CLASS(Triangle)
SOFA_LINK_CLASS(LineLocalMinDistanceFilter)
SOFA_LINK_CLASS(PointLocalMinDistanceFilter)


} // namespace component

} // namespace sofa
