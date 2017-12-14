#pragma once

#include <sofa/core/objectmodel/BaseObject.h>
#include "../../modules/SofaOpenglVisual/OglModel.h"
#include "../../modules/SofaDeformable/StiffSpringForceField.h"
#include <sofa/core/objectmodel/Data.h>
#include <sofa/core/objectmodel/Link.h>
#include <sofa/core/behavior/BaseMechanicalState.h>
#include <sofa/core/behavior/BaseForceField.h>
#include <sofa/core/visual/VisualParams.h>
#include <SofaBaseMechanics/MechanicalObject.h>
#include <SofaBaseMechanics/BarycentricMapping.h>
#include "../../modules/SofaLoader/MeshSTLLoader.h"
#include "../../modules/SofaSimpleFem/TetrahedronFEMForceField.h"
#include <sofa/helper/io/MeshSTL.h>
#include <sofa/helper/io/MeshVTK.h>
#include "../../modules/SofaLoader/MeshVTKLoader.h"
#include <sofa/gpu/cuda/CudaTypes.h>
#include <sofa/gpu/cuda/CudaVisualModel.h>
#include <sofa/defaulttype/VecTypes.h>
#include <future>

#include "calibration.hpp"
#include "glpp.hpp"

#include "pose_source.hpp"

#include "stats.hpp"
#include "rgbd_image.hpp"

#include "mutable_cuda_spring_force_field.hpp"
#include "spring_attacher.hpp"

#include "cudapp.hpp"

#include "associator.hpp"

#include "mesh_loader.hpp"
#include "volume_loader.hpp"
#include "ground_truth_loader.hpp"

class BiomechanicalDepthMatching : public sofa::core::objectmodel::BaseObject
{
	public:
		SOFA_CLASS(BiomechanicalDepthMatching, sofa::core::objectmodel::BaseObject);

		using vec_type = sofa::gpu::cuda::CudaVec3fTypes;
		using SpringForceField = MutableCudaSpringForceField;
		using MechanicalState = 
			sofa::component::container::MechanicalObject<vec_type>;
		using BarycentricMapping = 
			sofa::component::mapping::BarycentricMapping<vec_type,vec_type>;
		using STLLoader = sofa::component::loader::MeshSTLLoader;
		using OglModel = sofa::component::visualmodel::OglModel;
		using FEMForceField = sofa::component::forcefield::TetrahedronFEMForceField<vec_type>;
		using CudaVisualModel = sofa::component::visualmodel::CudaVisualModel<vec_type>;

		template<class T>
			using Data = sofa::core::objectmodel::Data<T>;

		BiomechanicalDepthMatching ();
		virtual ~BiomechanicalDepthMatching () = default;
		virtual void init() override;
		virtual void draw(const sofa::core::visual::VisualParams *) override;
		virtual void handleEvent(sofa::core::objectmodel::Event *) override;

		void write_surface_stl(const std::string &path);
		void write_surface_ascii(const std::string &path);

	private:
		void setup_scene();
		void read_next_frame();
		void update();

		void draw_data_positions();
		void draw_ground_truth();
		void draw_ground_truth_volume();
		void draw_error();
		void draw_association();

		Data<unsigned int> _pixel_stride;
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
		Data<std::string> _calibration_path;
		Data<std::string> _output_path;

		size_t _vertex_count = 0;
		size_t _width = 0;
		size_t _height = 0;
		size_t _pixel_count = 0;

		std::future<bool> read_rgbd_data_async(int frame);

		MechanicalState::SPtr _liver_dofs = nullptr;
		MechanicalState::SPtr _surface_dofs = nullptr;
		MechanicalState::SPtr _data_dofs = nullptr;
		OglModel::SPtr _marker_model = nullptr;
		MechanicalState::SPtr _volume_dofs = nullptr;
		MechanicalState::SPtr _rest_spring_anchors = nullptr;
		SpringForceField::SPtr _springs = nullptr;
		SpringForceField::SPtr _rest_springs = nullptr;
		OglModel::SPtr _ogl_model = nullptr;
		OglModel::SPtr _reference_ogl_model = nullptr;
		FEMForceField::SPtr _liver_force_field = nullptr;
		sofa::component::loader::MeshVTKLoader::SPtr _vtk_loader = nullptr;
		CudaVisualModel::SPtr _visual_model = nullptr;

		Calibration _calibration;

		glpp::Framebuffer _fbo;
		glpp::Renderbuffer _debug_render_buffer;

		std::vector<std::array<float,3>> _ground_truth;
		std::vector<std::array<float,3>> _ground_truth_read_buffer;
		std::future<bool> _ground_truth_future;

		sofa::helper::io::MeshSTL _reference_mesh;
		std::future<bool> _mesh_future;

		std::future<bool> _volume_future;
		std::vector<std::array<float,3>> _volume_points;

		std::vector<std::array<float,3>> _data_points;
		std::vector<std::array<float,3>> _data_points_read_buffer;

		std::vector<std::array<float,3>> _color_points;
		std::vector<std::array<float,3>> _color_points_read_buffer;

		std::future<bool> _data_points_future;

		mediassist::rgbd_image _img;
		mediassist::rgbd_image _draw_img;

		int _current_frame = -1;
		int _simulation_frame = -1;

		std::vector<std::array<float,3>> _vertex_colors;

		mediassist::PoseSource _pose_source;
		mediassist::Pose _pose;

		Associator _associator;

		CUDASpringAttacher _spring_attacher;

		MeshLoader _surface_mesh_loader;
		VolumeLoader _volume_mesh_loader;
		GroundTruthLoader _ground_truth_loader;
};
