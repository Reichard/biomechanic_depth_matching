#include "depth_input_component.hpp"
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/common/AnimateBeginEvent.h>
#include <sofa/simulation/common/AnimateEndEvent.h>

#include <iostream>
 
DepthInputComponent::DepthInputComponent()
{
	f_listening.setValue(true);
}

DepthInputComponent::~DepthInputComponent()
{
}

void DepthInputComponent::handleEvent(sofa::core::objectmodel::Event *event)
{
	using sofa::simulation::AnimateBeginEvent;

	if(AnimateBeginEvent* animate_begin_event = dynamic_cast<AnimateBeginEvent*>(event))
	{
		/*
		_data_dofs->resize(100);
		auto data_writer = _data_dofs->write(sofa::core::VecCoordId::position());
		auto positions = data_writer->beginEdit();
		for(int i=0; i<100; ++i){
			(*positions)[i].set(i*10,300,300);
		}
		data_writer->endEdit();
		*/

	}
}

void DepthInputComponent::init()
{
	std::string data_dofs_path = "data/dofs";
	getContext()->get<MechanicalState>(_data_dofs,data_dofs_path);
	if(_data_dofs == nullptr) {
		std::cout << "NO DATA MODEL FOUND" << std::endl;
	}
}

void DepthInputComponent::bwdInit()
{
	std::cout << "bwdInit" << std::endl;
}
 
int DepthInputComponentClass = sofa::core::RegisterObject("Updates the MechanicalState based on depth values.").add<DepthInputComponent>();
