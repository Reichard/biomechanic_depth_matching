
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
*                              SOFA :: Plugins                                *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "PythonMacros.h"
#include "PythonScriptEvent.h"

namespace sofa
{

namespace core
{

namespace objectmodel
{

SOFA_EVENT_CPP( PythonScriptEvent )

PythonScriptEvent::PythonScriptEvent(sofa::simulation::Node::SPtr sender, const char* eventName, PyObject* userData)
    : sofa::core::objectmodel::ScriptEvent(sender,eventName)
    , m_userData(userData)
{
    Py_INCREF(userData);
}

PythonScriptEvent::~PythonScriptEvent()
{
    Py_DECREF(m_userData);
}

} // namespace objectmodel

} // namespace core

} // namespace sofa
