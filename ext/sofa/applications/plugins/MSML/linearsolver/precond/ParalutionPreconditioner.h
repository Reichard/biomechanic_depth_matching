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
#ifndef SOFA_COMPONENT_LINEARSOLVER_ParalutionPreconditioner_H
#define SOFA_COMPONENT_LINEARSOLVER_ParalutionPreconditioner_H

#include <sofa/core/behavior/LinearSolver.h>
#include <sofa/component/linearsolver/MatrixLinearSolver.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/component/linearsolver/SparseMatrix.h>
#include <sofa/component/linearsolver/FullMatrix.h>
#include <sofa/helper/map.h>
#include <sofa/component/linearsolver/CompressedRowSparseMatrix.h>

#include <Preconditioner.h>


#include "ParalutionInitializer.h"
//#include "ParalutionPreconditionerOperator.h"

//class ParalutionPreconditionerOperator;

namespace sofa {

namespace component {

namespace linearsolver {

using namespace paralution;

template<class TReal>
class ParalutionPreconditionerInvertData : public MatrixInvertData {
  public :
    CompressedRowSparseMatrix<double> Mfiltered;
    void*            precond_args;
    helper::vector<double> B;
    helper::vector<double> R;
    int             (*precond_fn)(void*,void* x,void* b);
    int n;
    LocalMatrix<TReal> mat_paralution;
    LocalVector<TReal> b_paralution;
    LocalVector<TReal> x_paralution;

//    LocalMatrix<TReal> L_paralution;
//    LocalMatrix<TReal> LT_paralution;
    //Jacobi<LocalMatrix<Real>, LocalVector<Real>, Real > precond_paralution;
    //ILU<LocalMatrix<Real>, LocalVector<Real>, Real > precond_paralution;
    //	    MultiColoredILU<LocalMatrix<Real>, LocalVector<Real>, Real > precond_paralution;
    paralution::Preconditioner<LocalMatrix<TReal>, LocalVector<TReal>, TReal > *precond_paralution;
    Jacobi<LocalMatrix<TReal>, LocalVector<TReal>, TReal > j_precond_paralution;

    //CG<LocalMatrix<Real>, LocalVector<Real>, Real > solver_paralution;

    int numberOfRows;
    TReal* values;
    int* rowIndexArray;
    int* columnsArray;
    int numberOfEntries;


    ParalutionPreconditionerInvertData() {
        //initialization




        set_omp_threads_paralution(1);


//	        b_paralution.Allocate("b_paralution", 1);
//	        x_paralution.Allocate("x_paralution", 1);

    }

    ~ParalutionPreconditionerInvertData() {
    	delete precond_paralution;
    	std::cout<<"InvertData destructor called\n";
    }


};

template<class TReal, class TOperatorType>
class ParalutionPreconditioner : public sofa::component::linearsolver::Preconditioner<TReal>
{
public:
    //SOFA_CLASS(SOFA_TEMPLATE2(ParalutionPreconditioner,TMatrix,TVector),SOFA_TEMPLATE2(sofa::component::linearsolver::MatrixLinearSolver,TMatrix,TVector));

    SOFA_CLASS(SOFA_TEMPLATE2(ParalutionPreconditioner,TReal,TOperatorType),SOFA_TEMPLATE(sofa::component::linearsolver::Preconditioner,TReal));


	typedef CompressedRowSparseMatrix<TReal> Matrix;
	typedef FullVector<TReal> Vector;
    typedef TReal Real;

    //typedef sofa::component::linearsolver::MatrixLinearSolver< sofa::component::linearsolver::GraphScatteredMatrix, sofa::component::linearsolver::GraphScatteredVector >  OperatorBaseType;
    typedef TOperatorType  OperatorType;

    Data<int>    f_ordering;
    Data<double> f_dropTol;
    Data<bool>   f_modified_flag;
    bool isInitialized;

