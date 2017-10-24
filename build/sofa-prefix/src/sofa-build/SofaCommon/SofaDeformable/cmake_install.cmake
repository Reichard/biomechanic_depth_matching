# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaDeformable.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaDeformable.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaDeformable.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaDeformable.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaDeformable.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaDeformable.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaDeformable" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/FastTriangularBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/FastTriangularBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/FrameSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/FrameSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/MeshSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/MeshSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/QuadBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/QuadBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/QuadularBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/QuadularBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/RegularGridSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/RegularGridSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/RestShapeSpringsForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/RestShapeSpringsForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/SpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/SpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/StiffSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/StiffSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangleBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangleBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularBiquadraticSpringsForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularBiquadraticSpringsForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularQuadraticSpringsForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularQuadraticSpringsForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularTensorMassForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/TriangularTensorMassForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/VectorSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/VectorSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaDeformable/initDeformable.h"
    )
endif()

