#pragma once

#include <string>
#include <iostream>
#include <cuda.h>
#include <cudaGL.h>

#include "glpp.hpp"

#define CHECK_CUDA_CALL(cerr, str) if(cerr != CUDA_SUCCESS)\
                                    {\
                                      std::cout << str << ": " << cerr << "\n";\
                                      assert(false);\
                                    }
namespace cudapp {

	class Result
	{
		public:
			Result(CUresult res) : _result(res) {}

			inline operator bool()
			{
				return _result == CUDA_SUCCESS;
			}

			inline std::string to_string()
			{
				if(*this) { 
					return "CUDA SUCCESS";
				} else {
					switch(_result) {
						case CUDA_ERROR_DEINITIALIZED:
							return "CUDA_ERROR_DEINITIALIZED";
						case CUDA_ERROR_NOT_INITIALIZED:
							return "CUDA_ERROR_NOT_INITIALIZED";
						case CUDA_ERROR_INVALID_CONTEXT:
							return "CUDA_ERROR_INVALID_CONTEXT";
						case CUDA_ERROR_INVALID_HANDLE:
							return "CUDA_ERROR_INVALID_HANDLE";
						case CUDA_ERROR_ALREADY_MAPPED:
							return "CUDA_ERROR_ALREADY_MAPPED";
						case CUDA_ERROR_UNKNOWN:
							return "CUDA_ERROR_UNKNOWN";
						default: return "CUDA ERROR";
					}
				}
			}

			inline void print_debug()
			{
				//#ifndef NDEBUG
				if(!*this) std::cout << to_string() << std::endl;
				//#endif
			}
		private:
			CUresult _result;
	};

	inline Result debug_call(Result res)
	{
		res.print_debug();
		assert(res);
		return res;
	}

	class Array
	{
		public:
			Array() = default;
			Array(CUarray a) : _array(a)
			{
			}

			inline CUarray &get()
			{
				return _array;
			}

			inline static Array map(glpp::Texture2D &texture);
			inline static Array map(glpp::Renderbuffer &render_buffer);

		private:
			CUarray _array;
	};

	class GraphicsResource {
		public:
			GraphicsResource() = default;

			GraphicsResource(glpp::Texture2D &texture)
			{
				debug_call(cuGraphicsGLRegisterImage( &_resource,
    			texture.id(),
        		GL_TEXTURE_2D,
        		CU_GRAPHICS_MAP_RESOURCE_FLAGS_READ_ONLY));	
			}

			GraphicsResource(glpp::Renderbuffer &render_buffer)
			{
				debug_call(cuGraphicsGLRegisterImage( &_resource,
    			render_buffer.id(),
        		GL_RENDERBUFFER,
        		CU_GRAPHICS_MAP_RESOURCE_FLAGS_READ_ONLY));	
			}

			GraphicsResource(glpp::PixelPackBuffer &pbo)
			{
				debug_call(cuGraphicsGLRegisterBuffer( &_resource,
    			pbo.id(),
        		CU_GRAPHICS_MAP_RESOURCE_FLAGS_READ_ONLY));	
			}

			inline void map()
			{
				debug_call(cuGraphicsMapResources(1,&_resource,0));
			}

			inline void unmap()
			{
				debug_call(cuGraphicsUnmapResources(1,&_resource,0));
			}

			inline Array mapped_array()
			{
				Array array;
				debug_call(cuGraphicsSubResourceGetMappedArray(&array.get(), _resource, 0, 0));
				return array;
			}

			inline Array map_array()
			{
				map();
				return mapped_array();
				unmap();
			}

			template<class P>
			size_t mapped_pointer(P *&ptr)
			{
				CUdeviceptr _device_ptr;
				size_t size = 16;
				auto result = debug_call(cuGraphicsResourceGetMappedPointer(
					&_device_ptr,
					&size,
					_resource
				));
				ptr = reinterpret_cast<P*>(_device_ptr);
;	
				return size;
			}

		private:
			CUgraphicsResource _resource = 0;
	};


	Array Array::map(glpp::Texture2D &texture)
	{
		GraphicsResource res(texture);
		return res.map_array();
	}

	Array Array::map(glpp::Renderbuffer &render_buffer)
	{
		GraphicsResource res(render_buffer);
		return res.map_array();
	}

	class SurfaceReference {
		public:
			SurfaceReference() = default;

			SurfaceReference(Array& array)
			{
				set_array(array);
			}

			inline void set_array(Array& array)
			{
				debug_call(cuSurfRefSetArray(_surf, array.get(), 0));
			}

			inline CUsurfref &get()
			{
				return _surf;
			}

		private:
			CUsurfref _surf;
	};


	class Device {
		public:
			Device(int id) 
			{
    			debug_call(cuDeviceGet(&_device, id));
			}

			inline CUdevice &get()
			{
				return _device;
			}

		private:
			CUdevice _device;
	};

	inline Device default_device()
	{
		return Device(0);
	}


	class GLContext {
		public:
			GLContext(Device &device)
			{
  				debug_call(cuGLCtxCreate(&_context, CU_CTX_SCHED_AUTO, device.get()));
			}

			inline CUcontext &get()
			{
				return _context;
			}

		private:
			CUcontext _context;
	};

	template<class T>
	Result device_malloc(T** ptr, unsigned int size) {
		unsigned int size_in_bytes = size * sizeof(T);
		return Result(cuMemAlloc((CUdeviceptr*)ptr, size_in_bytes ));
	}
}
