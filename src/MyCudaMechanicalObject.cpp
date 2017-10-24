#include <sofa/core/ObjectFactory.h>
#include "MyCudaMechanicalObject.hpp"


int MyCudaMechanicalObjectClass = 
	sofa::core::RegisterObject("This component does nothing.")
		.add<MyCudaMechanicalObject>();
