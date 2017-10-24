# CMake package configuration file for the SofaCUDA plugin


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was SofaCUDAConfig.cmake.in                            ########

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

list(FIND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}" HAS_SOFACUDA_CMAKE_MODULE_PATH)
if(HAS_SOFACUDA_CMAKE_MODULE_PATH EQUAL -1)
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})
endif()

set(SOFACUDA_ARCH sm_50)

find_package(SofaAdvanced REQUIRED)
find_package(SofaMisc REQUIRED)
find_package(CUDA REQUIRED)

if(NOT TARGET SofaCUDA)
	include("${CMAKE_CURRENT_LIST_DIR}/SofaCUDATargets.cmake")
endif()

check_required_components(SofaCUDA)

include(SofaCUDANvccFlags)
