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
#ifndef SOFA_COMPONENT_LINEARSOLVER_ParalutionPreconditionerOperator_INL
#define SOFA_COMPONENT_LINEARSOLVER_ParalutionPreconditionerOperator_INL

#include <ParalutionPreconditionerOperator.h>
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
#include "ParalutionPreconditioner.h"

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
ParalutionPreconditionerOperator<TReal>::ParalutionPreconditionerOperator()
{
	this->m_Preconditioner = NULL;
	counter=0;
	m_OnGPU = false;
}

template<class TReal>
void ParalutionPreconditionerOperator<TReal>::setPreconditioner(PreconditionerType* precond)
{
	this->m_Preconditioner = precond;
}

/* in this function the precond matrix is prepared and all vectors are set up
 * the input data for the backsolve is stored in ParalutionPreconditionerOperatorInvertData
 *
 * For the GPU version: copy the matrix (i.e. the arrays m,a,ia,ja) to the gpu and
 * initialize the arrays b, r and x1
 * also: run the analysis of the matrix for later use in the solve kernel
 *
 *
 */
template<class TReal>
void ParalutionPreconditionerOperator<TReal>::invert(Matrix& M) {

	std::cout<<"Initializing Paralution Preconditioner Operator \n";

	//typedef typename sofa::component::linearsolver::IntelMKLPreconditioner < PrecondMatrixType, PrecondVectorType > RealPreconditionerType;
	typedef sofa::component::linearsolver::ParalutionPreconditionerInvertData<Real> InvertDataType;
	InvertDataType* preconInvertData = (InvertDataType* )((PreconditionerType* )this->m_Preconditioner)->getInvertData();

	ParalutionPreconditionerOperatorInvertData* data = (ParalutionPreconditionerOperatorInvertData*)this->getMatrixInvertData();
	
	//std::cout<<"NumberOfRows "<<preconInvertData->numberOfRows<<"\n";
	data->B.resize(preconInvertData->numberOfRows);
	//std::cout<<"Size of b: "<<data->B.capacity()<<"\n";
	data->R.resize(preconInvertData->numberOfRows);
	data->X1.resize(preconInvertData->numberOfRows);

    if(m_OnGPU)
    {
    	data->B_GPU.resize(preconInvertData->numberOfRows);
    	data->R_GPU.resize(preconInvertData->numberOfRows);
    	data->X1_GPU.resize(preconInvertData->numberOfRows);
    	std::cout<<"GPU Paralution Operator Enabled\n";
    }
    else
    {
    	data->B.resize(preconInvertData->numberOfRows);
    	//std::cout<<"Size of b: "<<data->B.capacity()<<"\n";
    	data->R.resize(preconInvertData->numberOfRows);
    	data->X1.resize(preconInvertData->numberOfRows);
    }



}

/* backsolve is executed twice in n order to solve llt z = r
 * For the GPU version; 2 calls to cusparseDcsrsv_solve
 *
 * the GraphScatteredVectors are copied into the array datastructures using visitors
 *
 */
template<class TReal>
void ParalutionPreconditionerOperator<TReal>::solve (Matrix& M, Vector& z, Vector& r) {

	//typedef typename sofa::component::linearsolver::IntelMKLPreconditioner < PrecondMatrixType, PrecondVectorType > RealPreconditionerType;
		typedef sofa::component::linearsolver::ParalutionPreconditionerInvertData<Real> InvertDataType;
		InvertDataType* preconInvertData = (InvertDataType* )((PreconditionerType* )this->m_Preconditioner)->getInvertData();

	ParalutionPreconditionerOperatorInvertData* data = (ParalutionPreconditionerOperatorInvertData *) this->getMatrixInvertData();

	//this visitor calls the copyFromArray from MechanicalObject
	//to change in GPU version: implement the copyFromArray() in CudaMEchanicalOBject (either directly or
	// through a derived class)
	// then it's just a gpu->gpu copy
	 //this->executeVisitor( simulation::MechanicalMultiVectorToArrayVisitor( core::ExecParams::defaultInstance(), (r), data->B) );

	sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::Basevec Copy");

	if(m_OnGPU)
		this->executeVisitor( simulation::MechanicalMultiVectorToBaseVectorVisitor( core::ExecParams::defaultInstance(), r, &(data->B_GPU)) );
	else
		this->executeVisitor( simulation::MechanicalMultiVectorToBaseVectorVisitor( core::ExecParams::defaultInstance(), r, &(data->B)) );
	
	sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::Basevec Copy");

	sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::ApplyPrecond");
//
//	preconInvertData->b_paralution.info();
//	preconInvertData->x_paralution.info();
//	preconInvertData->mat_paralution.info();

//	double* test = data->B.ptr();
//	std::cout<<"test entry "<<test[3]<<"\n";

	double *BPointer;
	double *RPointer;
	if(m_OnGPU)
	{
	    BPointer = (double*)data->B_GPU.deviceWrite();
	    RPointer = (double*)data->R_GPU.deviceWrite();

		preconInvertData->b_paralution.SetDataPtr(&BPointer, "rhs", preconInvertData->numberOfRows);
		preconInvertData->x_paralution.SetDataPtr(&RPointer,   "x",   preconInvertData->numberOfRows);
	}
	else
	{
		BPointer = data->B.ptr();
		RPointer = data->R.ptr();
		preconInvertData->b_paralution.SetDataPtr(&BPointer, "rhs", preconInvertData->numberOfRows);
		preconInvertData->x_paralution.SetDataPtr(&RPointer,   "x",   preconInvertData->numberOfRows);
	}





		preconInvertData->precond_paralution->Solve(preconInvertData->b_paralution, &preconInvertData->x_paralution);


	 sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::ApplyPrecond");


	 sofa::helper::AdvancedTimer::stepBegin("PrecondOperator::Basevec Copy");
	 //inverse of the visitor above
	 //this->executeVisitor( simulation::MechanicalArrayToMultiVectorVisitor( core::ExecParams::defaultInstance(), data->R, z) );
		if(m_OnGPU)
			 this->executeVisitor( simulation::MechanicalMultiVectorFromBaseVectorVisitor( core::ExecParams::defaultInstance(), z, &(data->R_GPU)) );
		else
			 this->executeVisitor( simulation::MechanicalMultiVectorFromBaseVectorVisitor( core::ExecParams::defaultInstance(), z, &(data->R)) );

//	 this->executeVisitor( simulation::MechanicalMultiVectorFromBaseVectorVisitor( core::ExecParams::defaultInstance(), z, &(data->B)) );

	 sofa::helper::AdvancedTimer::stepEnd("PrecondOperator::Basevec Copy");
	 preconInvertData->b_paralution.LeaveDataPtr(&BPointer);
	 preconInvertData->x_paralution.LeaveDataPtr(&RPointer);

}



template<class TReal>
void ParalutionPreconditionerOperator<TReal>::printVector(helper::vector<Real>& baseVec, char* filename, char* filenamePostfix, int counter)
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
