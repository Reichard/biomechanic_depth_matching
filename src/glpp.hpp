#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <Eigen/Dense>
#include <fstream>
#include <iostream>
#include <unordered_map>

namespace glpp {

class Shader 
{
	public:
		enum class Type { VERTEX, FRAGMENT, GEOMETRY };

		Shader(Type type)
		{
			unsigned int gl_type = GL_VERTEX_SHADER;
			switch(type){
				case Type::VERTEX:
					gl_type= GL_VERTEX_SHADER;
					break;
				case Type::FRAGMENT:
					gl_type = GL_FRAGMENT_SHADER;
					break;
				case Type::GEOMETRY:
					gl_type = GL_GEOMETRY_SHADER;
					break;
			}
			_id = glCreateShader(gl_type);
		}

		Shader( Type type, const std::string &path ) : Shader(type)
		{
			load(path);
		}

		inline bool load(const std::string &path)
		{
			std::ifstream code_file(path);
			std::string code((std::istreambuf_iterator<char>(code_file)),
                 std::istreambuf_iterator<char>());

			const char * code_ptr = code.c_str();
            glShaderSource(_id, 1, &code_ptr, nullptr);
            glCompileShader(_id);
			
			int compile_status;
			int info_log_length;
            glGetShaderiv(_id, GL_COMPILE_STATUS, &compile_status);
            glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &info_log_length);
            if(!compile_status) {
				std::string info_log("\0",info_log_length);
				glGetShaderInfoLog(_id, info_log_length, nullptr, const_cast<char*>(info_log.data()));
				std::cout << "[SHADER COMPILE ERROR]" << std::endl << info_log << std::endl;
				return false;
			}

			return true;
		}

		virtual ~Shader() 
		{
			glDeleteShader(_id);
		}

		inline unsigned int id() const { return _id; }

	private:
		unsigned int _id;
};

class VertexShader : public Shader
{
	public:

	VertexShader() : Shader(Shader::Type::VERTEX)
	{
	}

	VertexShader(const std::string &path) : Shader(Shader::Type::VERTEX, path)
	{
	}

	virtual ~VertexShader()
	{
	}
};
	
class FragmentShader : public Shader
{
	public: 
		
	FragmentShader() : Shader(Shader::Type::FRAGMENT)
	{
	}

	FragmentShader(const std::string &path) : Shader(Shader::Type::FRAGMENT, path)
	{
	}

	virtual ~FragmentShader()
	{
	}
};

class GeometryShader : public Shader
{
	public:

	GeometryShader() : Shader(Shader::Type::GEOMETRY)
	{
	}

	GeometryShader(const std::string &path) : Shader(Shader::Type::GEOMETRY, path)
	{
	}

	virtual ~GeometryShader()
	{
	}
};

class Uniform 
{
	public:
		Uniform() = default;
		Uniform( int location ) : _location(location) {}

		inline void set_location(int location)
		{
			_location = location;
		}

		inline void upload( const int value )
		{
			glUniform1i(_location, value);
		}

		inline void upload( const Eigen::Matrix4f &matrix )
		{
			glUniformMatrix4fv(_location,1,false,matrix.data());
		}

		inline int location() { return _location; }

	private:
		int _location=0;
};

class Program 
{
	public:
		class LinkStatus
		{
			public:
				LinkStatus(bool status, std::string info_log) 
					: _status(status), _info_log(info_log) {}

				operator bool() { return _status; }
				bool status() { return _status; }
				std::string info_log() { return _info_log; }

			private:
				bool _status;
				std::string _info_log;
		};

		Program()
		{
			_id = glCreateProgram();
		}

		~Program()
		{
			glDeleteProgram(_id);
		}
	
		inline void attach_shader(const Shader& shader)
		{
			glAttachShader(_id,shader.id());
		}

		inline void detach_shader(const Shader& shader)
		{
			glDetachShader(_id,shader.id());
		}

		inline void use()
		{
			glUseProgram(_id);
		}

		inline LinkStatus link()
		{
			glLinkProgram(_id);

			int status;
			int info_log_length;
			glGetProgramiv(_id, GL_LINK_STATUS, &status);
			glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &info_log_length);

