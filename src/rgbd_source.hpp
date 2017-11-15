#pragma once

#include "rgbd_image.hpp"

namespace mediassist {

class RgbdSource {
	public:
		rgbd_image read(int frame) {
			rgbd_image image;
			return image;
		}

		void prefetch(int frame) {}
};

}
