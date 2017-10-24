#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace mediassist {
	struct rgb {
		unsigned char r,g,b;
	};

	struct rgbd {
		rgb color;
		unsigned short depth;
	};

	class rgbd_image {
		public:
			rgbd_image() = default;
			rgbd_image(int width, int height)
			{
				resize(width,height);
			}

			rgbd at(int x, int y) const {
				if(x < 0 || x >= _width || y < 0 || y >= _height) return {{0,0,0},0};
				return {_color[index(x,y)],_depth[index(x,y)]};
			}

			void set(int x, int y, rgbd value) {
				if(x < 0 || x >= _width || y < 0 || y >= _height) return;
				_color[index(x,y)] = value.color;
				_depth[index(x,y)] = value.depth;
			}

			rgb color_at( int x, int y) const {
				if(x < 0 || x >= _width || y < 0 || y >= _height) return {0,0,0};
				return _color[index(x,y)];
			}

			short depth_at(int x, int y) const {
				if(x < 0 || x >= _width || y < 0 || y >= _height) return 0;
				return _depth[index(x,y)];
			}

			void resize( int width, int height) {
				_width = width;
				_height = height;
				_color.resize(width*height);
				_depth.resize(width*height);
			}

			inline int width() const { return _width; }
			inline int height() const { return _height; }

			inline void write(const std::string &path)
			{
				std::ofstream file(path, std::ofstream::binary );

				file.write(reinterpret_cast<char*>(&_width), sizeof(_width));
				file.write(reinterpret_cast<char*>(&_height), sizeof(_height));
				for(int y=0; y<_height; ++y)
				{
					for(int x=0; x<_width; ++x)
					{
						auto pixel = at(x,y);
						file.write(reinterpret_cast<char*>(&pixel), sizeof(pixel));
					}
				}
			}

			inline static rgbd_image read(const std::string &path)
			{
				std::ifstream file(path, std::ifstream::binary);

				int width, height;
				file.read(reinterpret_cast<char*>(&width), sizeof(width));
				file.read(reinterpret_cast<char*>(&height), sizeof(height));

				rgbd_image img(width, height);

				for(int y=0; y<height; ++y)
				{
					for(int x=0; x<width; ++x)
					{
						rgbd pixel;
						file.read(reinterpret_cast<char*>(&pixel), sizeof(pixel));
						img.set(x,y,pixel);
					}
				}

				return img;
			}

		private:
			int index(int x , int y) const { return x + y * _width; }

			int _width, _height;
			std::vector<rgb> _color;
			std::vector<unsigned short> _depth;
	};
}
