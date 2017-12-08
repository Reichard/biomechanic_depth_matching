#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <sofa/helper/io/MeshVTK.h>

#include "sequence_loader.hpp"

using GroundTruthData = std::vector<std::array<float,3>>;

class GroundTruthLoader : public SequenceLoader<GroundTruthData> {
	public:
		GroundTruthLoader() = default;
		virtual ~GroundTruthLoader() = default;
		GroundTruthLoader(std::string prefix, std::string postfix) : _prefix(prefix), _postfix(postfix) {}

		void setPrefixAndPostfix(std::string prefix, std::string postfix) {
			_prefix = prefix;
			_postfix = postfix;
		}

		void setSize(int width, int height) {
			_width = width;
			_height = height;
		}

	protected:
		item_type load_item(index_type index) {
			std::stringstream filename_stream;
			filename_stream << _prefix << std::setw(4) << std::setfill('0') << index << _postfix;
			auto filename = filename_stream.str();
			
			std::ifstream file(filename);

			GroundTruthData data(_width * _height);

			if(!file.good())
			{
				std::cout << "no ground truth available for current frame" << std::endl;
				std::cout << filename << std::endl;
			}

			//read and flip the image
			for(int y=_height-1; y>=0; --y)
			{
				for(int x=0; x<_width; ++x)
				{
					auto& pos = data[y*_width+x];

					if(!file.good()) 
					{
						std::cout << "error while reading ground truth! (too few points?)" << std::endl;
						std::cout << "( x = " << x << " y = " << y << " )" << std::endl;
						std::cout << "( width = " << _width << " height = " << _height << " )" << std::endl;
						break;
					}

					file >> pos[0];
					file.ignore();
					file >> pos[1];
					file.ignore();
					file >> pos[2];

					if(pos[0] > 1000000)
					{
						pos[0] = 1.0f/0.0f;
						pos[1] = 1.0f/0.0f;
						pos[2] = 1.0f/0.0f;
					}
				}
				if(!file.good()) break;
			}

			return data;
		}

	private:
		std::string _prefix = "";
		std::string _postfix = "";

		int _width = 0;
		int _height = 0;
};
