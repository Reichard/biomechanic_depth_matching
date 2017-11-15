#include "test_component.hpp"
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

TestComponent::TestComponent() 
	: _pixel_skip_count(sofa::core::objectmodel::Base::initData(&_pixel_skip_count,
			"pixel_skip_count", "use every Nth pixels when attaching springs")),
	_max_spring_stiffness(sofa::core::objectmodel::Base::initData(&_max_spring_stiffness,
			"max_spring_stiffness", "maximum stiffness of our interaction springs")),
	_max_data_distance(sofa::core::objectmodel::Base::initData(&_max_data_distance,
			"max_data_distance", "maximum valid data distance")),
	_stats_file_path(sofa::core::objectmodel::Base::initData(&_stats_file_path,
			"stats_file", "location of stats file (it is only written if this is not empty)")),
	_substep_count(sofa::core::objectmodel::Base::initData(&_substep_count,
			"substeps", "number of substeps per frame")),
	_volume_prefix(sofa::core::objectmodel::Base::initData(&_volume_prefix,
			"volume_prefix", "prefix of volume files")),
	_surface_prefix(sofa::core::objectmodel::Base::initData(&_surface_prefix,
			"surface_prefix", "prefix of surface files")),
	_rgbd_prefix(sofa::core::objectmodel::Base::initData(&_rgbd_prefix,
			"rgbd_prefix", "prefix of rgbd files")),
	_ground_truth_prefix(sofa::core::objectmodel::Base::initData(&_ground_truth_prefix,
			"ground_truth_prefix", "prefix of ground truth files")),
	_polaris_folder(sofa::core::objectmodel::Base::initData(&_polaris_folder,
			"polaris_folder", "location of polaris files")),
	_polaris_offset(sofa::core::objectmodel::Base::initData(&_polaris_offset,
			"polaris_offset", "offset of polaris data, relative to start of camera sequence")),
	_verify_option(sofa::core::objectmodel::Base::initData(&_verify_option,
			"verify", "enable or disable verification")),
	_calibration_path(sofa::core::objectmodel::Base::initData(&_calibration_path,
			"calibration", "path of the camera calibration file")),

	_reference_mesh("/media/haentsch/data/BioSim/pig_liver_seq2/0.stl"),
	_pose_source("/media/haentsch/data/BioSimCamera/pig_liver_seq2/")
{
	f_listening.setValue(true);
}

TestComponent::~TestComponent()
{
}

void TestComponent::setup_offscreen_rendering()
{
	std::cout << "setup offscreen rendering" << std::endl;
	cudapp::Device device = cudapp::default_device();
	cudapp::GLContext cuda_context(device);

	prepare_shaders();

 	_fbo.bind();

 	_depth_stencil_render_buffer.attach_depth_stencil(
 			_width, _height);

	_association_render_buffer.set_storage(GL_RGBA32F, 
			_width, _height);
	_association_render_buffer.attach(GL_COLOR_ATTACHMENT0);

	std::array<GLenum,1> draw_buffers = {{ GL_COLOR_ATTACHMENT0 }};
	glpp::set_draw_buffers(draw_buffers);

	if(auto status = _fbo.check_status())
	{
		std::cout << "FBO not complete (status: " << status.to_string() << ")" << std::endl;
	}

	_association_buffer.resize(_width * _height * 4);
	_association_pixel_buffer.resize(_width * _height * 4 * sizeof(float),
			GL_STREAM_DRAW);

	glpp::Framebuffer::default_framebuffer().bind();

	//connect with cuda
	_association_resource = cudapp::GraphicsResource(_association_pixel_buffer.back());

	if(auto gl_error = glpp::Error::get())
	{
		std::cout << gl_error.to_string() << std::endl;
	}
}

void TestComponent::prepare_shaders()
{
	glpp::VertexShader vertex_shader(
			"/home/students/haentsch/dev/depthmatch/src/shaders/associate.vert");
	glpp::GeometryShader geometry_shader(
			"/home/students/haentsch/dev/depthmatch/src/shaders/associate.geom");
	glpp::FragmentShader fragment_shader(
			"/home/students/haentsch/dev/depthmatch/src/shaders/associate.frag");

	_association_program.attach_shader(vertex_shader);
	_association_program.attach_shader(geometry_shader);
	_association_program.attach_shader(fragment_shader);
	auto link_status = _association_program.link();

	if(!link_status)
	{
		std::cout << link_status.info_log() << std::endl;
	}
}

