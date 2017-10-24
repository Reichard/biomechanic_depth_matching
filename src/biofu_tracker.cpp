#include "biofu_tracker.hpp"

#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/common/AnimateBeginEvent.h>
#include <sofa/simulation/common/AnimateEndEvent.h>
#include <sofa/core/visual/VisualParams.h>
#include "../../modules/SofaOpenglVisual/OglModel.h"

#include "rgbd_image.hpp"
#include "timer.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <unordered_set>
#include <type_traits>
#include <cassert>
#include <chrono>

void BiofuTracker::update(const std::vector<std::array<float,3>> &data);
	associate();
	_spring_attacher.attach_springs(_data_points); 
}

void BiofuTracker::associate() {
Timer timer;

	if(_ogl_model == nullptr) return;

	auto &vertices = _ogl_model->getVertices();
	auto &normals = _ogl_model->getVnormals();
	auto &triangles = _ogl_model->getTriangles();

	auto viewport = glpp::get_viewport();

	_fbo.bind();
	glpp::set_viewport(glpp::Viewport(0,0,_calibration.width,_calibration.height));

	_association_program.use();

	Eigen::Matrix4f world_to_camera = _pose.transformation.inverse().matrix();
	Eigen::Matrix4f modelview = 
		_calibration.gl_camera_transform * 
		Eigen::Affine3f(Eigen::Scaling(1.0f,1.0f,-1.0f)).matrix() *
		world_to_camera;

	glpp::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0,0,0,-1);

	auto model_view_uniform = _association_program.get_uniform("model_view_matrix");
	auto projection_uniform = _association_program.get_uniform("projection_matrix");
	model_view_uniform.upload(modelview);
	projection_uniform.upload(_calibration.gl_projection_matrix);

	glpp::disable_lighting();
	glpp::disable_culling();
	glpp::disable_stencil_test();
	glpp::enable_depth_test();

	timer.print(".prepare");

	glpp::enable_client_state(GL_VERTEX_ARRAY);
	glpp::enable_client_state(GL_NORMAL_ARRAY);
	glpp::set_vertex_pointer(3, GL_FLOAT, 0, vertices.getData());
	glpp::set_normal_pointer(GL_FLOAT, 0, normals.getData());
	glpp::draw_elements(GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, triangles.getData());
	glpp::finish();

	timer.print(".render");

	std::fill(_association_buffer.begin(), _association_buffer.end(), 0);

	_association_pixel_buffer.back().bind();
	glpp::read_pixels(GL_COLOR_ATTACHMENT0, 0,0,_width, _height,
			GL_RGBA, GL_FLOAT, 0);
	//_association_pixel_buffer.front().copy(_association_buffer);
	//_association_pixel_buffer.swap();
	//_association_pixel_buffer.back().copy(_association_buffer);

	//_association_texture.read(_association_buffer);
	glpp::finish();

	timer.print(".read pixels");

	glpp::Program::use_default();

	if(auto gl_error = glpp::Error::get())
	{
		std::cout << gl_error.to_string() << std::endl;
	}

	glpp::Framebuffer::bind_default();
	glpp::set_viewport(viewport);

	timer.print(".done");
}
