#include "test_component.hpp"
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/common/AnimateBeginEvent.h>
#include <sofa/simulation/common/AnimateEndEvent.h>
#include <sofa/core/visual/VisualParams.h>
#include "../../modules/SofaOpenglVisual/OglModel.h"

#include "rgbd_image.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <unordered_set>
#include <type_traits>
#include <cassert>
#include <chrono>

#include "mesh_loader.hpp"

TestComponent::TestComponent() 
	: _pixel_stride(sofa::core::objectmodel::Base::initData(&_pixel_stride,
			"pixel_stride", "use every Nth pixels when attaching springs")),
	_max_spring_stiffness(sofa::core::objectmodel::Base::initData(&_max_spring_stiffness,
			"max_spring_stiffness", "maximum stiffness of our interaction springs")),
	_max_data_distance(sofa::core::objectmodel::Base::initData(&_max_data_distance,
			"max_data_distance", "maximum valid data distance")),
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
	_calibration_path(sofa::core::objectmodel::Base::initData(&_calibration_path,
			"calibration", "path of the camera calibration file")),
	_output_path(sofa::core::objectmodel::Base::initData(&_output_path,
			"output_path", "path for output files")),
	_reference_mesh(""),
	_pose_source("")
{
	f_listening.setValue(true);
}

void TestComponent::setup_offscreen_rendering()
{
	std::cout << "setup offscreen rendering" << std::endl;
	cudapp::Device device = cudapp::default_device();
	cudapp::GLContext cuda_context(device);

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

void TestComponent::draw(const sofa::core::visual::VisualParams *params)
{
	_visual_params = *params;

	//draw_association();
	//draw_error();
	draw_data_positions();
	//draw_ground_truth();
	//draw_ground_truth_volume();
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
			if(!std::isinf(_data_points[i][0]))
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
			if(!std::isinf(_ground_truth[i][0]))
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
			if(!std::isinf(_volume_points[i][0]))
				glpp::legacy::emit_vertex3(_volume_points[i].data());
		}
	});

	glpp::enable_lighting();
}

void TestComponent::read_next_frame()
{
	if(_current_frame >= 0) _current_frame += 1;
	else _current_frame = 0;

	std::cout << "reading data for frame " << _current_frame << "..." << std::endl;

	int input_frame = _current_frame;

	if(auto current_pose = _pose_source.read(input_frame + _polaris_offset.getValue()))
	{
		_pose = current_pose;
	} else {
		std::cout << "NO VALID POSE (frame " << input_frame << ")" << std::endl;
	}

	bool points_status = _current_frame > 0 ?
		_data_points_future.get()
		: read_rgbd_data_async(input_frame).get();
	_data_points_future = read_rgbd_data_async(input_frame+1);

	if(points_status)
	{
		_data_points = _data_points_read_buffer;
	}
}

void TestComponent::update() 
{
	unsigned int simulation_substeps = _substep_count.getValue();
	_simulation_frame++;

	if(_simulation_frame % simulation_substeps == 0) 
	{
		read_next_frame();
	}
	std::cout << "frame " << _current_frame
		<< "/" << (_simulation_frame % simulation_substeps) << std::endl;

	_associator.update(_pose);

	_spring_attacher.set_association_resource(_associator.graphics_resource());
	_spring_attacher.attach_springs(_data_points); 

	if(_simulation_frame % simulation_substeps == simulation_substeps-1) 
	{
		std::string path = _output_path.getValue();
		if(path.size() > 0) {
			std::cout << "writing outputs..." << std::endl;
			write_surface_stl(path + "/" + std::to_string(_current_frame) + ".stl");
		}
	}
}

void TestComponent::handleEvent(sofa::core::objectmodel::Event *event)
{
	if(dynamic_cast<sofa::simulation::AnimateBeginEvent*>(event))
		update();
}

void TestComponent::setup_scene() 
{
	std::string visual_model_path = "liver/visual/cuda";
	getContext()->get(_visual_model,visual_model_path);
	if(_visual_model == nullptr) {
		std::cout << "NO VISUAL MODEL FOUND" << std::endl;
	}

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


}

void TestComponent::init()
{
	setup_scene();

	using triangle_list_t = sofa::defaulttype::ResizableExtVector<sofa::core::topology::BaseMeshTopology::Triangle>;
	auto &triangle_list = const_cast<triangle_list_t&>(_reference_ogl_model->getTriangles());
	triangle_list.clear();

	_calibration.read(DATA_DIR"/scenes/" + _calibration_path.getValue());
	_width = _calibration.width;
	_height = _calibration.height;
	_pixel_count = _width * _height;

	_surface_mesh_loader.setPrefixAndPostfix(DATA_DIR"/scenes/" +  _surface_prefix.getValue(), ".stl");
	_reference_mesh = _surface_mesh_loader.load(0);

	_volume_mesh_loader.setPrefixAndPostfix(DATA_DIR"/scenes/" + _volume_prefix.getValue(), ".vtk");
	_volume_points = _volume_mesh_loader.load(0);

	_ground_truth_loader.setPrefixAndPostfix(DATA_DIR"/scenes/" + _ground_truth_prefix.getValue(), ".txt");
	_ground_truth_loader.setSize(_width,_height);

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

	_pose_source.set_path(DATA_DIR"/scenes/" + _polaris_folder.getValue());

	for(size_t i=0; i<_vertex_count; ++i)
	{
		_springs->addSpring(i,i,_max_spring_stiffness.getValue(),100,0);
	}

	/*
	auto data_writer = _rest_spring_anchors->write(sofa::core::VecCoordId::position());
	auto &vertices = *data_writer->beginEdit();

	   for(size_t i=0; i<vertices.size(); ++i) {
	   vertices[i][0] += 0.0001f;
	   vertices[i][1] += 0.0001f;
	   vertices[i][2] += 0.0001f;
	   _rest_springs->addSpring(i,i,5.0f,100.0f,0);
	   }

	data_writer->endEdit();
	*/

	_associator.set_ogl_model(_ogl_model);
	_associator.set_calibration(_calibration);
	_associator.init();

	_spring_attacher.set_width(_width);
	_spring_attacher.set_height(_height);
	_spring_attacher.set_association_resource(_associator.graphics_resource());
	_spring_attacher.set_anchors(_data_dofs);
	_spring_attacher.set_surface(_surface_dofs, _reference_mesh.getFacets());
	_spring_attacher.set_visual_model(_visual_model);
}

std::future<bool> TestComponent::read_rgbd_data_async(int frame)
{
	return std::async(std::launch::async,[=]() -> bool {
			std::string prefix = DATA_DIR"/scenes/" + _rgbd_prefix.getValue();
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
 
int TestComponentClass = sofa::core::RegisterObject("").add<TestComponent>();
