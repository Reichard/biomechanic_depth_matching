#pragma once

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/behavior/MechanicalState.h>

class DepthInputComponent : public sofa::core::objectmodel::BaseObject
{
public:
SOFA_CLASS(DepthInputComponent, sofa::core::objectmodel::BaseObject);

	using MechanicalState = 
		sofa::core::behavior::MechanicalState<sofa::defaulttype::Vec3dTypes>;
 
    DepthInputComponent ();
    virtual ~DepthInputComponent ();

	virtual void init();
	virtual void bwdInit();
    virtual void handleEvent(sofa::core::objectmodel::Event *);

private:
	MechanicalState::SPtr _data_dofs = nullptr;
};
