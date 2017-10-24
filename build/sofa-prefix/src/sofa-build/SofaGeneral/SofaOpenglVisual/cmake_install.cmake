# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaOpenglVisual.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaOpenglVisual.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaOpenglVisual.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaOpenglVisual.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaOpenglVisual.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaOpenglVisual.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib:"
           NEW_RPATH "../lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaOpenglVisual" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/ClipPlane.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/ColorMap.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/CompositingVisualLoop.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/DataDisplay.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/Light.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/LightManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglCylinderModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglGrid.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglLabel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglLineAxis.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglRenderingSRGB.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglSceneFrame.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/PointSplatModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/initOpenGLVisual.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/MergeVisualModels.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglAttribute.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglAttribute.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglShader.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglShaderMacro.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglShaderVisualModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglShadowShader.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglTetrahedralModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglTetrahedralModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglTexture.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglVariable.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglVariable.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/OglViewport.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/PostProcessManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/SlicedVolumetricModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/VisualManagerPass.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaOpenglVisual/VisualManagerSecondaryPass.h"
    )
endif()

