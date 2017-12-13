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
#include "PythonScriptFunction.h"

namespace sofa
{

namespace core
{

namespace objectmodel
{

PythonScriptFunctionParameter::PythonScriptFunctionParameter() : ScriptFunctionParameter(),
	m_own(false),
	m_pyData(0)
{

}

PythonScriptFunctionParameter::PythonScriptFunctionParameter(PyObject* data, bool own) : ScriptFunctionParameter(),
	m_own(own),
	m_pyData(data)
{
    
}

PythonScriptFunctionParameter::~PythonScriptFunctionParameter()
{

}

PythonScriptFunctionResult::PythonScriptFunctionResult() : ScriptFunctionResult(),
	m_own(false),
	m_pyData(0)
{
	if(m_own && m_pyData)
		Py_DECREF(m_pyData);
}

PythonScriptFunctionResult::PythonScriptFunctionResult(PyObject* data, bool own) : ScriptFunctionResult(),
	m_own(own),
	m_pyData(data)
{

}

PythonScriptFunctionResult::~PythonScriptFunctionResult()
{
	if(m_own && m_pyData)
		Py_DECREF(m_pyData);
}

PythonScriptFunction::PythonScriptFunction(PyObject* pyCallableObject, bool own) : ScriptFunction(),
	m_own(own),
	m_pyCallableObject(pyCallableObject)
{
	
}

PythonScriptFunction::~PythonScriptFunction()
{
	if(m_own && m_pyCallableObject)
		Py_DECREF(m_pyCallableObject);
}

void PythonScriptFunction::onCall(const ScriptFunctionParameter* parameter, ScriptFunctionResult* result) const
{
	if(!m_pyCallableObject)
		return;

	const PythonScriptFunctionParameter* pythonScriptParameter = dynamic_cast<const PythonScriptFunctionParameter*>(parameter);
	PythonScriptFunctionResult* pythonScriptResult = dynamic_cast<PythonScriptFunctionResult*>(result);

	PyObject* pyParameter = 0;
	if(pythonScriptParameter)
		pyParameter = pythonScriptParameter->data();

	PyObject* pyResult = PyObject_CallObject(m_pyCallableObject, pyParameter);
	if(!pyResult)
	{
		SP_MESSAGE_EXCEPTION("in PythonScriptFunction: python function call failed")
		PyErr_Print();
	}
	else if(pythonScriptResult)
		pythonScriptResult->setData(pyResult, true);
}

} // namespace objectmodel

} // namespace core

} // namespace sofa



