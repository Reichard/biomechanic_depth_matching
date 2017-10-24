#include "my_cuda_spring_force_field.hpp"
#include <sofa/core/ObjectFactory.h>

int MyCudaSpringForceFieldClass = 
	sofa::core::RegisterObject("This component does nothing.")
			.add<MyCudaSpringForceField>();
