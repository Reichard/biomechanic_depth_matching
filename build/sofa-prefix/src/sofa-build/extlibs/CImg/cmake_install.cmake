# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg

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

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "CImg_headers")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CImg" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/CImg.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/cimgmatlab.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/jpeg_buffer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/nlmeans.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/skeleton.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/SOFACImg.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/CImg/vrml.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/CImg" TYPE FILE RENAME "CImgConfig.cmake" FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/CImg/InstalledCImgConfig.cmake")
endif()