    Data<bool>   onGPU;
    Data<int>   precondType;
    Data<int>   level;

	Data<bool> loadPreconditionerFromFile;
	Data<bool> savePreconditionerToFile;
	sofa::core::objectmodel::DataFileName filenameSave;
	sofa::core::objectmodel::DataFileName filenameLoad;

    ParalutionPreconditioner();
    virtual ~ParalutionPreconditioner() {}
    void solve (Matrix& M, Vector& x, Vector& b);
    void invert(Matrix& M);

   MatrixInvertData * createInvertData() {
      return new ParalutionPreconditionerInvertData<Real>();
    }

   virtual std::string getTemplateName() const
   {
 	  return templateName(this);
   }

   static std::string templateName(const ParalutionPreconditioner<TReal,TOperatorType>* = NULL)
   {
 	  return "ParalutionPrecond";
   }

   void OperatorSetSystemMBKMatrix(const core::MechanicalParams* mparams){this->m_ApplyMatrixOperator->setSystemMBKMatrix(mparams);}
   void OperatorUpdateSystemMatrix(){this->m_ApplyMatrixOperator->updateSystemMatrix();}
   void OperatorSetSystemLHVector(core::MultiVecDerivId x){this->m_ApplyMatrixOperator->setSystemLHVector(x);}
   void OperatorSetSystemRHVector(core::MultiVecDerivId b){this->m_ApplyMatrixOperator->setSystemRHVector(b);}
    void OperatorSolveSystem(){this->m_ApplyMatrixOperator->solveSystem();}

//   void OperatorSetSystemMBKMatrix(const core::MechanicalParams* mparams){ }
//   void OperatorUpdateSystemMatrix(){ }
//   void OperatorSetSystemLHVector(core::MultiVecDerivId x);
//   void OperatorSetSystemRHVector(core::MultiVecDerivId b);
//   void OperatorSolveSystem();

   void setParameters(int FSAI_Sparseness, int FSAI_MaxIter, double FSAI_Solver_RelEps, double FSAI_Solver_AbsEps, bool _loadPreconditionerFromFile, bool _savePreconditionerToFile, std::string _filenameLoad, std::string _filenameSave,double IC_DropTol, bool donGPU, int dprecondType, int dlevel)
   {
//	   f_FSAI_Sparseness.getValue(), f_FSAI_Solver_MaxIter.getValue(), f_FSAI_Solver_RelEps.getValue(), f_FSAI_Solver_AbsEps.getValue(),loadPreconditionerFromFile.getValue(), savePreconditionerToFile.getValue(),filenameLoad.getValue(),filenameSave.getValue(), f_IC_DropTol.getValue(), onGPU.getValue(), precondType.getValue(), ILUlevel.getValue())
//	   f_FSAI_Solver_MaxIter.setValue(FSAI_MaxIter);
//	   f_FSAI_Solver_RelEps.setValue(  FSAI_Solver_RelEps);
//	   f_FSAI_Solver_AbsEps.setValue(FSAI_Solver_AbsEps);
//	   f_FSAI_Sparseness.setValue(FSAI_Sparseness);
	   onGPU.setValue(donGPU);
	   precondType.setValue(dprecondType);
	   level.setValue(dlevel);

		loadPreconditionerFromFile.setValue(_loadPreconditionerFromFile);
		savePreconditionerToFile.setValue(_savePreconditionerToFile);
		filenameSave.setValue(_filenameSave);
		filenameLoad.setValue(_filenameLoad);

//		std::cout<<"Debug2:: saveFilename "<<_filenameSave<<"\n";
//		std::cout<<"Debug3:: saveFilename "<<filenameSave.getValue()<<"\n";

   }

   void initializeOperator();


    protected:
       OperatorType* m_ApplyMatrixOperator;

    private:
      sofa::helper::ParalutionInitializer initializer;
};

} // namespace linearsolver

} // namespace component

} // namespace sofa

#endif
