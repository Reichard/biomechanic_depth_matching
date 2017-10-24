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
*                              SOFA :: General                                *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFAGENERAL_CONFIG_H
#define SOFAGENERAL_CONFIG_H

#include <SofaCommon/config.h>

#define SOFA_HAVE_NEWMAT
#define SOFA_HAVE_CSPARSE
/* #undef SOFA_HAVE_TAUCS */
/* #undef SOFA_HAVE_PARDISO */
/* #undef SOFA_HAVE_METIS */

#ifdef SOFA_BUILD_VALIDATION
#  define SOFA_TARGET SofaValidation
#  define SOFA_VALIDATION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_VALIDATION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_EXPORTER
#  define SOFA_TARGET SofaExporter
#  define SOFA_EXPORTER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EXPORTER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_GRAPH_COMPONENT
#  define SOFA_TARGET SofaGraphComponent
#  define SOFA_GRAPH_COMPONENT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_GRAPH_COMPONENT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_TOPOLOGY_MAPPING
#  define SOFA_TARGET SofaTopologyMapping
#  define SOFA_TOPOLOGY_MAPPING_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_TOPOLOGY_MAPPING_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_BOUNDARY_CONDITION
#  define SOFA_TARGET SofaBoundaryCondition
#  define SOFA_BOUNDARY_CONDITION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_BOUNDARY_CONDITION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_COMPONENT_GENERAL
#  define SOFA_TARGET SofaComponentGeneral
#  define SOFA_COMPONENT_GENERAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_COMPONENT_GENERAL_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_ENGINE
#  define SOFA_TARGET SofaEngine
#  define SOFA_ENGINE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_ENGINE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_USER_INTERACTION
#  define SOFA_TARGET SofaUserInteraction
#  define SOFA_USER_INTERACTION_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_USER_INTERACTION_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_CONSTRAINT
#  define SOFA_TARGET SofaConstraint
#  define SOFA_CONSTRAINT_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_CONSTRAINT_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_HAPTICS
#  define SOFA_TARGET SofaHaptics
#  define SOFA_HAPTICS_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_HAPTICS_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_PRECONDITIONER
#  define SOFA_TARGET SofaPreconditioner
#  define SOFA_PRECONDITIONER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_PRECONDITIONER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_HAVE_NEWMAT
#  ifdef SOFA_BUILD_DENSE_SOLVER
#    define SOFA_TARGET SofaDenseSolver
#    define SOFA_DENSE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#  else
#    define SOFA_DENSE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#  endif
#endif

//#ifndef SOFA-MISC_NO_OPENGL
#  ifdef SOFA_BUILD_OPENGL_VISUAL
#    define SOFA_TARGET SofaOpenglVisual
#    define SOFA_OPENGL_VISUAL_API SOFA_EXPORT_DYNAMIC_LIBRARY
#  else
#    define SOFA_OPENGL_VISUAL_API SOFA_IMPORT_DYNAMIC_LIBRARY
#  endif
//#endif

#ifdef SOFA_BUILD_SPARSE_SOLVER
#  define SOFA_TARGET SofaSparseSolver
#  define SOFA_SPARSE_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_SPARSE_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_HAVE_TAUCS
#  ifdef SOFA_BUILD_TAUCS_SOLVER
#    define SOFA_TARGET SofaTaucsSolver
#    define SOFA_TAUCS_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#  else
#    define SOFA_TAUCS_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#  endif
#endif

#ifdef SOFA_BUILD_EIGEN2_SOLVER
#  define SOFA_TARGET SofaEigen2Solver
#  define SOFA_EIGEN2_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_EIGEN2_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_HAVE_PARDISO
#ifdef SOFA_BUILD_PARDISO_SOLVER
#  define SOFA_PARDISO_SOLVER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#  define SOFA_PARDISO_SOLVER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif
#endif

#endif
