cmake_minimum_required(VERSION 3.1)
project(DepthMatch)

set(HEADER_FILES
    config.h
    test_component.hpp
    depth_match_component.hpp
)

set(SOURCE_FILES
    init.cpp
    depth_match_component.cpp
    test_component.cpp
    depth_input_component.cpp
    calibration.cpp
	${CMAKE_SOURCE_DIR}/ext/tinyxml/tinyxml2.cpp
	mutable_cuda_spring_force_field.cpp
	mutable_cuda_mechanical_object.cpp
	spring_attacher.cpp
	spring_attacher.cu
	associator.cpp
)

set(EXECUTABLE_FILES
	main.cpp
)

set (CMAKE_CXX_STANDARD 11)

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_DIR}/applications/plugins/MyPlugin")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_DIR}/applications/plugins/MyPlugin")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_BUILD_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
#find_package(glfw REQUIRED)
#find_package(SofaSimpleGUI REQUIRED)

# nvcc uses a "host code compiler" to compile CPU code, specified by CUDA_HOST_COMPILER.
# With some versions of CMake, CUDA_HOST_COMPILER defaults to CMAKE_C_COMPILER,
# but few host compilers are actually supported. Workarounds should go here.
if (${CUDA_HOST_COMPILER} MATCHES "ccache$")
    message(STATUS "SofaCUDA: CUDA host compiler was set to ccache, changing to g++")
    set(CUDA_HOST_COMPILER "g++" CACHE STRING "Host side compiler used by NVCC" FORCE)
endif()
#fix for nvcc compatibility with -fno-partial-inlining flag
#set(CUDA_PROPAGATE_HOST_FLAGS OFF)

# Give include directories to nvcc compiler.
# Note: keep cuda_include_directories() before cuda_add_library().
cuda_include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
cuda_include_directories("${SOFA_BUILD_DIR}/include")

#set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_50,code=sm_50;)
#set(CUDA_HOST_COMPILATION_CPP ON)
#set(CUDA_PROPAGATE_HOST_FLAGS ON)

include_directories(SYSTEM "${SOFA_DIR}/applications/plugins/")
include_directories(SYSTEM "${SOFA_DIR}/applications/plugins/SofaCUDA")
include_directories(SYSTEM "${SOFA_DIR}/framework")
include_directories(SYSTEM "${SOFA_DIR}/extlibs/eigen-3.2.7")
include_directories(SYSTEM "${SOFA_DIR}/modules")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/ext/tinyxml")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/ext/glpp")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/ext/cudapp")

include_directories(SYSTEM "${SOFA_BUILD_DIR}/include")
include_directories(SYSTEM "${SOFA_BUILD_DIR}/install/include") 
include_directories(SYSTEM "${glfw_INCLUDE_DIRS}" ) 

link_directories("${SOFA_BUILD_DIR}/lib/")
link_directories(${glfw_LINK_DIRS})

cuda_add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} OPTIONS ${VERBOSE_PTXAS})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PLUGINEXAMPLE")

target_link_libraries(${PROJECT_NAME} SofaComponentGeneral )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(${PROJECT_NAME} SofaCUDA )
target_link_libraries(${PROJECT_NAME} ${MUMPS_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libcuda.so")

add_dependencies(${PROJECT_NAME} sofa)

add_executable(run ${EXECUTABLE_FILES})
add_dependencies(run sofa)

target_link_libraries(run SofaCUDA )
target_link_libraries(run SofaSimpleGUI )
target_link_libraries(run SofaCore )
target_link_libraries(run SofaHelper )
target_link_libraries(run SofaSimulationCommon )
target_link_libraries(run SofaBaseMechanics )
target_link_libraries(run SofaBoundaryCondition )
target_link_libraries(run SofaBaseTopology )
target_link_libraries(run SofaBaseLinearSolver )
target_link_libraries(run SofaBaseVisual )
target_link_libraries(run SofaOpenglVisual )
target_link_libraries(run SofaImplicitOdeSolver )
target_link_libraries(run SofaSimpleFem )

target_link_libraries(run ${OPENGL_LIBRARIES} )
target_link_libraries(run ${GLUT_LIBRARIES} )
target_link_libraries(run ${glfw_LIBRARIES} )

install(TARGETS ${PROJECT_NAME}
	COMPONENT DepthMatch_libraries
	EXPORT DepthMatchTargets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

