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
#include <sofa/core/objectmodel/BaseNode.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/BaseAnimationLoop.h>
#include <sofa/core/behavior/OdeSolver.h>
#include <sofa/core/collision/Pipeline.h>
#include <sofa/core/visual/VisualLoop.h>
#include <iostream>
using std::cerr;
using std::endl;

namespace sofa
{

namespace core
{

namespace objectmodel
{

BaseNode::BaseNode()
{}

BaseNode::~BaseNode()
{}

BaseNode* BaseNode::getRoot() const
{
    BaseNode* firstParent = getFirstParent();
    if (!firstParent) return const_cast<BaseNode*>(this);
    else return firstParent->getRoot();
}

core::behavior::BaseAnimationLoop* BaseNode::getAnimationLoop() const
{
    return this->getContext()->get<core::behavior::BaseAnimationLoop>();
}

core::behavior::OdeSolver* BaseNode::getOdeSolver() const
{
    return this->getContext()->get<core::behavior::OdeSolver>();
}

core::collision::Pipeline* BaseNode::getCollisionPipeline() const
{
    return this->getContext()->get<core::collision::Pipeline>();
}

core::visual::VisualLoop* BaseNode::getVisualLoop() const
{
    return this->getContext()->get<core::visual::VisualLoop>();
}

/// Set the context of an object to this
void BaseNode::setObjectContext(BaseObject::SPtr obj)
{
    obj->l_context.set(this->getContext());
}

/// Reset the context of an object
void BaseNode::clearObjectContext(BaseObject::SPtr obj)
{
    if (obj->getContext() == this->getContext())
        obj->l_context.reset();
}

std::string BaseNode::getPathName() const {
    std::string str;
    Parents parents = getParents();
    if (!parents.empty())
    {
        // for the full path name, we arbitrarily take the first parent of the list...
        // no smarter choice without breaking the "Node" heritage
        str = parents[0]->getPathName();
        str += '/';
        str += getName();
    }

    return str;
}

} // namespace objectmodel

} // namespace core

} // namespace sofa
