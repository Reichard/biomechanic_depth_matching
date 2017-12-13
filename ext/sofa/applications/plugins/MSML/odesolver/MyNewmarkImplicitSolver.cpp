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
#include "MyNewmarkImplicitSolver.h"
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/simulation/common/MechanicalOperations.h>
#include <sofa/simulation/common/VectorOperations.h>
#include <sofa/core/ObjectFactory.h>
#include <math.h>
#include <iostream>
#include <sofa/helper/system/thread/CTime.h>
#include <sofa/simulation/common/MechanicalMatrixVisitor.h>
#include <sofa/component/linearsolver/MatrixLinearSolver.h>
#include <sofa/core/objectmodel/BaseContext.h>
#include <sofa/core/behavior/MechanicalState.h>
#include <sofa/helper/system/thread/CTime.h>
#include <sofa/helper/AdvancedTimer.h>


// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
#include "MediAssistConfig.h"


namespace sofa
{

namespace component
{

namespace odesolver
{
using core::VecId;
using namespace sofa::defaulttype;
using namespace core::behavior;

MyNewmarkImplicitSolver::MyNewmarkImplicitSolver()
: f_rayleighStiffness(initData(&f_rayleighStiffness,0.1,"rayleighStiffness","Rayleigh damping coefficient related to stiffness") )
, f_rayleighMass( initData(&f_rayleighMass,0.1,"rayleighMass","Rayleigh damping coefficient related to mass"))
, f_velocityDamping( initData(&f_velocityDamping,0.,"vdamping","Velocity decay coefficient (no decay if null)") )
, f_verbose( initData(&f_verbose,false,"verbose","Dump system state at each iteration") )
, f_delta( initData(&f_delta, 0.5, "gamma", "Newmark scheme gamma coefficient"))
, f_alpha( initData(&f_alpha, 0.25, "beta", "Newmark scheme alpha coefficient") )
{
	cpt=0;
}


void MyNewmarkImplicitSolver::solve(const core::ExecParams* params /* PARAMS FIRST */, double dt, sofa::core::MultiVecCoordId xResult, sofa::core::MultiVecDerivId vResult)
{
    sofa::simulation::common::VectorOperations vop( params, this->getContext() );
    sofa::simulation::common::MechanicalOperations mop( params, this->getContext() );

    MultiVecCoord pos(&vop, core::VecCoordId::position() );
    MultiVecDeriv vel(&vop, core::VecDerivId::velocity() );
    MultiVecDeriv a(&vop);
    MultiVecDeriv newA(&vop);

    MultiVecCoord newPos(&vop, xResult );
    MultiVecDeriv newVel(&vop, vResult );
    MultiVecDeriv dx(&vop);

    MultiVecDeriv force(&vop, core::VecDerivId::force() );
    MultiVecDeriv b(&vop);

    const double h = dt;
    const double delta = f_delta.getValue();
    const double alpha= f_alpha.getValue();
    const double rM = (1)*f_rayleighMass.getValue();
    const double rK = f_rayleighStiffness.getValue();
    const bool verbose  = f_verbose.getValue();


    const double a0 = 1.0 / (alpha * h*h);
    const double a1 = delta / (alpha * h);
    const double a2 = 1.0 / (alpha * h);
    const double a3 = 1.0 / (2*alpha)-1;
    const double a4 = delta / (alpha )-1;
    const double a5 = h/2.0*( delta/alpha-2);
    const double a6 = h*(1-delta);
    const double a7 = delta * h;


    //initialize a

    if(cpt==0){
		a.clear();
	}
	cpt++;

	//printCurrentPositionAsMatlabFile(params,"NewmarkSolver_PositionOld_", cpt);

	// 1. position has already been projected in the AnimateVisitor

    // 2. Compute right hand term of equation on a_{t+h}

	force.clear();

	sofa::helper::AdvancedTimer::stepBegin("ComputeRHS");

	mop.computeForce(force);//,true,false);

	b.eq(force);

	mop.propagateDx(a);

    mop.addMBKdx(b, a3+rM*a5, 0, (-1)*rK*a5);

    mop.addMBKv(b, a2+rM*a4, 0, (-1)*rK*a4);

    mop.projectResponse(b);          // b is projected to the constrained space

   // Reff = R + K0 * x0 - K  * dofs;%  + M * ( Nma2*vn + Nma3*an+ Rayleighalpha*(Nma4*vn + Nma5*an)) + Rayleighbeta*K* ( Nma4*vn + Nma5*an) + Nma0*M*(un-dofs) + Nma1*C*(un-dofs);   %right one
   // Reff = R + K0 * x0 - K  * un  + (M * ( Nma2 + Rayleighalpha*Nma4 ) + Rayleighbeta*K* Nma4) *vn + (M * ( Nma3+ Rayleighalpha*Nma5 )+ Rayleighbeta*K* Nma5)*an;



    sofa::helper::AdvancedTimer::stepNext ("ComputeRHS", "AssembleMatrix");

     // 3. Solve system of equations on dx

    core::behavior::MultiMatrix<simulation::common::MechanicalOperations> matrix(&mop);

	matrix = MechanicalMatrix::K * (1+a1*rK)*(-1) +  MechanicalMatrix::M * (a0+a1*rM); //changed to fit SOFA convention

	sofa::helper::AdvancedTimer::stepNext ("AssembleMatrix", "SolveMatrix");

    matrix.solve(dx, b);

    sofa::helper::AdvancedTimer::stepNext ("SolveMatrix", "UpdateStateVectors");

    //printVectorAsMatlabFiles(params, dx ,"NewmarkSolver_dx_", cpt);

    //printVectorAsMatlabFiles(params, b ,"NewmarkSolver_b_", cpt);


    // 4. Compute the new position, velocity and acceleration

    newPos.eq(pos, dx,1.0);

    //printCurrentPositionAsMatlabFile(params,"NewmarkSolver_PositionNew_", cpt);

    newA.eq(dx);
    newA.teq(a0);
    newA.peq(a,-a3);
    newA.peq(vel,-a2);

    //printCurrentPositionAsMatlabFile(params,"NewmarkSolver_ANew_", cpt);

    newVel.eq(vel,a,a6);
    newVel.peq(newA,a7);

    //printCurrentPositionAsMatlabFile(params,"NewmarkSolver_VelNew_", cpt);

    a.eq(newA);

    sofa::helper::AdvancedTimer::stepEnd( "UpdateStateVectors");


}
bool MyNewmarkImplicitSolver::printCurrentPositionAsMatlabFile(const core::ExecParams* params, const char* filename,int stepNumber)
{

	typedef sofa::core::behavior::MechanicalState<sofa::defaulttype::StdVectorTypes<sofa::defaulttype::Vec<3, double>, sofa::defaulttype::Vec<3, double>, double> > MechanicalStateType;

	MechanicalStateType* mstate = dynamic_cast< MechanicalStateType* >(this->getContext()->getMechanicalState());
	const MechanicalStateType::VecCoord *X=mstate->getX();



	 QString completeFilename(MEDIASSIST_DATA_PATH);
	completeFilename += "/Temp/";
	completeFilename += filename;
	completeFilename += QString::number(stepNumber);
	completeFilename +=".mat";



	//open file & stream
		 std::ofstream myfileVector;
		// const char* theFilename2 = vectorFilename.c_str();
		 myfileVector.open (completeFilename.toStdString().c_str());
		 unsigned int size = X->size();
		 MechanicalStateType::Coord currentElement;

		 for(unsigned int i=0;i<size;i++)
		 {
			 currentElement = (*X)[i];
			 myfileVector <<currentElement[0] <<"\n";
			 myfileVector <<currentElement[1] <<"\n";
			 myfileVector <<currentElement[2] <<"\n";

		 }

		// myfileVector<<rhs;


		//close file stream
		myfileVector.close();


	return true;
}

bool MyNewmarkImplicitSolver::printVectorAsMatlabFiles(const core::ExecParams* params, core::MultiVecDerivId  vec,const char* filename, int stepNumber)
{

//	sofa::simulation::common::MechanicalOperations mop( params, this->getContext() );
//
//	//set MultiVecDerivId as LHS vector
//	mop.m_setSystemLHVector(vec);
//
////	//sofa::defaulttype::BaseVector  baseVec;
//
//	sofa::core::behavior::LinearSolver* s;
//	s = this->context_->get<sofa::core::behavior::LinearSolver>(this->context_->getTags(), sofa::core::objectmodel::BaseContext::SearchDown);
//
//	//s->getContext()->
//	sofa::defaulttype::BaseVector* baseVec = s->getSystemLHBaseVector();
//
//
//	 QString completeFilename(MEDIASSIST_DATA_PATH);
//	completeFilename += "/Temp/";
//	completeFilename += filename;
//	completeFilename += QString::number(stepNumber);
//	completeFilename +=".mat";
//
//
//
//	//open file & stream
//		 std::ofstream myfileVector;
//		// const char* theFilename2 = vectorFilename.c_str();
//		 myfileVector.open (completeFilename.toStdString().c_str());
//		 unsigned int size = baseVec->size();
//		 double currentElement;
//
//		 for(unsigned int i=0;i<size;i++)
//		 {
//			 currentElement = baseVec->element(i);
//			 myfileVector <<currentElement <<"\n";
//
//		 }
//
//		// myfileVector<<rhs;
//
//
//		//close file stream
//		myfileVector.close();


	return true;
}

SOFA_DECL_CLASS(MyNewmarkImplicitSolver)

int MyNewmarkImplicitSolverClass = core::RegisterObject("Implicit time integratorusing Newmark scheme")
.add< MyNewmarkImplicitSolver >()
.addAlias("MyNewmark");
;

} // namespace odesolver

} // namespace component

} // namespace sofa

