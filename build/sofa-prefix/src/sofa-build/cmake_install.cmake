# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/sofa" TYPE DIRECTORY FILES "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/share/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/sofa/examples" TYPE DIRECTORY FILES "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/examples/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/etc" TYPE FILE RENAME "sofa.ini" FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/etc/installedSofa.ini")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/sofa/component/typedef" TYPE DIRECTORY FILES "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/sofa/component/typedef/")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  
    find_package(Git REQUIRED)

    # get the current working branch
    execute_process(
      COMMAND  rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY /org/share/home/students/haentsch/dev/depthmatch/ext/sofa
      OUTPUT_VARIABLE SOFA_GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # get the current commit hash
    execute_process(
      COMMAND   rev-parse -q HEAD
      WORKING_DIRECTORY /org/share/home/students/haentsch/dev/depthmatch/ext/sofa
      OUTPUT_VARIABLE SOFA_GIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # get the current commit date
    execute_process(
      COMMAND  show -s --format=%ci
      WORKING_DIRECTORY /org/share/home/students/haentsch/dev/depthmatch/ext/sofa
      OUTPUT_VARIABLE SOFA_GIT_DATE
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string( TOLOWER "sofa" name_lower )
    if( name_lower STREQUAL "sofa" )
        file(WRITE "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/install/git.version" "sofa: ${SOFA_GIT_BRANCH} ${SOFA_GIT_HASH} ${SOFA_GIT_DATE} 
" )
    else()
        file(APPEND "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/install/git.version" "sofa: ${SOFA_GIT_BRANCH} ${SOFA_GIT_HASH} ${SOFA_GIT_DATE} 
" )
    endif()

endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE FILES "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/CMakeCache.txt")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/gtest/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/extlibs/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaFramework/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaSimulation/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/tools/SofaGTestMain/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaBase/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaCommon/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGeneral/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaAdvanced/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaMisc/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaGui/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SceneCreator/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaTest/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/SofaTest/SofaTest_test/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/applications/plugins/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/applications/projects/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
