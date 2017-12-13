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
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#define FLEXIBLE_BarycentricShapeFunction_CPP

#include <Flexible/config.h>
#include "../shapeFunction/BarycentricShapeFunction.h"
#include <sofa/core/ObjectFactory.h>

namespace sofa
{
namespace component
{
namespace shapefunction
{

using namespace core::behavior;

SOFA_DECL_CLASS(BarycentricShapeFunction)

// Register in the Factory
int BarycentricShapeFunctionClass = core::RegisterObject("Computes Barycentric shape functions")
#ifndef SOFA_FLOAT
        .add< BarycentricShapeFunction<ShapeFunctiond> >(true)
        .add< BarycentricShapeFunction<ShapeFunction2d> >()
#endif
#ifndef SOFA_DOUBLE
        .add< BarycentricShapeFunction<ShapeFunctionf> >()
        .add< BarycentricShapeFunction<ShapeFunction2f> >()
#endif
        ;

#ifndef SOFA_FLOAT
template class SOFA_Flexible_API BarycentricShapeFunction<ShapeFunctiond>;
template class SOFA_Flexible_API BarycentricShapeFunction<ShapeFunction2d>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_Flexible_API BarycentricShapeFunction<ShapeFunctionf>;
template class SOFA_Flexible_API BarycentricShapeFunction<ShapeFunction2f>;
#endif

}
}
}
