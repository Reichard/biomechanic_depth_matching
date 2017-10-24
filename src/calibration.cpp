#include "calibration.hpp"
#include <fstream>

void Calibration::write(const std::string &path)
{
	std::ofstream out_file(path);
	out_file << width << std::endl;
	out_file << height << std::endl;
	out_file << intrinsic_matrix << std::endl;
	out_file << projection_matrix << std::endl;
	out_file << gl_projection_matrix << std::endl;
	out_file << gl_camera_transform << std::endl;
}

void Calibration::read(const std::string &path)
{
	//TODO: save and load this as well

	std::ifstream in_file(path);

	in_file >> width ;
	in_file >> height;

	for(int j=0; j<3; ++j)
		for(int i=0; i<3; ++i)
		{
			in_file >> intrinsic_matrix(i,j);
		}

	for(int j=0; j<4; ++j)
		for(int i=0; i<4; ++i)
		{
			in_file >> projection_matrix(i,j);
		}

	for(int j=0; j<4; ++j)
		for(int i=0; i<4; ++i)
		{
			in_file >> gl_projection_matrix(i,j);
		}

	for(int j=0; j<4; ++j)
		for(int i=0; i<4; ++i)
		{
			in_file >> gl_camera_transform(i,j);
		}

	//TODO: fix read/write and convert existing file
	gl_projection_matrix.transposeInPlace();
	gl_camera_transform.transposeInPlace();
	
	gl_inverse_projection_matrix = gl_projection_matrix.inverse();
}

Eigen::Vector2f Calibration::project(const Eigen::Vector3f &point)
{
	Eigen::Vector4f ndc = gl_projection_matrix * point.homogeneous();
	ndc /= ndc.w();

	Eigen::Vector2f pixel;
	pixel << 0.5f*(ndc.x() + 1)*width, height - 0.5f*(ndc.y() + 1)*height;
	
	return pixel;
}

// image point given as (x(pixels),y(pixels),depth)
Eigen::Vector3f Calibration::reproject(const Eigen::Vector2f &image_point, float depth)
{
	Eigen::Vector4f ndc;
	ndc << 2.0f*image_point.x()/width-1.0f,-2.0f*image_point.y()/height+1.0f,1,1;
	Eigen::Vector4f point = gl_inverse_projection_matrix * ndc;
	point *= depth;
	point.w() = 1;
	return point.head<3>();
}

Eigen::Matrix4f Calibration::make_gl_orthogonal_projection_matrix( 
		float left, float right, float bottom, float top, float near, float far )
{
	Eigen::Matrix4f ortho;
	ortho << 
		2/(right-left), 0, 0, -(right+left)/(right-left),
		0, 2/(top-bottom), 0, -(top+bottom)/(top-bottom),
		0, 0, -2/(far-near), -(far+near)/(far-near),
		0, 0, 0, 1;

	return ortho;
}

Eigen::Matrix4f Calibration::make_gl_projection_matrix( const Eigen::Vector2f &focal_length,
		Eigen::Vector2f &principal_point, unsigned int width, unsigned int height,
		float near, float far )
{
	/* 
	 *  Construct the perspective projection matrix like this
	 *  
		fx   s   -cx  0
		0    fy  -cy  0
		0    0   A    B
		0    0   -1   0
		
		A = near + far and B = near * far
	*/

	auto f = focal_length;
	auto c = principal_point;

	auto a = near + far;
	auto b = near * far;

	//TODO: use distortion parameters
	
	Eigen::Matrix4f perspective_projection;
	perspective_projection << 
		f.x(),   0, -c.x(), 0,
		0,   f.y(), -c.y(), 0,
		0,     0,    a, b,
		0,     0,   -1, 0;

	Eigen::Matrix4f normalization = 
		make_gl_orthogonal_projection_matrix(0,width,height,0,near,far);

	return normalization * perspective_projection;
}

Eigen::Matrix4f Calibration::make_gl_modelview_matrix( const Eigen::Matrix3f &rotation,
		const Eigen::Vector3f &translation )
{
	auto r = rotation;
	auto t = translation;

	Eigen::Matrix4f modelview;
	modelview <<
		r(0,0), r(0,1), r(0,2), t.x(),
		r(1,0), r(1,1), r(1,2), t.y(),
		r(2,0), r(2,1), r(2,2), t.z(),
		0,       0,    0,   1;

	return modelview;
}
