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

/* Francois Faure, 2014 */
#ifndef SOFA_STANDARDTEST_ForceField_test_H
#define SOFA_STANDARDTEST_ForceField_test_H

#include "Sofa_test.h"
#include <sofa/simulation/graph/DAGSimulation.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <SofaEigen2Solver/EigenBaseSparseMatrix.h>
#include <SofaBaseLinearSolver/SingleMatrixAccessor.h>
#include <SceneCreator/SceneCreator.h>

#include <sofa/defaulttype/VecTypes.h>
#include <SofaBaseMechanics/MechanicalObject.h>


namespace sofa {


/** @brief Helper for writing ForceField tests.
 * The constructor creates a root node and adds it a State and a ForceField (of the paremeter type of this template class).
 * Pointers to node, state and force are available.
 * Deriving the ForceField test from this class makes it easy to write: just call function run_test with positions, velocities and the corresponding expected forces.
 * This function automatically checks not only the forces (function addForce), but also the stiffness (methods addDForce and addKToMatrix), using finite differences.
 * @author François Faure, 2014
 *
 */
template <typename _ForceFieldType>
struct ForceField_test : public Sofa_test<typename _ForceFieldType::DataTypes::Real>
{
    typedef _ForceFieldType ForceField;
    typedef typename ForceField::DataTypes DataTypes;

    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;

    typedef component::container::MechanicalObject<DataTypes> DOF;

    /// @name Scene elements
    /// {
    typename DOF::SPtr dof;
    typename ForceField::SPtr force;
    simulation::Node::SPtr node;
    /// }

    /// @name Precision and control parameters
    /// {
    SReal errorMax;       ///< tolerance in precision test. The actual value is this one times the epsilon of the Real numbers (typically float or double)
    /**
     * @brief Minimum/Maximum amplitudes of the random perturbation used to check the stiffness using finite differences
     * @warning Should be more than errorMax/stiffness. This is not checked automatically.
     */
    std::pair<Real,Real> deltaRange;
    bool checkStiffness;  ///< If false, stops the test after checking the force, without checking the stiffness. Default value is true.
    bool debug;           ///< Print debug messages. Default is false.
    /// }

    /// @name Tested API
    /// {
    static const unsigned char TEST_POTENTIAL_ENERGY = 1; ///< testing getPotentialEnergy function. The tests will only work with conservative forces (if dissipative forces such as viscosity or damping are computed, the test is wrong)
    static const unsigned char TEST_ALL = UCHAR_MAX; ///< testing everything
    unsigned char flags; ///< testing options. (all by default). To be used with precaution.
    /// }


    /** Create a scene with a node, a state and a forcefield.;
     *
     */
    ForceField_test()
        : errorMax( 100 )
        , deltaRange( 1, 1000 )
        , checkStiffness( true )
        , debug( false )
        , flags( TEST_ALL )
    {
        using modeling::addNew;
        simulation::Simulation* simu;
        sofa::simulation::setSimulation(simu = new sofa::simulation::graph::DAGSimulation());

        ///  node 1
        node = simu->createNewGraph("root");
        dof = addNew<DOF>(node);
        force = addNew<ForceField>(node);
    }
    
     /** Create a scene from a xml file.
     *
     */
    ForceField_test(std::string filename)
        : errorMax( 100 )
        , deltaRange( 1, 1000 )
        , checkStiffness( true )
        , debug( false )
        , flags( TEST_ALL )
    {
        using modeling::addNew;
        simulation::Simulation* simu;
        sofa::simulation::setSimulation(simu = new sofa::simulation::graph::DAGSimulation());

        /// Load the scene
        node = simu->createNewGraph("root");
        node = down_cast<sofa::simulation::Node>( sofa::simulation::getSimulation()->load(filename.c_str()).get() );

        ///  Get mechanical object
        dof = node->get<DOF>(node->SearchDown);

        // Add force field
        force = addNew<ForceField>(node);
    }

