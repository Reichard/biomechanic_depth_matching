#pragma once

#include "calibration.hpp"
#include "glpp.hpp"
#include "cudapp.hpp"
#include "../../modules/SofaOpenglVisual/OglModel.h"
#include "pose_source.hpp"
#include "rgbd_image.hpp"
#include "association.hpp"


class Associator {
	public:
		using OglModel = sofa::component::visualmodel::OglModel;

		Associator() {}

		inline void set_ogl_model(OglModel::SPtr ogl_model)
		{
			_ogl_model = ogl_model;
		}

		inline void set_calibration(Calibration calibration)
		{
			_calibration = calibration;
		}

		inline cudapp::GraphicsResource &graphics_resource()
		{
			return _association_resource;
		}

		void init();

		void update(const mediassist::Pose &pose);
		Association update(const mediassist::Pose &pose, const mediassist::rgbd_image &image);
	private:
		OglModel::SPtr _ogl_model;
		Calibration _calibration;
		bool initialized = false;

		int _width;
		int _height;

		glpp::Program _program;
		glpp::PixelPackDoubleBuffer _association_pixel_buffer;

		glpp::Framebuffer _fbo;
		glpp::Renderbuffer _depth_stencil_render_buffer;
		glpp::Renderbuffer _association_render_buffer;
		cudapp::GraphicsResource _association_resource;
};
