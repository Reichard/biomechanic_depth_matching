cmake_minimum_required(VERSION 2.8)

project(depthmatch)

include(ExternalProject)

set(SOFA_DIR "${CMAKE_SOURCE_DIR}/ext/sofa")
set(SOFA_BUILD_DIR "${CMAKE_BINARY_DIR}/sofa-prefix/src/sofa-build/")
ExternalProject_add(
	sofa
	SOURCE_DIR ${SOFA_DIR}
	CMAKE_ARGS "-DPLUGIN_SOFACUDA=ON -DSOFA_USE_C++11=ON \
	-DSOFACUDA_ARCH=sm_50 -DSOFACUDA_THRUST=ON -DAPPLICATION_MAKERIGID=OFF -DAPPLICATION_MODELER=OFF"
)

add_subdirectory("src")
