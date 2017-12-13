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
#include <sofa/helper/system/config.h>
#include <PersistentContact/config.h>


namespace sofa
{

namespace component
{

//Here are just several convenient functions to help user to know what contains the plugin

extern "C" {
    SOFA_PERSISTENTCONTACT_API void initExternalModule();
    SOFA_PERSISTENTCONTACT_API const char* getModuleName();
    SOFA_PERSISTENTCONTACT_API const char* getModuleVersion();
    SOFA_PERSISTENTCONTACT_API const char* getModuleLicense();
    SOFA_PERSISTENTCONTACT_API const char* getModuleDescription();
    SOFA_PERSISTENTCONTACT_API const char* getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }
}

const char* getModuleName()
{
    return "Plugin Persistent Contact";
}

const char* getModuleVersion()
{
    return "0.1";
}

const char* getModuleLicense()
{
    return "LGPL";
}

const char* getModuleDescription()
{
    return "Persistent contact management for friction contacts";
}

const char* getModuleComponentList()
{
    return "PersistentContactBarycentricMapping, PersistentContactMapping, PersistentFrictionContact, PersistentContactRigidMapping, PersistentUnilateralInteractionConstraint";
}

} // namespace component

} // namespace sofa

SOFA_LINK_CLASS(PersistentContactBarycentricMapping)
SOFA_LINK_CLASS(PersistentContactRigidMapping)
SOFA_LINK_CLASS(PersistentFrictionContact)
SOFA_LINK_CLASS(PersistentUnilateralInteractionConstraint)
