# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/tests

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

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBaseCollision/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBaseLinearSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBaseMechanics/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBaseTopology/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBaseVisual/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaBoundaryCondition/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaConstraint/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaDeformable/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaEngine/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaExplicitOdeSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaImplicitOdeSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaLoader/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaMeshCollision/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaMiscMapping/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaMiscSolver/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaMiscTopology/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaRigid/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/SofaSimpleFem/cmake_install.cmake")
  include("/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/modules/tests/tests/simulation/cmake_install.cmake")

endif()