    /**
     * @brief Given positions and velocities, checks that the expected forces are obtained, and that a small change of positions generates the corresponding change of forces.
     * @param x positions
     * @param v velocities
     * @param ef expected forces
     * This function first checks that the expected forces are obtained. Then, it checks getPotentialEnergy.
     * And then, it checks the stiffness, unless member checkStiffness is set to false.
     * A new position is created using a small random change, and the new force is computed.
     * The change of potential energy is compared to the dot product between displacement and force.
     * The  change of force is compared to the change computed by function addDForce, and to the product of the position change with the stiffness matrix.
     */
    void run_test( const VecCoord& x, const VecDeriv& v, const VecDeriv& ef )
    {
        if( deltaRange.second / errorMax <= g_minDeltaErrorRatio )
            ADD_FAILURE() << "The comparison threshold is too large for the finite difference delta";

        ASSERT_TRUE(x.size()==v.size());
        ASSERT_TRUE(x.size()==ef.size());
        std::size_t n = x.size();

        // copy the position and velocities to the scene graph
        this->dof->resize(n);
        typename DOF::WriteVecCoord xdof = this->dof->writePositions();
        copyToData( xdof, x );
        typename DOF::WriteVecDeriv vdof = this->dof->writeVelocities();
        copyToData( vdof, v );

        // init scene and compute force
        sofa::simulation::getSimulation()->init(this->node.get());
        core::MechanicalParams mparams;
        mparams.setKFactor(1.0);
        simulation::MechanicalResetForceVisitor resetForce(&mparams, core::VecDerivId::force());
        node->execute(resetForce);
        simulation::MechanicalComputeForceVisitor computeForce( &mparams, core::VecDerivId::force() );
        this->node->execute(computeForce);

        // check force
        typename DOF::ReadVecDeriv f= this->dof->readForces();
        if(debug){
            cout << "run_test,          x = " << x << endl;
            cout << "                   v = " << v << endl;
            cout << "            expected f = " << ef << endl;
            cout << "            actual f = " <<  f << endl;
        }
        ASSERT_TRUE( this->vectorMaxDiff(f,ef)< errorMax*this->epsilon() );

        if( !checkStiffness ) return;

        // to check the stiffness, generate a change of position, to check the change of force

        // store current force
        VecDeriv curF;
        copyFromData( curF, dof->readForces() );



        // Get potential Energy before applying a displacement to dofs
        double potentialEnergyBeforeDisplacement = ((const core::behavior::BaseForceField*)force.get())->getPotentialEnergy(&mparams);

        // change position
        VecDeriv dX(n);
        for( unsigned i=0; i<n; i++ ){
            dX[i] = DataTypes::randomDeriv( deltaRange.first * this->epsilon(), deltaRange.second * this->epsilon() );  // todo: better random, with negative values
            xdof[i] += dX[i];
        }

        // compute new force and difference between previous force
        node->execute(resetForce);
        node->execute(computeForce);
        VecDeriv newF;
        copyFromData( newF, dof->readForces() );
        VecDeriv changeOfForce(curF);
        for( unsigned i=0; i<curF.size(); ++i){
            changeOfForce[i] = newF[i] - curF[i];
        }

        if( flags & TEST_POTENTIAL_ENERGY )
        {
            // Get potential energy after displacement of dofs
            double potentialEnergyAfterDisplacement = ((const core::behavior::BaseForceField*)force.get())->getPotentialEnergy(&mparams);

            // Check getPotentialEnergy() we should have dE = -dX.F

            // Compute dE = E(x+dx)-E(x)
            double differencePotentialEnergy = potentialEnergyAfterDisplacement-potentialEnergyBeforeDisplacement;

            // Compute the expected difference of potential energy: -dX.F (dot product between applied displacement and Force)
            double expectedDifferencePotentialEnergy = 0;
            for( unsigned i=0; i<n; ++i){
                expectedDifferencePotentialEnergy = expectedDifferencePotentialEnergy - dot(dX[i],curF[i]);
            }

            double absoluteErrorPotentialEnergy = std::abs(differencePotentialEnergy - expectedDifferencePotentialEnergy);
            if( absoluteErrorPotentialEnergy> errorMax*this->epsilon() ){
                ADD_FAILURE()<<"dPotentialEnergy differs from -dX.F (threshold=" << errorMax*this->epsilon() << ")" << endl
                            << "dPotentialEnergy is " << differencePotentialEnergy << endl
                            << "-dX.F is " << expectedDifferencePotentialEnergy << endl
                            << "Failed seed number = " << BaseSofa_test::seed << endl;
            }
        }


        // check computeDf: compare its result to actual change
        node->execute(resetForce);
        dof->vRealloc( &mparams, core::VecDerivId::dx()); // dx is not allocated by default
        typename DOF::WriteVecDeriv wdx = dof->writeDx();
        copyToData ( wdx, dX );
        simulation::MechanicalComputeDfVisitor computeDf( &mparams, core::VecDerivId::force() );
        node->execute(computeDf);
        VecDeriv dF;
        copyFromData( dF, dof->readForces() );

        if( this->vectorMaxDiff(changeOfForce,dF)> errorMax*this->epsilon() ){
            ADD_FAILURE()<<"dF differs from change of force" << endl << "Failed seed number = " << BaseSofa_test::seed << endl;
        }

        // check stiffness matrix: compare its product with dx to actual force change
        typedef component::linearsolver::EigenBaseSparseMatrix<SReal> Sqmat;
        Sqmat K( n*DataTypes::deriv_total_size, n*DataTypes::deriv_total_size );
        component::linearsolver::SingleMatrixAccessor accessor( &K );
        mparams.setKFactor(1.0);
        force->addKToMatrix( &mparams, &accessor);
        K.compress();
        //        cout << "stiffness: " << K << endl;
        modeling::Vector dx;
        data_traits<DataTypes>::VecDeriv_to_Vector( dx, dX );

        modeling::Vector Kdx = K * dx;
        if( debug ){
            cout << "                  dX = " << dX << endl;
            cout << "                newF = " << newF << endl;
            cout << "     change of force = " << changeOfForce << endl;
            cout << "           addDforce = " << dF << endl;
            cout << "                 Kdx = " << Kdx.transpose() << endl;
        }

        modeling::Vector df;
        data_traits<DataTypes>::VecDeriv_to_Vector( df, changeOfForce );
        if( this->vectorMaxDiff(Kdx,df)> errorMax*this->epsilon() )
            ADD_FAILURE()<<"Kdx differs from change of force"<< endl << "Failed seed number = " << BaseSofa_test::seed << endl;;



    }


};


} // namespace sofa

#endif
