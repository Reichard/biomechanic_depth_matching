/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
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
// Author: Hadrien Courtecuisse
//
// Copyright: See COPYING file that comes with this distribution
#include <ExplicitPreconditionerOperator.inl>
#include <sofa/core/ObjectFactory.h>

namespace sofa {

namespace component {

namespace linearsolver {
//typedef double Real;

SOFA_DECL_CLASS(ExplicitPreconditionerOperator)

int ExplicitPreconditionerOperatorClass = core::RegisterObject("ExplicitPreconditionerOperator")
.add< ExplicitPreconditionerOperator<double > >(true) //for the cpu version
//.add< ExplicitPreconditionerOperator<GraphScatteredMatrix,GraphScatteredVector,double > >(true) //for the cpu version
//.add< ExplicitPreconditionerOperator< CompressedRowSparseMatrix<double>,FullVector<double> > >(true)
//.add< ExplicitPreconditionerOperator<SparseMatrix<double>, FullVector<double> > >()
// .add< ExplicitPreconditionerOperator<NewMatBandMatrix,NewMatVector> >()
// .add< ExplicitPreconditionerOperator<NewMatMatrix,NewMatVector> >()
// .add< ExplicitPreconditionerOperator<NewMatSymmetricMatrix,NewMatVector> >()
// .add< ExplicitPreconditionerOperator<NewMatSymmetricBandMatrix,NewMatVector> >()
//.add< ExplicitPreconditionerOperator<FullMatrix<double>, FullVector<double> > >()
.addAlias("ExplicitPreconditionerOperatorAlias")
;

template class  ExplicitPreconditionerOperator<double>;
//template class  ExplicitPreconditionerOperator<GraphScatteredMatrix,GraphScatteredVector,double>;
} // namespace linearsolver

} // namespace component

} // namespace sofa

