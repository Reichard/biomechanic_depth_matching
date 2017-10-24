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
#include "../../modules/SofaLoader/MeshSTLLoader.h"
#include "../../modules/SofaSimpleFem/TetrahedralCorotationalFEMForceField.h"
#include "../../modules/SofaMiscFem/StandardTetrahedralFEMForceField.h"
#include "../../modules/SofaMiscFem/FastTetrahedralCorotationalForceField.h"
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

#include "my_cuda_spring_force_field.hpp"
#include "MyCudaMechanicalObject.hpp"
#include "spring_attacher.hpp"

#include "cudapp.hpp"


class BiofuTracker {
	public:
		using vec_type = sofa::gpu::cuda::CudaVec3fTypes;
		using SpringForceField = MyCudaSpringForceField;
		using MechanicalState = 
			sofa::component::container::MechanicalObject<vec_type>;
		using FEMForceField = sofa::component::forcefield::TetrahedronFEMForceField<vec_type>;
		using CudaVisualModel = sofa::component::visualmodel::CudaVisualModel<vec_type>;
		using OglModel = sofa::component::visualmodel::OglModel;

		BiofuTracker() = default;

		inline void set_liver_model(MechanicalState::SPtr model) 
		{
			_liver_dofs = model;
		}

		inline void set_surface_model(MechanicalState::SPtr model)
		{
			_surface_dofs = model;
		}

		inline void set_data_model(MechanicalState::SPtr model) 
		{
			_data_dofs = model;
		}

		inline void set_spring_forcefield(SpringForceField::SPtr forcefield)
		{
			_springs = forcefield;
		}

		inline void set_visual_model(CudaVisualModel::SPtr visual_model)
		{
			_visual_model = visual_model;
		}

		inline void set_liver_forcefield(FEMForceField forcefield)
		{
			_liver_foce_field = forcefield;
		}

		inline void set_ogl_model(OglModel::SPtr ogl_model)
		{
			_ogl_model = ogl_model;
		}

		void update(const std::vector<std::array<float,3>> &data_points);

	private:
		void associate();

		MechanicalState::SPtr _liver_dofs = nullptr;
		MechanicalState::SPtr _surface_dofs = nullptr;
		MechanicalState::SPtr _data_dofs = nullptr;
		SpringForceField::SPtr _springs = nullptr;
		FEMForceField::SPtr _liver_force_field = nullptr;
		//TODO: only use the CUDAVisualModel
		OglModel::SPtr _ogl_model = nullptr;
		sofa::component::loader::MeshVTKLoader::SPtr _vtk_loader = nullptr;
		CudaVisualModel::SPtr _visual_model = nullptr;
		CUDASpringAttacher _spring_attacher;
};
