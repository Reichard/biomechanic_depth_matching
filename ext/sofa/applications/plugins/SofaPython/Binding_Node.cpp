/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
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
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "PythonMacros.h"
#include <sofa/simulation/common/Node.h>
#include <sofa/simulation/common/Simulation.h>
#include <sofa/core/objectmodel/KeypressedEvent.h>
#include <sofa/core/objectmodel/KeyreleasedEvent.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include "ScriptEnvironment.h"
using namespace sofa::simulation;
#include <sofa/core/ExecParams.h>
using namespace sofa::core;
using namespace sofa::core::objectmodel;
#include "Binding_Node.h"
#include "Binding_Context.h"
#include "PythonVisitor.h"
#include "PythonScriptEvent.h"

extern "C" PyObject * Node_executeVisitor(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    PyObject* pyVisitor;
    if (!PyArg_ParseTuple(args, "O",&pyVisitor))
        Py_RETURN_NONE;
    PythonVisitor visitor(ExecParams::defaultInstance(),pyVisitor);
    node->executeVisitor(&visitor);

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_getRoot(PyObject *self, PyObject * /*args*/)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    // BaseNode is not binded in SofaPython, so getRoot is binded in Node instead of BaseNode
    return SP_BUILD_PYSPTR(node->getRoot());
}

// step the simulation
extern "C" PyObject * Node_simulationStep(PyObject * self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    double dt;
    if (!PyArg_ParseTuple(args, "d",&dt))
        Py_RETURN_NONE;

    printf("Node_simulationStep node=%s dt=%f\n",node->getName().c_str(),(float)dt);

    getSimulation()->animate ( node, (SReal)dt );
//    simulation::getSimulation()->updateVisual( root );


    Py_RETURN_NONE;
}

// reset a node
extern "C" PyObject * Node_reset(PyObject * self, PyObject * /*args*/)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    getSimulation()->reset(node);

    Py_RETURN_NONE;
}

// init a node
extern "C" PyObject * Node_init(PyObject * self, PyObject * /*args*/)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    getSimulation()->init(node);

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_getChild(PyObject * self, PyObject * args)
{
    // BaseNode is not binded in SofaPython, so getChildNode is binded in Node instead of BaseNode
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    char *path;
    if (!PyArg_ParseTuple(args, "s",&path))
        Py_RETURN_NONE;
    if (!node || !path)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }

    const objectmodel::BaseNode::Children& children = node->getChildren();
    Node *childNode = 0;
    // BaseNode ne pouvant pas être bindé en Python, et les BaseNodes des graphes étant toujours des Nodes,
    // on caste directement en Node.
    for (unsigned int i=0; i<children.size(); ++i)
        if (children[i]->getName() == path)
        {
            childNode = down_cast<Node>(children[i]);
            break;
        }
    if (!childNode)
    {
        SP_MESSAGE_ERROR( "Node.getChildNode("<<path<<") not found.")
        Py_RETURN_NONE;
    }
    return SP_BUILD_PYSPTR(childNode);
}

extern "C" PyObject * Node_getChildren(PyObject * self, PyObject * /*args*/)
{
    // BaseNode is not binded in SofaPython, so getChildNode is binded in Node instead of BaseNode
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    const objectmodel::BaseNode::Children& children = node->getChildren();

    // BaseNode ne pouvant pas être bindé en Python, et les BaseNodes des graphes étant toujours des Nodes,
    // on caste directement en Node.
    PyObject *list = PyList_New(children.size());

    for (unsigned int i=0; i<children.size(); ++i)
        PyList_SetItem(list,i,SP_BUILD_PYSPTR(children[i]));

    return list;
}

extern "C" PyObject * Node_getParents(PyObject * self, PyObject * /*args*/)
{
    // BaseNode is not binded in SofaPython, so getChildNode is binded in Node instead of BaseNode
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    const objectmodel::BaseNode::Children& parents = node->getParents();

    // BaseNode ne pouvant pas être bindé en Python, et les BaseNodes des graphes étant toujours des Nodes,
    // on caste directement en Node.
    PyObject *list = PyList_New(parents.size());

    for (unsigned int i=0; i<parents.size(); ++i)
        PyList_SetItem(list,i,SP_BUILD_PYSPTR(parents[i]));

    return list;
}

extern "C" PyObject * Node_getPathName(PyObject * self, PyObject * /*args*/)
{
    // BaseNode is not binded in SofaPython, so getPathName is binded in Node instead
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    return PyString_FromString(node->getPathName().c_str());
}

// the same as 'getPathName' with a extra prefix '@'
extern "C" PyObject * Node_getLinkPath(PyObject * self, PyObject * /*args*/)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    return PyString_FromString(("@"+node->getPathName()).c_str());
}

extern "C" PyObject * Node_createChild(PyObject *self, PyObject * args)
{
    Node* obj=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    char *nodeName;
    if (!PyArg_ParseTuple(args, "s",&nodeName))
        Py_RETURN_NONE;
    Node* child = obj->createChild(nodeName).get();
    ScriptEnvironment::nodeCreatedByScript(child);
    return SP_BUILD_PYSPTR(child);
}

extern "C" PyObject * Node_addObject(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyChild;
    if (!PyArg_ParseTuple(args, "O",&pyChild))
        Py_RETURN_NONE;
    BaseObject* object=((PySPtr<Base>*)pyChild)->object->toBaseObject();
    if (!object)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    node->addObject(object);

    if (node->isInitialized())
        SP_MESSAGE_WARNING( "Sofa.Node.addObject called on a node("<<node->getName()<<") that is already initialized" )
//    if (!ScriptEnvironment::isNodeCreatedByScript(node))
//        SP_MESSAGE_WARNING( "Sofa.Node.addObject called on a node("<<node->getName()<<") that is not created by the script" )

    //object->init();
    // plus besoin !! node->init(sofa::core::ExecParams::defaultInstance());

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_removeObject(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyChild;
    if (!PyArg_ParseTuple(args, "O",&pyChild))
        Py_RETURN_NONE;
    BaseObject* object=((PySPtr<Base>*)pyChild)->object->toBaseObject();
    if (!object)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    node->removeObject(object);

    // no init, if you need to init, you can call it yourself!
//    node->init(sofa::core::ExecParams::defaultInstance());

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_addChild(PyObject *self, PyObject * args)
{
    Node* obj=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyChild;
    if (!PyArg_ParseTuple(args, "O",&pyChild))
        Py_RETURN_NONE;
    BaseNode* child=((PySPtr<Base>*)pyChild)->object->toBaseNode();
    if (!child)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    obj->addChild(child);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_removeChild(PyObject *self, PyObject * args)
{
    Node* obj=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyChild;
    if (!PyArg_ParseTuple(args, "O",&pyChild))
        Py_RETURN_NONE;
    BaseNode* child=((PySPtr<Base>*)pyChild)->object->toBaseNode();
    if (!child)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    obj->removeChild(child);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_moveChild(PyObject *self, PyObject * args)
{
    Node* obj=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyChild;
    if (!PyArg_ParseTuple(args, "O",&pyChild))
        Py_RETURN_NONE;
    BaseNode* child=((PySPtr<Base>*)pyChild)->object->toBaseNode();
    if (!child)
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    obj->moveChild(child);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_detachFromGraph(PyObject *self, PyObject * /*args*/)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    node->detachFromGraph();
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_sendScriptEvent(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    PyObject* pyUserData;
    char* eventName;
    if (!PyArg_ParseTuple(args, "sO",&eventName,&pyUserData))
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    PythonScriptEvent event(node,eventName,pyUserData);
    down_cast<Node>(node->getRoot())->propagateEvent(sofa::core::ExecParams::defaultInstance(), &event);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_sendKeypressedEvent(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    char* eventName;
    if (!PyArg_ParseTuple(args, "s",&eventName))
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    sofa::core::objectmodel::KeypressedEvent event(eventName ? eventName[0] : '\0');
    down_cast<Node>(node->getRoot())->propagateEvent(sofa::core::ExecParams::defaultInstance(), &event);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_sendKeyreleasedEvent(PyObject *self, PyObject * args)
{
    Node* node=down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());
    char* eventName;
    if (!PyArg_ParseTuple(args, "s",&eventName))
    {
        PyErr_BadArgument();
        Py_RETURN_NONE;
    }
    sofa::core::objectmodel::KeyreleasedEvent event(eventName ? eventName[0] : '\0');
    down_cast<Node>(node->getRoot())->propagateEvent(sofa::core::ExecParams::defaultInstance(), &event);
    Py_RETURN_NONE;
}

extern "C" PyObject * Node_getMechanicalState(PyObject * self, PyObject * /*args*/)
{
    Node* node = down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    const behavior::BaseMechanicalState* state = node->mechanicalState.get();

    if( state ) return SP_BUILD_PYSPTR((Base*)state);

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_getMechanicalMapping(PyObject * self, PyObject * /*args*/)
{
    Node* node = down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    const sofa::core::BaseMapping* mapping = node->mechanicalMapping.get();

    if( mapping ) return SP_BUILD_PYSPTR((Base*)mapping);

    Py_RETURN_NONE;
}

extern "C" PyObject * Node_propagatePositionAndVelocity(PyObject * self, PyObject * /*args*/)
{
    Node* node = down_cast<Node>(((PySPtr<Base>*)self)->object->toBaseNode());

    node->execute<MechanicalPropagatePositionAndVelocityVisitor>(sofa::core::MechanicalParams::defaultInstance());

    Py_RETURN_NONE;
}



SP_CLASS_METHODS_BEGIN(Node)
SP_CLASS_METHOD(Node,executeVisitor)
SP_CLASS_METHOD(Node,getRoot)
SP_CLASS_METHOD(Node,simulationStep)
SP_CLASS_METHOD(Node,reset)
SP_CLASS_METHOD(Node,init)
SP_CLASS_METHOD(Node,getChild)
SP_CLASS_METHOD(Node,getChildren)
SP_CLASS_METHOD(Node,getParents)
SP_CLASS_METHOD(Node,getPathName)
SP_CLASS_METHOD(Node,getLinkPath)
SP_CLASS_METHOD(Node,createChild)
SP_CLASS_METHOD(Node,addObject)
SP_CLASS_METHOD(Node,removeObject)
SP_CLASS_METHOD(Node,addChild)
SP_CLASS_METHOD(Node,removeChild)
SP_CLASS_METHOD(Node,moveChild)
SP_CLASS_METHOD(Node,detachFromGraph)
SP_CLASS_METHOD(Node,sendScriptEvent)
SP_CLASS_METHOD(Node,sendKeypressedEvent)
SP_CLASS_METHOD(Node,sendKeyreleasedEvent)
SP_CLASS_METHOD(Node,getMechanicalState)
SP_CLASS_METHOD(Node,getMechanicalMapping)
SP_CLASS_METHOD(Node,propagatePositionAndVelocity)
SP_CLASS_METHODS_END

SP_CLASS_TYPE_SPTR(Node,Node,Context)
