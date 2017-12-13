#include "test_component.hpp"
#include <sofa/core/ObjectFactory.h>

#include <sofa/simulation/common/AnimateBeginEvent.h>
#include <sofa/simulation/common/AnimateEndEvent.h>
#include <sofa/core/visual/VisualParams.h>
#include "../../modules/SofaOpenglVisual/OglModel.h"

#include <iostream>

 
TestComponent::TestComponent()
{
	f_listening.setValue(true);

	const std::string vertex_shader_code = 
		"#version 420 compatibility\n"
		"\n"
		"layout(location=0) in vec3 vertex_position_modelspace;\n"
		"\n"
		"uniform mat4 model_view_matrix;\n"
		"uniform mat4 projection_matrix;\n"
		"\n"
		"out int vertex_id;\n"
		"\n"
		"void main() {\n"
		//"	gl_Position = projection_matrix * model_view_matrix * vec4(vertex_position_modelspace,1);\n"
		"	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vertex_position_modelspace,1);\n"
		"	vertex_id = gl_VertexID;\n"
		"}\n";

	const std::string geometry_shader_code = 
		"#version 420 core\n"
		"\n"
		"layout(triangles) in;\n"
		"layout(triangle_strip) out;\n"
		"layout(max_vertices=3) out;\n"
		"\n"
		"uniform mat4 model_view_matrix;\n"
		"uniform mat4 projection_matrix;\n"
		"\n"
		"in int vertex_id[];\n"
		"\n"
		"out vec3 barycentric;\n"
		"out flat int vertex_id_0;\n"
		"out flat int vertex_id_1;\n"
		"out flat int vertex_id_2;\n"
		"\n"
		"void main() {\n"
		"	vertex_id_0 = vertex_id[0];\n"
		"	vertex_id_1 = vertex_id[1];\n"
		"	vertex_id_2 = vertex_id[2];\n"
		"	\n"
		"	gl_Position = gl_in[0].gl_Position;\n"
		"	barycentric = vec3(1,0,0);\n"
		"	EmitVertex();\n"
		"	\n"
		"	gl_Position = gl_in[1].gl_Position;\n"
		"	barycentric = vec3(0,1,0);\n"
		"	EmitVertex();\n"
		"	\n"
		"	gl_Position = gl_in[2].gl_Position;\n"
		"	barycentric = vec3(0,0,1);\n"
		"	EmitVertex();\n"
		"	\n"
		"}\n";

	const std::string fragment_shader_code = 
		"#version 420 compatibility\n"
		"\n"
		"in vec3 barycentric;\n"
		"in flat int vertex_id_0;\n"
		"in flat int vertex_id_1;\n"
		"in flat int vertex_id_2;\n"
		"\n"
		"out vec3 color;\n"
		"\n"
		"void main() {\n"
		"	color = vec3(float(vertex_id_0 % 16) / 16.0f);\n"
		"	color = barycentric;\n"
		"}\n";


	_associaton_program = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	GLuint geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	auto vsc = vertex_shader_code.c_str();
	auto gsc = geometry_shader_code.c_str();
	auto fsc = fragment_shader_code.c_str();
	glShaderSource(vertex_shader, 1, &vsc, 0);
	glShaderSource(geometry_shader, 1, &gsc, 0);
	glShaderSource(fragment_shader, 1, &fsc, 0);

	glAttachShader(_associaton_program, vertex_shader);
	glAttachShader(_associaton_program, geometry_shader);
	glAttachShader(_associaton_program, fragment_shader);
	glLinkProgram(_associaton_program);

	int info_log_length;
 	glGetProgramiv(_associaton_program, GL_INFO_LOG_LENGTH, &info_log_length);

 	if ( info_log_length > 0 ){
 		std::string message("",info_log_length+1);
 		glGetProgramInfoLog(_associaton_program, info_log_length, NULL, &message[0]);
		std::cout << message << std::endl;
 	}
}
 
TestComponent::~TestComponent()
{
}

void TestComponent::draw(const sofa::core::visual::VisualParams *params)
{
	//TODO: support multiple face groups

	if(_ogl_model == nullptr) return;

	const auto &vertices = _ogl_model->getVertices();
	const auto &normals = _ogl_model->getVnormals();
	const auto &indices = _ogl_model->getTriangles();

	double model_view_matrix[16];
	double projection_matrix[16];
	params->getModelViewMatrix(model_view_matrix);
	params->getProjectionMatrix(projection_matrix);

	float model_view_matrixf[16];
	float projection_matrixf[16];
	for(int i=0; i<16; i++) model_view_matrixf[i] = static_cast<float>(model_view_matrix[i]);
	for(int i=0; i<16; i++) projection_matrixf[i] = static_cast<float>(projection_matrix[i]);

	//_ogl_model->xforms[0].writeOpenGlMatrix(model_view_matrix);

	auto model_view_location = glGetUniformLocation(_associaton_program, "model_view_matrix");
	auto projection_location = glGetUniformLocation(_associaton_program, "projection_matrix");
	glUniformMatrix4fv(model_view_location, 1, GL_FALSE, model_view_matrixf);
	glUniformMatrix4fv(projection_location, 1, GL_FALSE, projection_matrixf);

    //glNormalPointer (GL_FLOAT, 0, vnormals.getData());
    //glEnableClientState(GL_NORMAL_ARRAY);

	glDisable(GL_CULL_FACE);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(projection_matrix);
	//glOrtho(-5,5,-5,5,-5,5);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glMultMatrixd(model_view_matrix);

	//glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor3f(0,1,0);

	glUseProgram(_associaton_program);

	/*
	glBegin(GL_TRIANGLES);
	{
		glVertex3f(-1,-1, 0);
		glVertex3f( 1,-1, 0);
		glVertex3f( 0, 1, 0);
	}
	glEnd();
	*/


    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer (3, GL_FLOAT, 0, vertices.getData());
    glDrawElements(GL_TRIANGLES, indices.size()*3, GL_UNSIGNED_INT, indices.getData());
    glDisableClientState(GL_VERTEX_ARRAY);

    glUseProgram(0);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void TestComponent::handleEvent(sofa::core::objectmodel::Event *event)
{
	using sofa::simulation::AnimateBeginEvent;

	if(AnimateBeginEvent* animate_begin_event = dynamic_cast<AnimateBeginEvent*>(event))
	{
		std::cout << "animate begin" << std::endl;
	}
}

void TestComponent::init()
{
	using sofa::component::visualmodel::OglModel;

	std::cout << "init" << std::endl;
	_ogl_model = searchFromRoot<OglModel>();
	if(_ogl_model == nullptr) {
		std::cout << "NO OPENGL MODEL FOUND" << std::endl;
	}
}

void TestComponent::bwdInit()
{
	std::cout << "bwdInit" << std::endl;
}
 
int TestComponentClass = sofa::core::RegisterObject("This component does nothing.").add<TestComponent>();
