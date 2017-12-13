#pragma once

#include <string>
#include <iomanip>

#include <Eigen/Dense>

#include "../xmlParser/tinyxml2.h"

namespace mediassist {

	struct Pose {
		Eigen::Affine3f transformation;
		bool valid = false;
	};

	//TODO: get rid of in_meters parameter
	//find a good convention for the whole project probably

	class PoseSource {
		public:
			PoseSource(std::string path, int offset=0, bool in_meters=false, bool use_simulation_data=true ) 
				: _path(path), _offset(offset), _frame(_offset), _in_meters(in_meters), 
				_use_simulation_data(use_simulation_data)
			{
				//TODO: why do we need this initialization
				_patient = Eigen::Translation3f(0.001*(619.320000),0.001*(-2084.500000),0.001*(-967.850000))*Eigen::AngleAxisf(Eigen::Quaternionf(0.825955,0.504534,0.062804,0.243516).toRotationMatrix()) ;
			}

			Pose read()
			{
				using namespace tinyxml2;

				auto filename = make_filename();

				//extract polaris position data from xml file
				XMLDocument doc;
				doc.LoadFile( filename.c_str() );
				XMLText* textNode, *textNodePatient;
				if(_use_simulation_data)
					textNode = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_camera" )->FirstChild()->ToText();
				else
				{
					textNode = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_Endoscope" )->FirstChild()->ToText();
					textNodePatient = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_Patient" )->FirstChild()->ToText();
				}

				const char* title = textNode->Value();
				std::istringstream iss(title);
				bool visible, visibleP;
				float x, y, z, w, tx, ty, tz;
				if(_use_simulation_data)
				{
					iss >> x >> y >> z >> w >> tx >> ty >> tz;
					visible = true;
				}
				else
					iss >> x >> y >> z >> w >> tx >> ty >> tz >> visible;
				//std::cout << std::setprecision(10) << a << " " << b << " " << c << " " << d << " " << e << " " << f << " " << g << std::endl;
				Eigen::Quaternionf quat(w, x, y, z);
				if(_in_meters)
				{
					tx *= 0.001;
					ty *= 0.001;
					tz *= 0.001;
				}
				Eigen::Translation3f translate(tx,ty,tz);
				Eigen::Affine3f pose = translate * Eigen::AngleAxisf(quat.toRotationMatrix());

				//for invivo: get pos in patient coord.
				if(!_use_simulation_data)
				{
					std::istringstream issP(textNodePatient->Value());
					issP >> x >> y >> z >> w >> tx >> ty >> tz >> visibleP;
					Eigen::Quaternionf quatP(w, x, y, z);
					if(_in_meters)
					{
						tx *= 0.001;
						ty *= 0.001;
						tz *= 0.001;
					}
					Eigen::Translation3f translateP(tx,ty,tz);
					Eigen::Affine3f poseP = translateP * Eigen::AngleAxisf(quatP.toRotationMatrix());

					_patient = poseP;
					pose = _patient.inverse(Eigen::Affine) * pose;

				}

				Pose ret;
				ret.transformation = pose;
				ret.valid = visible;
				return ret;
			}

			void next() {
				_frame++;
			}

			unsigned int frame() {
				return _frame;
			}

			void reset()
			{
				_frame = 0;
			}

		private:
			std::string make_filename() {
				std::stringstream filename;
				filename << _path << "/";
				if(_use_simulation_data)
					filename << "data";
				else
					filename << "PolarisData";
				filename<< std::setfill('0') << std::setw(4);
				filename << _frame << ".xml";

				return filename.str();
			}

			std::string _path;
			int _offset;
			int _frame;
			bool _in_meters;
			bool _use_simulation_data;
            Eigen::Affine3f _patient;
	};

}
