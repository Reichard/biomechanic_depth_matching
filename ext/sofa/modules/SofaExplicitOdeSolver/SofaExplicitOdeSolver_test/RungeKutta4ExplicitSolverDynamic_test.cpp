/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <SofaTest/Elasticity_test.h>
#include <SceneCreator/SceneCreator.h>

#include <sofa/core/ExecParams.h>

//Including Simulation
#include <sofa/simulation/common/Simulation.h>
#include <sofa/simulation/graph/DAGSimulation.h>
#include <sofa/simulation/common/Node.h>

// Including mechanical object
#include <SofaBaseMechanics/MechanicalObject.h>

// Solvers
#include <SofaExplicitOdeSolver/RungeKutta4Solver.h>
#include <SofaBaseLinearSolver/CGLinearSolver.h>

#include <sofa/defaulttype/VecTypes.h>

namespace sofa {

using namespace component;
using namespace defaulttype;
using namespace simulation;
using namespace modeling;

/**  Dynamic solver test.
Test the dynamic behavior of solver: study a mass-spring system under gravity initialize with spring rest length it will oscillate around its equilibrium position if there is no damping.
The movement follows the equation:
x(t)= A cos(wt + phi) with w the pulsation w=sqrt(K/M), K the stiffness, M the mass and phi the phase.
In this test: x(t=0)= 1 and v(t=0)=0 and K = spring stiffness and phi = 0 of material thus x(t)= cos(wt)
This tests generates the discrete mass position obtained with rungeKutta4 solver with different parameter values (K,M,h).
Then it compares the effective mass position to the computed mass position every time step.
*/

template <typename _DataTypes>
struct RungeKutta4ExplicitSolverDynamic_test : public Elasticity_test<_DataTypes>
{


    typedef _DataTypes DataTypes;
    typedef typename DataTypes::CPos CPos;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::Real Real;

    typedef container::MechanicalObject<DataTypes> MechanicalObject;
    typedef component::odesolver::RungeKutta4Solver RungeKutta4Solver;
    typedef component::linearsolver::CGLinearSolver<component::linearsolver::GraphScatteredMatrix, component::linearsolver::GraphScatteredVector> CGLinearSolver;


    /// Root of the scene graph
    simulation::Node::SPtr root;      
    /// Tested simulation
    simulation::Simulation* simulation;  
    /// Position, velocity and acceleration array
    vector<double> positionsArray;
    vector<double> velocitiesArray;
    vector<double> accelerationsArray;
    
    /// Create the context for the scene
    void createScene(double K, double m, double l0)
    {
        // Init simulation
        sofa::simulation::setSimulation(simulation = new sofa::simulation::graph::DAGSimulation());
        root = simulation::getSimulation()->createNewGraph("root");

        // Create the scene
        root->setGravity(Coord(0,-10,0));

        // Solver
        RungeKutta4Solver::SPtr rungeKutta4Solver = addNew<RungeKutta4Solver> (getRoot());

        CGLinearSolver::SPtr cgLinearSolver = addNew<CGLinearSolver> (getRoot());
        cgLinearSolver->f_maxIter=3000;
        cgLinearSolver->f_tolerance =1e-9;
        cgLinearSolver->f_smallDenominatorThreshold=1e-9;

        // Set initial positions and velocities of fixed point and mass
        MechanicalObject3::VecCoord xFixed(1);
        MechanicalObject3::DataTypes::set( xFixed[0], 0., 2.,0.);
        MechanicalObject3::VecDeriv vFixed(1);
        MechanicalObject3::DataTypes::set( vFixed[0], 0.,0.,0.);
        MechanicalObject3::VecCoord xMass(1);
        MechanicalObject3::DataTypes::set( xMass[0], 0., 1.,0.);
        MechanicalObject3::VecDeriv vMass(1);
        MechanicalObject3::DataTypes::set( vMass[0], 0., 0., 0.);

        // Mass spring system
        root = this-> createMassSpringSystem(
                root,   // add mass spring system to the node containing solver
                K,      // stiffness
                m,      // mass
                l0,     // spring rest length
                xFixed, // Initial position of fixed point
                vFixed, // Initial velocity of fixed point
                xMass,  // Initial position of mass
                vMass); // Initial velocity of mass
    }


