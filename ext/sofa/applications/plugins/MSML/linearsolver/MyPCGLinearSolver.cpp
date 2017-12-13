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
#include "MyPCGLinearSolver.h"
#include "LinearTetrahedralCorotationalFEMForceField.h"
#include "QuadraticTetrahedralCorotationalFEMForceField.h"

#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/component/linearsolver/SparseMatrix.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/core/MechanicalParams.h>
#include <sofa/core/ObjectFactory.h>

#include <boost/algorithm/string/predicate.hpp>
#include <iostream>

//#include "CudaExplicitPreconditionerOperator.h"
//#include "CudaPreconditionerOperator.h"

//#include "HiFlowPreconditioner.h"
//#include "IntelMKLPreconditioner.h"
//#include "MGPreconditioner.h"

#include "ParalutionPreconditioner.h"
#include "ParalutionPreconditionerOperator.h"

#include "VisitorCollection.h"
//#include "IncompleteCholeskyPreconditioner.h"

//#include "PETScPreconditioner.h"

// just for debugging
//#include <QString>
//#include <QFile>
//#include <QTextStream>
//FIXME this configure file can not used!!!
#include "MediAssistConfig.h"

namespace sofa
{

namespace component
{

namespace linearsolver
{

using namespace sofa::defaulttype;
using namespace sofa::core::behavior;
using namespace sofa::simulation;
using namespace sofa::core::objectmodel;
using sofa::helper::system::thread::CTime;
using sofa::helper::system::thread::ctime_t;
using std::cerr;
using std::endl;

template<class TMatrix, class TVector, class TPreconditioner>
MyPCGLinearSolver<TMatrix,TVector,TPreconditioner>::MyPCGLinearSolver()
: f_maxIter(initData(&f_maxIter,(unsigned)10000,"iterations","maximum number of iterations of the Conjugate Gradient solution") )
, f_tolerance(initData(&f_tolerance,1e-6,"tolerance","desired precision of the Conjugate Gradient Solution (ratio of current residual norm over initial residual norm)") )
, f_smallDenominatorThreshold(initData(&f_smallDenominatorThreshold,1e-12,"threshold","minimum value of the denominator in the conjugate Gradient solution") )
, f_verbose(initData(&f_verbose,false,"verbose","Dump system state at each iteration") )
, f_user_precond_mode(initData(&f_user_precond_mode, std::string("none"), "precondMode", "What mode to use for the preconditioner: none (no preconditioner)/stale (use the matrix built at the beginning)/rotWarp (use rotation warping)/fresh (use the matrix built in the current timestep)"))
, f_use_precond(initData(&f_use_precond,true,"use_precond","Deprecated, set precondMode instead") )
, f_update_precond(initData(&f_update_precond,false,"update_precond","Deprecated, set precondMode instead") )
, f_FSAI_Sparseness(initData(&f_FSAI_Sparseness,2,"FSAI_Sparseness","FSAI_Sparseness") )
, f_FSAI_Solver_MaxIter(initData(&f_FSAI_Solver_MaxIter,1000,"FSAI_Solver_MaxIter","FSAI_Solver_MaxIter") )
, f_FSAI_Solver_RelEps(initData(&f_FSAI_Solver_RelEps,1e-10,"FSAI_Solver_RelEps","FSAI_Solver_RelEps") )
, f_FSAI_Solver_AbsEps(initData(&f_FSAI_Solver_AbsEps,1e-10,"FSAI_Solver_AbsEps","FSAI_Solver_AbsEps") )
, filenameSave(initData(&filenameSave,"saveFilename","Save system matrix at each iteration") )
, filenameLoad(initData(&filenameLoad,"loadFilename","Load system matrix at each iteration") )
, loadPreconditionerFromFile(initData(&loadPreconditionerFromFile,false,"LoadPrecond","Choose if load system matrix at each iteration") )
, savePreconditionerToFile(initData(&savePreconditionerToFile,false,"SavePrecond","Choose if save system matrix at each iteration") )
, f_IC_DropTol(initData(&f_IC_DropTol,1e-6,"IC_DropTol","IC_DropTol") )
, precondType(initData(&precondType,7,"precondType","choose precondType 1=MultiColoredSGS/2=ILU/3=MCILU/4=MultiElimination/5=Chebyshev/6=SPAI/7=FSAI") )
, onGPU(initData(&onGPU,false,"onGPU","choose if should be run on GPU") )
, ILUlevel(initData(&ILUlevel,1,"level","level of ILU / ME preconditioner") )
, precondMode(NONE), m_PreconditionerIsInitialized(false)
, m_PreconditionerHasMatrix(false) {
	this->addAlias(&f_user_precond_mode, "preconditionMode");
	this->addAlias(&f_user_precond_mode, "preconditionerMode");
	m_Preconditioner=0;
}

template<class TMatrix, class TVector, class TPreconditioner>
void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::parse(
	sofa::core::objectmodel::BaseObjectDescription* arg) {
	Inherit::parse(arg);

	const std::string& userMode = f_user_precond_mode.getValue();
	if (boost::iequals(userMode, "NONE"))
		{
			precondMode = NONE;
			std::cout<<"Precond mode is NONE\n";
		}
	else if (boost::iequals(userMode, "STALE"))
		{
			precondMode = STALE;
			std::cout<<"Precond mode is STALE\n";
		}
	else if (boost::iequals(userMode, "ROTWARP"))
		{
			precondMode = ROTWARP;
			std::cout<<"Precond mode is ROTWARP\n";
		}
	else if (boost::iequals(userMode, "FRESH") || boost::iequals(userMode, "FULL"))
	{
		precondMode = FULL;
		std::cout<<"Precond mode is FRESH\n";

	}

	else {
		serr << "Unrecognized preconditioner mode " << userMode << sendl;
		precondMode = ROTWARP;
	}
}

template<class TMatrix, class TVector, class TPreconditioner>
void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::init() {

	if (f_use_precond.isSet()) {
		if (f_user_precond_mode.isSet()) {
			serr << "usePrecond and precondMode are not supported simultaneously" << sendl;
			serr << "Ignoring usePrecond" << sendl;
		} else if (!f_use_precond.getValue()) precondMode = NONE;
	}
	//create pre-conditioner
	if(precondMode != NONE) {
		using sofa::component::forcefield::LinearTetrahedralCorotationalFEMForceField;
		using sofa::component::forcefield::QuadraticTetrahedralCorotationalFEMForceField;

		BaseContext* currentNode = this->getContext();

		if (f_update_precond.isSet()) {
			if (f_update_precond.getValue()) {
				precondMode = FULL;
			} else if (precondMode != ROTWARP) precondMode = STALE;
		}
//		if ((precondMode == ROTWARP) && (currentNode->get<LinearTetrahedralCorotationalFEMForceField<Vec3dTypes> >() == NULL) &&
//			(currentNode->get<LinearTetrahedralCorotationalFEMForceField<Vec3fTypes> >() == NULL) &&
//			(currentNode->get<QuadraticTetrahedralCorotationalFEMForceField<Vec3dTypes> >() == NULL) &&
//			(currentNode->get<QuadraticTetrahedralCorotationalFEMForceField<Vec3fTypes> >() == NULL)) {
//			if (f_user_precond_mode.isSet()) serr << "No ForceField for rotation warping found, using the matrix built at the beginning" << sendl;
//			precondMode = STALE;
//		}

		this->m_Preconditioner = new PreconditionerType;
		this->m_Preconditioner->setParameters(f_FSAI_Sparseness.getValue(), f_FSAI_Solver_MaxIter.getValue(), f_FSAI_Solver_RelEps.getValue(), f_FSAI_Solver_AbsEps.getValue(),loadPreconditionerFromFile.getValue(), savePreconditionerToFile.getValue(),filenameLoad.getValue(),filenameSave.getValue(), f_IC_DropTol.getValue(), onGPU.getValue(), precondType.getValue(), ILUlevel.getValue());
		this->addSlave(this->m_Preconditioner);
		this->m_Preconditioner->initializeOperator();

//		std::cout<<"Debug1:: saveFilename "<<filenameSave.getValue()<<"\n";
	}
}

template<class TMatrix, class TVector, class TPreconditioner>
void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::setSystemMBKMatrix(const core::MechanicalParams* mparams) {
	sofa::helper::AdvancedTimer::stepBegin("PCG::setSystemMBKMatrix");
	Inherit::setSystemMBKMatrix(mparams);
	sofa::helper::AdvancedTimer::stepEnd("PCG::setSystemMBKMatrix");
	
	if(precondMode != NONE) {
		if((precondMode == FULL) || !m_PreconditionerHasMatrix || this->m_Preconditioner->needsNewMatrix()) {
			sofa::helper::AdvancedTimer::stepBegin("PCG::PrecondSetSystemMBKMatrix");
			//cerr<<"Begin set matrix preconditioner \n";
			this->m_Preconditioner->setSystemMBKMatrix(mparams);
			this->m_Preconditioner->OperatorSetSystemMBKMatrix(mparams);
			//cerr<<"End set matrix preconditioner \n";
			sofa::helper::AdvancedTimer::stepEnd("PCG::PrecondSetSystemMBKMatrix");
			m_PreconditionerHasMatrix = true;
		}
	}

}

template<class TMatrix, class TVector, class TPreconditioner>
inline void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::cgstep_beta(const core::ExecParams* /*params*/ /* PARAMS FIRST */, Vector& p, Vector& r, double beta) {
    p.eq(r,p,beta); // p = p*beta + r
}

template<class TMatrix, class TVector, class TPreconditioner>
inline void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::cgstep_alpha(const core::ExecParams* /*params*/ /* PARAMS FIRST */, Vector& x, Vector& r, Vector& p, Vector& q, double alpha) {
    x.peq(p,alpha);                 // x = x + alpha p
    r.peq(q,-alpha);                // r = r - alpha q
}

template<class TMatrix, class TVector, class TPreconditioner>
void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::solve (Matrix& M, Vector& x, Vector& b) {
	using std::cerr;
	using std::endl;
	bool usePrecond = (precondMode != NONE);
	
	if(!usePrecond)
		std::cout<<"No Preconditioner is used \n";

    const core::ExecParams* params = core::ExecParams::defaultInstance();
    core::MechanicalParams mparams( *params);

    sofa::simulation::common::VectorOperations vop( params, this->getContext() );
    typename Inherit::TempVectorContainer vtmp(this, params, M, x, b);
	Vector& p = *vtmp.createTempVector();
	Vector& q = *vtmp.createTempVector();
	Vector& r = *vtmp.createTempVector();
	Vector& z = *vtmp.createTempVector();
	Vector& z1 = *vtmp.createTempVector();
	Vector& z2 = *vtmp.createTempVector();

	Vector& q1 = *vtmp.createTempVector();
	Vector& q2 = *vtmp.createTempVector();

	p.setOps(&vop);
	q.setOps(&vop);
	r.setOps(&vop);
	z.setOps(&vop);
	z1.setOps(&vop);
	z2.setOps(&vop);
	q1.setOps(&vop);
	q2.setOps(&vop);




//    sofa::simulation::common::VectorOperations vop( params, this->getContext() );
//	MultiVecDeriv testz2(&vop);

	const bool printLog =  this->f_printLog.getValue();
	const bool verbose  = f_verbose.getValue();

	// -- solve the system using a conjugate gradient solution
	double rho, rho_1=0, alpha, beta;

	if (verbose) cerr << "MyPCGLinearSolver, b = " << b << endl;

	z2.clear();
	x.clear();
	r = b; // initial residual

	double normb2 = b.dot(b);
	double normb = sqrt(normb2);

	unsigned int nb_iter;

	for (nb_iter=1; nb_iter<=f_maxIter.getValue(); nb_iter++) {

		if (usePrecond) {
			sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::apply Precond");

//			this->m_Preconditioner->setSystemLHVector(r);
//			printVector(this->m_Preconditioner->getSystemLHBaseVector(),"ApplyRotationSolver","A",nb_iter);
//
//			//multiply with RT
//			M.applyRotation(z,r,true); // z1 = RT r
//
//			this->m_Preconditioner->setSystemLHVector(z);
//			printVector(this->m_Preconditioner->getSystemLHBaseVector(),"ApplyRotationSolver","B",nb_iter);
//
//			//multiply with R
//			M.applyRotation(r,z,false); //z= R z2
//
//			this->m_Preconditioner->setSystemLHVector(r);
//			printVector(this->m_Preconditioner->getSystemLHBaseVector(),"ApplyRotationSolver","C",nb_iter);

			sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::Preconditioner::ApplyRotation");
			//multiply with RT
			if (precondMode == ROTWARP)
			{
				this->executeVisitor(simulation::MechanicalApplyRotationVisitor(&mparams,r, z1, true) );
//				std::cout<<"Applying rotation...\n";
			}
			else z1 = r;
			//M.applyRotation(z1,r,true); // z1 = RT r
			//M.applyRotation(z2,z1,false); // z = R z2
			sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::Preconditioner::ApplyRotation");
			// make cholesky solve: M0 z2 = z1
			if(!m_PreconditionerIsInitialized)
			{
				sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::apply Precond");
				sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::PrecondFactorization");
				this->m_Preconditioner->updateSystemMatrix();
				this->m_Preconditioner->setSystemLHVector(z2);
				this->m_Preconditioner->setSystemRHVector(z1);
				this->m_Preconditioner->solveSystem();
				this->m_PreconditionerIsInitialized = true;
				sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::PrecondFactorization");
				sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::apply Precond");
			}

//			this->m_Preconditioner->OperatorUpdateSystemMatrix();
			this->m_Preconditioner->OperatorSetSystemLHVector(z2);
			this->m_Preconditioner->OperatorSetSystemRHVector(z1);
			this->m_Preconditioner->OperatorSolveSystem();

			sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::Preconditioner::ApplyRotation");
			//multiply with R
			if (precondMode == ROTWARP)
				this->executeVisitor( simulation::MechanicalApplyRotationVisitor( &mparams,z2, z, false) );
			else z=z2;
			//M.applyRotation(z,z2,false); // z = R z2
			sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::Preconditioner::ApplyRotation");
			//printVector(this->m_Preconditioner->getSystemLHBaseVector(),"SolutionPreConditioner",nb_iter);

			sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::apply Precond");
		} else z = r;

		//z=r;
		
		sofa::helper::AdvancedTimer::stepBegin("MyPCGLinearSolver::CG Iteration");

		rho = r.dot(z);

		if (nb_iter>1) {
			double normr = sqrt(r.dot(r));
			double err = normr/normb;
			//cerr<<"iter:"<<nb_iter<<"\n";
			//cerr<<"Error:"<<err<<"\n";
			if (err <= f_tolerance.getValue()) {
//				std::cout<<"CG has converged \n";
				break;
			}
		}

		if( nb_iter==1 ) p = z;
		else {
			beta = rho / rho_1;
			//p = p*beta + z;
			cgstep_beta(params /* PARAMS FIRST */, p,z,beta);
		}

		if( verbose ) cerr<<"p : "<<p<<endl;

		// matrix-vector product
//		this->m_Preconditioner->setSystemLHVector(p);
//		printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration", "p",nb_iter);
//
//		M.applyRotation(q1,p,false); // q1 = RT p
//
//		this->m_Preconditioner->setSystemLHVector(q1);
//		printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration","q1",nb_iter);
//
//		M.applyRotation(q2,q1,true); // q1 = R q1
//
//		this->m_Preconditioner->setSystemLHVector(q2);
//		printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration", "q2",nb_iter);
//
		q = M*p;

		//new matrix-vector product adds co-rotation
		//this->m_Preconditioner->setSystemLHVector(p);
		//printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration", "p",nb_iter);

		//M.applyRotation(q1,p,true); // q1 = RT p

		//this->m_Preconditioner->setSystemLHVector(q1);
		//printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration","q1",nb_iter);

		//q2 = M*q1;

		//this->m_Preconditioner->setSystemLHVector(q2);
		//printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration", "q2",nb_iter);

		//M.applyRotation(q,q2,false); // q = R q2

		//this->m_Preconditioner->setSystemLHVector(q);
		//printVector(this->m_Preconditioner->getSystemLHBaseVector(),"CGIteration", "q",nb_iter);


		if( verbose ) cerr<<"q = M p : "<<q<<endl;

		double den = p.dot(q);


		if( fabs(den)<f_smallDenominatorThreshold.getValue() ) {
			if( verbose ) cerr<<"MyPCGLinearSolver, den = "<<den<<", smallDenominatorThreshold = "<<f_smallDenominatorThreshold.getValue()<<endl;
			break;
		}

		alpha = rho/den;
		//x.peq(p,alpha);                 // x = x + alpha p
		//r.peq(q,-alpha);                // r = r - alpha q
		cgstep_alpha(params /* PARAMS FIRST */, x,r,p,q,alpha);
		
		if( verbose ) {
			cerr<<"den = "<<den<<", alpha = "<<alpha<<endl;
			cerr<<"x : "<<x<<endl;
			cerr<<"r : "<<r<<endl;
		}
		
		rho_1 = rho;
		
		sofa::helper::AdvancedTimer::stepEnd("MyPCGLinearSolver::CG Iteration");
	}


	sofa::helper::AdvancedTimer::valSet("PCG iterations", nb_iter);


	// x is the solution of the system

	if( printLog ) cerr<<"MyPCGLinearSolver::solve, nbiter = "<<nb_iter;//<<" stop because of "<<endcond<<endl;
	
	if( verbose ) cerr<<"MyPCGLinearSolver::solve, solution = "<<x<<endl;
	
	vtmp.deleteTempVector(&p);
	vtmp.deleteTempVector(&q);
	vtmp.deleteTempVector(&r);
	vtmp.deleteTempVector(&z);
	vtmp.deleteTempVector(&z1);
	vtmp.deleteTempVector(&z2);
	vtmp.deleteTempVector(&q1);
	vtmp.deleteTempVector(&q2);
}

template<class TMatrix, class TVector, class TPreconditioner>
void MyPCGLinearSolver<TMatrix,TVector, TPreconditioner>::printVector(sofa::defaulttype::BaseVector* baseVec, char* filename, char* filenamePostfix, int counter)
{

//			//get the base vector
//			//sofa::defaulttype::BaseVector* baseVec;
//			//baseVec= vec;//static_cast<sofa::defaulttype::BaseVector>(rhs);
//
//
//			//open file and generate textstream
//			QString completeFilename("/home/mediassi/MediAssistData/");
//			completeFilename += "Temp/";
//			completeFilename += filename;
//			completeFilename += QString::number(counter);
//			completeFilename += filenamePostfix;
//			completeFilename +=".csv";
//
//			QFile file(completeFilename);
//
//			 if (!file.open(QFile::WriteOnly | QFile::Truncate))
//				 return ;
//
//			 QTextStream out(&file);
//
//			 //matrix line by line
//			 unsigned int rowSize2 = baseVec->size();
//			 //unsigned int columnSize = 3;
//			double currentElement;
//
//			 for(unsigned int m=0;m<rowSize2;m++)
//			 {
//				 currentElement = baseVec->element(m);
//
//				 if((m+1)%3)
//					 out <<currentElement <<",";
//				 else
//					 out <<currentElement <<"\n";
//			//	  					 for(unsigned int n=0;n<columnSize;n++)
//			//	  					 {
//			//	  						 out << vec[m][n];
//			//	  						 if(n!=(columnSize-1))
//			//	  							 out<<",";
//			//	  					 }
//			//	  					 out<<"\n";
//			// }
//
//			}

}

SOFA_DECL_CLASS(MyPCGLinearSolver)

int MyPCGLinearSolverClass = core::RegisterObject("My Linear system solver using the preconditioned conjugate gradient iterative algorithm")
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, HiFlowPreconditioner<double, ExplicitPreconditionerOperator<double> > > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, HiFlowPreconditioner<double, CudaExplicitPreconditionerOperator<double> > > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, IntelMKLPreconditioner<double, PreconditionerOperator<double> > > >(true)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, IntelMKLPreconditioner<double, CudaPreconditionerOperator<double> > > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, IncompleteCholeskyPreconditioner<double, PreconditionerOperator<double> > > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, IncompleteCholeskyPreconditioner<double, CudaPreconditionerOperator<double> > > >(false)
.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, ParalutionPreconditioner <double, ParalutionPreconditionerOperator<double> > > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, MGPreconditioner<double> > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, MGScatteredPreconditioner<double> > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, MGGPUScatteredPreconditioner<double> > >(false)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, PETScPreconditioner< double, PreconditionerOperator< double> > > >(true)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector,PETScPreconditioner< double, CudaPreconditionerOperator< double> > > >(true)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector, PETScPreconditioner< double, ExplicitPreconditionerOperator< double> > > >(true)
//.add< MyPCGLinearSolver<GraphScatteredMatrix,GraphScatteredVector,PETScPreconditioner< double, CudaExplicitPreconditionerOperator< double> > > >(true)
//.add< MyPCGLinearSolver<NewMatMatrix,NewMatVector> >()
//.add< MyPCGLinearSolver<NewMatSymmetricMatrix,NewMatVector> >()
//.add< MyPCGLinearSolver<NewMatBandMatrix,NewMatVector> >()
//.add< MyPCGLinearSolver<NewMatSymmetricBandMatrix,NewMatVector> >()
//.add< MyPCGLinearSolver< FullMatrix<double>, FullVector<double> > >()
//.add< MyPCGLinearSolver< SparseMatrix<double>, FullVector<double> > >()
.addAlias("MyPCGSolver")
.addAlias("MyPConjugateGradient")
;

} // namespace linearsolver

} // namespace component

} // namespace sofa

