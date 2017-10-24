#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <cuda.h>
#include <cudaGL.h>

struct vec3f {
	float x,y,z;
};

struct vec3i {
	int x,y,z;
};

void attach_gpu_springs( float *data, float* association_data, float* springs, unsigned int* triangles, 
		float *vertices, float *normals, float* weights, unsigned int size, unsigned int width, unsigned int height );

void map_association_surface(CUarray &array);
