///******************************************************************************
//*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
//*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
//*                                                                             *
//* This library is free software; you can redistribute it and/or modify it     *
//* under the terms of the GNU Lesser General Public License as published by    *
//* the Free Software Foundation; either version 2.1 of the License, or (at     *
//* your option) any later version.                                             *
//*                                                                             *
//* This library is distributed in the hope that it will be useful, but WITHOUT *
//* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
//* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
//* for more details.                                                           *
//*                                                                             *
//* You should have received a copy of the GNU Lesser General Public License    *
//* along with this library; if not, write to the Free Software Foundation,     *
//* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
//*******************************************************************************
//*                               SOFA :: Modules                               *
//*                                                                             *
//* Authors: The SOFA Team and external contributors (see Authors.txt)          *
//*                                                                             *
//* Contact information: contact@sofa-framework.org                             *
//******************************************************************************/
//// Author: Hadrien Courtecuisse
////
// Copyright: See COPYING file that comes with this distribution
#include "ParalutionSolver.h"
#include "ExternalTypesUtils.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/AdvancedTimer.h>

namespace sofa {

namespace component {

namespace linearsolver {

using std::cerr;
using std::endl;
using namespace paralution;

template<class TMatrix, class TVector>
ParalutionSolver<TMatrix,TVector>::ParalutionSolver()
:
  f_ordering( initData(&f_ordering,2,"ordering","ose ordering 0=identity/1=tree/2=metis/3=natural/4=genmmd/5=md/6=mmd/7=amd") )
, f_dropTol( initData(&f_dropTol,(double) 0.0,"ic_dropTol","Drop tolerance use for incomplete factorization") )
, f_modified_flag( initData(&f_modified_flag,true,"ic_modifiedFlag","Modified ICC : maintains rowsums") )
{

	isInitialized = false;
}

template<class TMatrix, class TVector>
void ParalutionSolver<TMatrix,TVector>::invert(Matrix& M) {
    M.compress();

    ParalutionSolverInvertData * data = (ParalutionSolverInvertData *) this->getMatrixInvertData(&M);

    Matrix Mfiltered;
    ExternalTypesUtils::copyCRSMatrix(M, Mfiltered);

    //fill in matrix
    ExternalTypesUtils::initParalutionMatrix(Mfiltered, data->mat_paralution);

    data->precond_paralution.Clear();
    data->solver_paralution.Clear();
    data->solver_paralution.SetOperator(data->mat_paralution);
    data->solver_paralution.SetPreconditioner(data->precond_paralution);

    //set max iterations
    data->solver_paralution.SetRelaxation(1.3);
    data->solver_paralution.InitMaxIter(10000);
    data->solver_paralution.RecordResidualHistory();

    data->solver_paralution.Build();
}

template<class TMatrix, class TVector>
void ParalutionSolver<TMatrix,TVector>::solve (Matrix& M, Vector& z, Vector& r) {
    ParalutionSolverInvertData * data = (ParalutionSolverInvertData *) this->getMatrixInvertData(&M);
    LocalVector<Real> b_paralution;
    LocalVector<Real> x_paralution;

    ExternalTypesUtils::initParalutionVector(z, x_paralution);
    ExternalTypesUtils::initParalutionVector(r, b_paralution);

    b_paralution.info();
    x_paralution.info();
    data->mat_paralution.info();

	sofa::helper::AdvancedTimer::stepBegin("ParalutionSolver::Solve");
    data->solver_paralution.Solve(b_paralution, &x_paralution);
    data->solver_paralution.RecordHistory("/tmp/Residuals.txt");
    sofa::helper::AdvancedTimer::stepEnd("ParalutionSolver::Solve");

    ExternalTypesUtils::destroyParalutionVector(b_paralution);
    ExternalTypesUtils::destroyParalutionVector(x_paralution);
}

template class sofa::component::linearsolver::ParalutionSolver< sofa::component::linearsolver::CompressedRowSparseMatrix<double>,sofa::component::linearsolver::FullVector<double> >;
template class sofa::component::linearsolver::ParalutionSolver< sofa::component::linearsolver::CompressedRowSparseMatrix<float>,sofa::component::linearsolver::FullVector<float> >;

SOFA_DECL_CLASS(ParalutionSolver)

int ParalutionSolverClass = core::RegisterObject("Awesome solvers from the paralution library")
.add< ParalutionSolver< CompressedRowSparseMatrix<double>,FullVector<double> > >(true)
.add< ParalutionSolver< CompressedRowSparseMatrix<float>,FullVector<float> > >(false)
//.add< ParalutionSolver< CompressedRowSparseMatrix<defaulttype::Mat<3,3,double> >,FullVector<double> > >(true)
;

} // namespace linearsolver

} // namespace component

} // namespace sofa

