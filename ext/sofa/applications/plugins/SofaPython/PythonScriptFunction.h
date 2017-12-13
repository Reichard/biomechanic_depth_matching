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
#ifndef PYTHONSCRIPTFUNCTION_H
#define PYTHONSCRIPTFUNCTION_H

#include "PythonMacros.h"
#include <SofaPython/config.h>
#include "ScriptFunction.h"


namespace sofa
{

namespace core
{

namespace objectmodel
{

class SOFA_SOFAPYTHON_API PythonScriptFunctionParameter : public ScriptFunctionParameter
{
public:
	PythonScriptFunctionParameter();
	explicit PythonScriptFunctionParameter(PyObject* data, bool own);
	virtual ~PythonScriptFunctionParameter();

	PyObject* data() const					{return m_pyData;}
	void setData(PyObject* data, bool own)	{m_pyData = data; m_own = own;}

private:
	bool		m_own;
	PyObject*	m_pyData;

};

class SOFA_SOFAPYTHON_API PythonScriptFunctionResult : public ScriptFunctionResult
{
public:
	PythonScriptFunctionResult();
	explicit PythonScriptFunctionResult(PyObject* data, bool own);
	virtual ~PythonScriptFunctionResult();

	PyObject* data() const					{return m_pyData;}
	void setData(PyObject* data, bool own)	{m_pyData = data; m_own = own;}

private:
	bool		m_own;
	PyObject*	m_pyData;

};

class SOFA_SOFAPYTHON_API PythonScriptFunction : public ScriptFunction
{
public:
	explicit PythonScriptFunction(PyObject* pyCallableObject, bool own);
	virtual ~PythonScriptFunction();

	PyObject* callableObject() const					{return m_pyCallableObject;}
	void setCallableObject(PyObject* callableObject)	{m_pyCallableObject = callableObject;}

private:
	virtual void onCall(const ScriptFunctionParameter*, ScriptFunctionResult*) const;

private:
	bool		m_own;
	PyObject*	m_pyCallableObject;

};


} // namespace objectmodel

} // namespace core

} // namespace sofa

#endif // SCRIPTCONTROLLER_H
