## add custom find packages to module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/applications/plugins/MSML/CMake")

project(MSML)

#########################
# Find VTK
#########################

FIND_PACKAGE(VTK REQUIRED)

INCLUDE(${VTK_USE_FILE})

#########################
# Find ITK
#########################

#FIND_PACKAGE(ITK REQUIRED)
#INCLUDE(${ITK_USE_FILE})  


#FIND_PACKAGE(TAUCS REQUIRED)
#INCLUDE_DIRECTORIES(${TAUCS_INCLUDE_DIRS})

#FIND_PACKAGE(PARALUTION REQUIRED)
#INCLUDE_DIRECTORIES(${PARALUTION_INCLUDE_DIRS})

#FIND_PACKAGE(LEVMAR REQUIRED)
#INCLUDE_DIRECTORIES(${LEVMAR_INCLUDE_DIRS})

FIND_PACKAGE(MUMPS REQUIRED)
INCLUDE_DIRECTORIES(${MUMPS_INCLUDE_DIRS})

#########################
# SOLID
#########################
#FIND_PACKAGE(SOLID REQUIRED)
#INCLUDE_DIRECTORIES(${SOLID_INCLUDE_DIRS})




# Semi-colon required here. See FindCUDA.cmake.
#SET(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS};-Xcompiler; -Wall")

#set data paths
SET(CMAKE_MEDIASSIST_DATA_PATH "/home/mediassi/MSMLData/"
    CACHE PATH "Choose the data path.")

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/applications/plugins/MSML/CMake/MSMLConfig.h.in ${CMAKE_SOURCE_DIR}/applications/plugins/MSML/MediAssistConfig.h)


SET(SOFA_MODULES_INCLUDE_DIRECTORIES
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/constraint
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/forcefield
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/helper
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/io
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/linearsolver
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/linearsolver/precond
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/mapper
	${CMAKE_SOURCE_DIR}/applications/plugins/MSML/odesolver
)




set(HEADER_FILES


)

set(SOURCE_FILES
	#constraint/DirichletBoundaryConstraint.cpp
	#forcefield/LinearMeshMatrixMass.cpp
	#forcefield/LinearTetrahedralCorotationalFEMForceField.cpp
	#forcefield/QuadraticMeshMatrixMass.cpp
	#forcefield/QuadraticTetrahedralCorotationalFEMForceField.cpp
	#forcefield/ShapeMatchingElectricPotentialForcefield.cpp
	#forcefield/ShapeMatchingPoissonPotentialForcefield.cpp
	#forcefield/ShapeMatchingPoissonPotentialForcefieldSimulationDriver.cpp
	#helper/ParalutionInitializer.cpp
	#helper/PETScInitializer.cpp
	#io/ExtendedVTKExporter.cpp
	#io/MeshExtendedVTKLoader.cpp
	linearsolver/MumpsSolver.cpp
	#linearsolver/ParalutionSolver.cpp
	#mapper/CollisionDetectionMeshSampler.cpp
	#mapper/MapperQuadraticTetrahedronSetTopology.cpp
	#mapper/MapperLinearTetrahedronSetTopology.cpp
	#mapper/StructuredHexahedralMeshSampler.cpp
	#mapper/UnstructuredTetrahedralMeshSampler.cpp
	#odesolver/MyNewmarkImplicitSolver.cpp
	#odesolver/MyStaticSolver.cpp
	#odesolver/ShapeMatchingNewmarkImplicitSolver.cpp
	#BoxROIQuadraticTetras.cpp
	#QuadraticMeshTopology.cpp
	#forcefield/LandmarkSpringsForceField.cpp
)

# these header files do not need MOCcing
list(APPEND HEADER_FILES

	#constraint/DirichletBoundaryConstraint.h
	#constraint/DirichletBoundaryConstraint.inl

	
	#forcefield/LandmarkSpringsForceField.h
	#forcefield/LandmarkSpringsForceField.cpp
	#forcefield/LinearMeshMatrixMass.h
	#forcefield/LinearMeshMatrixMass.inl

	forcefield/LinearTetrahedralCorotationalFEMForceField.h
	forcefield/LinearTetrahedralCorotationalFEMForceField.inl
	forcefield/PoissonPotentialForcefieldUtil.h

	forcefield/QuadraticMeshMatrixMass.h
	forcefield/QuadraticMeshMatrixMass.inl

	forcefield/QuadraticTetrahedralCorotationalFEMForceField.h
	forcefield/QuadraticTetrahedralCorotationalFEMForceField.inl

	#forcefield/ShapeMatchingElectricPotentialForcefield.h
	#forcefield/ShapeMatchingElectricPotentialForcefield.inl

	forcefield/ShapeMatchingPoissonPotentialForcefield.h
	forcefield/ShapeMatchingPoissonPotentialForcefield.inl

	#forcefield/ShapeMatchingPoissonPotentialForcefieldSimulationDriver.h
	#forcefield/ShapeMatchingPoissonPotentialForcefieldSimulationDriver.inl

	
	#helper/ParalutionInitializer.h


	helper/VisitorCollection.h



	io/ExtendedVTKExporter.h

	io/MeshExtendedVTKLoader.h





	
	linearsolver/MumpsSolver.h
	#linearsolver/ParalutionSolver.h



	mapper/CollisionDetectionMeshSampler.h
	

	#mapper/MapperQuadraticTetrahedronSetTopology.h
	#mapper/MapperQuadraticTetrahedronSetTopology.inl
	

	#mapper/MapperLinearTetrahedronSetTopology.h
	#mapper/MapperLinearTetrahedronSetTopology.inl
	
	mapper/MeshSampler.h

	mapper/StructuredHexahedralMeshSampler.h

	mapper/UnstructuredTetrahedralMeshSampler.h


	odesolver/MyNewmarkImplicitSolver.h

	odesolver/MyStaticSolver.h

	odesolver/ShapeMatchingNewmarkImplicitSolver.h


	BoxROIQuadraticTetras.h
	BoxROIQuadraticTetras.inl
	QuadraticBaseMeshTopology.h

	QuadraticMeshTopology.h
	QuadraticTetrahedronData.h
   
   
   


	)

  
 

include_directories(
${SOFA_MODULES_INCLUDE_DIRECTORIES}

   )

include_directories("../../../modules")
   
  

   
   message(STATUS "Include directories: ${INCLUDE_DIRECTORIES}")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} )

target_link_libraries(${PROJECT_NAME} tinyxml)
target_link_libraries(${PROJECT_NAME} SofaGuiQt)
#target_link_libraries(${PROJECT_NAME} ITKCommon)
#target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${MUMPS_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${SOLID_LIBRARIES})

#include(${SOFA_CMAKE_DIR}/postProject.cmake)
