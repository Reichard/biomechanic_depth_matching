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
#ifndef SOFA_COMPONENT_LINEARSOLVER_ExplicitPreconditionerOperator_INL
#define SOFA_COMPONENT_LINEARSOLVER_ExplicitPreconditionerOperator_INL

#include <ExplicitPreconditionerOperator.h>
#include <sofa/core/ObjectFactory.h>
#include <iostream>
#include "sofa/helper/system/thread/CTime.h"
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/behavior/LinearSolver.h>
#include <sofa/helper/system/thread/CTime.h>
//#include "IncompleteCholeskyPreconditioner.h"
//#include "IntelMKLPreconditioner.h"
#include <sofa/helper/AdvancedTimer.h>
#include "Preconditioner.h"


// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>



namespace sofa {

namespace component {

namespace linearsolver {

//using namespace sofa::defaulttype;
//using namespace sofa::core::behavior;
//using namespace sofa::simulation;
//using namespace sofa::core::objectmodel;
//using sofa::helper::system::thread::CTime;
//using sofa::helper::system::thread::ctime_t;
//using std::cerr;
//using std::endl;

template<class TReal>
ExplicitPreconditionerOperator<TReal>::ExplicitPreconditionerOperator()
{
	this->m_Preconditioner = NULL;
	counter=0;

	//set number of threads
	mkl_set_num_threads(1);
}

template<class TReal>
void ExplicitPreconditionerOperator<TReal>::setPreconditioner(PreconditionerType* precond)
{
	this->m_Preconditioner = precond;
}

/* in this function the precond matrix is prepared and all vectors are set up
 * the input data for the backsolve is stored in ExplicitPreconditionerOperatorInvertData
 *
 * For the GPU version: copy the matrix (i.e. the arrays m,a,ia,ja) to the gpu and
 * initialize the arrays b, r and x1
 * also: run the analysis of the matrix for later use in the solve kernel
 *
 *
 */
template<class TReal>
void ExplicitPreconditionerOperator<TReal>::invert(Matrix& M) {

	std::cout<<"Initializing CPU Explicit Preconditioner Operator \n";

	//typedef typename sofa::component::linearsolver::IntelMKLPreconditioner < PrecondMatrixType, PrecondVectorType > RealPreconditionerType;
	typedef sofa::component::linearsolver::PETScPreconditionerInvertData<Real> InvertDataType;
	InvertDataType* preconInvertData = (InvertDataType* )((PreconditionerType* )this->m_Preconditioner)->getInvertData();

	ExplicitPreconditionerOperatorInvertData* data = (ExplicitPreconditionerOperatorInvertData*)this->getMatrixInvertData();
	


	//fill in data from cholesky preconditioner
//	data->uplo = 'u'; //matris is upper triangular (in fact it is a lower triangular CCS matrix, but that is the same as the upper CSR matrix)
//	data->diag = 'n'; //matrix is not unitriangular (diag is 1)
//	data->m = preconInvertData->L->m;
//	data->a = (Real*)preconInvertData->L->values.v;
//	data->ia = preconInvertData->L->colptr;
//	data->ja = preconInvertData->L->rowind;


//	data->uplo = 'u'; //matris is upper triangular (in fact it is a lower triangular CCS matrix, but that is the same as the upper CSR matrix)
//	data->diag = 'n'; //matrix is not unitriangular (diag is 1)
//	data->m = preconInvertData->numberOfRows;
//	data->a = (Real*)preconInvertData->valuesTemp;
//	data->ia = preconInvertData->rowIndexArrayM;
//	data->ja = preconInvertData->columnsArrayM;

	data->B.resize(preconInvertData->numberOfRows);
	data->R.resize(preconInvertData->numberOfRows);
	data->X1.resize(preconInvertData->numberOfRows);



}

/* backsolve is executed twice in n order to solve llt z = r
 * For the GPU version; 2 calls to cusparseDcsrsv_solve
 *
 * the GraphScatteredVectors are copied into the array datastructures using visitors
 *
 */
template<class TReal>
void ExplicitPreconditionerOperator<TReal>::solve (Matrix& M, Vector& z, Vector& r) {

	//typedef typename sofa::component::linearsolver::IntelMKLPreconditioner < PrecondMatrixType, PrecondVectorType > RealPreconditionerType;
		typedef sofa::component::linearsolver::PETScPreconditionerInvertData<Real> InvertDataType;
		InvertDataType* preconInvertData = (InvertDataType* )((PreconditionerType* )this->m_Preconditioner)->getInvertData();

	ExplicitPreconditionerOperatorInvertData* data = (ExplicitPreconditionerOperatorInvertData *) this->getMatrixInvertData();

	//this visitor calls the copyFromArray from MechanicalObject
	//to change in GPU version: implement the copyFromArray() in CudaMEchanicalOBject (either directly or
	// through a derived class)
	// then it's just a gpu->gpu copy
	 //this->executeVisitor( simulation::MechanicalMultiVectorToArrayVisitor( core::ExecParams::defaultInstance(), (r), data->B) );

	sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::Basevec Copy");

	this->executeVisitor( simulation::MechanicalMultiVectorToBaseVectorVisitor( core::ExecParams::defaultInstance(), r, &(data->B)) );
	
	sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::Basevec Copy");

	sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::ForwardSolve");

	char transa;


	if(preconInvertData->transpL)
	  transa='T';
	else
	   transa ='N';


	// mkl_cspblas_dcsrtrsv(&uplo, &transa, &diag, &preconInvertData->numberOfRows, preconInvertData->valuesL,preconInvertData->rowIndexArrayL, preconInvertData->columnsArrayL, data->B.ptr(), data->X1.ptr());

	 mkl_cspblas_dcsrgemv(&transa, &preconInvertData->numberOfRows, preconInvertData->valuesL,preconInvertData->rowIndexArrayL, preconInvertData->columnsArrayL, data->B.ptr(), data->X1.ptr());



	if(preconInvertData->transpU)
	  transa='T';
	else
	   transa ='N';

	 mkl_cspblas_dcsrgemv(&transa, &preconInvertData->numberOfRows, preconInvertData->valuesU, preconInvertData->rowIndexArrayU, preconInvertData->columnsArrayU, data->X1.ptr(), data->R.ptr());




	 sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::ForwardSolve");


	 sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::Basevec Copy");
	 //inverse of the visitor above
	 //this->executeVisitor( simulation::MechanicalArrayToMultiVectorVisitor( core::ExecParams::defaultInstance(), data->R, z) );
	 this->executeVisitor( simulation::MechanicalMultiVectorFromBaseVectorVisitor( core::ExecParams::defaultInstance(), z, &(data->R)) );
//	 this->executeVisitor( simulation::MechanicalMultiVectorFromBaseVectorVisitor( core::ExecParams::defaultInstance(), z, &(data->B)) );

	 sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::Basevec Copy");

}



template<class TReal>
void ExplicitPreconditionerOperator<TReal>::printVector(helper::vector<Real>& baseVec, char* filename, char* filenamePostfix, int counter)
{

			//get the base vector
			//sofa::defaulttype::BaseVector* baseVec;
			//baseVec= vec;//static_cast<sofa::defaulttype::BaseVector>(rhs);


			//open file and generate textstream
			QString completeFilename("/home/mediassi/MediAssistData/");
			completeFilename += "Temp/";
			completeFilename += filename;
			completeFilename += QString::number(counter);
			completeFilename += filenamePostfix;
			completeFilename +=".csv";

			QFile file(completeFilename);

			 if (!file.open(QFile::WriteOnly | QFile::Truncate))
				 return ;

			 QTextStream out(&file);

			 //matrix line by line
			 unsigned int rowSize = baseVec.size();
			 //unsigned int columnSize = 3;
			Real currentElement;

			 for(unsigned int m=0;m<rowSize;m++)
			 {
				 currentElement = baseVec[m];

				 if((m+1)%3)
					 out <<currentElement <<",";
				 else
					 out <<currentElement <<"\n";


			}

}

} // namespace linearsolver

} // namespace component

} // namespace sofa

#endif