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
#include <SceneCreator/SceneCreator.h>

#include <sofa/simulation/common/Node.h>

namespace sofa {

using namespace modeling;

struct Node_test : public Sofa_test<>
{
    Node_test()
    {
        /* create trivial DAG :
         *
         * R
         * |
         * A
         * |
         * B
         *
         */
        sofa::simulation::setSimulation(new simulation::graph::DAGSimulation());
        root = clearScene();
        root->setName("R");
        A = root->createChild("A");
        B = A->createChild("B");

    }

    simulation::Node::SPtr root;
    simulation::Node::SPtr A;
    simulation::Node::SPtr B;

};

TEST_F( Node_test, getPathName)
{
    EXPECT_EQ("", root->getPathName());
    EXPECT_EQ("/A/B", B->getPathName());
}

}// namespace sofa







