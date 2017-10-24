#pragma once

#include <sofa/gpu/cuda/CudaTypes.h>
#include <SofaBaseMechanics/MechanicalObject.h>
	
class MyCudaMechanicalObject : public sofa::component::container::MechanicalObject<
							   sofa::gpu::cuda::CudaVec3fTypes>
{
	public:
		SOFA_CLASS(MyCudaMechanicalObject, 
				SOFA_TEMPLATE(sofa::component::container::MechanicalObject,
					sofa::gpu::cuda::CudaVec3fTypes));

		sofa::component::container::MechanicalObjectInternalData<
			sofa::gpu::cuda::CudaVec3fTypes> &internal_data()
			{
				return data;
			}

};
