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
#include <SofaTest/Sofa_test.h>
#include <sofa/simulation/graph/DAGSimulation.h>
#include <sofa/simulation/tree/TreeSimulation.h>
#include <SceneCreator/SceneCreator.h>
#include <SofaBaseMechanics/UniformMass.h>


namespace sofa {
using namespace modeling;

static int objectCounter;

/// Component class with an instance counter
template <class Object>
struct InstrumentedObject : public Object
{
    InstrumentedObject()  { objectCounter++; }
    ~InstrumentedObject() { objectCounter--; }
};

/// Component with a sub-component
template <class O1, class _O2>
struct ParentObject : public O1
{
    typedef InstrumentedObject<_O2> O2;
    typename O2::SPtr o2;
    ParentObject()  { o2 = core::objectmodel::New<O2>(); }
};




/** Test the Simulation class
*/
struct Simulation_test: public Sofa_test<SReal>
{
    // root
    simulation::Simulation* simulation;
    simulation::Node::SPtr root;

    Simulation_test()
    {
        //std::cerr << "Simulation_test::Simulation_test" << std::endl;
        sofa::simulation::setSimulation(simulation = new sofa::simulation::graph::DAGSimulation());
        //sofa::simulation::setSimulation(simulation = new sofa::simulation::tree::TreeSimulation());
    }

    /// Test Simulation::computeBBox
    void computeBBox()
    {
        // Init Sofa
        simulation::Simulation* simulation;
        sofa::simulation::setSimulation(simulation = new sofa::simulation::graph::DAGSimulation());

        root = simulation::getSimulation()->createNewGraph("root");

        // create DOFs and its expected bounding box
        MechanicalObject3::SPtr DOF = core::objectmodel::New<MechanicalObject3>();
        root->addObject(DOF);
        DOF->resize(4);
        MechanicalObject3::WriteVecCoord x = DOF->writePositions();
        x[0] = defaulttype::Vector3(0,0,0);
        x[1] = defaulttype::Vector3(1,0,0);
        x[2] = defaulttype::Vector3(0,1,0);
        x[3] = defaulttype::Vector3(0,0,1);
        defaulttype::Vector3 expectedMin(0,0,0), expectedMax(1,1,1);
        DOF->showObject.setValue(true); // bbox is updated only for drawn MO

        // end create scene
        //*********
        initScene();
        //*********

        defaulttype::Vector3 sceneMinBBox, sceneMaxBBox;
        simulation->computeBBox(root.get(), sceneMinBBox.ptr(), sceneMaxBBox.ptr());

        if( vectorMaxDiff(sceneMinBBox,expectedMin)>this->epsilon() || vectorMaxDiff(sceneMaxBBox,expectedMax)>this->epsilon() )
        {
            ADD_FAILURE() << "Wrong bounding box, expected (" << expectedMin <<", "<<expectedMax<<") , got ("<< sceneMinBBox <<", "<<sceneMaxBBox << ")" << endl;
        }

    }

    /// create a component and replace it with an other one
    void objectDestruction_replace()
    {
        typedef InstrumentedObject<component::mass::UniformMass<defaulttype::Vec3Types, SReal> > Component;

        objectCounter = 0;
        Component::SPtr toto = core::objectmodel::New<Component>();
        toto = core::objectmodel::New<Component>(); // this should first delete the previous one
        if(objectCounter != 1)
            ADD_FAILURE() << objectCounter << " objects, should be only 1 ! " <<endl;
    }

    /// create a component and replace it with an other one
    void objectDestruction_delete()
    {
        typedef InstrumentedObject<component::mass::UniformMass<defaulttype::Vec3Types, SReal> > Component;

        objectCounter = 0;
        Component::SPtr toto = core::objectmodel::New<Component>();
    }

    /// create a component and set it to NULL
    void objectDestruction_setNull()
    {
        typedef InstrumentedObject<component::mass::UniformMass<defaulttype::Vec3Types, SReal> > Component;

        objectCounter = 0;
        Component::SPtr toto = core::objectmodel::New<Component>();
        toto = NULL;
        checkDeletions();
    }

    /// create a component and set it to NULL
    void objectDestruction_reset()
    {
        typedef InstrumentedObject<component::mass::UniformMass<defaulttype::Vec3Types, SReal> > Component;

        objectCounter = 0;
        Component::SPtr toto = core::objectmodel::New<Component>();
        toto.reset();
        checkDeletions();
    }

    /// create and delete a component with a sub-component
    void objectDestruction_subObject()
    {
        typedef ParentObject<MechanicalObject3,MechanicalObject3> PO;

        objectCounter = 0;
        PO::SPtr toto = core::objectmodel::New<PO>();
        // deletion of toto at function exit
    }

    /// create and unload a scene and check if all the objects have been destroyed.
    void sceneDestruction_unload()
    {
        createScene();
        simulation->unload(root);
        if(objectCounter>0)
            ADD_FAILURE() << objectCounter << " objects not deleted " <<endl;
    }

    /// create and replace a scene and check if all the objects have been destroyed.
    void sceneDestruction_createnewgraph()
    {
        createScene();
        root = simulation::getSimulation()->createNewGraph("root2");
        checkDeletions();
    }

    /// create a new scene and reset the root
    void sceneDestruction_reset()
    {
        createScene();
        root.reset();
        checkDeletions();
    }

    /// create a new scene and set the root to NULL
    void sceneDestruction_setNull()
    {
        createScene();
        root = NULL;
        checkDeletions();
    }

protected:
    void createScene()
    {
        typedef component::mass::UniformMass<defaulttype::Vec3Types, SReal> UniformMass3;

        objectCounter = 0;

        root = simulation::getSimulation()->createNewGraph("root");
        root->addObject(core::objectmodel::New<InstrumentedObject<MechanicalObject3> >());
        root->addObject(core::objectmodel::New<InstrumentedObject<UniformMass3> >());
        simulation::Node::SPtr child  = simulation::getSimulation()->createNewNode("child");
        root->addChild(child);
        child->addObject(core::objectmodel::New<InstrumentedObject<MechanicalObject3> >());

    }
    void checkDeletions() {
        if(objectCounter>0)
            ADD_FAILURE() << objectCounter << " objects not deleted " <<endl;
    }
};

// run the tests
TEST_F( Simulation_test,computeBBox) { this->computeBBox(); }

// component destruction
TEST_F( Simulation_test,objectDestruction_replace) { this->objectDestruction_replace(); }
TEST_F( Simulation_test,objectDestruction_delete) { this->objectDestruction_delete(); checkDeletions(); }
TEST_F( Simulation_test,objectDestruction_setNull) { this->objectDestruction_setNull(); }
TEST_F( Simulation_test,objectDestruction_reset) { this->objectDestruction_reset(); }
TEST_F( Simulation_test,objectDestruction_subObject) { this->objectDestruction_subObject(); checkDeletions(); }

// graph destruction
TEST_F( Simulation_test,sceneDestruction_unload) { this->sceneDestruction_unload(); }
TEST_F( Simulation_test,sceneDestruction_createnewgraph) { this->sceneDestruction_createnewgraph(); }
// Node destruction does not trigger sub-graph destruction. You need to unload the node before. The two following tests are thus irrelevant.
//TEST_F( Simulation_test,sceneDestruction_reset) { this->sceneDestruction_reset(); }
//TEST_F( Simulation_test,sceneDestruction_setNull) { this->sceneDestruction_setNull(); }


}// namespace sofa







