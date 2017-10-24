#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SofaComponentAdvanced" for configuration "Release"
set_property(TARGET SofaComponentAdvanced APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaComponentAdvanced PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaComponentAdvanced.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaComponentAdvanced.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaComponentAdvanced )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaComponentAdvanced "${_IMPORT_PREFIX}/lib/libSofaComponentAdvanced.so.15.12.0" )

# Import target "SofaEulerianFluid" for configuration "Release"
set_property(TARGET SofaEulerianFluid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaEulerianFluid PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaEulerianFluid.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaEulerianFluid.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaEulerianFluid )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaEulerianFluid "${_IMPORT_PREFIX}/lib/libSofaEulerianFluid.so.15.12.0" )

# Import target "SofaNonUniformFem" for configuration "Release"
set_property(TARGET SofaNonUniformFem APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaNonUniformFem PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaNonUniformFem.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaNonUniformFem.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaNonUniformFem )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaNonUniformFem "${_IMPORT_PREFIX}/lib/libSofaNonUniformFem.so.15.12.0" )

# Import target "SofaSphFluid" for configuration "Release"
set_property(TARGET SofaSphFluid APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaSphFluid PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaSphFluid.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaSphFluid.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaSphFluid )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaSphFluid "${_IMPORT_PREFIX}/lib/libSofaSphFluid.so.15.12.0" )

# Import target "SofaVolumetricData" for configuration "Release"
set_property(TARGET SofaVolumetricData APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SofaVolumetricData PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libSofaVolumetricData.so.15.12.0"
  IMPORTED_SONAME_RELEASE "libSofaVolumetricData.so.15.12.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SofaVolumetricData )
list(APPEND _IMPORT_CHECK_FILES_FOR_SofaVolumetricData "${_IMPORT_PREFIX}/lib/libSofaVolumetricData.so.15.12.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
