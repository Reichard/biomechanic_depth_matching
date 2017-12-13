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
#include <sofa/helper/system/config.h>
#include <SofaComponentGeneral/initComponentGeneral.h>
#include <SofaValidation/initValidation.h>
#include <SofaExporter/initExporter.h>
#include <SofaEngine/initEngine.h>
#include <SofaGraphComponent/initGraphComponent.h>
#include <SofaTopologyMapping/initTopologyMapping.h>
#include <SofaBoundaryCondition/initBoundaryCondition.h>
#include <SofaUserInteraction/initUserInteraction.h>
#include <SofaConstraint/initConstraint.h>
#include <SofaHaptics/initHaptics.h>

#ifdef SOFA_HAVE_NEWMAT
#include <SofaDenseSolver/initDenseSolver.h>
#endif
#ifdef SOFA_HAVE_CSPARSE
#include <SofaSparseSolver/initSparseSolver.h>
#endif
#ifdef SOFA_HAVE_TAUCS
#include <SofaTaucsSolver/initTaucsSolver.h>
#endif
#include <SofaEigen2Solver/initEigen2Solver.h>
#ifdef SOFA_HAVE_PARDISO
#include <SofaPardisoSolver/initPardisoSolver.h>
#endif

#include <SofaPreconditioner/initPreconditioner.h>
#include <SofaOpenglVisual/initOpenGLVisual.h>


namespace sofa
{

namespace component
{


void initComponentGeneral()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }

    initValidation();
    initExporter();
    initEngine();
    initGraphComponent();
    initTopologyMapping();
    initBoundaryCondition();
    initUserInteraction();
    initConstraint();
    initHaptics();
#ifdef SOFA_HAVE_NEWMAT
    initDenseSolver();
#endif
#ifdef SOFA_HAVE_CSPARSE
    initSparseSolver();
#endif
#ifdef SOFA_HAVE_TAUCS
    initTaucsSolver();
#endif
initEigen2Solver();
#ifdef SOFA_HAVE_PARDISO
    initPardisoSolver();
#endif
    initPreconditioner();
#ifndef SOFA_NO_OPENGL
    initOpenGLVisual();
#endif
}


} // namespace component

} // namespace sofa
