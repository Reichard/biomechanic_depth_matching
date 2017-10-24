# CMake package configuration file for SofaFramework


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SofaFrameworkConfig.cmake.in                            ########

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

list(FIND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}" HAS_SOFAFRAMEWORK_CMAKE_MODULE_PATH)
if(HAS_SOFAFRAMEWORK_CMAKE_MODULE_PATH EQUAL -1)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
endif()

include(SofaMacros)

find_package(TinyXML REQUIRED)
sofa_create_target(TinyXML SofaFramework "${TinyXML_LIBRARIES}" "${TinyXML_INCLUDE_DIRS}")

set(SOFA_HAVE_ZLIB "TRUE")
set(SOFA_HAVE_PNG "TRUE")
set(SOFA_HAVE_GLEW "TRUE")
set(SOFA_HAVE_GLUT "TRUE")
set(SOFA_HAVE_BOOST "1")
set(SOFA_NO_OPENGL "OFF")
set(SOFA_HAVE_GTEST "1")

if(SOFA_HAVE_GTEST)
    find_package(GTest CONFIG REQUIRED)
endif()

if(SOFA_HAVE_ZLIB)
    find_package(ZLIB REQUIRED)
    sofa_create_target(ZLIB SofaFramework "${ZLIB_LIBRARIES}" "${ZLIB_INCLUDE_DIRS}")
endif()

if(SOFA_HAVE_PNG)
    find_package(PNG REQUIRED)
    sofa_create_target(PNG SofaFramework "${PNG_LIBRARY}" "${PNG_INCLUDE_DIRS}")
endif()

if(SOFA_HAVE_GLEW)
    find_package(GLEW REQUIRED)
    sofa_create_target(GLEW SofaFramework "${GLEW_LIBRARIES}" "${GLEW_INCLUDE_DIRS}")
endif()

if(SOFA_HAVE_GLUT)
    find_package(GLUT REQUIRED)
    sofa_create_target(GLUT SofaFramework "${GLUT_glut_LIBRARY}" "${GLUT_INCLUDE_DIR}")
endif()

if(SOFA_HAVE_BOOST)
    find_package(Boost QUIET REQUIRED COMPONENTS thread system date_time chrono)
    sofa_create_target(BoostSystem SofaFramework "${Boost_SYSTEM_LIBRARY}" "${Boost_INCLUDE_DIRS}")
    sofa_create_target(BoostThread SofaFramework "${Boost_THREAD_LIBRARY}" "${Boost_INCLUDE_DIRS}")
    sofa_create_target(BoostDateTime SofaFramework "${Boost_DATE_TIME_LIBRARY}" "${Boost_INCLUDE_DIRS}")
    sofa_create_target(BoostChrono SofaFramework "${Boost_CHRONO_LIBRARY}" "${Boost_INCLUDE_DIRS}")
endif()

if(NOT TARGET SofaCore)
	include("${CMAKE_CURRENT_LIST_DIR}/SofaFrameworkTargets.cmake")
endif()

check_required_components(SofaHelper SofaDefaultType SofaCore)
