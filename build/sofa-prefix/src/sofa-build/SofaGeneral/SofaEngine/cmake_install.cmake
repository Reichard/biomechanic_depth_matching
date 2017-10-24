# Install script for directory: /org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine

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
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaEngine.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaEngine.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "../lib")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaEngine.so.15.12.0"
    "/org/share/home/students/haentsch/dev/depthmatch/build/sofa-prefix/src/sofa-build/lib/libSofaEngine.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaEngine.so.15.12.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSofaEngine.so"
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SofaEngine" TYPE FILE FILES
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/AverageCoord.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/AverageCoord.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/BoxROI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/BoxROI.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ClusteringEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ClusteringEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/DilateEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/DilateEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeEdgesAndGenerateQuads.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeEdgesAndGenerateQuads.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeQuadsAndGenerateHexas.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeQuadsAndGenerateHexas.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeSurface.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ExtrudeSurface.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateCylinder.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateCylinder.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateGrid.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateGrid.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateRigidMass.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateRigidMass.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateSphere.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GenerateSphere.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GroupFilterYoungModulus.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/GroupFilterYoungModulus.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/HausdorffDistance.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/HausdorffDistance.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/IndexValueMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/IndexValueMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Indices2ValuesMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Indices2ValuesMapper.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/IndicesFromValues.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/IndicesFromValues.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/JoinPoints.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/JoinPoints.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MapIndices.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MapIndices.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MathOp.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MathOp.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeMeshes.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeMeshes.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergePoints.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergePoints.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeROIs.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeSets.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeSets.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeVectors.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MergeVectors.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshBarycentricMapperEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshBarycentricMapperEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshClosingEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshClosingEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshROI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshROI.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSampler.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSampler.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSplittingEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSplittingEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSubsetEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/MeshSubsetEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/NormEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/NormEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/NormalsFromPoints.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/NormalsFromPoints.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PairBoxRoi.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PairBoxRoi.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PlaneROI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PlaneROI.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PointsFromIndices.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/PointsFromIndices.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ProximityROI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ProximityROI.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/QuatToRigidEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/QuatToRigidEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ROIValueMapper.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/RandomPointDistributionInSurface.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/RandomPointDistributionInSurface.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/RigidToQuatEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/RigidToQuatEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ShapeMatching.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ShapeMatching.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SmoothMeshEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SmoothMeshEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SphereROI.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SphereROI.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Spiral.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Spiral.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SubsetTopology.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/SubsetTopology.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TextureInterpolation.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TextureInterpolation.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TransformEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TransformEngine.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TransformMatrixEngine.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TransformPosition.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/TransformPosition.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ValuesFromIndices.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ValuesFromIndices.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ValuesFromPositions.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/ValuesFromPositions.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Vertex2Frame.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/Vertex2Frame.inl"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/config.h"
    "/org/share/home/students/haentsch/dev/depthmatch/ext/sofa/modules/SofaEngine/initEngine.h"
    )
endif()

