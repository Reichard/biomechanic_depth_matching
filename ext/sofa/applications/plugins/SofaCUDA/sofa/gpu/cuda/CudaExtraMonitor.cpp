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
#include "CudaTypes.h"

#include <SofaValidation/ExtraMonitor.h>
#include <SofaValidation/ExtraMonitor.inl>
#include <SofaValidation/Monitor.inl>

#include <sofa/core/ObjectFactory.h>
#include <sofa/defaulttype/RigidTypes.h>

namespace sofa
{

namespace component
{

namespace misc
{

template class ExtraMonitor<gpu::cuda::CudaRigid3fTypes>;
template class ExtraMonitor<gpu::cuda::CudaVec6fTypes>;
#ifdef SOFA_GPU_CUDA_DOUBLE
template class ExtraMonitor<gpu::cuda::CudaRigid3dTypes>;
template class ExtraMonitor<gpu::cuda::CudaVec6dTypes>;
#endif // SOFA_GPU_CUDA_DOUBLE

}// namespace misc

}// namespace component


namespace gpu
{

namespace cuda
{

SOFA_DECL_CLASS(CudaExtraMonitor)

int ExtraMonitorCudaClass = core::RegisterObject("Supports GPU-side computation using CUDA")
        .add< component::misc::ExtraMonitor<CudaRigid3fTypes> >()
        .add< component::misc::ExtraMonitor<CudaVec6fTypes> >()
#ifdef SOFA_GPU_CUDA_DOUBLE
        .add< component::misc::ExtraMonitor<CudaRigid3dTypes> >()
        .add< component::misc::ExtraMonitor<CudaVec6dTypes> >()
#endif // SOFA_GPU_CUDA_DOUBLE
        ;

}// namespace cuda

}// namespace gpu

}// namespace sofa
