#pragma once

#include <Eigen/Dense>
#include <string>

class Calibration {
	public:
		void write(const std::string &path);
		void read(const std::string &path);

		Eigen::Vector2f project(const Eigen::Vector3f &point);
		Eigen::Vector3f reproject(const Eigen::Vector2f &image_point, float depth);

		int width,height;
		Eigen::Matrix3f intrinsic_matrix;
		Eigen::Matrix4f projection_matrix;
		Eigen::Matrix4f gl_projection_matrix;
		Eigen::Matrix4f gl_inverse_projection_matrix;
		Eigen::Matrix4f gl_camera_transform;


		static Eigen::Matrix4f make_gl_projection_matrix( const Eigen::Vector2f &focal_length,
			Eigen::Vector2f &principal_point, unsigned int width, unsigned int height,  float near, float far );

		static Eigen::Matrix4f make_gl_modelview_matrix( const Eigen::Matrix3f &rotation,
			const Eigen::Vector3f &translation );

		static Eigen::Matrix4f make_gl_orthogonal_projection_matrix( 
			float left, float right, float bottom, float top, float near, float far );
};
