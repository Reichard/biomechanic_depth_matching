#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SofaCUDA" for configuration "Release"
set_property(TARGET SofaCUDA APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaCUDA PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaCUDA.so"
  IMPORTED_SONAME_RELEASE "libSofaCUDA.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaCUDA )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaCUDA "${_IMPORT_PREFIX}/lib/libSofaCUDA.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
