#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "QGLViewer" for configuration "Release"
set_property(TARGET QGLViewer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(QGLViewer PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libQGLViewer.so.2.6.3"
  IMPORTED_SONAME_RELEASE "libQGLViewer.so.2.6.3"
  )

list(APPEND _IMPORT_CHECK_TARGETS QGLViewer )
list(APPEND _IMPORT_CHECK_FILES_FOR_QGLViewer "${_IMPORT_PREFIX}/lib/libQGLViewer.so.2.6.3" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
