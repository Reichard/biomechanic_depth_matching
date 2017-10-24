#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "newmat" for configuration "Release"
set_property(TARGET newmat APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(newmat PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libnewmat.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS newmat )
list(APPEND _IMPORT_CHECK_FILES_FOR_newmat "${_IMPORT_PREFIX}/lib/libnewmat.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
