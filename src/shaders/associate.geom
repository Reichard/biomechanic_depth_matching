#version 450 core

layout(triangles) in;
layout(triangle_strip) out;
layout(max_vertices=3) out;

out vec3 barycentric;
out flat int primitive_id;

void main() {
	primitive_id = gl_PrimitiveIDIn;

	gl_Position = gl_in[0].gl_Position;
	barycentric = vec3(1,0,0);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	barycentric = vec3(0,1,0);
	EmitVertex();

	gl_Position = gl_in[2].gl_Position;
	barycentric = vec3(0,0,1);
	EmitVertex();

};


