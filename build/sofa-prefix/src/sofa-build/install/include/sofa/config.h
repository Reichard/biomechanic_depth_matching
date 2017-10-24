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
#ifndef SOFA_CONFIG_H
#define SOFA_CONFIG_H

#define SOFA_HAVE_ZLIB

#define SOFA_HAVE_PNG

#define SOFA_HAVE_GLEW

#define SOFA_HAVE_GLUT

#define SOFA_HAVE_BOOST

#define SOFA_HAVE_BOOST_THREAD

#define SOFA_HAVE_BOOST_SYSTEM

#define SOFA_HAVE_BOOST_DATE_TIME

#define SOFA_HAVE_BOOST_CHRONO

#define SOFA_HAVE_TINYXML

/* #undef SOFA_DUMP_VISITOR_INFO */

/* #undef SOFA_NO_OPENGL */

/* #undef SOFA_NO_UPDATE_BBOX */

/* #undef DETECTIONOUTPUT_FREEMOTION */

/* #undef DETECTIONOUTPUT_BARYCENTRICINFO */

/* #undef SOFA_FLOAT */

/* #undef SOFA_DOUBLE */

#ifdef SOFA_FLOAT
typedef float SReal;
#else
typedef double SReal;
#endif

// The SOFA_EXTERN_TEMPLATE macro controls the use of extern templates in Sofa.
// That is, every extern template declaration in Sofa is inside a block
// protected by
//
//   #if defined(SOFA_EXTERN_TEMPLATE)
//
// Not only are extern template declarations necessary for DLLs on windows, but
// they can also fix some RTTI issues on OS X (typeid, dynamic_cast), and they
// hugely speed up the build process on every platform, so they are enabled by
// default. (Even though they were not part of C++ until C++11, they are
// supported by all majors compilers.)
#ifndef SOFA_NO_EXTERN_TEMPLATE
#  define SOFA_EXTERN_TEMPLATE
#endif

#ifndef WIN32
#	define SOFA_EXPORT_DYNAMIC_LIBRARY
#   define SOFA_IMPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_EXPORT_DYNAMIC_LIBRARY __declspec( dllexport )
#   define SOFA_IMPORT_DYNAMIC_LIBRARY __declspec( dllimport )
#   ifdef _MSC_VER
#       pragma warning(disable : 4231)
#       pragma warning(disable : 4910)
#   endif
#endif

#ifdef SOFA_BUILD_HELPER
#   define SOFA_TARGET SofaHelper
#	define SOFA_HELPER_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_HELPER_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_DEFAULTTYPE
#   define SOFA_TARGET SofaDefaulttype
#	define SOFA_DEFAULTTYPE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_DEFAULTTYPE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#ifdef SOFA_BUILD_CORE
#   define SOFA_TARGET   SofaCore
#	define SOFA_CORE_API SOFA_EXPORT_DYNAMIC_LIBRARY
#else
#	define SOFA_CORE_API SOFA_IMPORT_DYNAMIC_LIBRARY
#endif

#endif
