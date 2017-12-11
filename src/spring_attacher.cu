#include "spring_attacher_gpu.hpp"
#include <sofa/gpu/cuda/CudaCommon.h>
#include <iostream>
#include <cuda_gl_interop.h>

#include "kernel_math.hpp"

struct assoc_t{
	int prim;
	float3 bary;
	bool valid;

	__device__
	bool is_valid()
	{
		return valid;
	}
};

__device__ 
assoc_t unpack_association_data(float4 &packed)
{
	assoc_t assoc;

	//for(int i=0; i<3; ++i) get(assoc.bary,i) = get(packed,i);
	assoc.bary.x = packed.x;
	assoc.bary.y = packed.y;
	assoc.bary.z = packed.z;
	if(packed.w >= 0){
		assoc.prim = (unsigned int) packed.w;
		assoc.valid = true;
	} else {
		assoc.prim = 0;
		assoc.valid = false;
	}

	return assoc;
}

__device__
int get_index_x()
{
	int index = blockDim.x * blockIdx.x + threadIdx.x;
	return index;
}

__device__
int2 get_index_xy()
{
	int2 index;
	index.x = blockDim.x * blockIdx.x + threadIdx.x;
	index.y = blockDim.y * blockIdx.y + threadIdx.y;
	return index;
}

__device__
void fetch_vertices(float3* vertices, const float3* vertex_list, uint3 &vertex_indices)
{
	for(size_t i=0; i<3; ++i) vertices[i] = vertex_list[get(vertex_indices,i)];
}

__global__
void clear_kernel(float3* anchor_list, float* weights, unsigned int size)
{
	int index = get_index_x();

	if(index < size) {
		anchor_list[index].x = 0;
		anchor_list[index].y = 0;
		anchor_list[index].z = 0;
		weights[index] = 0; 
	}
}

__global__
void accumulate_forces_kernel(float3* data, float4* association_data, float3* anchor_list, uint3* triangles,
		float3* vertex_list, float3* normal_list, float* weights,
		unsigned int width, unsigned int height)
{
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;

	if( x >= width || y >= height) return;

	float3 data_point = data[x+y*width];

	if(!isfinite(data_point)) return;

	assoc_t assoc = unpack_association_data(association_data[x+y*width]);

	if(!assoc.valid) {
		for(int dx=-2; dx<=2; ++dx) {
			for(int dy=-2; dy <=2; ++dy) {
				int idx = (x+dx)+(y+dy)*width;
				assoc = unpack_association_data(association_data[idx]);
				if(assoc.valid) break;
			}
		}
	}

	if(!assoc.valid) return;

	uint3 triangle = triangles[assoc.prim];

	for(int i=0; i<3; ++i)
	{
		unsigned int vertex_index = get(triangle,i);
		float3 vertex = vertex_list[vertex_index];
		float3 normal = normal_list[vertex_index];
		normalize(normal);
		float3 &anchor = anchor_list[vertex_index];
		float &weight = weights[vertex_index];

		float3 relative = data_point - vertex;

		float len = norm(relative);
		
		if(len < 25) {
			float l = relative*normal;
			if(l > 5) l = 5;
			float3 projected = normal * l;

			if(len>5) projected *= 25/(len*len);

			if(isfinite(projected)){
				accumulate(anchor, projected);
				atomicAdd(&weight ,1);
			}
		}
	}
}

__global__
void attach_kernel(float3* anchors, float3* vertices, float* weights, unsigned int vertex_count)
{
	int index = get_index_x();
	if(index >= vertex_count) return;

	float3 &anchor = anchors[index];
	const float3 &vertex = vertices[index];
	const float &weight = weights[index];

	float3 force = anchor;
	float3 displacement_vector = {0.0001f,0,0};

	if(weight > 0 && isfinite(force)) {
		displacement_vector = force / weight;
	}

	if(norm2(displacement_vector) < 0.00001f) {
		displacement_vector.x = 0.0001f;
		displacement_vector.y = 0;
		displacement_vector.z = 0;
	}
	anchor = vertex + displacement_vector;
}


void attach_gpu_springs(
		float *data,
		float *association_data,
		float *anchors,
		unsigned int *triangles,
		float *vertices,
		float *normals,
		float* weights,
		unsigned int vertex_count,
		unsigned int width,
		unsigned int height)
{
	dim3 vertex_dim = dim3(vertex_count,1,1);
	dim3 num_threads_vertices = dim3(BSIZE,1,1);
	dim3 num_blocks_vertices = calculate_block_count(vertex_dim, num_threads_vertices);

	dim3 image_dim = dim3(width,height,1);
	dim3 num_threads_image = dim3(8,8,1);
	dim3 num_blocks_image = calculate_block_count(image_dim, num_threads_image);


	clear_kernel<<<num_blocks_vertices,num_threads_vertices>>>(
			(float3*) anchors,
			weights,
			vertex_count
	);
	cudaDeviceSynchronize();
	accumulate_forces_kernel<<<num_blocks_image, num_threads_image>>>(
			(float3*) data,
			(float4*) association_data,
			(float3*) anchors,
			(uint3*) triangles,
			(float3*) vertices,
			(float3*) normals,
			weights,
			width,
			height
	);
	cudaDeviceSynchronize();

	attach_kernel<<<num_blocks_vertices, num_threads_vertices>>>(
		(float3*) anchors,
		(float3*) vertices,
		weights,
		vertex_count
	);
	cudaDeviceSynchronize();
}

void map_association_surface(CUarray &array)
{
}
