#include "depth_match.hpp"

#include <sofa/core/ObjectFactory.h>


DepthMatchComponent::DepthMatchComponent()
	: _pixel_skip_count(sofa::core::objectmodel::Base::initData(&_pixel_skip_count,
			"pixel_skip_count", "use every Nth pixels when attaching springs")),
	_max_spring_stiffness(sofa::core::objectmodel::Base::initData(&_max_spring_stiffness,
			"max_spring_stiffness", "maximum stiffness of our interaction springs")),
	_max_data_distance(sofa::core::objectmodel::Base::initData(&_max_data_distance,
			"max_data_distance", "maximum valid data distance")),
	_stats_file_path(sofa::core::objectmodel::Base::initData(&_stats_file_path,
			"stats_file", "location of stats file (it is only written if this is not empty)")),
	_substep_count(sofa::core::objectmodel::Base::initData(&_substep_count,
			"substeps", "number of substeps per frame")),
	_volume_prefix(sofa::core::objectmodel::Base::initData(&_volume_prefix,
			"volume_prefix", "prefix of volume files")),
	_surface_prefix(sofa::core::objectmodel::Base::initData(&_surface_prefix,
			"surface_prefix", "prefix of surface files")),
	_rgbd_prefix(sofa::core::objectmodel::Base::initData(&_rgbd_prefix,
			"rgbd_prefix", "prefix of rgbd files")),
	_ground_truth_prefix(sofa::core::objectmodel::Base::initData(&_ground_truth_prefix,
			"ground_truth_prefix", "prefix of ground truth files")),
	_polaris_folder(sofa::core::objectmodel::Base::initData(&_polaris_folder,
			"polaris_folder", "location of polaris files")),
	_polaris_offset(sofa::core::objectmodel::Base::initData(&_polaris_offset,
			"polaris_offset", "offset of polaris data, relative to start of camera sequence")),
	_verify_option(sofa::core::objectmodel::Base::initData(&_verify_option,
			"verify", "enable or disable verification")),
	_calibration_path(sofa::core::objectmodel::Base::initData(&_calibration_path,
			"calibration", "path of the camera calibration file"))
{
}

DepthMatchComponent::~DepthMatchComponent() {
}

void DepthMatchComponent::on_frame(int frame) {
	auto image = rgbd_source[frame];
}

void DepthMatchComponent::update() {
	auto association = associator.update(pose, image);
	boundary_conditions.update(association);
}

void DepthMatchComponent::init() {
	read_config();
}

void DepthMatchComponent::read_config() {
	std::string visual_model_path = "liver/visual/cuda";
	//getContext()->get(_visual_model,visual_model_path);
	//if(_visual_model == nullptr) {
	//	std::cout << "NO VISUAL MODEL FOUND" << std::endl;
	//}

	//std::string ogl_model_path = "liver/visu/VisualModel";
	//getContext()->get(_ogl_model,ogl_model_path);
	//if(_ogl_model == nullptr) {
	//	std::cout << "NO OPENGL MODEL FOUND" << std::endl;
	//}
	//_ogl_model->setColor(1,1,1,0);

	std::string reference_ogl_model_path = "liver/visu/reference_model";
	//getContext()->get(_reference_ogl_model,reference_ogl_model_path);
	//if(_reference_ogl_model == nullptr) {
	//	std::cout << "NO REFERENCE OPENGL MODEL FOUND" << std::endl;
	//}
	//_reference_ogl_model->setColor(1,1,1,0);

	std::string surface_dofs_path = "liver/surface/dofs";
	//getContext()->get(_surface_dofs,surface_dofs_path);
	//if(_surface_dofs == nullptr) {
	//	std::cout << "NO SURFACE MODEL FOUND" << std::endl;
	//}

	std::string liver_dofs_path = "liver/dofs";
	//getContext()->get(_liver_dofs,liver_dofs_path);
	//if(_liver_dofs == nullptr) {
	//	std::cout << "NO LIVER MODEL FOUND" << std::endl;
	//}

	std::string volume_dofs_path = "liver/dofs";
	//getContext()->get(_volume_dofs,volume_dofs_path);
	//if(_volume_dofs == nullptr) {
	//	std::cout << "NO VOLUME MODEL FOUND" << std::endl;
	//}

	std::string data_dofs_path = "data/dofs";
	//getContext()->get(_data_dofs,data_dofs_path);
	//if(_data_dofs == nullptr) {
	//	std::cout << "NO DATA MODEL FOUND" << std::endl;
	//}

	std::string marker_model_path = "liver/markers/model";
	//getContext()->get(_marker_model,marker_model_path);
	//if(_marker_model== nullptr) {
	//	std::cout << "NO MARKERS FOUND" << std::endl;
	//}

	std::string rest_dofs = "anchors/rest_spring_anchors";
	//getContext()->get(_rest_spring_anchors,rest_dofs);
	//if(_rest_spring_anchors == nullptr) {
	//	std::cout << "NO REST MODEL FOUND" << std::endl;
	//}

	std::string springs_path = "springs";
	//getContext()->get(_springs,springs_path);
	//if(_springs == nullptr) {
	//	std::cout << "NO SPRING FORCEFIELD FOUND" << std::endl;
	//}

	std::string rest_springs_path = "rest_springs";
	//getContext()->get(_rest_springs,rest_springs_path);
	//if(_rest_springs == nullptr) {
	//	std::cout << "NO REST SPRING FORCEFIELD FOUND" << std::endl;
	//}

	std::string liver_force_field_path = "liver/FEM";
	//getContext()->get(_liver_force_field,liver_force_field_path);
	//if(_liver_force_field == nullptr) {
	//	std::cout << "NO LIVER FORCEFIELD FOUND" << std::endl;
	//}

	std::string vtk_loader_path = "vtk_loader";
	//getContext()->get(_vtk_loader,vtk_loader_path);
	//if(_vtk_loader == nullptr) {
	//	std::cout << "NO VTK LOADER FOUND" << std::endl;
	//}
}

void DepthMatchComponent::bwdInit() {

}

void DepthMatchComponent::draw(const sofa::core::visual::VisualParams *) {

}

void DepthMatchComponent::handleEvent(sofa::core::objectmodel::Event *) {

}

int DepthMatchComponentClass = sofa::core::RegisterObject("").add<DepthMatchComponent>();
