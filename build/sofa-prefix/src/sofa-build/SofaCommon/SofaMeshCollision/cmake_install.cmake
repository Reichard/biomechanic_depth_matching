# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMeshCollision.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMeshCollision.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMeshCollision.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMeshCollision.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMeshCollision.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMeshCollision.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaMeshCollision" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/BarycentricContactMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/BarycentricContactMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/BarycentricPenalityContact.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/BarycentricPenalityContact.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/CollisionPM.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/DirectSAP.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/DirectSAP.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/EndPoint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IdentityContactMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IdentityContactMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IncrSAP.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IncrSAP.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IntrMeshUtility.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IntrMeshUtility.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IntrTriangleOBB.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/IntrTriangleOBB.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/Line.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/LineLocalMinDistanceFilter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/LineModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/LineModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/LocalMinDistanceFilter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshDiscreteIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshDiscreteIntersection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshIntTool.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshIntTool.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshMinProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshNewProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/MeshNewProximityIntersection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/Point.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/PointLocalMinDistanceFilter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/PointModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/PointModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/RayTriangleIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/RigidContactMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/RigidContactMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/SubsetContactMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/SubsetContactMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/Triangle.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/TriangleLocalMinDistanceFilter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/TriangleModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/TriangleModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/TriangleOctree.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/TriangleOctreeModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMeshCollision/initMeshCollision.h"
    )
endif()

