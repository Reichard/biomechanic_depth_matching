# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/SofaGeneral

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaGeneral" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/include/SofaGeneral/config.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral/SofaGeneralTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral/SofaGeneralTargets.cmake"
         "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/CMakeFiles/Export/lib/cmake/SofaGeneral/SofaGeneralTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral/SofaGeneralTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral/SofaGeneralTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/CMakeFiles/Export/lib/cmake/SofaGeneral/SofaGeneralTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/CMakeFiles/Export/lib/cmake/SofaGeneral/SofaGeneralTargets-release.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaGeneralConfigVersion.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaGeneral" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/cmake/SofaGeneralConfig.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaBoundaryCondition/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaComponentGeneral/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaConstraint/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaEngine/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaExporter/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaGraphComponent/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaHaptics/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaPreconditioner/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaSparseSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaTopologyMapping/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaUserInteraction/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaValidation/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaDenseSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaOpenglVisual/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/SofaEigen2Solver/cmake_install.cmake")

endif()

