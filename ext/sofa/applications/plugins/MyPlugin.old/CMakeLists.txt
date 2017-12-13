cmake_minimum_required(VERSION 2.8.12)
project(MyPlugin)

set(HEADER_FILES
    config.h
    test_component.hpp
)

set(SOURCE_FILES
    initEmptyCmakePlugin.cpp
    test_component.cpp
)

#SET(CMAKE_CXX_FLAGS "-std=c++1y")

find_package(SofaGeneral REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES} ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_PLUGINEXAMPLE")
target_link_libraries(${PROJECT_NAME} SofaComponentGeneral)
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

install(TARGETS ${PROJECT_NAME}
        COMPONENT EmptyCmakePlugin_libraries
        EXPORT EmptyCmakePluginTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
