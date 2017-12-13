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
#include <PluginExample/config.h>

extern "C" {

SOFA_PLUGINEXAMPLE_API void initExternalModule()
{
    // Nothing to initialize
}

SOFA_PLUGINEXAMPLE_API const char* getModuleName()
{
    return "PluginExample";
}

SOFA_PLUGINEXAMPLE_API const char* getModuleVersion()
{
    return "0.2";
}

SOFA_PLUGINEXAMPLE_API const char* getModuleLicense()
{
    return "LGPL";
}

SOFA_PLUGINEXAMPLE_API const char* getModuleDescription()
{
    return "Simple example of a Sofa plugin";
}

SOFA_PLUGINEXAMPLE_API const char* getModuleComponentList()
{
    return "MyBehaviorModel, MyMappingPendulumInPlane, MyProjectiveConstraintSet";
}

}


SOFA_LINK_CLASS(MyBehaviorModel)
SOFA_LINK_CLASS(MyMappingPendulumInPlane)
SOFA_LINK_CLASS(MyProjectiveConstraintSet)
