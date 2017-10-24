# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscForceField.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscForceField.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscForceField.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscForceField.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscForceField.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscForceField.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaMiscForceField" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/GearSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/GearSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/LennardJonesForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/LennardJonesForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/LineBendingSprings.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/LineBendingSprings.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/MatrixMass.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/MatrixMass.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/MeshMatrixMass.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/MeshMatrixMass.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/WashingMachineForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/WashingMachineForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscForceField/initMiscForcefield.h"
    )
endif()

