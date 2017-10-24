#pragma once

#include <sofa/gpu/cuda/CudaTypes.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <SofaOpenglVisual/OglModel.h>

#include <thrust/device_vector.h>
#include <thrust/host_vector.h>

#include <sofa/helper/io/MeshSTL.h>

#include <cassert>
#include <array>

#include <sofa/gpu/cuda/CudaVisualModel.h>

#include "cudapp.hpp"


class CUDASpringAttacher
{
	using vec_type = sofa::gpu::cuda::CudaVec3fTypes;
	using MechanicalState = sofa::component::container::MechanicalObject<vec_type>;
	using Mesh = sofa::helper::io::MeshSTL;
	using OglModel = sofa::component::visualmodel::OglModel;
	using CudaVisualModel = sofa::component::visualmodel::CudaVisualModel<vec_type>;

	public:
		void attach_springs(std::vector<std::array<float,3>> &_data_points);

		void set_association_array(cudapp::Array &array);
		void set_association_resource(cudapp::GraphicsResource &resource)
		{
			_association_resource = &resource;
		}

		void set_association_render_buffer(glpp::Renderbuffer &render_buffer)
		{
			_renderbuffer_id = render_buffer.id();
		}

		template<class Container>
		void set_surface(MechanicalState::SPtr surface, const Container &topology)
		{
			std::vector<int> triangles;
			for(auto &face_group : topology)
			{
				for(auto &face : face_group)
				{
					assert(face.size() == 3);
					triangles.push_back(face[0]);
					triangles.push_back(face[1]);
					triangles.push_back(face[2]);
				}
			}
			_triangles = triangles;
			_surface = surface;

			_vertex_count = _surface->readPositions().size();
			thrust::host_vector<float> weights(_vertex_count,0);
			_weights = weights;
		}

		void set_anchors(MechanicalState::SPtr anchors)
		{
			_anchors = anchors;
		}

		void set_ogl_model(OglModel::SPtr ogl_model)
		{
			//_ogl_model = ogl_model;
		}

		void set_visual_model(CudaVisualModel::SPtr visual_model)
		{
			_visual_model = visual_model;
		}

		void set_width(int width) {
			_width = width;
		}

		void set_height(int height) {
			_height= height;
		}


	private:
		void accumulate_forces()
		{
		}

		cudapp::GraphicsResource *_association_resource;

		thrust::device_vector<int> _triangles;
		thrust::device_vector<float> _weights;
		thrust::device_vector<float> _data_points;

		//TODO: only use these temporarily
		thrust::device_vector<float> _normals;

		MechanicalState::SPtr _surface;
		MechanicalState::SPtr _anchors;
		//OglModel::SPtr _ogl_model;
		CudaVisualModel::SPtr _visual_model;

		int _renderbuffer_id;

		size_t _vertex_count;

		float *_device_data = nullptr;

		int _width=0;
		int _height=0;
};
