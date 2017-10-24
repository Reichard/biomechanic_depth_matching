# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQGLViewer.so.2.6.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQGLViewer.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libQGLViewer.so.2.6.3"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libQGLViewer.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQGLViewer.so.2.6.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libQGLViewer.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "::::::"
           NEW_RPATH "../lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/QGLViewer" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/camera.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/frame.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/keyFrameInterpolator.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/manipulatedCameraFrame.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/manipulatedFrame.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/qglviewer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/AxisAlignedBox.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Exporter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/NVector3.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Optimizer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/ParserGL.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Primitive.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/PrimitivePositioning.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/SortMethod.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Types.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/VRender.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Vector2.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/Vector3.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/VRender/gpc.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/constraint.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/domUtils.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/mouseGrabber.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/quaternion.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/extlibs/libQGLViewer-2.6.3/QGLViewer/vec.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer/QGLViewerTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer/QGLViewerTargets.cmake"
         "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/QGLViewer/CMakeFiles/Export/lib/cmake/QGLViewer/QGLViewerTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer/QGLViewerTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer/QGLViewerTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/QGLViewer/CMakeFiles/Export/lib/cmake/QGLViewer/QGLViewerTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/QGLViewer/CMakeFiles/Export/lib/cmake/QGLViewer/QGLViewerTargets-release.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/QGLViewer/QGLViewerConfigVersion.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/QGLViewer" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/cmake/QGLViewerConfig.cmake")
endif()

