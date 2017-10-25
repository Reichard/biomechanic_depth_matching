#include "mutable_cuda_spring_force_field.hpp"
#include <sofa/core/ObjectFactory.h>

int MutableCudaSpringForceFieldClass = 
	sofa::core::RegisterObject("This component does nothing.")
			.add<MutableCudaSpringForceField>();
