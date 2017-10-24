# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaRigid.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaRigid.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaRigid.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaRigid.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaRigid.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaRigid.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaRigid" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/ArticulatedHierarchyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/ArticulatedHierarchyContainer.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/ArticulatedSystemMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/ArticulatedSystemMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/JointSpringForceField.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/JointSpringForceField.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/LaparoscopicRigidMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/LaparoscopicRigidMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/LineSetSkinningMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/LineSetSkinningMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/RigidMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/RigidMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/RigidRigidMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/RigidRigidMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/SkinningMapping.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/SkinningMapping.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaRigid/initRigid.h"
    )
endif()