			std::string info_log("\0", info_log_length);
			glGetProgramInfoLog(_id, info_log_length, nullptr,
					const_cast<char*>(info_log.data()));

			return LinkStatus(status, info_log);
		}

		inline int get_uniform_location(const std::string &name)
		{
			return glGetUniformLocation(_id, name.c_str());
		}

		inline Uniform get_uniform(const std::string &name)
		{
			return Uniform(glGetUniformLocation(_id, name.c_str()));
		}

		inline static void use_default()
		{
			glUseProgram(0);
		}

		unsigned int id() { return _id; }
	private:
		unsigned int _id;
};

class VertexArray 
{
	public:
		VertexArray() 
		{
			glGenVertexArrays(1,&_id);
		}

		~VertexArray()
		{
			glDeleteVertexArrays(1,&_id);
		}

		inline void bind()
		{
			glBindVertexArray(_id);
		}

		/*
		inline void enable_attribute(int index) 
		{
			glEnableVertexArrayAttrib(_id, index);
		}

		void disable_attribute(int index) 
		{
			glDisableVertexArrayAttrib(_id, index);
		}
		*/

	private:
		unsigned int _id;
};

class ElementBuffer
{
	public:
		ElementBuffer()
		{
			glGenBuffers(1,&_id);
		}

		~ElementBuffer()
		{
			glDeleteBuffers(1,&_id);
		}

		inline void bind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		}

		inline void unbind()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		inline void upload_static(size_t buffer_data_size, const unsigned int* buffer_data)
		{
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * buffer_data_size,
					buffer_data, GL_STATIC_DRAW);
		}

		inline void upload_stream(size_t buffer_data_size, const unsigned int* buffer_data)
		{
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * buffer_data_size,
					buffer_data, GL_STREAM_DRAW);
		}

		template<class container_type>
		void upload_static(const container_type &container )
		{
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(typename container_type::value_type)*container.size(),
					container.data(), GL_STATIC_DRAW);
		}

		template<class container_type>
		void upload_stream(const container_type &container )
		{
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(typename container_type::value_type)*container.size(),
					container.data(), GL_STREAM_DRAW);
		}

		inline unsigned int id() { return _id; }

		static inline void bind_default()
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		unsigned int _id;
};

class VertexBuffer 
{
	public:
		VertexBuffer()
		{
			glGenBuffers(1,&_id);
		}

		~VertexBuffer()
		{
			glDeleteBuffers(1,&_id);
		}

		inline void bind()
		{
			glBindBuffer(GL_ARRAY_BUFFER, _id);
		}

		inline void upload_static(size_t buffer_data_size, const float* buffer_data)
		{
			bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer_data_size,
					buffer_data, GL_STATIC_DRAW);
		}

		inline void upload_stream(size_t buffer_data_size, const float* buffer_data)
		{
			bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * buffer_data_size,
					buffer_data, GL_STREAM_DRAW);
		}

		template<class container_type>
		inline void upload_static(const container_type &container )
		{
			bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(typename container_type::value_type)*container.size(),
					container.data(), GL_STATIC_DRAW);
		}

		template<class container_type>
		inline void upload_stream(const container_type &container )
		{
			bind();
			glBufferData(GL_ARRAY_BUFFER, sizeof(typename container_type::value_type)*container.size(),
					container.data(), GL_STREAM_DRAW);
		}

		inline unsigned int id() { return _id; }

		static inline void bind_default()
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		static inline unsigned int current_id()
		{
			int id;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id);
			return static_cast<unsigned int>(id);
		}

	private:
		unsigned int _id;
};

//TODO: allow other texture types
class Texture2D
{
	public:
		Texture2D()
		{
			glGenTextures(1,&_id);
		}

		~Texture2D()
		{
			glDeleteTextures(1,&_id);
		}

		inline void bind()
		{
			glBindTexture(GL_TEXTURE_2D,_id);
		}

		inline void set(unsigned int width, unsigned int height)
		{
			GLenum target = GL_TEXTURE_2D;
  			GLint level = 0;
  			GLint internalFormat = GL_RGBA32F;
  			GLint border = 0;
  			GLenum format = GL_RGBA;
  			GLenum type = GL_FLOAT;
  			const GLvoid * data = nullptr;

			bind();
  			glTexImage2D(target,level,internalFormat,width,height,border,format,type,data);
		}

