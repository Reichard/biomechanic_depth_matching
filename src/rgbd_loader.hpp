#pragma once

#include <iostream>

#include "rgbd_image.hpp"
#include "sequence_loader.hpp"

class RgbdLoader : public SequenceLoader<mediassist::rgbd_image> {
	public:
		RgbdLoader() = default;
		~RgbdLoader() = default;
		RgbdLoader(std::string prefix, std::string postfix) : _prefix(prefix), _postfix(postfix) {}

		void setPrefixAndPostfix(std::string prefix, std::string postfix) {
			_prefix = prefix;
			_postfix = postfix;
		}

	protected:
		item_type load_item(index_type index) {
			std::string filename = _prefix + std::to_string(index) + _postfix;
			std::cout << filename << std::endl;
			return mediassist::rgbd_image::read(filename);
		}
	
	private:
		std::string _prefix = "";
		std::string _postfix = "";
};
