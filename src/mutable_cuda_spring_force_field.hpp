#pragma once

#include <sofa/gpu/cuda/CudaSpringForceField.h>

class MutableCudaSpringForceField : 
	public sofa::component::interactionforcefield::StiffSpringForceField<sofa::gpu::cuda::CudaVec3fTypes>
{
	public:
		SOFA_CLASS(MutableCudaSpringForceField, 
				SOFA_TEMPLATE(sofa::component::interactionforcefield::StiffSpringForceField,
					sofa::gpu::cuda::CudaVec3fTypes));

		sofa::component::interactionforcefield::SpringForceFieldInternalData
			<sofa::gpu::cuda::CudaVec3fTypes>& internal_data()
			{
				return data;
			}
};