		inline void upload(unsigned int width, unsigned int height, float* data)
		{
			GLenum target = GL_TEXTURE_2D;
  			GLint level = 0;
  			GLint internalFormat = GL_RGBA32F;
  			GLint border = 0;
  			GLenum format = GL_RGBA;
  			GLenum type = GL_FLOAT;

			bind();
  			glTexImage2D(target,level,internalFormat,width,height,border,format,type,data);
		}

		inline void attach(unsigned int attachment_point)
		{
			bind();
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+attachment_point,
				GL_TEXTURE_2D, _id, 0);
		}

		inline unsigned int id() 
		{ 
			return _id; 
		}

		template<class Container>
		void read(Container &container)
		{
			bind();
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, container.data());
		}

	private:
		unsigned int _id;
};

inline void set_active_texture_unit(unsigned int i)
{
	glActiveTexture(GL_TEXTURE0 + i);
}

class Renderbuffer
{
	public:
		Renderbuffer()
		{
			glGenRenderbuffers(1,&_id);
		}

		~Renderbuffer()
		{
			glDeleteRenderbuffers(1,&_id);
		}

		inline void bind()
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _id);
		}

		inline void set_storage(GLenum internal_format, GLsizei width, GLsizei height )
		{
			bind();
			glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
		}

		inline void set_storage(GLsizei width, GLsizei height )
		{
			bind();
			set_storage( GL_RGBA8, width, height );
		}

		inline void attach(GLenum attachment_point)
		{
			bind();
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_point,
					GL_RENDERBUFFER, _id);
		}

		inline void attach_depth_stencil(GLsizei width, GLsizei height)
		{
			set_storage(GL_DEPTH24_STENCIL8, width, height);
			attach(GL_DEPTH_STENCIL_ATTACHMENT);
		}

		inline unsigned int id()
		{
			return _id;
		}

		static inline void bind_default()
		{
			glBindRenderbuffer(GL_RENDERBUFFER,0);
		}

	private:
		unsigned int _id;
};

class Framebuffer
{
	public:
		class Status
		{
		public:
			
			Status(GLenum status) : _status(status)
			{}

			std::string to_string()
			{
				switch(_status)
				{
					case GL_FRAMEBUFFER_COMPLETE:
						return "complete";
						break;
					case GL_FRAMEBUFFER_UNDEFINED:
						return "undefined";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
						return "incomplete attachment";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
						return "incomplete missing attachment";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
						return "incomplete draw buffer";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
						return "incomplete read buffer";
						break;
					case GL_FRAMEBUFFER_UNSUPPORTED:
						return "unsupported";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
						return "incomplete multisample";
						break;
					case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
						return "incomplete layer targets";
						break;
					default:
						return "unknown status";
				}
			}

			bool complete()
			{
				return _status == GL_FRAMEBUFFER_COMPLETE;
			}

			operator bool()
			{
				return _status != GL_FRAMEBUFFER_COMPLETE;
			}

			private:
				GLenum _status;
		};

		Framebuffer()
		{
			glGenFramebuffers(1,&_id);
		}

		~Framebuffer()
		{
			glDeleteFramebuffers(1,&_id);
		}

		inline void bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _id);
		}

		inline void bind_draw() const
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _id);
		}

		inline void bind_read() const
		{
			glBindFramebuffer(GL_READ_FRAMEBUFFER, _id);
		}

		static inline void bind_default()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		static inline void blit(int src_x0, int src_y0, int src_x1, int src_y1,
				int dst_x0, int dst_y0, int dst_x1, int dst_y1,
				GLbitfield mask, GLenum filter)
		{
			glBlitFramebuffer(src_x0, src_y0, src_x1, src_y1,
					dst_x0, dst_y0, dst_x1, dst_y1,
					mask, filter);
		}

		static inline const Framebuffer &default_framebuffer()
		{
			static Framebuffer _default(0);
			return _default;
		}

		inline void unbind() const
		{
			default_framebuffer().bind();
		}

		inline unsigned int id() const
		{
			return _id;
		}

		inline Status check_status() const
		{
			bind();
			GLenum fbo_status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			return Status(fbo_status);
		}

		inline void attach(Renderbuffer &rb, GLenum attachment_point) const
		{
			bind();
			rb.attach(attachment_point);
		}

		inline void attach_color_buffer(Renderbuffer &rb, unsigned int index)
		{
			bind();
			rb.attach(GL_COLOR_ATTACHMENT0 + index);
		}

	private:
		Framebuffer(unsigned int id) : _id(id)
		{
		}

		unsigned int _id;
};

