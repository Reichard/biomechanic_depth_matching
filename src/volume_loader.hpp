#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <sofa/helper/io/MeshVTK.h>

#include "../../modules/SofaLoader/MeshVTKLoader.h"

#include "sequence_loader.hpp"

using VolumeData = std::vector<std::array<float,3>>;

class VolumeLoader : public SequenceLoader<VolumeData> {
	public:
		VolumeLoader() = default;
		virtual ~VolumeLoader() = default;
		VolumeLoader(std::string prefix, std::string postfix) : _prefix(prefix), _postfix(postfix) {}

		void setPrefixAndPostfix(std::string prefix, std::string postfix) {
			_prefix = prefix;
			_postfix = postfix;
		}

	protected:
		item_type load_item(index_type index) {
			std::string filename = _prefix + std::to_string(index) + _postfix;

			auto loader = sofa::core::objectmodel::New<sofa::component::loader::MeshVTKLoader>();
			loader->setFilename(filename);
			loader->load();
			loader->updateMesh();
			const auto &points = loader->positions.getValue();

			VolumeData volume(points.size());

			for(size_t i=0; i<points.size(); ++i)
			{
				std::array<float,3> point {{
					static_cast<float>(points[i][0]), 
					static_cast<float>(points[i][1]), 
					static_cast<float>(points[i][2])
				}};
				volume[i] = point;
			}

			return volume;
		}
	
	private:
		std::string _prefix = "";
		std::string _postfix = "";
};
