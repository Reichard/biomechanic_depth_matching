# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaUserInteraction.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaUserInteraction.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaUserInteraction.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaUserInteraction.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaUserInteraction.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaUserInteraction.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaUserInteraction" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/AddRecordedCameraPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/ArticulatedHierarchyBVHController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/ArticulatedHierarchyController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/AttachBodyPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/AttachBodyPerformer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/ComponentMouseInteraction.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/ComponentMouseInteraction.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/Controller.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/CuttingPoint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/DisabledContact.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/EdgeSetController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/EdgeSetController.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/FixParticlePerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/FixParticlePerformer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/GraspingManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/InciseAlongPathPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/InteractionPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/InterpolationController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MechanicalStateController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MechanicalStateController.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MechanicalStateControllerOmni.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MechanicalStateControllerOmni.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MouseInteractor.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/MouseInteractor.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/NodeToggleController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/Ray.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayContact.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayDiscreteIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayDiscreteIntersection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayModel.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayNewProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RayTraceDetection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RemovePrimitivePerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/RemovePrimitivePerformer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/SleepController.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/StartNavigationPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/SuturePointPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/SuturePointPerformer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/TopologicalChangeManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaUserInteraction/initUserInteraction.h"
    )
endif()

