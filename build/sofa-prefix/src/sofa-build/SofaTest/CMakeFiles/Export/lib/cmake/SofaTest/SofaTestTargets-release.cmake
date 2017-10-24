#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SofaTest" for configuration "Release"
set_property(TARGET SofaTest APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaTest PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaTest.so"
  IMPORTED_SONAME_RELEASE "libSofaTest.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaTest )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaTest "${_IMPORT_PREFIX}/lib/libSofaTest.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
