# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscFem.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscFem.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscFem.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaMiscFem.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscFem.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaMiscFem.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaMiscFem" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/BaseMaterial.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/BoyceAndArruda.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/Costa.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/FastTetrahedralCorotationalForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/FastTetrahedralCorotationalForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/HyperelasticMaterial.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/MooneyRivlin.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/NeoHookean.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/Ogden.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/STVenantKirchhoff.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/StandardTetrahedralFEMForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/StandardTetrahedralFEMForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TetrahedralTensorMassForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TetrahedralTensorMassForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/VerondaWestman.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/initMiscFEM.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangleFEMForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangleFEMForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangularAnisotropicFEMForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangularAnisotropicFEMForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangularFEMForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/TriangularFEMForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaMiscFem/PlasticMaterial.h"
    )
endif()

