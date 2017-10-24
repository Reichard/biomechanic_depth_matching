#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SofaHelper" for configuration "Release"
set_property(TARGET SofaHelper APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaHelper PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaHelper.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaHelper.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaHelper )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaHelper "${_IMPORT_PREFIX}/lib/libSofaHelper.so.15.12.0" )

# Import target "SofaDefaultType" for configuration "Release"
set_property(TARGET SofaDefaultType APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaDefaultType PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaDefaultType.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaDefaultType.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaDefaultType )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaDefaultType "${_IMPORT_PREFIX}/lib/libSofaDefaultType.so.15.12.0" )

# Import target "SofaCore" for configuration "Release"
set_property(TARGET SofaCore APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaCore PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaCore.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaCore.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaCore )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaCore "${_IMPORT_PREFIX}/lib/libSofaCore.so.15.12.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
