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
*                              SOFA :: Simulation                             *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_SIMULATION_CONFIG_H
#define SOFA_SIMULATION_CONFIG_H

#include <sofa/config.h>

#define SOFA_HAVE_DAG

#ifdef SOFA_BUILD_SIMULATION_COMMON
#   define SOFA_TARGET SofaSimulationCommon
#	define SOFA_SIMULATION_COMMON_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_SIMULATION_COMMON_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SIMULATION_TREE
#   define SOFA_TARGET SofaSimulationTree
#	define SOFA_SIMULATION_TREE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_SIMULATION_TREE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_SIMULATION_GRAPH
#   define SOFA_TARGET SofaSimulationGraph
#	define SOFA_SIMULATION_GRAPH_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_SIMULATION_GRAPH_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif


// tinyxml is currently the default XML parser
#if !defined(SOFA_XML_PARSER_LIBXML) && !defined(SOFA_XML_PARSER_TINYXML)
#   define SOFA_XML_PARSER_TINYXML
#endif

#endif
