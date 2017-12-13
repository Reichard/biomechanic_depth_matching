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
#define SOFA_HELPER_KDTREE_CPP
#include "kdTree.inl"

#include <sofa/defaulttype/VecTypes.h>

namespace sofa
{

namespace helper
{

#ifndef SOFA_FLOAT
template class SOFA_HELPER_API kdTree<sofa::defaulttype::Vec2d>;
template class SOFA_HELPER_API kdTree<sofa::defaulttype::Vec3d>;
#endif

#ifndef SOFA_DOUBLE
template class SOFA_HELPER_API kdTree<sofa::defaulttype::Vec2f>;
template class SOFA_HELPER_API kdTree<sofa::defaulttype::Vec3f>;
#endif

}

}
