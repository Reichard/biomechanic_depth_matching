# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver

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
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseLinearSolver.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseLinearSolver.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseLinearSolver.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseLinearSolver.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseLinearSolver.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseLinearSolver.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHANGE
           FILE "${file}"
           OLD_RPATH "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib:"
           NEW_RPATH "../lib")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaBaseLinearSolver" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/BTDLinearSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/BTDLinearSolver.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/BlocMatrixWriter.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CGLinearSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CGLinearSolver.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CholeskySolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CholeskySolver.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CompressedRowSparseMatrix.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/CompressedRowSparseMatrix.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/DefaultMultiMatrixAccessor.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/DiagonalMatrix.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/FullMatrix.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/FullVector.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/GraphScatteredTypes.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/MatrixExpr.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/MatrixLinearSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/MatrixLinearSolver.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/MinResLinearSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/MinResLinearSolver.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/SingleMatrixAccessor.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/SparseMatrix.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/initBaseLinearSolver.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseLinearSolver/matrix_bloc_traits.h"
    )
endif()

