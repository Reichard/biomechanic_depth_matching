#pragma once

#include <sofa/core/objectmodel/BaseObject.h>

#include "rgbd_source.hpp"
#include "non_rigid_tracker.hpp"
#include "rgbd_image.hpp"
#include "associator.hpp"
#include "boundary_conditions.hpp"

class DepthMatchComponent : public sofa::core::objectmodel::BaseObject {
	public:
		SOFA_CLASS(DepthMatchComponent, sofa::core::objectmodel::BaseObject);

		template<class T>
		using Data = sofa::core::objectmodel::Data<T>;

		DepthMatchComponent();
		virtual ~DepthMatchComponent();

		virtual void init();
		virtual void bwdInit();
		virtual void draw(const sofa::core::visual::VisualParams *);
		virtual void handleEvent(sofa::core::objectmodel::Event *);

		void read_config();

		void on_frame(int frame);
		void update();

	private:
		mediassist::RgbdSource rgbd_source;
		mediassist::rgbd_image image;

		mediassist::PoseSource pose_source;
		mediassist::Pose pose;

		Associator associator;
		BoundaryConditions boundary_conditions;

		Data<unsigned int> _pixel_skip_count;
		Data<float> _max_spring_stiffness;
		Data<float> _max_data_distance;
		Data<std::string> _stats_file_path;
		Data<unsigned int> _substep_count;
		Data<std::string> _volume_prefix;
		Data<std::string> _surface_prefix;
		Data<std::string> _rgbd_prefix;
		Data<std::string> _ground_truth_prefix;
		Data<std::string> _polaris_folder;
		Data<unsigned int> _polaris_offset;
		Data<bool> _verify_option;
		Data<std::string> _calibration_path;
};
