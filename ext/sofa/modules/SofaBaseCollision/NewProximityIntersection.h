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
#ifndef SOFA_COMPONENT_COLLISION_NEWPROXIMITYINTERSECTION_H
#define SOFA_COMPONENT_COLLISION_NEWPROXIMITYINTERSECTION_H
#include "config.h"

#include <SofaBaseCollision/BaseProximityIntersection.h>
#include <sofa/helper/FnDispatcher.h>
#include <SofaBaseCollision/CapsuleModel.h>
#include <SofaBaseCollision/SphereModel.h>
#include <SofaBaseCollision/CubeModel.h>
#include <SofaBaseCollision/CapsuleIntTool.h>
#include <SofaBaseCollision/OBBModel.h>
#include <SofaBaseCollision/OBBIntTool.h>

namespace sofa
{

namespace component
{

namespace collision
{

class SOFA_BASE_COLLISION_API NewProximityIntersection : public BaseProximityIntersection
{
public:
    SOFA_CLASS(NewProximityIntersection,BaseProximityIntersection);

    Data<bool> useLineLine;
protected:
    NewProximityIntersection();
public:

    typedef core::collision::IntersectorFactory<NewProximityIntersection> IntersectorFactory;

    virtual void init();

    static inline int doIntersectionPointPoint(SReal dist2, const defaulttype::Vector3& p, const defaulttype::Vector3& q, OutputVector* contacts, int id);

};

} // namespace collision

} // namespace component

namespace core
{
namespace collision
{
#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_COLLISION_NEWPROXIMITYINTERSECTION_CPP)
extern template class SOFA_BASE_COLLISION_API IntersectorFactory<component::collision::NewProximityIntersection>;
#endif
}
}

} // namespace sofa

#endif
