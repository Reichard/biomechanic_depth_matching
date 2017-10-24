cmake_minimum_required(VERSION 2.8.12)
project(DepthMatch)

set(HEADER_FILES
    config.h
    test_component.hpp
)

set(SOURCE_FILES
    init.cpp
    test_component.cpp
    depth_input_component.cpp
    calibration.cpp
    tinyxml2.cpp
	my_cuda_spring_force_field.cpp
	MyCudaMechanicalObject.cpp
	spring_attacher.cpp
	spring_attacher.cu
	associator.cpp
)

set (CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_DIR}/applications/plugins/MyPlugin")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_DIR}/applications/plugins/MyPlugin")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SOFA_BUILD_DIR}/cmake")

find_package(Threads REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenGL REQUIRED)

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

INCLUDE_DIRECTORIES(${MUMPS_INCLUDE_DIRS})
include_directories(SYSTEM "${SOFA_DIR}/applications/plugins/")
include_directories(SYSTEM "${SOFA_DIR}/applications/plugins/SofaCUDA")
include_directories(SYSTEM "${SOFA_DIR}/framework")
include_directories(SYSTEM "${SOFA_DIR}/extlibs/eigen-3.2.7")
include_directories(SYSTEM "${SOFA_DIR}/modules")

include_directories(SYSTEM "${SOFA_BUILD_DIR}/include")
include_directories(SYSTEM "${SOFA_BUILD_DIR}/install/include")

cuda_add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES} OPTIONS ${VERBOSE_PTXAS})
#set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PLUGINEXAMPLE")
target_link_libraries(${PROJECT_NAME} SofaComponentGeneral )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(${PROJECT_NAME} SofaCUDA )
target_link_libraries(${PROJECT_NAME} ${MUMPS_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
	COMPONENT DepthMatch_libraries
	EXPORT DepthMatchTargets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

