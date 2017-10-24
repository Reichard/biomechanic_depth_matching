# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaConstraint.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaConstraint.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaConstraint.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaConstraint.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaConstraint.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaConstraint.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaConstraint" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/BilateralInteractionConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/BilateralInteractionConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/ConstraintAnimationLoop.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/ConstraintAttachBodyPerformer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/ConstraintAttachBodyPerformer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/ConstraintSolverImpl.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/ContactDescription.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/FreeMotionAnimationLoop.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/FrictionContact.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/FrictionContact.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/GenericConstraintCorrection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/GenericConstraintCorrection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/GenericConstraintSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LCPConstraintSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LMDNewProximityIntersection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LMDNewProximityIntersection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LinearSolverConstraintCorrection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LinearSolverConstraintCorrection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LocalMinDistance.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LocalMinDistance.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/PrecomputedConstraintCorrection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/PrecomputedConstraintCorrection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/SlidingConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/SlidingConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/StickContactConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/StickContactConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/StopperConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/StopperConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/UncoupledConstraintCorrection.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/UncoupledConstraintCorrection.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/UnilateralInteractionConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/UnilateralInteractionConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/initConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/BarycentricDistanceLMConstraintContact.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/BarycentricDistanceLMConstraintContact.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DOFBlockerLMConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DOFBlockerLMConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/FixedLMConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/FixedLMConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DistanceLMContactConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DistanceLMContactConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DistanceLMConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/DistanceLMConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LMConstraintSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaConstraint/LMConstraintDirectSolver.h"
    )
endif()

