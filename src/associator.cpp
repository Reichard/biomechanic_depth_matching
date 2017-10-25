#include "associator.hpp"
#include "timer.hpp"
#include <Eigen/Dense>
#include <Eigen/Geometry>

void Associator::init()
{
	cudapp::Device device = cudapp::default_device();
	cudapp::GLContext cuda_context(device);

	glpp::VertexShader vertex_shader(
			"/home/haentsch/src/sofa/applications/plugins/MyPlugin/associate.vert");
	glpp::GeometryShader geometry_shader(
			"/home/haentsch/src/sofa/applications/plugins/MyPlugin/associate.geom");
	glpp::FragmentShader fragment_shader(
			"/home/haentsch/src/sofa/applications/plugins/MyPlugin/associate.frag");

	_program.attach_shader(vertex_shader);
	_program.attach_shader(geometry_shader);
	_program.attach_shader(fragment_shader);
	auto link_status = _program.link();

	if(!link_status)
	{
		std::cout << link_status.info_log() << std::endl;
	}

 	_fbo.bind();
 	_depth_stencil_render_buffer.attach_depth_stencil(
 			_calibration.width, _calibration.height);
	_association_render_buffer.set_storage(GL_RGBA32F, 
			_calibration.width, _calibration.height);
	_association_render_buffer.attach(GL_COLOR_ATTACHMENT0);

	std::array<GLenum,1> draw_buffers = {{ GL_COLOR_ATTACHMENT0 }};
	glpp::set_draw_buffers(draw_buffers);

	if(auto status = _fbo.check_status())
	{
		std::cout << "FBO not complete (status: " << status.to_string() << ")" << std::endl;
	}

	_association_pixel_buffer.resize(_calibration.width * _calibration.height * 4 * sizeof(float),
			GL_STREAM_DRAW);

	glpp::Framebuffer::default_framebuffer().bind();

	_association_resource = cudapp::GraphicsResource(_association_pixel_buffer);

	initialized = true;
}

void Associator::update(const mediassist::Pose &pose)
{
	if(!initialized) init();

	Timer timer;

	if(_ogl_model == nullptr) return; //TODO: throw an approriate exception

	auto &vertices = _ogl_model->getVertices();
	auto &normals = _ogl_model->getVnormals();
	auto &triangles = _ogl_model->getTriangles();

	auto viewport = glpp::get_viewport();

	_fbo.bind();
	glpp::set_viewport(glpp::Viewport(0,0,_calibration.width,_calibration.height));

	_program.use();

	Eigen::Matrix4f world_to_camera = pose.transformation.inverse().matrix();
	Eigen::Matrix4f modelview = 
		_calibration.gl_camera_transform * 
		Eigen::Affine3f(Eigen::Scaling(1.0f,1.0f,-1.0f)).matrix() *
		world_to_camera;

	glpp::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, 0,0,0,-1);

	auto model_view_uniform = _program.get_uniform("model_view_matrix");
	auto projection_uniform = _program.get_uniform("projection_matrix");
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

	_association_pixel_buffer.bind();
	glpp::read_pixels(GL_COLOR_ATTACHMENT0, 0,0,_calibration.width, _calibration.height,
			GL_RGBA, GL_FLOAT, 0);

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


Association Associator::update(const mediassist::Pose &pose, const mediassist::rgbd_image &image)
{
	Association association;
	return association;
}


