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

#ifndef SOFA_SIMPLEOBJECTCREATOR_H
#define SOFA_SIMPLEOBJECTCREATOR_H

#include <SceneCreator/config.h>
#include <string>
#include <sofa/simulation/common/Simulation.h>
#include <sofa/simulation/common/Node.h>
#include <sofa/core/objectmodel/BaseData.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <SofaBaseLinearSolver/FullVector.h>
#include <SofaEigen2Solver/EigenSparseMatrix.h>

namespace sofa
{

namespace modeling {

typedef SReal Scalar;
typedef sofa::defaulttype::Vec3Types::Deriv Deriv3;
typedef sofa::defaulttype::Vec<3,SReal> Vec3;
typedef sofa::defaulttype::Vec<1,SReal> Vec1;
typedef component::container::MechanicalObject<defaulttype::Vec3Types> MechanicalObject3;
typedef component::container::MechanicalObject<defaulttype::Rigid3Types> MechanicalObjectRigid3;

SOFA_SceneCreator_API simulation::Node::SPtr createRootWithCollisionPipeline(const std::string &responseType=std::string("default"));
SOFA_SceneCreator_API simulation::Node::SPtr createEulerSolverNode(simulation::Node::SPtr parent, const std::string& name, const std::string &integrationScheme=std::string("Implicit"));


SOFA_SceneCreator_API simulation::Node::SPtr createObstacle(simulation::Node::SPtr parent, const std::string &filenameCollision, const std::string filenameVisual, const std::string& color,
                                                            const Deriv3& translation=Deriv3(), const Deriv3 &rotation=Deriv3());

//Create a collision node using Barycentric Mapping, using a 3d model specified by filename.
//elements is a vector of type of collision models (Triangle, Line, Point, Sphere)
//an initial transformation can be performed
SOFA_SceneCreator_API simulation::Node::SPtr createCollisionNodeVec3(simulation::Node::SPtr parent, MechanicalObject3::SPtr dof, const std::string &filename, const std::vector<std::string> &elements,
                                                                     const Deriv3& translation=Deriv3(), const Deriv3 &rotation=Deriv3());
SOFA_SceneCreator_API simulation::Node::SPtr createVisualNodeVec3(simulation::Node::SPtr parent, MechanicalObject3::SPtr dof,  const std::string &filename, const std::string& color,
                                                                  const Deriv3& translation=Deriv3(), const Deriv3 &rotation=Deriv3());



//Create a collision node using Rigid Mapping, using a 3d model specified by filename.
//elements is a vector of type of collision models (Triangle, Line, Point, Sphere)
//an initial transformation can be performed
SOFA_SceneCreator_API simulation::Node::SPtr createCollisionNodeRigid(simulation::Node::SPtr parent, MechanicalObjectRigid3::SPtr dofRigid,  const std::string &filename, const std::vector<std::string> &elements,
                                                                      const Deriv3& translation=Deriv3(), const Deriv3 &rotation=Deriv3());
SOFA_SceneCreator_API simulation::Node::SPtr createVisualNodeRigid(simulation::Node::SPtr parent, MechanicalObjectRigid3::SPtr  dofRigid,  const std::string &filename, const std::string& color,
                                                                   const Deriv3& translation=Deriv3(), const Deriv3 &rotation=Deriv3());

SOFA_SceneCreator_API simulation::Node::SPtr createGridScene(Vec3 startPoint, Vec3 endPoint, unsigned numX, unsigned numY, unsigned numZ, double totalMass, double stiffnessValue=1.0, double dampingRatio=0 );


SOFA_SceneCreator_API void addCollisionModels(simulation::Node::SPtr CollisionNode, const std::vector<std::string> &elements);


#ifndef SOFA_FLOAT
typedef Eigen::VectorXd Vector;
#else
typedef Eigen::VectorXf Vector;
#endif
typedef Eigen::Matrix<SReal, Eigen::Dynamic,Eigen::Dynamic> DenseMatrix;
typedef Eigen::SparseMatrix<SReal, Eigen::RowMajor> SparseMatrix;


/// Dense state vector deriving from BaseVector, used to access data in the scene graph
typedef component::linearsolver::FullVector<SReal> FullVector;



/** Create a string composed of particles (at least 2) and springs */
SOFA_SceneCreator_API simulation::Node::SPtr massSpringString(
        simulation::Node::SPtr parent,
        double x0, double y0, double z0, // start point,
        double x1, double y1, double z1, // end point
        unsigned numParticles,
        double totalMass,
        double stiffnessValue=1.0,
        double dampingRatio=0
        );


/** Helper class to create a component and add it as a child of a given Node */
template<class T>
class addNew : public core::objectmodel::New<T>
{
    typedef typename T::SPtr SPtr;
public:
    addNew( simulation::Node::SPtr parent, const char* name="")
    {
        parent->addObject(*this);
        (*this)->setName(name);
    }
};


SOFA_SceneCreator_API simulation::Node::SPtr getRoot();

/// Get a state vector from the scene graph. Includes only the independent state values, or also the mapped ones, depending on the flag.
SOFA_SceneCreator_API Vector getVector( core::ConstVecId id, bool independentOnly=true );

/** Initialize the sofa library and create the root of the scene graph
  */
SOFA_SceneCreator_API simulation::Node::SPtr initSofa();

/** Initialize the scene graph
  */
SOFA_SceneCreator_API void initScene();

/// Clear the scene graph and return a pointer to the new root
SOFA_SceneCreator_API simulation::Node::SPtr clearScene();

/// Create a link from source to target.  
SOFA_SceneCreator_API void setDataLink(core::objectmodel::BaseData* source, core::objectmodel::BaseData* target);



}// modeling

}// sofa

#endif
