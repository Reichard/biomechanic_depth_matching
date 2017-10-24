#pragma once

#include <string>
#include <iomanip>
#include <fstream>

#include <Eigen/Dense>

#include "tinyxml2.h"

namespace mediassist {

	struct Pose {
		Eigen::Affine3f transformation;
		bool valid = false;

		operator bool()
		{
			return valid;
		}
	};

	//TODO: get rid of in_meters parameter
	//find a good convention for the whole project probably

	class PoseSource {
		public:
			PoseSource(std::string path, int offset=0, bool in_meters=false ) 
				: _path(path), _offset(offset), _frame(_offset), _in_meters(in_meters)
			{
			}

			Pose read(int frame)
			{
				using namespace tinyxml2;

				std::string filename;

				for(auto f : make_possible_filenames(frame))
				{
					std::ifstream file(f);
					if(file.good())
					{
						filename = f;
						break;
					}
				}

				if(filename.empty())
				{
					Pose invalid_pose;
					invalid_pose.valid = false;
					return invalid_pose;
				}

				//extract polaris position data from xml file
				XMLDocument doc;
				doc.LoadFile( filename.c_str() );
				XMLText* textNode = nullptr;
				XMLText* textNodePatient = nullptr;

				auto endoscope_position_node = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_Endoscope" );
				auto patient_position_node = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_Patient" );
				auto camera_position_node = doc.FirstChildElement( "ImageData" )->FirstChildElement( "position_camera" );

				if(endoscope_position_node)
				{
					textNode = endoscope_position_node->FirstChild()->ToText();
					textNodePatient = patient_position_node->FirstChild()->ToText();
				} else {
					textNode = camera_position_node->FirstChild()->ToText();
				}

				const char* title = textNode->Value();
				bool visible = true;
				bool visibleP = true;
				float x, y, z, w, tx, ty, tz;

				std::istringstream iss(title);
				if(camera_position_node)
				{
					iss >> x >> y >> z >> w >> tx >> ty >> tz;
					visible = true;
				}
				else
				{
					iss >> x >> y >> z >> w >> tx >> ty >> tz >> visible;
				}

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
				if(patient_position_node)
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

					pose = poseP.inverse(Eigen::Affine) * pose;
				}

				Pose ret;
				ret.transformation = pose;
				ret.valid = visible;
				return ret;
			}

			Pose read()
			{
				return read(_frame);
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

			void set_path(const std::string &path)
			{
				_path = path;
			}

		private:
			std::array<std::string,2> make_possible_filenames(int frame) {
				std::stringstream filename_sim;
				filename_sim << _path << "/";
				filename_sim << "data";
				filename_sim << std::setfill('0') << std::setw(4);
				filename_sim << frame << ".xml";

				std::stringstream filename_real;
				filename_real << _path << "/";
				filename_real << "PolarisData";
				filename_real << std::setfill('0') << std::setw(4);
				filename_real << frame << ".xml";

				return {{filename_sim.str(), filename_real.str()}};
			}

			std::string _path;
			int _offset;
			int _frame;
			bool _in_meters;
	};

}
