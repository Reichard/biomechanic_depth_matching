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
#ifndef SOFA_COMPONENT_COLLISION_TRIANGLEMODELINREGULARGRID_H
#define SOFA_COMPONENT_COLLISION_TRIANGLEMODELINREGULARGRID_H
#include "config.h"

#include <SofaMeshCollision/TriangleModel.h>
#include <sofa/core/topology/BaseMeshTopology.h>

namespace sofa
{

namespace component
{

namespace collision
{


class TriangleModelInRegularGrid : public TriangleModel
{
public:
    SOFA_CLASS(TriangleModelInRegularGrid, TriangleModel);

    virtual void init();
    virtual void computeBoundingTree ( int maxDepth=0 );

    sofa::core::topology::BaseMeshTopology* _topology;
    sofa::core::topology::BaseMeshTopology* _higher_topo;
    core::behavior::MechanicalState<defaulttype::Vec3Types>* _higher_mstate;

protected:
    TriangleModelInRegularGrid();
    ~TriangleModelInRegularGrid();
};

}

}

}

#endif
