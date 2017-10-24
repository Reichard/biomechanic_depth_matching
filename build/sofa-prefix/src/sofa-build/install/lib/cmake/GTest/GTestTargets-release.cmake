#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "gtest" for configuration "Release"
set_property(TARGET gtest APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gtest PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libgtest.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS gtest )
list(APPEND _IMPORT_CHECK_FILES_FOR_gtest "${_IMPORT_PREFIX}/lib/libgtest.a" )

# Import target "gtest_main" for configuration "Release"
set_property(TARGET gtest_main APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(gtest_main PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libgtest_main.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS gtest_main )
list(APPEND _IMPORT_CHECK_FILES_FOR_gtest_main "${_IMPORT_PREFIX}/lib/libgtest_main.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