class Error
{
	public:
		Error(GLenum type) : _type(type)
		{}

		inline std::string to_string()
		{
			switch(_type)
			{
				case GL_NO_ERROR: return "no error"; break;
				case GL_INVALID_ENUM: return "invalid enum"; break;
				case GL_INVALID_VALUE: return "invalid value"; break;
				case GL_INVALID_OPERATION: return "invalid operation"; break;
				case GL_STACK_OVERFLOW: return "stack overflow"; break;
				case GL_STACK_UNDERFLOW: return "stack underflow"; break;
				case GL_OUT_OF_MEMORY: return "out of memory"; break;
				case GL_INVALID_FRAMEBUFFER_OPERATION: return "invalid framebuffer operation"; break;
				//case GL_CONTEXT_LOST: return "context lost"; break;
				case GL_TABLE_TOO_LARGE: return "table too large"; break;
				default: return "unknown error";
			}
		}

		inline GLenum type() { return _type; }

		inline operator bool()
		{
			return _type != GL_NO_ERROR;
		}

		static inline Error get() { return Error(glGetError()); }

	private:
		GLenum _type;
};

class Viewport
{
	public:

		Viewport(int x, int y, int width, int height)
			: _x(x), _y(y), _width(width), _height(height)
		{
		}

		static Viewport current()
		{
			Viewport viewport(0,0,0,0);
			glGetIntegerv(GL_VIEWPORT, reinterpret_cast<GLint*>(&viewport));
			return viewport;
		}

		inline void use() const
		{
			glViewport(_x,_y,_width,_height);
		}

		inline int x() const
		{
			return _x;
		}

		inline int y() const
		{
			return _y;
		}

		inline int width() const
		{
			return _width;
		}

		inline int height() const
		{
			return _height;
		}

	private:
		int _x, _y, _width, _height;
};

inline void set_viewport(const Viewport& viewport)
{
	viewport.use();
}

inline Viewport get_viewport()
{
	return Viewport::current();
}

inline void finish()
{
	glFinish();
}

inline void flush()
{
	glFlush();
}

inline void read_pixels(GLenum buffer_name, int x, int y, int width, int height,
		GLenum format, GLenum data_type, void *data)
{
	glReadBuffer(buffer_name);
	glReadPixels(x,y,width,height,format,data_type,data);
}

inline void disable_client_state(GLenum capability)
{
	glDisableClientState(capability);
}

inline void enable_client_state(GLenum capability)
{
	glEnableClientState(capability);
}

inline void set_clear_color(float r, float g, float b, float a)
{
	glClearColor(r,g,b,a);
}

inline void clear(GLbitfield buffers)
{
	glClear(buffers);
}

inline void clear(GLbitfield buffers, float r, float g, float b, float a)
{
	set_clear_color(r,g,b,a);
	clear(buffers);
}

inline void draw_elements(GLenum primitive_type, unsigned int index_count,
		GLenum index_data_type, const void* data)
{
    glDrawElements(primitive_type, index_count, index_data_type, data);
}

inline void set_normal_pointer(GLenum data_type, unsigned int offset, const void* data)
{
    glNormalPointer (data_type, offset, data);
}

inline void set_vertex_pointer(unsigned int component_count, 
		GLenum data_type, unsigned int offset, const void* data)
{
   	glVertexPointer (component_count, data_type, offset, data);
}

inline void set_color_pointer(unsigned int component_count,
		GLenum data_type, unsigned int offset, const void* data)
{
    glColorPointer (component_count, data_type, offset, data);
}


inline void enable(GLenum feature)
{
	glEnable(feature);
}

inline void disable(GLenum feature)
{
	glDisable(feature);
}

