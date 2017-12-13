
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
#include <ParalutionPreconditioner.h>
#include "ParalutionPreconditionerOperator.h"
#include "PreconditionerOperator.h"
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/AdvancedTimer.h>
#include <boost/lexical_cast.hpp>
//#include <iostream>
//#include "sofa/helper/system/thread/CTime.h"
//#include <sofa/core/objectmodel/BaseContext.h>
//#include <sofa/core/behavior/LinearSolver.h>
//#include <sofa/helper/system/thread/CTime.h>
//#include <sofa/component/linearsolver/ParallelMatrixLinearSolver.inl>
//#include <sofa/component/linearsolver/CompressedRowSparseMatrix.inl>



namespace sofa {

namespace component {

namespace linearsolver {

//using namespace sofa::defaulttype;
//using namespace sofa::core::behavior;
//using namespace sofa::simulation;
//using namespace sofa::core::objectmodel;
//using sofa::helper::system::thread::CTime;
//using sofa::helper::system::thread::ctime_t;
using std::cerr;
using std::endl;
using namespace paralution;

template<class TReal, class TOperatorType>
ParalutionPreconditioner<TReal,TOperatorType>::ParalutionPreconditioner()
:
  f_ordering( initData(&f_ordering,2,"ordering","ose ordering 0=identity/1=tree/2=metis/3=natural/4=genmmd/5=md/6=mmd/7=amd") )
, f_dropTol( initData(&f_dropTol,(double) 0.0,"ic_dropTol","Drop tolerance use for incomplete factorization") )
, f_modified_flag( initData(&f_modified_flag,true,"ic_modifiedFlag","Modified ICC : maintains rowsums") )
, precondType( initData(&precondType,2,"precondType","choose precondType 1=MultiColoredSGS/2=ILU/3=MCILU/4=MultiElimination/5=Jacobi/6=SPAI/7=FSAI") )
, onGPU( initData(&onGPU,false,"onGPU","choose if should be run on GPU") )
, filenameSave(initData(&filenameSave,"saveFilename","Save system matrix at the beginning of the simulation") )
, filenameLoad(initData(&filenameLoad,"loadFilename","Load system matrix at the beginning of the simulation") )
, loadPreconditionerFromFile(initData(&loadPreconditionerFromFile,false,"LoadPrecond","Choose if load system matrix at the beginning of the simulation") )
, savePreconditionerToFile(initData(&savePreconditionerToFile,false,"SavePrecond","Choose if save system matrix at the beginning of the simulation") )
, level( initData(&level,2,"ILU level","level of ILU / ME preconditioner") )
{

	isInitialized = false;
}



template<class TReal, class TOperatorType>
void ParalutionPreconditioner<TReal,TOperatorType>::invert(Matrix& M) {
    M.compress();

    ParalutionPreconditionerInvertData<TReal> * data = (ParalutionPreconditionerInvertData<TReal> *) getMatrixInvertData(&M);

//    if (data->perm) free(data->perm);
//    if (data->invperm) free(data->invperm);

    data->Mfiltered.clear();
    data->Mfiltered.copyNonZeros(M);
    //data->Mfiltered.compress();
    data->Mfiltered.fullRows();
    data->Mfiltered.fullDiagonal();

    data->numberOfRows = data->Mfiltered.colSize();
    data->values  = (Real*) &(data->Mfiltered.getColsValue()[0]);
    data->rowIndexArray = (int *) &(data->Mfiltered.getRowBegin()[0]);
    data->columnsArray = (int *) &(data->Mfiltered.getColsIndex()[0]);
    data->numberOfEntries = data->rowIndexArray[data->numberOfRows];//-1;

    int testNumberOfEntries = data->Mfiltered.getColsValue().size();

    std::cout<<"(Re-) initializing FSAI preconditioner\n";
    std::cerr << "NumberOfEntries: "<<data->numberOfEntries<<"\n";
    std::cerr << "NumberOfEntriesTest: "<<testNumberOfEntries<<"\n";


    //fill in matrix
    data->mat_paralution.Clear();

    data->mat_paralution.AllocateCSR("mat_paralution", data->numberOfEntries, data->numberOfRows, data->numberOfRows );
    data->mat_paralution.CopyFromCSR(data->rowIndexArray, data->columnsArray, data->values);

//    data->b_paralution.Clear();
//    data->x_paralution.Clear();

    if(!isInitialized)
    {
    	std::cout<<"Allocate Paralution data\n";
		data->b_paralution.Allocate("b_paralution", data->numberOfRows);
		data->x_paralution.Allocate("x_paralution", data->numberOfRows);
		isInitialized = true;
    }


    switch(precondType.getValue())
    {
    case 1: MultiColoredSGS<LocalMatrix<Real>, LocalVector<Real>, Real > *mcsgs;
    		mcsgs = new MultiColoredSGS<LocalMatrix<Real>, LocalVector<Real>, Real >;
    	 	 data->precond_paralution = mcsgs;
    	 	 std::cout<<"Using MCSGS preconditioner\n";
             	 break;

     case 2: ILU<LocalMatrix<Real>, LocalVector<Real>, Real > *ilu;
     	 	 ilu = new ILU<LocalMatrix<Real>, LocalVector<Real>, Real >;
     	 	 ilu->Set(level.getValue());

     	 	 data->precond_paralution = ilu;
     	 	std::cout<<"Using ILU preconditioner\n";
             	 break;

     case 3: MultiColoredILU<LocalMatrix<Real>, LocalVector<Real>, Real > *mcilu;
			 mcilu = new MultiColoredILU<LocalMatrix<Real>, LocalVector<Real>, Real >;
			 mcilu->Set(level.getValue());
			 data->precond_paralution = mcilu;
			 std::cout<<"Using MCILU preconditioner\n";
			 	 break;

     case 4: MultiElimination<LocalMatrix<Real>, LocalVector<Real>, Real > *mcme;
			 mcme = new MultiElimination<LocalMatrix<Real>, LocalVector<Real>, Real >;
			 mcme->Set(data->j_precond_paralution,level.getValue());
			 data->precond_paralution = mcme;
			 std::cout<<"Using ME preconditioner\n";
			 	 break;

	 case 5: Jacobi<LocalMatrix<Real>, LocalVector<Real>, Real > *jac;
			 jac = new Jacobi<LocalMatrix<Real>, LocalVector<Real>, Real >;
			 data->precond_paralution = jac;
				 break;

	 case 6: SPAI<LocalMatrix<Real>, LocalVector<Real>, Real > *spai;
			 spai = new SPAI<LocalMatrix<Real>, LocalVector<Real>, Real >;
			 data->precond_paralution = spai;
				 break;

	 case 7: FSAI<LocalMatrix<Real>, LocalVector<Real>, Real > *fsai;
			 fsai = new FSAI<LocalMatrix<Real>, LocalVector<Real>, Real >;
			 fsai->Set(level.getValue());
			 std::cout<<"FSAI level is "<<level.getValue()<<"\n";
			 data->precond_paralution = fsai;
				 break;

     default: std::cout << "No preconditioner with that number available\n" << endl;
     	 	 // TODO: Standard einsetzen?
     	 	 	 break;
    }



    if(loadPreconditionerFromFile.getValue() && ( precondType.getValue() == 7) )
    {
    	//dummy build
//    	data->mat_paralution.Clear();
//    	data->mat_paralution.AllocateCSR("mat_paralution", data->numberOfEntries, data->numberOfRows, data->numberOfRows );
//    	data->mat_paralution.AddScalarDiagonal(1.0);
//		data->precond_paralution->Build();
    	data->precond_paralution->SetOperator(data->mat_paralution);


    	if(precondType.getValue() == 7)
    	{
    		typedef FSAI<LocalMatrix<Real>, LocalVector<Real>, Real > FSAIType;

    		std::string currentFilename = filenameLoad.getValue();
    		currentFilename += "Level_";
    		currentFilename +=  boost::lexical_cast<std::string>( level);
    		currentFilename += "L.mtx";
       		((FSAIType*)data->precond_paralution)->FSAI_L_.ReadFileMTX(currentFilename);

    		currentFilename = filenameLoad.getValue();
    		currentFilename += "Level_";
    		currentFilename +=  boost::lexical_cast<std::string>( level);
    		currentFilename += "LT.mtx";
       		((FSAIType*)data->precond_paralution)->FSAI_LT_.ReadFileMTX(currentFilename);

    		std::cout<<"FSAI loaded from "<<currentFilename<<"\n";

        	data->precond_paralution->build_ = true;
        	((FSAIType*)data->precond_paralution)->t_.CloneBackend(*data->precond_paralution->op_);
        	((FSAIType*)data->precond_paralution)->t_.Allocate("temporary", data->precond_paralution->op_->get_nrow());

    	}




    }

    else
    {
		sofa::helper::AdvancedTimer::stepBegin("ParalutionPreconditioner::BuildPreconditioner");
		data->precond_paralution->SetOperator(data->mat_paralution);
		data->precond_paralution->Build();
		std::cout<<"Preconditioner is built\n";
		sofa::helper::AdvancedTimer::stepEnd("ParalutionPreconditioner::BuildPreconditioner");
    }

    if(savePreconditionerToFile.getValue())
    {
    	if(precondType.getValue() == 2)
    	{
    		typedef ILU<LocalMatrix<Real>, LocalVector<Real>, Real > ILUType;
    		std::string currentFilename = filenameSave.getValue();

    		currentFilename += "ILU.mtx";
    		((ILUType*)data->precond_paralution)->ILU_.WriteFileMTX(currentFilename);

    		std::cout<<"ILU preconditioner saved to "<<currentFilename<<"\n";

    	}

    	if(precondType.getValue() == 7)
    	{
    		typedef FSAI<LocalMatrix<Real>, LocalVector<Real>, Real > FSAIType;
    		std::string currentFilename = filenameSave.getValue();
    		currentFilename += "Level_";
    		currentFilename +=  boost::lexical_cast<std::string>( level);
    		currentFilename += "L.mtx";
    		((FSAIType*)data->precond_paralution)->FSAI_L_.WriteFileMTX(currentFilename);
    		currentFilename = filenameSave.getValue();
    		currentFilename += "Level_";
    		currentFilename +=  boost::lexical_cast<std::string>( level);
    		currentFilename += "LT.mtx";
    		((FSAIType*)data->precond_paralution)->FSAI_LT_.WriteFileMTX(currentFilename);

    		std::cout<<"FSAI preconditioner saved to "<<currentFilename<<"\n";

    	}
    }

//	if(precondType.getValue() == 7)
//	{
//		typedef FSAI<LocalMatrix<Real>, LocalVector<Real>, Real > FSAIType;
//
//		std::string currentFilename = filenameSave.getValue();
//		currentFilename += "L.mtx";
//   		((FSAIType*)data->precond_paralution)->FSAI_L_.ReadFileMTX(currentFilename);
//
//		currentFilename = filenameSave.getValue();
//		currentFilename += "L.mtx";
//   		((FSAIType*)data->precond_paralution)->FSAI_L_.ReadFileMTX(currentFilename);
//
//		std::cout<<"FSAI loaded from "<<currentFilename<<"\n";
//
//	}

    if(onGPU.getValue())
    {
    	std::cout<<"Move Paralution data to accelerator\n";
		data->b_paralution.MoveToAccelerator();
		data->x_paralution.MoveToAccelerator();
		data->mat_paralution.MoveToAccelerator();
		data->precond_paralution->MoveToAccelerator();
    }




}

template<class TReal, class TOperatorType>
void ParalutionPreconditioner<TReal,TOperatorType>::solve (Matrix& M, Vector& z, Vector& r) {
    ParalutionPreconditionerInvertData<TReal> * data = (ParalutionPreconditionerInvertData<TReal> *) getMatrixInvertData(&M);

//    data->b_paralution.SetDataPtr(r.ptr(), "rhs", data->numberOfRows);
//    data->x_paralution.SetDataPtr(z.ptr(),   "x",   data->numberOfRows);
//
////    data->b_paralution.info();
////    data->x_paralution.info();
////    data->mat_paralution.info();
//
//
//	sofa::helper::AdvancedTimer::stepBegin("ParalutionSolver::Solve");
//
//
//    data->precond_paralution->Solve(data->b_paralution, &data->x_paralution);
//
//    sofa::helper::AdvancedTimer::stepEnd("ParalutionSolver::Solve");

}

//template<class TReal>
//void ParalutionPreconditioner<TReal>::OperatorSetSystemLHVector(core::MultiVecDerivId x)
//{
//	this->setSystemLHVector(x);
//}
//
//template<class TReal>
//void ParalutionPreconditioner<TReal>::OperatorSetSystemRHVector(core::MultiVecDerivId b)
//{
//	this->setSystemRHVector(b);
//}
//
//template<class TReal>
//void ParalutionPreconditioner<TReal>::OperatorSolveSystem()
//{
//		this->solveSystem();
//}

template<class TReal, class TOperatorType>
void ParalutionPreconditioner<TReal,TOperatorType>::initializeOperator()
{
	//this->m_ApplyMatrixOperator = new ParalutionPreconditionerOperator<TReal>();
	this->m_ApplyMatrixOperator = new OperatorType;
	simulation::Node* currentNode = static_cast<simulation::Node*>(this->getContext());
	currentNode->addObject(this->m_ApplyMatrixOperator);
	//((ParalutionPreconditionerOperator<TReal>*)this->m_ApplyMatrixOperator)->setPreconditioner(this);
	this->m_ApplyMatrixOperator->setPreconditioner(this);
	this->m_ApplyMatrixOperator->setGPUMode(onGPU.getValue());
}




} // namespace linearsolver

} // namespace component

} // namespace sofa

