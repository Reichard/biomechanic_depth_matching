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

#include <sofa/core/behavior/ForceField.inl>

#include "../types/StrainTypes.h"
#include "../types/DeformationGradientTypes.h"

namespace sofa
{
namespace core
{
namespace behavior
{

using namespace defaulttype;

template class SOFA_Flexible_API ForceField< E331Types >;
template class SOFA_Flexible_API ForceField< E321Types >;
template class SOFA_Flexible_API ForceField< E311Types >;
template class SOFA_Flexible_API ForceField< E332Types >;
template class SOFA_Flexible_API ForceField< E333Types >;
template class SOFA_Flexible_API ForceField< E221Types >;

template class SOFA_Flexible_API ForceField< U331Types >;
template class SOFA_Flexible_API ForceField< U321Types >;

template class SOFA_Flexible_API ForceField< I331Types >;

template class SOFA_Flexible_API ForceField< F331Types >;

}
}
}