inline void enable_depth_test()
{
	enable(GL_DEPTH_TEST);
}

inline void disable_depth_test()
{
	disable(GL_DEPTH_TEST);
}

inline void enable_culling()
{
	enable(GL_CULL_FACE);
}

inline void disable_culling()
{
	disable(GL_CULL_FACE);
}

inline void enable_lighting()
{
	enable(GL_LIGHTING);
}

inline void disable_lighting()
{
	disable(GL_LIGHTING);
}

inline void enable_stencil_test()
{
	enable(GL_STENCIL_TEST);
}

inline void disable_stencil_test()
{
	disable(GL_STENCIL_TEST);
}

template<class Container> 
inline void set_draw_buffers( const Container &buffers_list )
{
 	glDrawBuffers(buffers_list.size(),buffers_list.data());
}

class PixelPackBuffer
{
	public:

		PixelPackBuffer(size_t size = 0)
		{
			glGenBuffers(1,&_id);
			resize(size);
		}

		~PixelPackBuffer()
		{
			glDeleteBuffers(1,&_id);
		}

		inline unsigned int id()
		{
			return _id;
		}

		inline void bind()
		{
			glBindBuffer(GL_PIXEL_PACK_BUFFER, _id);
		}

		inline void resize(size_t size, int access = GL_STREAM_READ)
		{
			bind();
			glBufferData(GL_PIXEL_PACK_BUFFER, size, nullptr, access);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

		template<class T=void>
		inline T *map()
		{
			bind();
			T *data = reinterpret_cast<T*>(glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY));
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			return data;
		}

		template<class Container>
		inline bool copy(Container &c)
		{
			auto *data = map<typename Container::value_type>();
			for(size_t i=0; i<c.size(); ++i)
			{
				c[i] = data[i];
			}
			return unmap();
		}

		inline bool unmap()
		{
			bind();
			bool status = glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
			return status;
		}

		inline static void unbind()
		{
			glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
		}

	private:

		unsigned int _id;
};

class PixelPackDoubleBuffer
{
	public:
		PixelPackDoubleBuffer() = default;
		~PixelPackDoubleBuffer() = default;

		inline PixelPackBuffer& front()
		{
			return _buffers[_front];
		}

		inline PixelPackBuffer& back()
		{
			return _buffers[_back];
		}

		inline void swap()
		{
			_front = (_front + 1) % 2;
			_back = (_front + 1) % 2;
		}

		inline void resize(size_t size, int access = GL_STREAM_READ)
		{
			for(auto &b : _buffers) b.resize(size, access);
		}

	private:
		unsigned int _front = 0;
		unsigned int _back = 1;
		PixelPackBuffer _buffers[2];
};

namespace legacy {
	inline void set_color( float r, float g, float b, float a )
	{
		glColor4f(r,g,b,a);
	}

	inline void set_color( float r, float g, float b )
	{
		glColor3f(r,g,b);
	}

	inline void begin(GLenum primitive_type)
	{
		glBegin(primitive_type);
	}

	inline void end()
	{
		glEnd();
	}

	inline void render(GLenum primitive_type, std::function<void()> block)
	{
		begin(primitive_type);
		block();
		end();
	}

	inline void render_triangles(std::function<void()> block)
	{
		begin(GL_TRIANGLES);
		block();
		end();
	}

	inline void render_points(std::function<void()> block)
	{
		begin(GL_POINTS);
		block();
		end();
	}

	inline void emit_vertex2(float* data)
	{
		glVertex2fv(data);
	}

	inline void emit_vertex3(float* data)
	{
		glVertex3fv(data);
	}

	inline void emit_vertex4(float* data)
	{
		glVertex4fv(data);
	}
	
	inline void set_matrix_mode(GLenum mode)
	{
		glMatrixMode(mode);
	}

	inline void push_matrix()
	{
		glPushMatrix();
	}

	inline void pop_matrix()
	{
		glPopMatrix();
	}

	inline void load_identity_matrix()
	{
		glLoadIdentity();
	}

	inline void multiply_matrix(float * data)
	{
		glMultMatrixf(data);
	}

	inline void multiply_matrix(double * data)
	{
		glMultMatrixd(data);
	}

}

}