    /// Generate discrete mass position values with euler implicit solver
    void generateDiscreteMassPositions (double h, double K, double m, double z0, double v0,double g, double finalTime)
    {
        int size = 0 ;

        // During t=finalTime
        if((finalTime/h) > 0)
        {
            size = int(finalTime/h);
            positionsArray.reserve(size);
            velocitiesArray.reserve(size);
            accelerationsArray.reserve(size);
        }

        // First acceleration
        accelerationsArray.push_back((-K*(z0-z0)-m*g)/m);

        // First position is z0
        positionsArray.push_back(double(z0));

        // First velocity is v0
        velocitiesArray.push_back(v0);

        // Constants
        double stepBy2 = h/2.0;
        double stepBy3 = h/3.0;
        double stepBy6 = h/6.0;
        double pos,vel,newX,k1v,k2v,k3v,k4v,k1a,k2a,k3a,k4a,pos2,vel2,acc2;

        for(int i=1;i< size+1; i++)
        {

            // Step 1
            pos = positionsArray[i-1];
            vel = velocitiesArray[i-1];
            k1v = vel;
            k1a = accelerationsArray[i-1];

            // Step 2
            newX = pos + k1v*stepBy2;
            k2v  = vel + k1a*stepBy2;
            k2a  = (-K*(newX-z0)-m*g)/m;

            // Step 3
            newX = pos + k2v * stepBy2;
            k3v  = vel + k2a * stepBy2;
            k3a  = (-K*(newX-z0)-m*g)/m;

            // Step 4
            newX = pos + k3v * h;
            k4v  = vel + k3a * h;
            k4a  = (-K*(newX-z0)-m*g)/m;

            // Position pos2 at time t+h
            pos2 = pos + k1v * stepBy6 + k2v * stepBy3 + k3v * stepBy3 + k4v * stepBy6;

            // Velocity vel2 at time t+h
            vel2 = vel + k1a * stepBy6 + k2a * stepBy3 + k3a * stepBy3 + k4a * stepBy6;

            // Acceleration at time t+h
            acc2 = (-K*(pos2-z0)-m*g)/m;

            // Fill array
            positionsArray.push_back(pos2);
            velocitiesArray.push_back(vel2);
            accelerationsArray.push_back(acc2);

        }

    }

    /// After simulation compare the positions of points to the theoretical positions.
    bool compareSimulatedToTheoreticalPositions(double tolerance,double h)
    {
        int i = 0;
        // Init simulation
        sofa::simulation::getSimulation()->init(root.get());
        double time = root->getTime();

        // Get mechanical object
        simulation::Node::SPtr massNode = root->getChild("MassNode");
        typename MechanicalObject::SPtr dofs = massNode->get<MechanicalObject>(root->SearchDown);

        // Animate
        do
        {              
            // Record the mass position
            Coord p0=dofs.get()->read(sofa::core::ConstVecCoordId::position())->getValue()[0];

            double absoluteError = fabs(p0[1]-positionsArray[i]);

            // Compare mass position to the theoretical position
            if( absoluteError > tolerance )
            {
                ADD_FAILURE() << "Position of mass at time " << time << " is wrong: "  << std::endl
                    <<" expected Position is " << positionsArray[i] << std::endl
                    <<" actual Position is   " << p0[1] << std::endl
                    << "absolute error     = " << absoluteError << std::endl;
                return false;
            }

            //Animate
            sofa::simulation::getSimulation()->animate(root.get(),h);
            time = root->getTime();
            // Iterate
            i++;
        }
        while (time < 2);
        return true;
    }

};

// Define the list of DataTypes to instanciate
using testing::Types;
typedef Types<
    Vec3Types
> DataTypes; // the types to instanciate.

// Test suite for all the instanciations
TYPED_TEST_CASE(RungeKutta4ExplicitSolverDynamic_test, DataTypes);

// Test case: h=0.1 k=100 m =10 rm=0.1 rk=0.1
TYPED_TEST( RungeKutta4ExplicitSolverDynamic_test , rungeKutta4ExplicitSolverDynamicTest_medium_dt)
{
   this->createScene(100,10,1); // k,m,l0
   this->generateDiscreteMassPositions (0.01, 100, 10, 1, 0, 10, 2);
   this-> compareSimulatedToTheoreticalPositions(5e-16,0.01);
}

// Test case: h=0.1 K=1000 m = 10
TYPED_TEST( RungeKutta4ExplicitSolverDynamic_test , rungeKutta4ExplicitSolverDynamicTest_high_dt)
{
   this->createScene(1000,10,1); // k,m,l0
   this->generateDiscreteMassPositions (0.1, 1000, 10, 1, 0, 10, 2);
   this-> compareSimulatedToTheoreticalPositions(5e-16,0.1);
}

// Test case: h=0.001 k=100 m=100
TYPED_TEST( RungeKutta4ExplicitSolverDynamic_test , rungeKutta4ExplicitSolverDynamicTest_small_dt)
{
   this->createScene(100,100,1); // k,m,l0
   this->generateDiscreteMassPositions (0.001, 100, 100, 1, 0, 10, 2);
   this-> compareSimulatedToTheoreticalPositions(9e-16,0.001);
}

} // namespace sofa
