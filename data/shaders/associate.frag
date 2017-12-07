#version 450 core

in vec3 barycentric;
in flat int primitive_id;

layout(location=0) out vec4 out_assoc;

void main() {
	out_assoc = vec4(barycentric, primitive_id);
};


