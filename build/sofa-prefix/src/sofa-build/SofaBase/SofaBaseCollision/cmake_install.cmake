# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseCollision.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseCollision.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseCollision.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseCollision.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseCollision.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseCollision.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaBaseCollision" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/BaseContactMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/BaseIntTool.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/BaseProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/BruteForceDetection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CapsuleIntTool.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CapsuleIntTool.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CapsuleModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CapsuleModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/ContactListener.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/Cube.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CubeModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CylinderModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/CylinderModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/DefaultContactManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/DefaultPipeline.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/DiscreteIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/Intersector.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrCapsuleOBB.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrCapsuleOBB.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrOBBOBB.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrOBBOBB.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrSphereOBB.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrSphereOBB.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrUtility3.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/IntrUtility3.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/MinProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/NewProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/NewProximityIntersection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/OBBIntTool.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/OBBModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/OBBModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/RigidCapsuleModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/RigidCapsuleModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/Sphere.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/SphereModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/SphereModel.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseCollision/initBaseCollision.h"
    )
endif()

