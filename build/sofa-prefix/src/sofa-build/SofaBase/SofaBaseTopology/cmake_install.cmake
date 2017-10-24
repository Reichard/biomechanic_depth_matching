# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseTopology.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseTopology.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseTopology.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaBaseTopology.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseTopology.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaBaseTopology.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaBaseTopology" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTetrahedronSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTetrahedronSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTetrahedronSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTriangleSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTriangleSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/BezierTriangleSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/CommonAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/CubeTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/CylinderGridTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/EdgeSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/GridTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/HexahedronSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/MeshTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/NumericalIntegrationDescriptor.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/NumericalIntegrationDescriptor.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/PointSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/QuadSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/RegularGridTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/SparseGridTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/SphereQuadTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TetrahedronSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyData.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyData.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyDataHandler.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyDataHandler.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologyEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySparseData.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySparseData.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySparseDataHandler.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySparseDataHandler.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySubsetData.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySubsetData.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySubsetDataHandler.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TopologySubsetDataHandler.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetGeometryAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetGeometryAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetTopologyAlgorithms.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetTopologyAlgorithms.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetTopologyContainer.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/TriangleSetTopologyModifier.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaBaseTopology/initBaseTopology.h"
    )
endif()

