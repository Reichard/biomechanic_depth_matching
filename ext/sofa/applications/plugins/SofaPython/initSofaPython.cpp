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
#include "SceneLoaderPY.h"
#include <SofaPython/config.h>


extern "C" {

SOFA_SOFAPYTHON_API void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;        
    }
}

SOFA_SOFAPYTHON_API const char* getModuleName()
{
    return "SofaPython";
}

SOFA_SOFAPYTHON_API const char* getModuleVersion()
{
    return SOFAPYTHON_VERSION_STR;
}

SOFA_SOFAPYTHON_API const char* getModuleLicense()
{
    return "LGPL";
}

SOFA_SOFAPYTHON_API const char* getModuleDescription()
{
    return "Imbeds Python scripts in Sofa";
}

SOFA_SOFAPYTHON_API const char* getModuleComponentList()
{
    /// string containing the names of the classes provided by the plugin
    return "PythonScriptController";
}

}


/// Use the SOFA_LINK_CLASS macro for each class, to enable linking on all platforms
SOFA_LINK_CLASS(PythonController)


// register the loader in the factory
static sofa::simulation::SceneLoader* loaderPY = sofa::simulation::SceneLoaderFactory::getInstance()->addEntry(new sofa::simulation::SceneLoaderPY());
