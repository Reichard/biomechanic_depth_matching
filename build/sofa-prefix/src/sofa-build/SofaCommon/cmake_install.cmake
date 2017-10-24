# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/SofaCommon

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaCommon" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/include/SofaCommon/config.h")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon/SofaCommonTargets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon/SofaCommonTargets.cmake"
         "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/CMakeFiles/Export/lib/cmake/SofaCommon/SofaCommonTargets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon/SofaCommonTargets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon/SofaCommonTargets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/CMakeFiles/Export/lib/cmake/SofaCommon/SofaCommonTargets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/CMakeFiles/Export/lib/cmake/SofaCommon/SofaCommonTargets-release.cmake")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaCommonConfigVersion.cmake")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/SofaCommon" TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/cmake/SofaCommonConfig.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaComponentCommon/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaDeformable/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaExplicitOdeSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaImplicitOdeSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaLoader/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaMeshCollision/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaObjectInteraction/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaRigid/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/SofaSimpleFem/cmake_install.cmake")

endif()

