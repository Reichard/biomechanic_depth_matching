#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "csparse" for configuration "Release"
set_property(TARGET csparse APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(csparse PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libcsparse.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS csparse )
list(APPEND _IMPORT_CHECK_FILES_FOR_csparse "${_IMPORT_PREFIX}/lib/libcsparse.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
