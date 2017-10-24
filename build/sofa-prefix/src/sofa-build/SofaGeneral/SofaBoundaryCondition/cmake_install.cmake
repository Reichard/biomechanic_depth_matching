# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBoundaryCondition.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBoundaryCondition.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBoundaryCondition.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBoundaryCondition.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBoundaryCondition.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBoundaryCondition.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaBoundaryCondition" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/AffineMovementConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/AffineMovementConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/BuoyantForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/BuoyantForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ConicalForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ConicalForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ConstantForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ConstantForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/DiagonalVelocityDampingForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/DiagonalVelocityDampingForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/EdgePressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/EdgePressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/EllipsoidForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/EllipsoidForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedPlaneConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedPlaneConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedRotationConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedRotationConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedTranslationConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/FixedTranslationConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/HermiteSplineConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/HermiteSplineConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearMovementConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearMovementConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearVelocityConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/LinearVelocityConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/OscillatingTorsionPressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/OscillatingTorsionPressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/OscillatorConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/OscillatorConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ParabolicConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ParabolicConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PartialFixedConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PartialFixedConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PartialLinearMovementConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PartialLinearMovementConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PatchTestMovementConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PatchTestMovementConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PlaneForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PlaneForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PointConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PointConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PositionBasedDynamicsConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/PositionBasedDynamicsConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/QuadPressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/QuadPressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SkeletalMotionConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SkeletalMotionConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SphereForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SphereForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SurfacePressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/SurfacePressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/TaitSurfacePressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/TaitSurfacePressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/TrianglePressureForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/TrianglePressureForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/UniformVelocityDampingForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/UniformVelocityDampingForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/VaccumSphereForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/VaccumSphereForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/initBoundaryCondition.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToLineConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToLineConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToPlaneConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToPlaneConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToPointConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectToPointConstraint.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectDirectionConstraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBoundaryCondition/ProjectDirectionConstraint.inl"
    )
endif()

