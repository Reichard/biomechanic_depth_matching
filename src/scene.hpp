#pragma once

#include <SofaGraphComponent/Gravity.h>
#include <SofaImplicitOdeSolver/EulerImplicitSolver.h>
#include <SofaBaseLinearSolver/CGLinearSolver.h>
#include <sofa/component/typedef/Sofa_typedef.h>
#include <SofaOpenglVisual/OglModel.h>
#include <SofaBaseVisual/VisualStyle.h>

#include <sofa/core/objectmodel/Context.h>
#include <sofa/core/VecId.h>

#include <sofa/simulation/graph/DAGSimulation.h>

#include <iostream>
#include <fstream>

#include "component.hpp"

using namespace sofa;
using sofa::simulation::Node;
using sofa::component::odesolver::EulerImplicitSolver;
typedef component::linearsolver::CGLinearSolver<component::linearsolver::GraphScatteredMatrix, component::linearsolver::GraphScatteredVector> CGLinearSolver;
using sofa::component::topology::MeshTopology;
using sofa::component::visualmodel::OglModel;
using sofa::core::objectmodel::Data;
using sofa::helper::ReadAccessor;
using sofa::helper::WriteAccessor;
using sofa::core::VecId;
using sofa::core::objectmodel::New;

Node::SPtr make_scene() 
{
	sofa::simulation::getSimulation()->unload(sofa::simulation::getSimulation()->GetRoot());
    auto groot = sofa::simulation::getSimulation()->createNewGraph("root");

	auto test_component = sofa::core::objectmodel::New<BiomechanicalDepthMatching>();
	groot->addObject(test_component);

	auto data = groot.get()->createChild("data");

	return groot;
}

Node::SPtr base_scene()
{
	sofa::simulation::getSimulation()->unload(sofa::simulation::getSimulation()->GetRoot());
    Node::SPtr groot = sofa::simulation::getSimulation()->createNewGraph("root");

    // solver
    EulerImplicitSolver::SPtr solver = sofa::core::objectmodel::New<EulerImplicitSolver>();
    solver->setName("solver");
    solver->f_printLog.setValue(false);
    groot->addObject(solver);

    CGLinearSolver::SPtr linearSolver = New<CGLinearSolver>();
    linearSolver->setName("linearSolver");
    groot->addObject(linearSolver);


    // Tetrahedron degrees of freedom
    MechanicalObject3::SPtr DOF = sofa::core::objectmodel::New<MechanicalObject3>();
    groot->addObject(DOF);
    DOF->resize(4);
    DOF->setName("DOF");
    //get write access to the position vector of mechanical object DOF
    WriteAccessor<Data<VecCoord3> > x = *DOF->write(VecId::position());
    x[0] = Coord3(0,10,0);
    x[1] = Coord3(10,0,0);
    x[2] = Coord3(-10*0.5,0,10*0.866);
    x[3] = Coord3(-10*0.5,0,-10*0.866);
    DOF->showObject.setValue(true);
    DOF->showObjectScale.setValue(10.);

    // Tetrahedron uniform mass
    UniformMass3::SPtr mass = sofa::core::objectmodel::New<UniformMass3>();
    groot->addObject(mass);
    mass->setMass(2);
    mass->setName("mass");

    // Tetrahedron topology
    MeshTopology::SPtr topology = sofa::core::objectmodel::New<MeshTopology>();
    topology->setName("mesh topology");
    groot->addObject( topology );
    topology->addTetra(0,1,2,3);

    // Tetrahedron constraints
    FixedConstraint3::SPtr constraints = sofa::core::objectmodel::New<FixedConstraint3>();
    constraints->setName("constraints");
    groot->addObject(constraints);
    constraints->addConstraint(0);

    // Tetrahedron force field
    TetrahedronFEMForceField3::SPtr fem = sofa::core::objectmodel::New<TetrahedronFEMForceField3>();
    fem->setName("FEM");
    groot->addObject(fem);
    fem->setMethod("polar");
    fem->setUpdateStiffnessMatrix(true);
    fem->setYoungModulus(6);

    // Tetrahedron skin
    Node::SPtr skin = groot.get()->createChild("skin");
    // The visual model
    OglModel::SPtr visual = sofa::core::objectmodel::New<OglModel>();
    visual->setName( "visual" );
    visual->load(sofa::helper::system::DataRepository.getFile("mesh/liver-smooth.obj"), "", "");
    visual->setColor("red");
    visual->applyScale(0.7, 0.7, 0.7);
    visual->applyTranslation(1.2, 0.8, 0);
    skin->addObject(visual);

    // The mapping between the tetrahedron (DOF) and the liver (visual)
    BarycentricMapping3_to_Ext3::SPtr mapping = sofa::core::objectmodel::New<BarycentricMapping3_to_Ext3>();
    mapping->setModels(DOF.get(), visual.get());
    mapping->setName( "mapping" );
    skin->addObject(mapping);

    // Display Flags
    sofa::component::visualmodel::VisualStyle::SPtr style = sofa::core::objectmodel::New<sofa::component::visualmodel::VisualStyle>();
    groot->addObject(style);
    sofa::core::visual::DisplayFlags& flags = *style->displayFlags.beginEdit();
    flags.setShowNormals(true);
    flags.setShowInteractionForceFields(true);
    flags.setShowMechanicalMappings(true);
    flags.setShowCollisionModels(true);
    flags.setShowBoundingCollisionModels(true);
    flags.setShowMappings(true);
    flags.setShowForceFields(true);
    flags.setShowWireFrame(true);
    flags.setShowVisualModels(true);
    flags.setShowBehaviorModels(true);
    style->displayFlags.endEdit();

    return groot;
}
