# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaGuiQt.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaGuiQt.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaGuiQt.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaGuiQt.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaGuiQt.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaGuiQt.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sofa/gui/qt" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/FileManagement.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/GraphListenerQListView.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/ImageQt.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/PickHandlerCallBacks.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QMenuFilesRecentlyOpened.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QModelViewTableDataContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/SofaGUIQt.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/StructDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/TableDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/viewer/SofaViewer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/viewer/VisualModelPolicy.h"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sofa/gui/qt" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/AddObject.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/DataFilenameWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/DataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/DisplayFlagsDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/GenGraphForm.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/LinkWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/MaterialDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/ModifyObject.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QDataDescriptionWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QDisplayDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QDisplayLinkWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QDisplayPropertyWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QModelViewTableUpdater.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QMouseOperations.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QSofaListView.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QSofaRecorder.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QSofaStatWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QTabulationModifyObject.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/QTransformationWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/RealGUI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/SimpleDataWidget.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/SofaMouseManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/SofaPluginManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/SofaVideoRecorderManager.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/WDoubleLineEdit.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/viewer/qt/QtViewer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/applications/sofa/gui/qt/viewer/qgl/QtGLViewer.h"
    )
endif()

