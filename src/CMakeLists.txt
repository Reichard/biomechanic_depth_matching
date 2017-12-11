cmake_minimum_required(VERSION 3.1)

project(DepthMatch)

set(LIBRARY_NAME ${PROJECT_NAME})
set(EXECUTABLE_NAME run)

set(HEADER_FILES
    config.h
    test_component.hpp
)

set(SOURCE_FILES
    init.cpp
    test_component.cpp
    calibration.cpp
	${CMAKE_SOURCE_DIR}/ext/tinyxml/tinyxml2.cpp
	mutable_cuda_spring_force_field.cpp
	spring_attacher.cpp
	spring_attacher.cu
	associator.cpp
)

set(EXECUTABLE_FILES
	main.cpp
)

set (CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)
find_package(CUDA REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

find_package(GLEW REQUIRED)
if(GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	link_libraries(${GLEW_LIBRARIES})
endif()

if (${CUDA_HOST_COMPILER} MATCHES "ccache$")
    message(STATUS "SofaCUDA: CUDA host compiler was set to ccache, changing to g++")
    set(CUDA_HOST_COMPILER "g++" CACHE STRING "Host side compiler used by NVCC" FORCE)
endif()

cuda_include_directories("${CMAKE_CURRENT_SOURCE_DIR}")
cuda_include_directories("${SOFA_BUILD_DIR}/include")

add_definitions(-DDATA_DIR="${CMAKE_SOURCE_DIR}/data")

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

target_link_libraries(${PROJECT_NAME} SofaComponentGeneral )
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} )
target_link_libraries(${PROJECT_NAME} SofaCUDA )
target_link_libraries(${PROJECT_NAME} ${MUMPS_LIBRARIES})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${CUDA_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "/usr/lib/x86_64-linux-gnu/libcuda.so")

add_dependencies(${PROJECT_NAME} sofa)

add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_FILES})
add_dependencies(${EXECUTABLE_NAME} ${PROJECT_NAME})
add_dependencies(${EXECUTABLE_NAME} sofa)

target_link_libraries(${EXECUTABLE_NAME} SofaCUDA )
target_link_libraries(${EXECUTABLE_NAME} SofaSimpleGUI )
target_link_libraries(${EXECUTABLE_NAME} SofaCore )
target_link_libraries(${EXECUTABLE_NAME} SofaHelper )
target_link_libraries(${EXECUTABLE_NAME} SofaSimulationCommon )
target_link_libraries(${EXECUTABLE_NAME} SofaBaseMechanics )
target_link_libraries(${EXECUTABLE_NAME} SofaBoundaryCondition )
target_link_libraries(${EXECUTABLE_NAME} SofaBaseTopology )
target_link_libraries(${EXECUTABLE_NAME} SofaBaseLinearSolver )
target_link_libraries(${EXECUTABLE_NAME} SofaBaseVisual )
target_link_libraries(${EXECUTABLE_NAME} SofaOpenglVisual )
target_link_libraries(${EXECUTABLE_NAME} SofaImplicitOdeSolver )
target_link_libraries(${EXECUTABLE_NAME} SofaSimpleFem )
target_link_libraries(${EXECUTABLE_NAME} ${PROJECT_NAME})

target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES} )
target_link_libraries(${EXECUTABLE_NAME} ${GLUT_LIBRARIES} )
target_link_libraries(${EXECUTABLE_NAME} ${glfw_LIBRARIES} )

install(TARGETS ${PROJECT_NAME}
	COMPONENT DepthMatch_libraries
	EXPORT DepthMatchTargets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)

