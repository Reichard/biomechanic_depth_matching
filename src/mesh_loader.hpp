#pragma once

#include <iostream>
#include <sofa/helper/io/MeshSTL.h>

#include "sequence_loader.hpp"

class MeshLoader : public SequenceLoader<sofa::helper::io::MeshSTL> {
	public:
		MeshLoader() = default;
		virtual ~MeshLoader() = default;
		MeshLoader(std::string prefix, std::string postfix) : _prefix(prefix), _postfix(postfix) {}

		void setPrefixAndPostfix(std::string prefix, std::string postfix) {
			_prefix = prefix;
			_postfix = postfix;
		}

	protected:
		item_type load_item(index_type index) {
			std::string filename = _prefix + std::to_string(index) + _postfix;
			return item_type(filename);
		}
	
	private:
		std::string _prefix = "";
		std::string _postfix = "";
};
