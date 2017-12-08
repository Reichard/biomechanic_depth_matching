#include "spring_attacher.hpp"
#include "spring_attacher_gpu.hpp"
#include <cassert>

void CUDASpringAttacher::attach_springs( std::vector< std::array<float,3> > &data_points)
{
	assert(data_points.size() == _width*_height);
	
	int data_size_in_bytes = data_points.size()*sizeof(float)*3;
	if(_device_data == nullptr) {
		std::cout << cudapp::device_malloc(&_device_data, _width*_height*3).to_string() << std::endl; }
	cuMemcpyHtoD((CUdeviceptr)_device_data, data_points.data(), data_size_in_bytes );

	auto anchors = reinterpret_cast<float*>(_anchors->writePositions().wref().deviceWrite());
	auto surface = reinterpret_cast<float*>(_surface->readPositions().ref().deviceRead());
	auto normals = reinterpret_cast<float*>(_visual_model->vnormals.deviceRead());
	auto triangles = reinterpret_cast<unsigned int*>(_visual_model->triangles.deviceRead());

	auto data = _device_data;
	auto weights = thrust::raw_pointer_cast(_weights.data());
	
	float* association_data;
	_association_resource->map();
	auto size = _association_resource->mapped_pointer(association_data);

	attach_gpu_springs( data, association_data, 
			anchors, triangles, surface, normals, weights, _vertex_count, _width, _height );

	_association_resource->unmap();
}

void CUDASpringAttacher::set_association_array(cudapp::Array &array)
{
	map_association_surface(array.get());
}
