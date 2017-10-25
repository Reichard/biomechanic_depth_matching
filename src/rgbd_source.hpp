#pragma once

#include "rgbd_image.hpp"

namespace mediassist {

class RgbdSource {
	public:
		rgbd_image operator[](int frame) {
			rgbd_image image;
			return image;
		}
};

}
