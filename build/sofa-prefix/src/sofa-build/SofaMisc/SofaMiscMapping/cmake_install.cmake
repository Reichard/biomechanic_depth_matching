# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscMapping.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscMapping.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscMapping.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscMapping.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscMapping.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscMapping.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaMiscMapping" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/BarycentricMappingRigid.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/BarycentricMappingRigid.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/BeamLinearMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/BeamLinearMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CatmullRomSplineMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CatmullRomSplineMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMulti2Mapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMulti2Mapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMultiMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterOfMassMultiMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterPointMechanicalMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CenterPointMechanicalMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CurveMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/CurveMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DeformableOnRigidFrameMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DeformableOnRigidFrameMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DistanceFromTargetMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DistanceFromTargetMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DistanceMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/DistanceMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ExternalInterpolationMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ExternalInterpolationMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/IdentityMultiMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/IdentityMultiMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ProjectionToLineMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ProjectionToLineMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ProjectionToPlaneMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/ProjectionToPlaneMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/SubsetMultiMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/SubsetMultiMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/TubularMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/TubularMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/VoidMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscMapping/initMiscMapping.h"
    )
endif()

