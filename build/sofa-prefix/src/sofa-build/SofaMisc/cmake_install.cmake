# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/SofaMisc

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaMisc" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/include/SofaMisc/config.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc/SofaMiscTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc/SofaMiscTargets.cmake"
         "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/CMakeFiles/Export/lib/cmake/SofaMisc/SofaMiscTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc/SofaMiscTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc/SofaMiscTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/CMakeFiles/Export/lib/cmake/SofaMisc/SofaMiscTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/CMakeFiles/Export/lib/cmake/SofaMisc/SofaMiscTargets-release.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscConfigVersion.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaMisc" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/cmake/SofaMiscConfig.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaComponentMisc/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMisc/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscCollision/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscEngine/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscFem/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscForceField/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscMapping/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/SofaMiscTopology/cmake_install.cmake")

endif()

