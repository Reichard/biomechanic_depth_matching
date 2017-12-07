#version 450 core

layout(location=0) in vec3 vertex_position_modelspace;

uniform mat4 model_view_matrix;
uniform mat4 projection_matrix;

void main() {
	gl_Position = projection_matrix * model_view_matrix 
		* vec4(vertex_position_modelspace,1);
};


