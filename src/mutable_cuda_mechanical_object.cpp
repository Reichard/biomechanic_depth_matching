#include <sofa/core/ObjectFactory.h>

#include "mutable_cuda_mechanical_object.hpp"

int MutableCudaMechanicalObjectClass = 
	sofa::core::RegisterObject("A mutable variant of the cuda mechanical object.")
		.add<MutableCudaMechanicalObject>();
