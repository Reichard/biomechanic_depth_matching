/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
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
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define SOFA_COMPONENT_ENGINE_BOXROI_CPP
#include <BoxROIQuadraticTetras.inl>
#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/Vec3Types.h>

namespace sofa
{

namespace component
{

namespace engine
{

SOFA_DECL_CLASS(BoxROIQuadraticTetras)

int BoxROIQuadraticTetrasClass = core::RegisterObject("Find the primitives (vertex/quadratictetrahedron) inside a given box")
#ifndef SOFA_FLOAT
        .add< BoxROIQuadraticTetras<Vec3dTypes> >()
        .add< BoxROIQuadraticTetras<Rigid3dTypes> >()
        .add< BoxROIQuadraticTetras<Vec6dTypes> >() //Phuoc
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
        .add< BoxROIQuadraticTetras<Vec3fTypes> >()
        .add< BoxROIQuadraticTetras<Rigid3fTypes> >()
        .add< BoxROIQuadraticTetras<Vec6fTypes> >() //Phuoc
#endif //SOFA_DOUBLE
        ;

#ifndef SOFA_FLOAT
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Vec3dTypes>;
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Rigid3dTypes>;
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Vec6dTypes>; //Phuoc
#endif //SOFA_FLOAT
#ifndef SOFA_DOUBLE
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Vec3fTypes>;
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Rigid3fTypes>;
template class SOFA_ENGINE_API BoxROIQuadraticTetras<Vec6fTypes>; //Phuoc
#endif //SOFA_DOUBLE


} // namespace constraint

} // namespace component

} // namespace sofa