void TestComponent::associate_live_frame()
{
	Timer timer;


	if(_ogl_model == nullptr) return;

	auto &vertices = _ogl_model->getVertices();
	auto &normals = _ogl_model->getVnormals();
	auto &triangles = _ogl_model->getTriangles();

	auto viewport = glpp::get_viewport();

	_fbo.bind();
	glpp::set_viewport(glpp::Viewport(0,0,_width,_height));

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

void TestComponent::draw(const sofa::core::visual::VisualParams *params)
{
	Timer timer;

	_visual_params = *params;

	//draw_association();
	//draw_error();
	draw_data_positions();
	//draw_ground_truth();
	//draw_ground_truth_volume();
	
	timer.print("draw");
}

void TestComponent::draw_association()
{
	_fbo.bind_read();
	glpp::Framebuffer::blit(0,0,_width,_height,0,0,_width,_height,GL_COLOR_BUFFER_BIT,GL_LINEAR);
	glpp::Framebuffer::default_framebuffer().bind();
}

std::array<float,3> map_color_blue_to_red(float value, float min, float max)
{
	std::array<float,3> color {{1,1,1}};

	value = std::max(min, std::min(max, value));
	auto dv = max - min;

	if(value < min + 0.25f * dv)
	{
		color[0] = 0;
		color[1] = 4 * (value - min) / dv;
	}
	else if(value < (min + 0.5f * dv))
	{
		color[0] = 0;
		color[2] = 1 + 4 * (min + 0.25f * dv - value) / dv;
	}
	else if(value < (min +0.75f * dv))
	{
		color[0] = 4 * (value - min - 0.5f * dv) / dv;
		color[2] = 0;
	}
	else
	{
		color[1] = 1 + 4 * (min + 0.75f * dv - value) / dv;
		color[2] = 0;
	}

	return color;
}

void TestComponent::draw_error()
{
	auto &vertices = _ogl_model->getVertices();
	auto &normals = _ogl_model->getVnormals();
	auto &triangles = _ogl_model->getTriangles();

	auto &reference_vertices = _reference_ogl_model->getVertices();


	for(size_t i=0; i<vertices.size(); ++i)
	{
		float dist = (vertices[i] - reference_vertices[i]).norm();
		_vertex_colors[i] = map_color_blue_to_red(dist,0,10);
	}

	glpp::Framebuffer::default_framebuffer().bind();

	glpp::enable_client_state(GL_VERTEX_ARRAY);
	glpp::enable_client_state(GL_NORMAL_ARRAY);
	glpp::enable_client_state(GL_COLOR_ARRAY);

	glpp::disable_lighting();

	glpp::set_vertex_pointer(3, GL_FLOAT, 0, vertices.getData());
	glpp::set_normal_pointer(GL_FLOAT, 0, normals.getData());
	glpp::set_color_pointer(3, GL_FLOAT, 0, _vertex_colors.data());
	glpp::draw_elements(GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, triangles.getData());
	glpp::finish();

	glpp::enable_lighting();
}

void TestComponent::draw_data_positions()
{
	glpp::disable_lighting();

	glpp::legacy::set_color(1,1,0);
	glpp::legacy::render_points([=]{
		for(size_t i=0; i<_data_points.size(); i+=8)
		{
			if(std::isinf(_data_points[i][0])) continue;

			glpp::legacy::emit_vertex3(_data_points[i].data());
		}
	});

	glpp::enable_lighting();
}

void TestComponent::draw_ground_truth()
{
	glpp::disable_lighting();

	glpp::legacy::set_color(1,0,1);
	glpp::legacy::render_points([=]{
		for(size_t i=0; i<_ground_truth.size(); ++i)
		{
			if(std::isinf(_ground_truth[i][0])) continue;

			glpp::legacy::emit_vertex3(_ground_truth[i].data());
		}
	});

	glpp::enable_lighting();
}

void TestComponent::draw_ground_truth_volume()
{
	glpp::disable_lighting();

	glpp::legacy::set_color(1,1,0);
	glpp::legacy::render_points([=]{
		for(size_t i=0; i<_volume_points.size(); ++i)
		{
			if(std::isinf(_volume_points[i][0])) continue;
			glpp::legacy::emit_vertex3(_volume_points[i].data());
		}
	});

	glpp::enable_lighting();
}

void TestComponent::read_next_frame()
{
	const int offset = 100;

	if(_current_frame >= 0) _current_frame += 1;
	else _current_frame = 0;

	std::cout << "read data for frame " << _current_frame << std::endl;

	int input_frame = _current_frame + offset;

	if(_current_frame > 0 && _verify)
	{
		_stats.advance();
		_stats.current().projective_error_surface_to_ground_truth = _projective_deviation;
		_stats.current().projective_error_depth_to_ground_truth = _projective_stereo_deviation;
		_stats.current().surface_rmsd = calculate_registration_deviation();
		_stats.current().avg_surface_distance = calculate_average_registration_deviation();
		_stats.current().volume_rmsd = calculate_volume_registration_deviation();
		_stats.current().avg_volume_distance = calculate_average_volume_registration_deviation();
	}

	if(auto current_pose = _pose_source.read(input_frame + _polaris_offset.getValue()))
	{
		_pose = current_pose;
	} else {
		std::cout << "NO VALID POSE (frame " << input_frame << ")" << std::endl;
		if(_verify) {
			_stats.write(_stats_file_path.getValue());
			exit(0);
		}
	}

	bool points_status = _current_frame > 0 ?
		_data_points_future.get()
		: read_rgbd_data_async(input_frame).get();
	_data_points_future = read_rgbd_data_async(input_frame+1);

	if(points_status)
	{
		_data_points = _data_points_read_buffer;
	}

	if(_verify)
	{
		bool mesh_status = _current_frame ? 
			_mesh_future.get() 
			: read_mesh_async(input_frame).get();
		_mesh_future = read_mesh_async(input_frame+1);
		if(mesh_status)
		{
			//we need to do this because they dont clear the triangles when updating the mesh
			using triangle_list_t = sofa::defaulttype::ResizableExtVector<
				sofa::core::topology::BaseMeshTopology::Triangle>;

			auto &triangle_list = const_cast<triangle_list_t&>(
					_reference_ogl_model->getTriangles());
			triangle_list.clear();

			_reference_ogl_model->setMesh(_reference_mesh);
			_reference_ogl_model->updateBuffers();
		}

		bool volume_status = _current_frame ?
			_volume_future.get()
			: read_volume_mesh_async(input_frame).get();
		_volume_future = read_volume_mesh_async(input_frame+1);
		if(volume_status)
		{
			_volume_points = _volume_read_buffer;
		}

		bool ground_truth_status = _current_frame ? 
			_ground_truth_future.get() 
			: read_ground_truth_async(input_frame).get();
		_ground_truth_future = read_ground_truth_async(input_frame+1);
		if(ground_truth_status)
		{
			_ground_truth = _ground_truth_read_buffer;
		}
	}


}

void TestComponent::update()
{
	static Timer runtimer;
	runtimer.print("runtime");

	Timer timer;

	unsigned int simulation_substeps = _substep_count.getValue();
	_simulation_frame++;

	logging::d() << "----------------------------" << std::endl
	 	<< "Simulation Frame " << _simulation_frame << std::endl;

	if(_simulation_frame % simulation_substeps == 0) 
	{
		read_next_frame();
	}

	timer.print("read frame");

	/*
	   float young_modulus = (_simulation_frame % simulation_substeps + 1) * (4000.0f/simulation_substeps);
	   young_modulus += 1000.0f;
	   _liver_force_field->setYoungModulus(young_modulus);
	//_liver_force_field->setUpdateStiffnessMatrix(true);
	_liver_force_field->reinit();
	*/

	//associate_live_frame();
	_associator.update(_pose);

	timer.print("associate");

	if(_use_gpu)
		update_springs_on_gpu();
	else 
		update_springs();

	if(_simulation_frame % simulation_substeps == simulation_substeps-1) 
	{
		std::stringstream surface_str;
		surface_str << "/media/haentsch/out/"
			<< _current_frame << ".stl";
		write_surface_stl(surface_str.str());

		std::stringstream marker_str;
		marker_str << "/media/haentsch/out/markers_"
			<< _current_frame << ".stl";
		write_marker_stl(marker_str.str());
	}

	timer.print("update springs");
}

void TestComponent::update_springs_on_gpu()
{

	//_spring_attacher.set_association_resource(_association_resource);
	_spring_attacher.set_association_resource(_associator.graphics_resource());
	_spring_attacher.attach_springs(_data_points); 
}

void TestComponent::update_springs()
{
	Timer timer;

	auto &triangles = _ogl_model->getTriangles();
	auto &vertices = _ogl_model->getVertices();
	auto &normals = _ogl_model->getVnormals();
	auto positions = _data_dofs->writePositions();
	//_springs->clear();

	for(size_t i=0; i<positions.size(); ++i) 
	{
		positions[i].set(0,0,0);
	}

	_projective_deviation = 0;
	_projective_stereo_deviation = 0;
	int deviation_sample_count = 0;

	int step = _pixel_skip_count.getValue();
	float max_data_dist = _max_data_distance.getValue();

	step = std::max(1,step);
	max_data_dist = std::max(0.0f,max_data_dist);

	timer.print(".prepare");

	//auto springs = _springs->springs.beginEdit();
	std::vector<SpringForceField::Deriv> forces(vertices.size());
	std::vector<unsigned int> force_count(vertices.size());

	for(size_t y=0; y<_height; y+=step)
	{
		for(size_t x=0; x<_width; x+=step)
		{
			auto index = y*_width + x;

			//unpack values from assoc buffer
			auto bary_0 = _association_buffer[index*4];
			auto bary_1 = _association_buffer[index*4+1];
			auto bary_2 = _association_buffer[index*4+2];
			auto prim_id_f = _association_buffer[index*4+3];

			auto prim_id = static_cast<unsigned int>(prim_id_f);

			if(prim_id_f < 0) continue;

			std::array<unsigned int,3> vertex_ids;
			for(int i=0; i<3; ++i) vertex_ids[i] = triangles[prim_id][i];

			auto surface_pos = vertices[vertex_ids[0]] * bary_0
				+ vertices[vertex_ids[1]] * bary_1
				+ vertices[vertex_ids[2]] * bary_2;

			auto surface_normal = normals[vertex_ids[0]] * bary_0
				+ normals[vertex_ids[1]] * bary_1
				+ normals[vertex_ids[2]] * bary_2;
			surface_normal.normalize();

			decltype(surface_pos) data_pos(
					_data_points[index][0],
					_data_points[index][1],
					_data_points[index][2]);

			if(std::isinf(data_pos[0])) continue;

			auto data_dist = (surface_pos - data_pos).norm();

			if(_verify)
			{
				decltype(surface_pos) ground_truth_pos(
						_ground_truth[index][0],
						_ground_truth[index][1],
						_ground_truth[index][2]);

				if(std::isfinite(ground_truth_pos[0])) 
				{
					auto dist2 = (surface_pos - ground_truth_pos).norm2();
					auto stereo_dist2 = (data_pos - ground_truth_pos).norm2();

					_projective_deviation += dist2;
					_projective_stereo_deviation += stereo_dist2;

					deviation_sample_count++;
				}
			}

			if(data_dist > max_data_dist) continue;

			auto data_rel_pos = data_pos - surface_pos;
			auto projected_length = surface_normal * data_rel_pos;
			auto projected_pos = surface_pos + surface_normal * projected_length;

			for(int i=0; i<3; ++i)
			{
				auto initial_length = (surface_pos - vertices[vertex_ids[i]]).norm();
				auto direction = projected_pos - vertices[vertex_ids[i]];
				auto length = direction.norm();

				if(length > 0.0001f)
				{
					direction /= length;
					auto displacement = length - initial_length;
					auto force = direction * (displacement * displacement);

					positions[vertex_ids[i]] += force;
					forces[vertex_ids[i]] += force;
					force_count[vertex_ids[i]]++;
				}
			}
		}
	}

	timer.print(".accumulate forces");

	for(size_t i=0; i<positions.size(); ++i)
	{
		auto force = forces[i];
		auto count = force_count[i];

		if(count == 0 || force.norm2() < 0.000001f )
		{
			positions[i] = vertices[i] + SpringForceField::Deriv(0.0001f,0,0);
			continue;
		}

		auto norm = force.norm();
		auto direction = force / norm;
		auto scalar_force = norm / count;
		scalar_force *= 20.0f/(1 + std::exp(20.0f-count));

		auto displacement = 2*std::sqrt(scalar_force);
		auto displacement_vector = displacement * direction;

		if(displacement_vector.norm2() < 0.000001f) displacement_vector.set(0.0001f,0,0);
		positions[i] = vertices[i] + displacement_vector;
	}
	timer.print(".attach");

	if(deviation_sample_count > 0)
	{
		_projective_deviation = std::sqrt(_projective_deviation/deviation_sample_count);
		_projective_stereo_deviation = std::sqrt(_projective_stereo_deviation/deviation_sample_count);
	}

	positions.wref().deviceWrite();
	timer.print(".transfer");
}

void TestComponent::handleEvent(sofa::core::objectmodel::Event *event)
{
	using sofa::simulation::AnimateBeginEvent;
	using sofa::simulation::AnimateEndEvent;

	if(dynamic_cast<AnimateBeginEvent*>(event))
	{
		update();
	}
	else if(dynamic_cast<AnimateEndEvent*>(event))
	{
	}
}

void TestComponent::init()
{
	if(_use_gpu)
	{
		std::string visual_model_path = "liver/visual/cuda";
		getContext()->get(_visual_model,visual_model_path);
		if(_visual_model == nullptr) {
			std::cout << "NO VISUAL MODEL FOUND" << std::endl;
		}

		//TODO: remove thos two once we render with the cuda visual model
		std::cout << "init" << std::endl;
		std::string ogl_model_path = "liver/visu/VisualModel";
		getContext()->get(_ogl_model,ogl_model_path);
		if(_ogl_model == nullptr) {
			std::cout << "NO OPENGL MODEL FOUND" << std::endl;
		}
		_ogl_model->setColor(1,1,1,0);

		std::string reference_ogl_model_path = "liver/visu/reference_model";
		getContext()->get(_reference_ogl_model,reference_ogl_model_path);
		if(_reference_ogl_model == nullptr) {
			std::cout << "NO REFERENCE OPENGL MODEL FOUND" << std::endl;
		}
		_reference_ogl_model->setColor(1,1,1,0);

	} else {
		std::string visual_model_path = "liver/visual/cuda";
		getContext()->get(_visual_model,visual_model_path);
		if(_visual_model == nullptr) {
			std::cout << "NO cuda MODEL FOUND" << std::endl;
		}
		//_visual_model->setColor(1,1,1,0);
		
		std::cout << "init" << std::endl;
		std::string ogl_model_path = "liver/visu/VisualModel";
		getContext()->get(_ogl_model,ogl_model_path);
		if(_ogl_model == nullptr) {
			std::cout << "NO OPENGL MODEL FOUND" << std::endl;
		}
		_ogl_model->setColor(1,1,1,1);

		std::string reference_ogl_model_path = "liver/visu/reference_model";
		getContext()->get(_reference_ogl_model,reference_ogl_model_path);
		if(_reference_ogl_model == nullptr) {
			std::cout << "NO REFERENCE OPENGL MODEL FOUND" << std::endl;
		}
		_reference_ogl_model->setColor(1,1,1,0);

	}

	std::string surface_dofs_path = "liver/surface/dofs";
	getContext()->get(_surface_dofs,surface_dofs_path);
	if(_surface_dofs == nullptr) {
		std::cout << "NO SURFACE MODEL FOUND" << std::endl;
	}

	std::string liver_dofs_path = "liver/dofs";
	getContext()->get(_liver_dofs,liver_dofs_path);
	if(_liver_dofs == nullptr) {
		std::cout << "NO LIVER MODEL FOUND" << std::endl;
	}

	std::string volume_dofs_path = "liver/dofs";
	getContext()->get(_volume_dofs,volume_dofs_path);
	if(_volume_dofs == nullptr) {
		std::cout << "NO VOLUME MODEL FOUND" << std::endl;
	}

	std::string data_dofs_path = "data/dofs";
	getContext()->get(_data_dofs,data_dofs_path);
	if(_data_dofs == nullptr) {
		std::cout << "NO DATA MODEL FOUND" << std::endl;
	}

	std::string marker_model_path = "liver/markers/model";
	getContext()->get(_marker_model,marker_model_path);
	if(_marker_model== nullptr) {
		std::cout << "NO MARKERS FOUND" << std::endl;
	}

	/*
	std::string marker_mapping_path = "liver/markers/mapping";
	getContext()->get(_marker_mapping,marker_mapping_path);
	if(_marker_mapping== nullptr) {
		std::cout << "NO MARKER MAPPING FOUND" << std::endl;
	}
	*/

	std::string rest_dofs = "anchors/rest_spring_anchors";
	getContext()->get(_rest_spring_anchors,rest_dofs);
	if(_rest_spring_anchors == nullptr) {
		std::cout << "NO REST MODEL FOUND" << std::endl;
	}

	std::string springs_path = "springs";
	getContext()->get(_springs,springs_path);
	if(_springs == nullptr) {
		std::cout << "NO SPRING FORCEFIELD FOUND" << std::endl;
	}

	std::string rest_springs_path = "rest_springs";
	getContext()->get(_rest_springs,rest_springs_path);
	if(_rest_springs == nullptr) {
		std::cout << "NO REST SPRING FORCEFIELD FOUND" << std::endl;
	}

	std::string liver_force_field_path = "liver/FEM";
	getContext()->get(_liver_force_field,liver_force_field_path);
	if(_liver_force_field == nullptr) {
		std::cout << "NO LIVER FORCEFIELD FOUND" << std::endl;
	}

	std::string vtk_loader_path = "vtk_loader";
	getContext()->get(_vtk_loader,vtk_loader_path);
	if(_vtk_loader == nullptr) {
		std::cout << "NO VTK LOADER FOUND" << std::endl;
	}

	_verify = _verify_option.getValue();

	using triangle_list_t = sofa::defaulttype::ResizableExtVector<sofa::core::topology::BaseMeshTopology::Triangle>;
	auto &triangle_list = const_cast<triangle_list_t&>(_reference_ogl_model->getTriangles());
	triangle_list.clear();

	_calibration.read(_calibration_path.getValue());
	_width = _calibration.width;
	_height = _calibration.height;
	_pixel_count = _width * _height;

	read_mesh_async(0).get();
	read_volume_mesh_async(0).get();
	_volume_points = _volume_read_buffer;
	_vertex_count = _reference_mesh.getVertices().size();

	std::cout << "sizes" << std::endl;
	std::cout << "pixel count: " << _pixel_count << std::endl;
	std::cout << "vertex count: " << _vertex_count << std::endl;
	std::cout << "vertex count (volume): " << _volume_points.size() << std::endl;

	setup_offscreen_rendering();
	_ground_truth.resize(_pixel_count);
	_ground_truth_read_buffer.resize(_pixel_count);
	_data_points.resize(_pixel_count);
	_data_points_read_buffer.resize(_pixel_count);
	_color_points.resize(_pixel_count);
	_color_points_read_buffer.resize(_pixel_count);

	_data_dofs->resize(_vertex_count);
	_vertex_colors.resize(_vertex_count);

	_stats.substeps = _substep_count.getValue();
	_stats.pixel_step = _pixel_skip_count.getValue();
	_stats.max_stiffness = _max_spring_stiffness.getValue();
	_stats.max_data_dist = _max_data_distance.getValue();

	std::cout << _polaris_folder.getValue() << std::endl;
	_pose_source.set_path(_polaris_folder.getValue());

	for(size_t i=0; i<_vertex_count; ++i)
	{
		_springs->addSpring(i,i,_max_spring_stiffness.getValue(),100,0);
	}

	auto data_writer = _rest_spring_anchors->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();

	/*
	for(size_t i=0; i<vertices.size(); ++i) {
		vertices[i][0] += 0.0001f;
		vertices[i][1] += 0.0001f;
		vertices[i][2] += 0.0001f;
		_rest_springs->addSpring(i,i,5.0f,100.0f,0);
	}
	*/

	data_writer->endEdit();

	_associator.set_ogl_model(_ogl_model);
	_associator.set_calibration(_calibration);

	_spring_attacher.set_width(_width);
	_spring_attacher.set_height(_height);
	_spring_attacher.set_association_resource(_association_resource);
	//_spring_attacher.set_association_resource(_associator.graphics_resource());
	_spring_attacher.set_anchors(_data_dofs);
	_spring_attacher.set_surface(_surface_dofs, _reference_mesh.getFacets());
	_spring_attacher.set_visual_model(_visual_model);

	_associator.set_ogl_model(_ogl_model);
	_associator.set_calibration(_calibration);

}

void TestComponent::bwdInit()
{
	std::cout << "bwdInit" << std::endl;
}

/*
void TestComponent::cleanup()
{
	_data_dofs->resize(0);
	_springs->clear();

	_rest_spring_anchors->resize(0);
	_rest_springs->clear();
}

void TestComponent::reset()
{
	if(_ground_truth_future.valid()) _ground_truth_future.get();
	if(_mesh_future.valid()) _mesh_future.get();
	if(_data_points_future.valid()) _data_points_future.get();

	_simulation_frame = -1;
	_current_frame = -1;
}
*/

std::future<bool> TestComponent::read_mesh_async(int frame)
{
	return std::async(std::launch::async,[=]() -> bool {
			std::string mesh_prefix = _surface_prefix.getValue();
			std::string mesh_postfix = ".stl";

			int number = frame * 4;
			if(frame > 100) number = 800 - number;

			std::stringstream mesh_path;
			mesh_path << mesh_prefix << number << mesh_postfix;
			
			{
				std::ifstream file(mesh_path.str());
				if(!file.good()) {
					std::cout << "no reference mesh available for current frame" << std::endl;
					return false;
				}
			}

			_reference_mesh = sofa::helper::io::MeshSTL(mesh_path.str());
			return _reference_mesh.getVertices().size() > 0;
	});
}

std::future<bool> TestComponent::read_volume_mesh_async(int frame)
{
	return std::async(std::launch::async,[=]() -> bool {
			std::string mesh_prefix = _volume_prefix.getValue();
			std::string mesh_postfix = ".vtk";

			int number = frame * 4;
			if(frame > 100) number = 800 - number;

			std::stringstream mesh_path;
			mesh_path << mesh_prefix << number << mesh_postfix;
			{
				std::ifstream file(mesh_path.str());
				if(!file.good()) {
					std::cout << "no volume reference mesh available for current frame" << std::endl;
					return false;
				}
			}

			auto *positions = _vtk_loader->positions.beginEdit();
			positions->clear();
			_vtk_loader->positions.endEdit();

			_vtk_loader->setFilename(mesh_path.str());
			_vtk_loader->load();
			_vtk_loader->updateMesh();

			const auto &points = _vtk_loader->positions.getValue();

			_volume_read_buffer.resize(points.size());
			for(size_t i=0; i<points.size(); ++i)
			{
				std::array<float,3> point {{
					static_cast<float>(points[i][0]), 
					static_cast<float>(points[i][1]), 
					static_cast<float>(points[i][2])
				}};
				_volume_read_buffer[i] = point;
			}

			return true;
	});
}

std::future<bool> TestComponent::read_ground_truth_async(int frame)
{
	return std::async(std::launch::async,[=]() -> bool {
			return read_ground_truth(frame);
			});
}

std::future<bool> TestComponent::read_rgbd_data_async(int frame)
{
	return std::async(std::launch::async,[=]() -> bool {
			std::string prefix = _rgbd_prefix.getValue();
			std::string postfix = ".rgbd";

			std::stringstream path;
			path << prefix << frame << postfix;
			{
				std::ifstream file(path.str());
				if(!file.good()) {
					std::cout << "no rgbd image available for current frame" << std::endl;
					std::cout << path.str() << std::endl;
					return false;
				}
			}

			Eigen::Matrix4f world_to_camera = _pose.transformation.inverse().matrix();
			Eigen::Matrix4f modelview = 
				_calibration.gl_camera_transform * 
				Eigen::Affine3f(Eigen::Scaling(1.0f,1.0f,-1.0f)).matrix() *
				world_to_camera;
			Eigen::Matrix4f inverse_modelview = modelview.inverse();

			_img = mediassist::rgbd_image::read(path.str());
			for(int y=0	; y<_img.height(); ++y)
			{
				for(int x=0; x<_img.width(); ++x)
				{
					Eigen::Vector2f pixel_pos(x,_img.height()-y);
					auto depth = _img.depth_at(x,_img.height()-y-1);

					Eigen::Vector3f p;
					if(depth != 0)
					{
						p = Eigen::Vector3f(_calibration.reproject(pixel_pos,depth));
						p = (inverse_modelview * p.homogeneous()).head<3>();
					} else {
						p = Eigen::Vector3f(1.0f/0.0f,1.0f/0.0f,1.0f/0.0f);
					}
					Eigen::Vector3f c = Eigen::Vector3f(_calibration.reproject(pixel_pos,60));
					c = (inverse_modelview * c.homogeneous()).head<3>();

					auto &point = _data_points_read_buffer[y*_img.width() + x];
					point = {{p.x(),p.y(),p.z()}};

					auto &color = _color_points_read_buffer[y*_img.width() + x];
					color = {{c.x(),c.y(),c.z()}};
				}
			}
			return true;
	});
}

bool TestComponent::read_ground_truth(int frame)
{
	std::string prefix = _ground_truth_prefix.getValue();
	const std::string postfix = ".txt";

	std::stringstream filename;
	filename << prefix << std::setw(4) << std::setfill('0') << frame << postfix;

	std::ifstream file(filename.str());

	if(!file.good())
	{
		std::cout << "no ground truth available for current frame" << std::endl;
		std::cout << filename.str() << std::endl;
		return false;
	}

	//read and flip the image
	for(int y=_height-1; y>=0; --y)
	{
		for(int x=0; x<_width; ++x)
		{
			auto& pos = _ground_truth_read_buffer[y*_width+x];

			if(!file.good()) 
			{
				std::cout << "error while reading ground truth! (too few points?)" << std::endl;
				std::cout << "( x = " << x << " y = " << y << " )" << std::endl;
				std::cout << "( width = " << _width << " height = " << _height << " )" << std::endl;
				break;
			}

			file >> pos[0];
			file.ignore();
			file >> pos[1];
			file.ignore();
			file >> pos[2];
			//do we need to flip here???

			if(pos[0] > 1000000)
			{
				pos[0] = 1.0f/0.0f;
				pos[1] = 1.0f/0.0f;
				pos[2] = 1.0f/0.0f;
			}
		}
		if(!file.good()) break;
	}

	return true;
}

float TestComponent::calculate_registration_deviation()
{
	auto data_writer = _surface_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();
	auto &reference_vertices = _reference_ogl_model->getVertices();

	assert(vertices.size() == reference_vertices.size());

	float msd = 0.0f;
	for(size_t vertex_id=0; vertex_id < vertices.size(); ++vertex_id)
	{
		msd += (vertices[vertex_id] - reference_vertices[vertex_id]).norm2();
	}

	auto rmsd = std::sqrt(msd/vertices.size());

	data_writer->endEdit();

	return rmsd;
}

float TestComponent::calculate_average_registration_deviation()
{
	auto data_writer = _surface_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();
	auto &reference_vertices = _reference_ogl_model->getVertices();

	assert(vertices.size() == reference_vertices.size());

	float mean = 0.0f;
	for(size_t vertex_id=0; vertex_id < vertices.size(); ++vertex_id)
	{
		auto dist = (vertices[vertex_id] - reference_vertices[vertex_id]).norm();
		mean += dist;
	}
	mean /= vertices.size();

	data_writer->endEdit();

	return mean;
}

float TestComponent::calculate_volume_registration_deviation()
{
	auto data_writer = _volume_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();

	auto dummy = vertices[0];

	float msd = 0.0f;
	for(size_t vertex_id=0; vertex_id < vertices.size(); ++vertex_id)
	{
		decltype(dummy) reference_vertex(
			_volume_points[vertex_id][0],
			_volume_points[vertex_id][1],
			_volume_points[vertex_id][2]
		);

		msd += (vertices[vertex_id] - reference_vertex).norm2();
	}

	auto rmsd = std::sqrt(msd/vertices.size());

	data_writer->endEdit();

	return rmsd;
}

void TestComponent::write_surface_ascii(const std::string &path)
{
	auto &triangles = _ogl_model->getTriangles();

	auto data_writer = _surface_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();

	std::ofstream file(path);
	file << triangles.size() << std::endl;

	for(int i=0; i<triangles.size(); i++) {
		file << 1 << " " << 0 << " " << 0 << std::endl; //normal

		auto &a = vertices[triangles[i][0]];
		auto &b = vertices[triangles[i][1]];
		auto &c = vertices[triangles[i][2]];

		file << a.x() << " " << a.y() << " " << a.z() << std::endl;
		file << b.x() << " " << b.y() << " " << b.z() << std::endl;
		file << c.x() << " " << c.y() << " " << c.z() << std::endl;
	}

	data_writer->endEdit();
}

void TestComponent::write_marker_stl(const std::string &path)
{
	float normal[3] = {1,0,0};
	char header[80] = "";
	char attribute_count[2] = "";

	auto &triangles = _marker_model->getTriangles();
	auto &vertices = _marker_model->getVertices();

	uint32_t triangle_count = triangles.size();

	std::ofstream file(path, std::ios::out | std::ios::binary );
	file.write(header,80);
	file.write(reinterpret_cast<char*>(&triangle_count),4);

	for(int i=0; i<triangles.size(); i++) {
		auto &a = vertices[triangles[i][0]];
		auto &b = vertices[triangles[i][1]];
		auto &c = vertices[triangles[i][2]];

		file.write(reinterpret_cast<char*>(&normal),3*4);

		float x = a.x(); float y = a.y(); float z = a.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		x = b.x(); y = b.y(); z = b.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		x = c.x(); y = c.y(); z = c.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		file.write(attribute_count,2);
	}
}


void TestComponent::write_surface_stl(const std::string &path)
{
	float normal[3] = {1,0,0};
	char header[80] = "";
	char attribute_count[2] = "";


	auto &triangles = _ogl_model->getTriangles();

	auto data_writer = _surface_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();
	uint32_t triangle_count = triangles.size();

	std::ofstream file(path, std::ios::out | std::ios::binary );
	file.write(header,80);
	file.write(reinterpret_cast<char*>(&triangle_count),4);

	for(int i=0; i<triangles.size(); i++) {
		auto &a = vertices[triangles[i][0]];
		auto &b = vertices[triangles[i][1]];
		auto &c = vertices[triangles[i][2]];

		file.write(reinterpret_cast<char*>(&normal),3*4);

		float x = a.x(); float y = a.y(); float z = a.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		x = b.x(); y = b.y(); z = b.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		x = c.x(); y = c.y(); z = c.z();
		file.write(reinterpret_cast<char*>(&x),4);
		file.write(reinterpret_cast<char*>(&y),4);
		file.write(reinterpret_cast<char*>(&z),4);

		file.write(attribute_count,2);
	}

	data_writer->endEdit();
}

float TestComponent::calculate_average_volume_registration_deviation()
{
	auto data_writer = _volume_dofs->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();

	auto dummy = vertices[0];

	float mean = 0.0f;
	for(size_t vertex_id=0; vertex_id < vertices.size(); ++vertex_id)
	{
		decltype(dummy) reference_vertex(
			_volume_points[vertex_id][0],
			_volume_points[vertex_id][1],
			_volume_points[vertex_id][2]
		);
		
		auto dist = (vertices[vertex_id] - reference_vertex).norm();
		mean += dist;
	}
	mean /= vertices.size();

	data_writer->endEdit();

	return mean;
}
 
int TestComponentClass = sofa::core::RegisterObject("This component does nothing.").add<TestComponent>();
