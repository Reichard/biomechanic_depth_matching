#pragma once

#include <sofa/core/objectmodel/BaseObject.h>
#include "../../modules/SofaOpenglVisual/OglModel.h"
#include <memory>
#include <GL/gl.h>
 
class TestComponent : public sofa::core::objectmodel::BaseObject
{
public:
SOFA_CLASS(TestComponent, sofa::core::objectmodel::BaseObject);
 
    TestComponent ();
    virtual ~TestComponent ();

	virtual void init();
	virtual void bwdInit();
    virtual void draw(const sofa::core::visual::VisualParams *);
    virtual void handleEvent(sofa::core::objectmodel::Event *);

private:
	sofa::component::visualmodel::OglModel::SPtr _ogl_model;

	GLuint _associaton_program;
	GLuint _vertex_index_vbo;
};
