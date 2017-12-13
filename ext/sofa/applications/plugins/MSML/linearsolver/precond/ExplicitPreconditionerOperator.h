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
#ifndef SOFA_COMPONENT_LINEARSOLVER_ExplicitPreconditionerOperator_H
#define SOFA_COMPONENT_LINEARSOLVER_ExplicitPreconditionerOperator_H


#include <sofa/component/linearsolver/MatrixLinearSolver.h>
#include <sofa/defaulttype/VecTypes.h>
//#include <IncompleteCholeskyPreconditioner.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/simulation/common/MechanicalMatrixVisitor.h>
#include "Preconditioner.h"


//extern "C" {
//#ifndef INCLUDE_PETSc_H
//#define INCLUDE_PETSc_H
//#include <petscksp.h>
//#endif
//}



namespace sofa {

namespace component {

namespace linearsolver {


template<class TReal>
class ExplicitPreconditionerOperator : public sofa::component::linearsolver::MatrixLinearSolver< sofa::component::linearsolver::GraphScatteredMatrix, sofa::component::linearsolver::GraphScatteredVector >
{
public:
	SOFA_CLASS(SOFA_TEMPLATE(ExplicitPreconditionerOperator,TReal),SOFA_TEMPLATE2(sofa::component::linearsolver::MatrixLinearSolver, sofa::component::linearsolver::GraphScatteredMatrix,  sofa::component::linearsolver::GraphScatteredVector) );

	typedef  sofa::component::linearsolver::GraphScatteredMatrix Matrix;
	typedef  sofa::component::linearsolver::GraphScatteredVector Vector;

  //  typedef typename Matrix::Real Real;
    typedef TReal       Real;

    typedef sofa::component::linearsolver::CompressedRowSparseMatrix<Real > PrecondMatrixType;
	typedef sofa::component::linearsolver::FullVector<Real> PrecondVectorType;



	typedef sofa::component::linearsolver::Preconditioner < Real >  PreconditionerType;

    ExplicitPreconditionerOperator();
    void solve (Matrix& M, Vector& x, Vector& b);
    void invert(Matrix& M);
    void setPreconditioner(PreconditionerType* precond);
    void printVector(helper::vector<Real>& baseVec, char* filename, char* filenamePostfix, int counter);

   MatrixInvertData * getMatrixInvertData()
   {
       if (invertData==NULL) invertData=createInvertData();
       return invertData;
   }
   
   MatrixInvertData * createInvertData() {
      return new ExplicitPreconditionerOperatorInvertData();
    }

   static std::string Name() { return "CPUExplicit"; }

protected:
    class ExplicitPreconditionerOperatorInvertData : public MatrixInvertData {
      public :
    	PrecondVectorType B;
    	PrecondVectorType R;
    	PrecondVectorType X1;
//    	 char uplo;
//		 // char* transa;
//		  char diag;
//		  int m;
//		  Real *a;
//		  int* ia;
//		  int* ja;
//		  Real *x;
//		  Real *y;

	    ExplicitPreconditionerOperatorInvertData() {
//	      uplo='0';
//		  diag ='0';
//		  m =0;
//		  a = NULL;
//		  ia = NULL;
//		  ja = NULL;
//		  x = NULL;
//		  y = NULL;
	    }

	    ~ExplicitPreconditionerOperatorInvertData() {
	     // if (perm) taucs_free(perm);
	    //  if (invperm) taucs_free(invperm);
//	      freeL();
//	      freeRL();
//	      perm         = NULL;
//	      invperm      = NULL;
//	      precond_args = NULL;
//	      n = 0;
	    }




    };

    PreconditionerType* m_Preconditioner;
    int counter;
};

} // namespace linearsolver

} // namespace component

} // namespace sofa

namespace sofa
{

namespace simulation
{


//class SOFA_SIMULATION_COMMON_API MechanicalMultiVectorToArrayVisitor : public BaseMechanicalVisitor
//{
//public:
//    ConstMultiVecId src;
//    helper::vector<double> &vect;
//
//	/// Return a class name for this visitor
//	/// Only used for debugging / profiling purposes
//	virtual const char* getClassName() const { return "MechanicalMultiVectorToArrayVisitor"; }
//
//	MechanicalMultiVectorToArrayVisitor(
//      const core::ExecParams* params /* PARAMS FIRST  = core::ExecParams::defaultInstance()*/, ConstMultiVecId _src, helper::vector<double>& _vect )
//    : BaseMechanicalVisitor(params) , src(_src), vect(_vect)
//    {
//    }
//
//    virtual Result fwdMechanicalState(simulation::Node* /*node*/, core::behavior::BaseMechanicalState* mm)
//    {
//        if (&vect != NULL )
//        {
//            mm->copyToArray(vect,src.getId(mm));
//        }
//
//        return RESULT_CONTINUE;
//    }
//};
//
//class SOFA_SIMULATION_COMMON_API MechanicalArrayToMultiVectorVisitor : public BaseMechanicalVisitor
//{
//public:
//	MultiVecId dest;
//	helper::vector<double> &vect;
//
//	/// Return a class name for this visitor
//	/// Only used for debugging / profiling purposes
//	virtual const char* getClassName() const { return "MechanicalArrayToMultiVectorVisitor"; }
//
//	MechanicalArrayToMultiVectorVisitor(
//	  const core::ExecParams* params /* PARAMS FIRST  = core::ExecParams::defaultInstance()*/,  helper::vector<double>& _vect, MultiVecId _dest )
//	: BaseMechanicalVisitor(params) , dest(_dest), vect(_vect)
//	{
//	}
//
//	virtual Result fwdMechanicalState(simulation::Node* /*node*/, core::behavior::BaseMechanicalState* mm)
//	{
//		if (&vect != NULL )
//		{
//			mm->copyFromArray( dest.getId(mm),vect);
//		}
//
//		return RESULT_CONTINUE;
//	}
//};

}

}

#endif
