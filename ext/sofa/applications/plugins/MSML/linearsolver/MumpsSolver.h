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
#ifndef SOFA_COMPONENT_LINEARSOLVER_MumpsSolver_H
#define SOFA_COMPONENT_LINEARSOLVER_MumpsSolver_H

#include <sofa/core/behavior/LinearSolver.h>
#include <SofaBaseLinearSolver/MatrixLinearSolver.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <SofaBaseLinearSolver/SparseMatrix.h>
#include <SofaBaseLinearSolver/FullMatrix.h>
#include <sofa/helper/map.h>
#include <math.h>
#include <SofaBaseLinearSolver/CompressedRowSparseMatrix.h>

#include <assert.h>
#include <float.h>
#include <stdlib.h>

#include "dmumps_c.h"

namespace sofa {

namespace component {

namespace linearsolver {



/// Direct linear solvers implemented with the TAUCS library
template<class TMatrix, class TVector>
class MumpsSolver:
	public sofa::component::linearsolver::MatrixLinearSolver<TMatrix, TVector> {
	public:
	SOFA_CLASS(SOFA_TEMPLATE2(MumpsSolver,TMatrix,TVector),SOFA_TEMPLATE2(sofa::component::linearsolver::MatrixLinearSolver,TMatrix,TVector));

	typedef TMatrix Matrix;
	typedef TVector Vector;
	typedef typename Matrix::Real Real;
	typedef sofa::component::linearsolver::MatrixLinearSolver<TMatrix, TVector> Inherit;

	Data<int> f_ordering;
	Data<double> f_dropTol;
	Data<bool> f_modified_flag;
	Data<int> f_symmetric;
	
	
	

	MumpsSolver();
	virtual ~MumpsSolver() {}
	void solve (Matrix& M, Vector& x, Vector& b);
	void invert(Matrix& M);

	MatrixInvertData * createInvertData() {
		return new MumpsSolverInvertData();
	}

	protected:

	class MumpsSolverInvertData : public MatrixInvertData {
		public :
		DMUMPS_STRUC_C id;
		
		CompressedRowSparseMatrix<double> Mfiltered;

		MumpsSolverInvertData() {			  
			 
		}

		~MumpsSolverInvertData() {
			id.job = -2; 
			dmumps_c(&id);
			std::cout<<"InvertData destructor called\n";
		}
		
		private:
		

	};
};

} // namespace linearsolver

} // namespace component

} // namespace sofa

#endif