#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SofaSimulationCommon" for configuration "Release"
set_property(TARGET SofaSimulationCommon APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaSimulationCommon PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaSimulationCommon.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaSimulationCommon.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaSimulationCommon )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaSimulationCommon "${_IMPORT_PREFIX}/lib/libSofaSimulationCommon.so.15.12.0" )

# Import target "SofaSimulationTree" for configuration "Release"
set_property(TARGET SofaSimulationTree APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaSimulationTree PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaSimulationTree.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaSimulationTree.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaSimulationTree )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaSimulationTree "${_IMPORT_PREFIX}/lib/libSofaSimulationTree.so.15.12.0" )

# Import target "SofaSimulationGraph" for configuration "Release"
set_property(TARGET SofaSimulationGraph APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaSimulationGraph PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaSimulationGraph.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaSimulationGraph.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaSimulationGraph )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaSimulationGraph "${_IMPORT_PREFIX}/lib/libSofaSimulationGraph.so.15.12.0" )

# Import target "SofaGTestMain" for configuration "Release"
set_property(TARGET SofaGTestMain APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaGTestMain PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaGTestMain.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaGTestMain )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaGTestMain "${_IMPORT_PREFIX}/lib/libSofaGTestMain.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
