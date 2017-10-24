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
*                              SOFA :: SofaAvanced                            *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFAADVANCED_CONFIG_H
#define SOFAADVANCED_CONFIG_H

#include <SofaCommon/config.h>

#define SOFA_HAVE_FLOWVR

#ifdef SOFA_BUILD_EULERIAN_FLUID
#  define SOFA_TARGET SofaEulerianFluid
#  define SOFA_EULERIAN_FLUID_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EULERIAN_FLUID_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SPH_FLUID
#  define SOFA_TARGET SofaSphFluid
#  define SOFA_SPH_FLUID_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_SPH_FLUID_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_VOLUMETRIC_DATA
#  define SOFA_TARGET SofaVolumetricData
#  define SOFA_VOLUMETRIC_DATA_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_VOLUMETRIC_DATA_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_NON_UNIFORM_FEM
#  define SOFA_TARGET SofaNonUniformFem
#  define SOFA_NON_UNIFORM_FEM_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_NON_UNIFORM_FEM_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_ADVANCED
#  define SOFA_TARGET SofaComponentAdvanced
#  define SOFA_COMPONENT_ADVANCED_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_ADVANCED_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif
