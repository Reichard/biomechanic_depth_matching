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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include "PythonMacros.h"
#include "PythonEnvironment.h"
#include "SceneLoaderPY.h"
#include "ScriptEnvironment.h"


#include <sofa/simulation/common/Simulation.h>
#include <sofa/simulation/common/xml/NodeElement.h>
#include <sofa/simulation/common/FindByTypeVisitor.h>

#include <sstream>

#include "PythonMainScriptController.h"
#include "PythonEnvironment.h"

using namespace sofa::core::objectmodel;

namespace sofa
{

namespace simulation
{

std::string SceneLoaderPY::OurHeader;

void SceneLoaderPY::setHeader(const std::string& header)
{
    OurHeader = header;
}

bool SceneLoaderPY::canLoadFileExtension(const char *extension)
{
    std::string ext = extension;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return (ext=="py" || ext=="pyscn");
}

bool SceneLoaderPY::canWriteFileExtension(const char *extension)
{
    return canLoadFileExtension(extension);
}

/// get the file type description
std::string SceneLoaderPY::getFileTypeDesc()
{
    return "Python Scenes";
}

/// get the list of file extensions
void SceneLoaderPY::getExtensionList(ExtensionList* list)
{
    list->clear();
    list->push_back("pyscn");
    list->push_back("py");
}

sofa::simulation::Node::SPtr SceneLoaderPY::load(const char *filename)
{
    return loadSceneWithArguments(filename);
}

sofa::simulation::Node::SPtr SceneLoaderPY::loadSceneWithArguments(const char *filename, const std::vector<std::string>& arguments)
{
    if(!OurHeader.empty() && 0 != PyRun_SimpleString(OurHeader.c_str()))
    {
        SP_MESSAGE_ERROR( "header script run error." )
        return NULL;
    }

    PythonEnvironment::runString("createScene=None");
    PythonEnvironment::runString("createSceneAndController=None");

    PythonEnvironment::runString(std::string("__file__=\"") + filename + "\"");

    // We go the the current file's directory so that all relative path are correct
    helper::system::SetDirectory chdir ( filename );

    if(!PythonEnvironment::runFile(helper::system::SetDirectory::GetFileName(filename).c_str(), arguments))
    {
        // LOAD ERROR
        SP_MESSAGE_ERROR( "scene script load error." )
        return NULL;
    }

    PyObject* pDict = PyModule_GetDict(PyImport_AddModule("__main__"));

    // pFunc is also a borrowed reference
    PyObject *pFunc = PyDict_GetItemString(pDict, "createScene");
    if (PyCallable_Check(pFunc))
    {
        Node::SPtr rootNode = getSimulation()->createNewGraph("root");
        ScriptEnvironment::enableNodeQueuedInit(false);
        SP_CALL_MODULEFUNC(pFunc, "(O)", SP_BUILD_PYSPTR(rootNode.get()))
        ScriptEnvironment::enableNodeQueuedInit(true);

        return rootNode;
    }
    else
    {
        PyObject *pFunc = PyDict_GetItemString(pDict, "createSceneAndController");
        if (PyCallable_Check(pFunc))
        {
            Node::SPtr rootNode = getSimulation()->createNewGraph("root");
            ScriptEnvironment::enableNodeQueuedInit(false);
            SP_CALL_MODULEFUNC(pFunc, "(O)", SP_BUILD_PYSPTR(rootNode.get()))
            ScriptEnvironment::enableNodeQueuedInit(true);

            rootNode->addObject( core::objectmodel::New<component::controller::PythonMainScriptController>( filename ) );

            return rootNode;
        }
    }

    SP_MESSAGE_ERROR( "cannot create Scene, no \"createScene(rootNode)\" nor \"createSceneAndController(rootNode)\" module method found." )
    return NULL;
}


bool SceneLoaderPY::loadTestWithArguments(const char *filename, const std::vector<std::string>& arguments)
{
    if(!OurHeader.empty() && 0 != PyRun_SimpleString(OurHeader.c_str()))
    {
        SP_MESSAGE_ERROR( "header script run error." )
        return false;
    }

    PythonEnvironment::runString("createScene=None");
    PythonEnvironment::runString("createSceneAndController=None");

    PythonEnvironment::runString(std::string("__file__=\"") + filename + "\"");

    // it runs the unecessary SofaPython script but it is not a big deal
    if(!PythonEnvironment::runFile(filename,arguments))
    {
        // LOAD ERROR
        SP_MESSAGE_ERROR( "script load error." )
        return false;
    }

    PyObject* pDict = PyModule_GetDict(PyImport_AddModule("__main__"));

    // pFunc is also a borrowed reference
    PyObject *pFunc = PyDict_GetItemString(pDict, "run");
    if (PyCallable_Check(pFunc))
    {
        ScriptEnvironment::enableNodeQueuedInit(false);

        PyObject *res = PyObject_CallObject(pFunc,0);
        printPythonExceptions();

        if( !res )
        {
            SP_MESSAGE_ERROR( "Python test 'run' function does not return any value" )
            return false;
        }
        else if( !PyBool_Check(res) )
        {
            SP_MESSAGE_ERROR( "Python test 'run' function does not return a boolean" )
            Py_DECREF(res);
            return false;
        }

        bool result = ( res == Py_True );
        Py_DECREF(res);


        ScriptEnvironment::enableNodeQueuedInit(true);
        return result;
    }
    else
    {
        SP_MESSAGE_ERROR( "Python test has no 'run'' function" )
        return false;
    }
}



void SceneLoaderPY::write(Node* node, const char *filename)
{
    exportPython( node, filename );
}


//////////////////////////////////////////////////////////////////////////////



static const std::string s_tab = "    ";


inline void printBaseHeader( std::ostream& out, Node* node )
{
    out << "import Sofa\n\n\n";
    out << "def createScene(root):\n";

    out << s_tab << "root.dt = " << node->getDt() << std::endl;
    const Context::Vec3& g = node->getGravity();
    out << s_tab << "root.gravity = [" << g[0] << "," << g[1] << "," << g[2] << "]" << std::endl;
}


void exportPython( Node* node, const char* fileName )
{
    if ( !node ) return;

    if ( fileName!=NULL )
    {
        std::ofstream out( fileName );

        printBaseHeader( out, node );

        PythonExporterVisitor print( out );
        node->execute( print );
    }
    else
    {
        printBaseHeader( std::cout, node );

        PythonExporterVisitor print( std::cout );
        node->execute( print );
    }
}




///////////////////////////////////////////////////////////////////////////////




template<class T>
void PythonExporterVisitor::processObject( T obj, const std::string& nodeVariable )
{
    std::string classname = obj->getClassName();
    std::string templatename = obj->getTemplateName();

    m_out << s_tab << nodeVariable << ".createObject( '" << classname <<"', template='" << templatename << "'";

    obj->writeDatas( m_out, ", " );

    m_out << ")" << std::endl;
}


Visitor::Result PythonExporterVisitor::processNodeTopDown(Node* node)
{
    m_out << "\n\n";

    m_variableIndex++;

    sofa::helper::vector< core::objectmodel::BaseNode* > parents = node->getParents();


    std::string nodeName = node->getName();
    std::stringstream nodeVariable;


    if( !parents.empty() ) // not root
    {
        nodeVariable << nodeName << "_Node" << m_variableIndex;
        m_mapNodeVariable[node] = nodeVariable.str();

        const std::string& parentVariable = m_mapNodeVariable[parents[0]];

        m_out << s_tab << nodeVariable.str() << " = "<<parentVariable<<".createChild( '"<<nodeName <<"' )" << std::endl;
    }
    else
    {
        nodeVariable << "root";
        m_mapNodeVariable[node] = nodeVariable.str();
    }


    for (Node::ObjectIterator it = node->object.begin(); it != node->object.end(); ++it)
    {
        sofa::core::objectmodel::BaseObject* obj = it->get();
        this->processObject( obj, nodeVariable.str() );
    }

    return RESULT_CONTINUE;
}

void PythonExporterVisitor::processNodeBottomUp(Node* node)
{
    sofa::helper::vector< core::objectmodel::BaseNode* > parents = node->getParents();

    const std::string& nodeVariable = m_mapNodeVariable[node];


    // add all its parents to a multinode
    for( size_t i = 1 ; i<parents.size() ; ++ i)
    {
        const std::string& parentVariable = m_mapNodeVariable[parents[i]];

        m_out << s_tab << parentVariable << ".addChild(" << nodeVariable << ")\n";
    }
}


} // namespace simulation

} // namespace sofa

