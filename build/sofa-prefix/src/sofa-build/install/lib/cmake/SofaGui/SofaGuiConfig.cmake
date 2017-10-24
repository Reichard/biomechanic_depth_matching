# CMake package configuration file for SofaGui


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SofaGuiConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

find_package(SofaGeneral REQUIRED)
find_package(SofaMisc REQUIRED)
find_package(SofaAdvanced REQUIRED)

set(SOFA_HAVE_QT 1)
set(SOFA_GUI_GLUT )
set(SOFA_GUI_QT 1)
set(SOFA_GUI_INTERACTION )
set(SOFA_GUI_QTVIEWER 1)
set(SOFA_GUI_QGLVIEWER 1)
set(SOFA_GUI_EDITORVIEWER )
set(SOFA_QT5 1)

if(SOFA_HAVE_QT)
    set(QT_USE_IMPORTED_TARGETS 1)
    set(QT4_NO_LINK_QTMAIN 1)
    if(SOFA_QT5)
        find_package(Qt5 QUIET REQUIRED COMPONENTS Core Gui OpenGL Xml)
    else()
        find_package(Qt4 QUIET REQUIRED COMPONENTS qtcore qtgui qtopengl qtxml)
    endif()

    if (SOFA_GUI_QGLVIEWER)
        find_package(QGLViewer REQUIRED)
    endif()
endif()

if(NOT TARGET SofaGuiCommon)
	include("${CMAKE_CURRENT_LIST_DIR}/SofaGuiTargets.cmake")
endif()

check_required_components(SofaGuiCommon;SofaGuiQt;SofaGuiMain)
