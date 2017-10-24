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
#ifndef SOFA_HELPER_SYSTEM_GL_H
#define SOFA_HELPER_SYSTEM_GL_H

#ifndef SOFA_NO_OPENGL

#include <sofa/helper/helper.h>
#include <sofa/helper/system/config.h>
#include <string>

#if defined(SOFA_HAVE_GLEW) && !defined(PS3)
#  include <GL/glew.h>
#elif defined(PS3)
#  include <sofa/helper/gl/ps3gl_compat.h>
#elif defined(__APPLE__)
#  include <OpenGL/gl.h>
#else
#  define GL_GLEXT_PROTOTYPES // for glext.h : necessary to use glBindBuffer without glew and make GLSLShader file
#  if defined(WIN32)
#    include <gl/GL.h>
#  else
#    include <GL/gl.h>
#    include <GL/glext.h> // necessary when you don't have glew
#  endif // WIN32
#endif

extern SOFA_HELPER_API const char* GetGlExtensionsList();

extern SOFA_HELPER_API bool CanUseGlExtension(const std::string& ext);

#endif // SOFA_NO_OPENGL

#endif
