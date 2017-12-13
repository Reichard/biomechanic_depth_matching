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
#ifndef SOFA_COMPONENT_LINEARSOLVER_MyPCGLinearSolver_H
#define SOFA_COMPONENT_LINEARSOLVER_MyPCGLinearSolver_H

#include <sofa/core/behavior/LinearSolver.h>
#include <sofa/component/linearsolver/MatrixLinearSolver.h>
#include <sofa/helper/map.h>
//#include <IncompleteCholeskyPreconditioner.h>
#include <IntelMKLPreconditioner.h>

#include <math.h>

namespace sofa
{

namespace component
{

namespace linearsolver
{

typedef enum { NONE, STALE, ROTWARP, FULL } PrecondMode;

/// Linear system solver using the conjugate gradient iterative algorithm
template<class TMatrix, class TVector, class TPreconditioner>
class MyPCGLinearSolver : public sofa::component::linearsolver::MatrixLinearSolver<TMatrix,TVector>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE3(MyPCGLinearSolver,TMatrix,TVector,TPreconditioner),SOFA_TEMPLATE2(sofa::component::linearsolver::MatrixLinearSolver,TMatrix,TVector));

    typedef TMatrix Matrix;
    typedef TVector Vector;
    typedef double Real;
    typedef sofa::component::linearsolver::MatrixLinearSolver<TMatrix,TVector> Inherit;

    Data<unsigned> f_maxIter;
    Data<double> f_tolerance;
    Data<double> f_smallDenominatorThreshold;
    Data<bool> f_verbose;
    Data<std::string> f_user_precond_mode;
    // Deprecated
    Data<bool> f_use_precond;
    Data<bool> f_update_precond;

    Data<int> f_FSAI_Sparseness;
    Data<int> f_FSAI_Solver_MaxIter;
    Data<double> f_FSAI_Solver_RelEps;
    Data<double> f_FSAI_Solver_AbsEps;
	Data<bool> loadPreconditionerFromFile;
	Data<bool> savePreconditionerToFile;
	sofa::core::objectmodel::DataFileName filenameSave;
	sofa::core::objectmodel::DataFileName filenameLoad;

    Data<double> f_IC_DropTol;

    Data<bool>   onGPU;
    Data<int>   precondType;
    Data<int>   ILUlevel;

    PrecondMode precondMode;

//    typedef sofa::component::linearsolver::CompressedRowSparseMatrix<Real> PrecondMatrixType;
//    typedef sofa::component::linearsolver::FullVector<Real> PrecondVectorType;
//
//    typedef sofa::component::linearsolver::IncompleteCholeskyPreconditioner <PrecondMatrixType,PrecondVectorType> CholeskyPreconditionerType;
//    typedef sofa::component::linearsolver::IntelMKLPreconditioner <PrecondMatrixType,PrecondVectorType> PreconditionerType;

    typedef TPreconditioner PreconditionerType;
    PreconditionerType* m_Preconditioner;

    MyPCGLinearSolver();
    virtual ~MyPCGLinearSolver() {}

	virtual void parse(sofa::core::objectmodel::BaseObjectDescription* arg);
	void solve (Matrix& M, Vector& x, Vector& b);
    void init();
    void setSystemMBKMatrix(const core::MechanicalParams* mparams);
    //void setSystemRHVector(VecId v);
    //void setSystemLHVector(VecId v);

    virtual std::string getTemplateName() const
	 {
	  return templateName(this);
	 }

	static std::string templateName(const MyPCGLinearSolver<TMatrix,  TVector,  TPreconditioner>* = NULL)
	{
		return TPreconditioner::templateName();
	}

	bool usesRotWarp() { return precondMode == ROTWARP; }

private:
	int iteration;
	bool m_PreconditionerIsInitialized;
	bool m_PreconditionerHasMatrix;

protected:
    /// This method is separated from the rest to be able to use custom/optimized versions depending on the types of vectors.
    /// It computes: p = p*beta + r
    inline void cgstep_beta(const core::ExecParams* params /* PARAMS FIRST */, Vector& p, Vector& r, double beta);
    /// This method is separated from the rest to be able to use custom/optimized versions depending on the types of vectors.
    /// It computes: x += p*alpha, r -= q*alpha
    inline void cgstep_alpha(const core::ExecParams* params /* PARAMS FIRST */, Vector& x, Vector& r, Vector& p, Vector& q, double alpha);

    void printVector(sofa::defaulttype::BaseVector* vec, char* filename, char* filenamePostfix, int counter);
};

//template<class TMatrix, class TVector, class TPreconditioner>
//inline void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::cgstep_beta(const core::ExecParams* /*params*/ /* PARAMS FIRST */, Vector& p, Vector& r, double beta)
//{
//    p *= beta;
//    p += r; //z;
//}
//
//template<class TMatrix, class TVector, class TPreconditioner>
//inline void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::cgstep_alpha(const core::ExecParams* /*params*/ /* PARAMS FIRST */, Vector& x, Vector& r, Vector& p, Vector& q, double alpha)
//{
//    x.peq(p,alpha);                 // x = x + alpha p
//    r.peq(q,-alpha);                // r = r - alpha q
//}

//template<>
//inline void MyPCGLinearSolver:GraphScatteredVector>::cgstep_beta(const core::ExecParams* params /* PARAMS FIRST */, Vector& p, Vector& r, double beta);
//
//template<>
//inline void MyPCGLinearSolver<component::linearsolver::GraphScatteredMatrix,component::linearsolver::GraphScatteredVector>::cgstep_alpha(const core::ExecParams* params /* PARAMS FIRST */, Vector& x, Vector& r, Vector& p, Vector& q, double alpha);

} // namespace linearsolver

} // namespace component

} // namespace sofa

#endif
