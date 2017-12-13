/******************************************************************************
 *       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
 *                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
 *                                                                             *
 * This library is free software; you can redistribute it and/or modify it     *
 * under the terms of the GNU Lesser General Public License as published by    *
 * the Free Software Foundation; either version 2.1 of the License, or (at     *
 * your option) any later version.                                             *
 *                                                                             *
 * This library is distributed in the hope that it will be useful, but WITHOUT *f
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
 * for more details.                                                           *f
 *                                                                             *
 * You should have received a copy of the GNU Lesser General Public License    *
 * along with this library; if not, write to the Free Software Foundation,     *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
 *******************************************************************************
 *                               SOFA :: Modules                               *
 *                                                                             *
 * Authors: The SOFA Team and external contributors (see Authors.txt)          *
 *                                                                             *
 * Contact information: contact@sofa-framework.org                             *
 ******************************************************************************/
#ifndef SOFA_COMPONENT_MAPPING_MapperQuadraticTetrahedronSetTopology_INL
#define SOFA_COMPONENT_MAPPING_MapperQuadraticTetrahedronSetTopology_INL

#include <MapperQuadraticTetrahedronSetTopology.h>
#include <sofa/core/visual/VisualParams.h>

#include <sofa/core/Mapping.inl>
#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/core/behavior/MechanicalState.h>


#include <sofa/component/topology/RegularGridTopology.h>
#include <sofa/component/topology/SparseGridTopology.h>
#include <sofa/component/topology/EdgeSetTopologyContainer.h>
#include <sofa/component/topology/TriangleSetTopologyContainer.h>
#include <sofa/component/topology/QuadSetTopologyContainer.h>
#include <sofa/component/topology/TetrahedronSetTopologyContainer.h>
#include <sofa/component/topology/HexahedronSetTopologyContainer.h>
#include <sofa/component/topology/EdgeSetGeometryAlgorithms.h>
#include <sofa/component/topology/TriangleSetGeometryAlgorithms.h>
#include <sofa/component/topology/QuadSetGeometryAlgorithms.h>
#include <sofa/component/topology/TetrahedronSetGeometryAlgorithms.h>
#include <sofa/component/topology/HexahedronSetGeometryAlgorithms.h>
#include <sofa/component/topology/TopologyData.inl>

#include <sofa/helper/system/thread/CTime.h>
#include <sofa/helper/AdvancedTimer.h>

#include <sofa/helper/vector.h>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/system/config.h>

#include <sofa/simulation/common/Simulation.h>

#include <algorithm>
#include <iostream>

#include "QuadraticMeshTopology.h"
#include "QuadraticBaseMeshTopology.h"

#include <sstream>

#include "levmar.h"

#define TOLERANCE 5e-6

//just 4 debug purposes
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkSTLReader.h"
#include "vtkPolyData.h"
#include "vtkStructuredGridWriter.h"
#include "vtkStructuredGrid.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellArray.h"
#include "vtkVertexGlyphFilter.h"

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"

#include "math.h"

#define PRINTDEBUGOUTPUT 200
#define PRINTDEBUGPOINT 21341

namespace sofa
{

	namespace component
	{

		namespace mapping
		{

			using namespace sofa::defaulttype;


			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::clear1d ( int reserve )
			{
//				updateJ = true;
//				map1d.clear(); if ( reserve>0 ) map1d.reserve ( reserve );
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::clear2d ( int reserve )
			{
//				updateJ = true;
//				map2d.clear(); if ( reserve>0 ) map2d.reserve ( reserve );
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::clear3d ( int reserve )
			{
//				updateJ = true;
				map3d.clear(); if ( reserve>0 ) map3d.reserve ( reserve );
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::clear ( int reserve )
			{
//				updateJ = true;
//				map1d.clear(); if ( reserve>0 ) map1d.reserve ( reserve );
//				map2d.clear(); if ( reserve>0 ) map2d.reserve ( reserve );
				map3d.clear(); if ( reserve>0 ) map3d.reserve ( reserve );
			}


			template <class In, class Out>
			int MapperQuadraticTetrahedronSetTopology<In,Out>::addPointInTetra ( const int tetraIndex, const SReal* baryCoords )
			{
//				map3d.resize ( map3d.size() +1 );
//				MappingData3D& data = *map3d.rbegin();
//				data.in_index = tetraIndex;
//				data.baryCoords[0] = ( Real ) baryCoords[0];
//				data.baryCoords[1] = ( Real ) baryCoords[1];
//				data.baryCoords[2] = ( Real ) baryCoords[2];
//				return map3d.size()-1;
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::addPointInQuadTetra(const int pointIndex, const int tetraIndex, const SReal* baryCoords, double distance, bool isOutside, double* normal0, double* pos0, double* outPos0)
			{
				//map3d.resize ( map3d.size() +1 );
				//MappingData3D& data = *map3d.rbegin();
				QuadraticMappingData data;// = *map3d[pointIndex];
				data.in_index = tetraIndex;
				data.baryCoords[0] = ( Real ) baryCoords[0];
				data.baryCoords[1] = ( Real ) baryCoords[1];
				data.baryCoords[2] = ( Real ) baryCoords[2];
				data.distance = distance;
				data.isOutside = isOutside;
				data.normal0[0]=normal0[0];
				data.normal0[1]=normal0[1];
				data.normal0[2]=normal0[2];
				data.pos0[0]=pos0[0];
				data.pos0[1]=pos0[1];
				data.pos0[2]=pos0[2];
				data.outPos0[0]=outPos0[0];
				data.outPos0[1]=outPos0[1];
				data.outPos0[2]=outPos0[2];
				map3d[pointIndex] = data;

			}



			template <class In, class Out>
							void MapperQuadraticTetrahedronSetTopology<In,Out>::LocalSurfaceCoordsToLocalVolumeCoords(double* localSurfaceCoords, int faceNumber, double * localVolumeCoords)
			{
				//std::cout<<"FaceNumber "<<faceNumber<<"\n";
				switch (faceNumber)
				{
				case 0:
					localVolumeCoords[0] = 0;
					localVolumeCoords[1] = localSurfaceCoords[1];
					localVolumeCoords[2] = localSurfaceCoords[0];
					break;
				case 1:
					localVolumeCoords[0] = localSurfaceCoords[0];
					localVolumeCoords[1] = 0;
					localVolumeCoords[2] = localSurfaceCoords[1];
					break;
				case 2:
					localVolumeCoords[0] = localSurfaceCoords[0];
					localVolumeCoords[1] = localSurfaceCoords[1];
					localVolumeCoords[2] = 1-localSurfaceCoords[0]-localSurfaceCoords[1];
					break;
				case 3:
					localVolumeCoords[0] = localSurfaceCoords[1];
					localVolumeCoords[1] = localSurfaceCoords[0];
					localVolumeCoords[2] = 0;
					break;
				default:
					std::cout<<"Error, faceNo wrong\n";
					break;
				}
			}

			template <class In, class Out>
					void MapperQuadraticTetrahedronSetTopology<In,Out>::FindBestNormalFitOnSurfaceTriangle(unsigned int triangleId, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, double & distance, double & error, double & angle, sofa::defaulttype::Vector3 & differenceVector, bool verbose)
					{
						sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

						sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(triangleId);

						double nodesAndNormalsAndPos[39];


						for(int iteri=0; iteri<3; iteri++)
						{
							for(int p=0; p<6; p++)
							{
								nodesAndNormalsAndPos[6*iteri+p] = (in[currentTriangle[p]])[iteri];
							}
						}

						for(int iteri=0; iteri<3; iteri++)
						{
							for(int p=0; p<6; p++)
							{
								nodesAndNormalsAndPos[18+6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
							}
						}

						nodesAndNormalsAndPos[36]=referencePosition[0];
						nodesAndNormalsAndPos[37]=referencePosition[1];
						nodesAndNormalsAndPos[38]=referencePosition[2];

						double lb[2];
						lb[0]=0.0;
						lb[1]=0.0;

						double ub[2];
						ub[0]=1.0;
						ub[1]=1.0;

						double C[2];
						C[0]=-1.0;
						C[1]=-1.0;


						double d[1];
						d[0] = -1.0;

//						double lb[2];
//						lb[0]=-TOLERANCE;//0.0;
//						lb[1]=-TOLERANCE;//0.0;
//
//						double ub[2];
//						ub[0]=1.0+TOLERANCE;
//						ub[1]=1.0+TOLERANCE;
//
//						double C[2];
//						C[0]=-1.0;
//						C[1]=-1.0;
//
//
//						double d[1];
//						d[0] = -1.0-TOLERANCE;

						double info[LM_INFO_SZ];

						Vector3 normal, testPos;
						double scalarAngle;

						double opts[4];
						opts[0] =   1E-03;//tau=LM_CNST(LM_INIT_MU);
						opts[1] =   1E-17;//  eps1=LM_CNST(LM_STOP_THRESH);
						opts[2] =   1E-100;//17;//100;// eps2=LM_CNST(LM_STOP_THRESH); ||Dp||_2
						opts[3] =   1E-17;//eps3=LM_CNST(LM_STOP_THRESH);




//						if(localSurfaceCoords[2] == 0)
						{
							localSurfaceCoords[0]= 0.3;
							localSurfaceCoords[1]= 0.3;
							localSurfaceCoords[2]= 0.0;

							dlevmar_bleic_dif(CalculateGlobalCoordsSurface,  localSurfaceCoords.ptr(), referencePosition.ptr(), 2, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodesAndNormalsAndPos[0]);
						}

						Vector3 reference;
						reference[0] = 0.0;
						reference[1] = 0.0;
						reference[2] = 0.0;


						if(verbose)
						{

							CalculateAngleBetweenNormal(localSurfaceCoords.ptr(), &scalarAngle, 3,1,nodesAndNormalsAndPos);

							CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), testPos.ptr(),3,3,&nodesAndNormalsAndPos[0]);

							CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), normal.ptr(),3,3,&nodesAndNormalsAndPos[18]);

							CalculateDifferenceVector(localSurfaceCoords.ptr(),  differenceVector.ptr(), 2, 3, &nodesAndNormalsAndPos[0]);

							normal.normalize();

							distance = (referencePosition - testPos).norm();

							error = ((testPos+normal*distance)-referencePosition).norm();
							//std::cout<<"LevmarOptimizationInfo 0:"<<info[0]<<" 1:"<<info[1]<<" 2:"<<info[2]<<" 3:"<<info[3]<<" 4:"<<info[4]<<" 5:"<<info[5]<<" 6:"<<info[6]<<" 7:"<<info[7]<<" 8:"<<info[8]<<" 9:"<<info[9]<<"\n";

							std::cout<<"Error before normal correction: "<<error<<", angle "<<scalarAngle<<", difference vector "<<differenceVector<<", coords "<<localSurfaceCoords;

						}


						dlevmar_bleic_dif(CalculateDifferenceVectorBetweenNormal,  localSurfaceCoords.ptr(), reference.ptr(), 2, 3,lb,NULL,NULL,NULL,0,C,d,1,1000000,NULL, info, NULL, NULL, (void*)&nodesAndNormalsAndPos[0]);

						dlevmar_bleic_dif(CalculateDifferenceVector,  localSurfaceCoords.ptr(), reference.ptr(), 2, 3,lb,NULL,NULL,NULL,0,C,d,1,1000000,opts, info, NULL, NULL, (void*)&nodesAndNormalsAndPos[0]);



						CalculateAngleBetweenNormal(localSurfaceCoords.ptr(), &scalarAngle, 3,1,nodesAndNormalsAndPos);

						CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), testPos.ptr(),3,3,&nodesAndNormalsAndPos[0]);

						CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), normal.ptr(),3,3,&nodesAndNormalsAndPos[18]);
						normal.normalize();

						distance = (referencePosition - testPos).norm();

						error = ((testPos+normal*distance)-referencePosition).norm();

						angle = scalarAngle;//acos(scalarAngle);


						CalculateDifferenceVector(localSurfaceCoords.ptr(),  differenceVector.ptr(), 2, 3, &nodesAndNormalsAndPos[0]);
//						differenceVector = (referencePosition - testPos);
//						differenceVector.normalize();
//						differenceVector = differenceVector - normal;

						if(verbose)
						{
							std::cout<<"LevmarOptimizationInfo 0:"<<info[0]<<" 1:"<<info[1]<<" 2:"<<info[2]<<" 3:"<<info[3]<<" 4:"<<info[4]<<" 5:"<<info[5]<<" 6:"<<info[6]<<" 7:"<<info[7]<<" 8:"<<info[8]<<" 9:"<<info[9]<<"\n";
							std::cout<<",error after normal correction: "<<error<<", angle"<<scalarAngle<<", difference vector "<<differenceVector <<", coords "<<localSurfaceCoords<<"\n";

						}


					}

	template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::RecursiveFindBestNormalFitOnSurfaceTriangle(unsigned int startTriangleId, double currentMinError, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, unsigned int & finalTriangleIndex, double & distance, double & error, double & angle,  std::set<unsigned int> & trianglesVisited, bool printDebugOutput)
			{
				sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				//sanity check: is startTriangleId a surface Triangle?
				unsigned int noTetras = theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId).size();
				if(noTetras != 1)
					std::cout<<"Error, triangle is no surface triangle!!\n";


				sofa::defaulttype::Vector3  tempDifferenceVector;
				double currentDistance;
				//first optimize on given starttriangleid with given local coords
				FindBestNormalFitOnSurfaceTriangle(startTriangleId, referencePosition,  in, localSurfaceCoords,  currentDistance, error,  angle,tempDifferenceVector, printDebugOutput);

				finalTriangleIndex = startTriangleId;

				if(printDebugOutput)
				{
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(startTriangleId);


					double nodesAndNormals[36];

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodesAndNormals[6*iteri+p] = (in[currentTriangle[p]])[iteri];
						}
					}

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodesAndNormals[18+6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
						}
					}

					double currentVTKPoint[3];
					double currentVTKNormal[3];

					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKPoint[0],3,3,&nodesAndNormals[0]);
					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKNormal[0],3,3,&nodesAndNormals[18]);

					std::cout<<"Writing triangle no "<<startTriangleId<<" with nodes "<<currentTriangle<<" to vtk file, local coords are"<< localSurfaceCoords<<", current point is"<<currentVTKPoint[0]<<" "<<currentVTKPoint[1]<<" "<<currentVTKPoint[2]<<" "<<"\n";

					vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
					m_VectorsOptiDebug->InsertNextTuple3(currentVTKNormal[0],currentVTKNormal[1],currentVTKNormal[2]);
					m_ScalarsOptiDebug->InsertNextTuple1(pIndex);

					vtkIdType nodeIndices[6];
					vtkIdType nodeIndices2[3];


					for(unsigned int itera=0; itera<6; itera++)
					{
						currentVTKPoint[0] = (in[ currentTriangle[itera] ])[0];
						currentVTKPoint[1] = (in[ currentTriangle[itera] ])[1];
						currentVTKPoint[2] = (in[ currentTriangle[itera] ])[2];
						unsigned int currentPointId2 = m_PointContainerDebugTriangles->InsertNextPoint( currentVTKPoint);
						nodeIndices[itera] = currentPointId2;
					}

					//not necessary -> already vtk!!!!!
					nodeIndices2[0] = nodeIndices[0];
					nodeIndices2[1] = nodeIndices[1];
					nodeIndices2[2] = nodeIndices[2];

					m_TriangleContainerDebugTriangles->InsertNextCell(3, nodeIndices2);

					m_ScalarsDebugTriangles->InsertNextTuple1(pIndex);

					m_DebugCounter++;
				}



				//get all faces along current optimization front (1-2)
				sofa::helper::vector< unsigned int >  currentEdges;
				sofa::helper::vector< double>  currentEdgeCoordVec;



				std::set<unsigned int>::iterator itt;

				if(localSurfaceCoords[0] < TOLERANCE)
				{
					currentEdges.push_back(0);
					currentEdgeCoordVec.push_back(localSurfaceCoords[1]);

				}

				if(localSurfaceCoords[1] < TOLERANCE)
				{
					currentEdges.push_back(1);
					currentEdgeCoordVec.push_back(localSurfaceCoords[0]);
				}

				if((1-localSurfaceCoords[0]-localSurfaceCoords[1]) < TOLERANCE)
				{
					currentEdges.push_back(2);
					currentEdgeCoordVec.push_back(localSurfaceCoords[1]);
				}

				unsigned int currentEdgeNr = currentEdges.size();

				if(printDebugOutput)
					std::cout<<"DebugInformation: triangle no:"<<startTriangleId<<", startTriangle:"<<theTopology->getQuadraticTriangleVTK(startTriangleId)<<", local coords:"<<localSurfaceCoords<<", currentEdges:"<<currentEdges<<", currentDistance"<<currentDistance<<"\n";



				//if angle error is below tolerance: return
				//if angle error is significantly greater than given current min error: return
				//if current point is not on edge: return
				//if((currentDistance<TOLERANCE)||(currentDistance>(currentMinError+TOLERANCE))||(!currentEdgeNr))
				if((error<TOLERANCE)||(!currentEdgeNr))
				{
					finalTriangleIndex = startTriangleId;
					distance = currentDistance;
//					currentMinError = error;
					if(printDebugOutput)
					{
						std::cout<<"Returning to upper call with triangleID: "<<finalTriangleIndex<<" and with error "<<error<<"\n";
					}
					return;
				}

				//only go on if angle error gets smaller, in this case
				if(error<currentMinError)
					currentMinError = error;

				finalTriangleIndex = startTriangleId;
				distance = currentDistance;
				//std::cout<<"Recursion for triangle "<<startTriangleId<<" successful, current angle error is:"<<currentMinError<<" \n ";

				unsigned int currentTriangleId;
				unsigned int tempTriangleIndex;
				int currentOrientation, startTriangleEdgeOrientation;
				sofa::defaulttype::Vector3 newSurfaceCoords;
				newSurfaceCoords[2] =1;
				double tempDistance;
				double tempError;
				double tempAngle;

				double currentEdgeCoord;


				for(unsigned int edgeIter = 0; edgeIter<currentEdgeNr; ++edgeIter)
				{
					unsigned int currentEdgeGlobalId = theTopology->getQuadraticEdgesInQuadraticTriangle(startTriangleId)[currentEdges[edgeIter] ];


					//std::cout<<"Start recursion on active edge:"<< currentEdgeGlobalId <<", with nodes: "<<theTopology->getQuadraticEdge(currentEdgeGlobalId)<<"\n";

					//get surface quadratic triangles around edge
					sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdge(currentEdgeGlobalId );
					sofa::helper::vector<unsigned int> currentTriangleIdVec;

					if(triangles.size() != 2) //this is indeed possible!!!!!
					{
						for(unsigned int iterTriangles=0; iterTriangles<triangles.size();iterTriangles++)
						{
							if(triangles[iterTriangles] != startTriangleId)
								currentTriangleIdVec.push_back(triangles[iterTriangles]);
						}

//						std::cout<<"Error: getSurfaceQuadraticTrianglesAroundQuadraticEdge must be wrong, triangles size is:"<<triangles.size()<<", edge Id is:"<<currentEdgeGlobalId<<", triangleId is:"<<startTriangleId<<", tetraId is:"<<theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId)[0]<<" , triangles are:\n";
//						for(unsigned int testIter = 0; testIter<triangles.size(); ++testIter)
//						{
//								std::cout<<"Triangle no "<<testIter<<": "<<triangles[testIter]<<" nodes:"<<theTopology->getQuadraticTriangleVTK(triangles[testIter])<<", number of tetras around triangle"<< theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[testIter]).size()<<"\n";
//						}
					}

					else
					{
						//one triangle is current triangle -> neglect
						if(triangles[0] == startTriangleId)
						{
							currentTriangleIdVec.push_back(triangles[1]);
	//								currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentEdgeGlobalId)[1];
	//								startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(TriangleID i);
						}
						else
						{
							currentTriangleIdVec.push_back(triangles[0]);
	//								currentOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[0];
	//								startTriangleEdgeOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[1];
						}
					}

					if(printDebugOutput)
					{
						std::cout<<"\n"<<"CurrentGlobalEdgeId: "<<currentEdgeGlobalId<<" EdgesInStartTriangle:"<<theTopology->getQuadraticEdgesInQuadraticTriangle(startTriangleId)<<", triangles around edge "<<triangles;
					}

					for(unsigned int iterVec=0; iterVec<currentTriangleIdVec.size();iterVec++)
					{
						//std::cout<<"Testing if triangle is in set\n";
						//if neighbour triangle is not in set: add to set
						currentTriangleId = currentTriangleIdVec[iterVec];
						itt=trianglesVisited.find(currentTriangleId);

						if(itt == trianglesVisited.end() )
						{
							//std::cout<<"Triangle is in set\n";
							trianglesVisited.insert(currentTriangleId);

							//calculate new start local coords
							const sofa::component::topology::QuadraticMeshTopology::EdgesInTriangle&  currentEdgesInTriangle = theTopology->getQuadraticEdgesInQuadraticTriangle(currentTriangleId);
							int currentLocalEdgeNumber = theTopology->getQuadraticEdgeIndexInTriangle(currentEdgesInTriangle,currentEdgeGlobalId);
							if(currentLocalEdgeNumber == -1)
								std::cout<<"Error, current local edge number wrong \n";
							//get edge coordinate on startTriangleId (adjust for orientation)
							startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(startTriangleId)[currentEdges[edgeIter]];
							LocalSurfaceCoordsToLocalEdgeCoord(localSurfaceCoords.ptr(), currentEdges[edgeIter], &currentEdgeCoord);
							if(startTriangleEdgeOrientation = -1)
								currentEdgeCoord = 1-currentEdgeCoord;
							//Error, there must be two triangles around edge, number of triangles
							//adjust orientation on currentTriangleId

							//currentEdgeCoord = startTriangleEdgeOrientation*currentEdgeCoord;

							//calculate surface coord on currentTriangleId
							currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentTriangleId)[currentLocalEdgeNumber];
							if(currentOrientation = -1)
								currentEdgeCoord = 1-currentEdgeCoord;
							LocalEdgeCoordToLocalSurfaceCoords(currentEdgeCoord, currentLocalEdgeNumber, newSurfaceCoords.ptr());

							if(printDebugOutput)
								std::cout<<"Edges in current Triangle:"<<currentEdgesInTriangle<<"\n";
							if(printDebugOutput)
								std::cout<<"Nodes of neighbour triangle:"<<theTopology->getQuadraticTriangleVTK(currentTriangleId)<<", with local edge no:"<<currentLocalEdgeNumber<<"\n";

							//recursively call method with updated startTriangleId, currentMinError
							//RecursiveClosestPointOnSurfaceTriangle(currentTriangleId, currentMinError, referencePosition, in, newSurfaceCoords, tempTriangleIndex, tempDistance, trianglesVisited,printDebugOutput);
							if(printDebugOutput)
							{
								std::cout<<"Start recursion on triangle "<<currentTriangleId<<" and with error "<<error<<" and currentMinError "<<currentMinError<<"\n";
							}

							RecursiveFindBestNormalFitOnSurfaceTriangle(currentTriangleId, currentMinError, referencePosition,in, newSurfaceCoords,tempTriangleIndex, tempDistance, tempError, tempAngle, trianglesVisited, printDebugOutput);

							if(printDebugOutput)
							{
								std::cout<<"Recursion that started on triangle "<<currentTriangleId<<" and with error "<<error<<", ended with triangle "<<tempTriangleIndex<<" and error "<<tempError<<" and currentMinError "<<currentMinError<<"\n";
							}

							//currentDifferenceToPerfectAngle = 1- tempAngle;

							//if given error is smaller than current error, update
							if(tempError < currentMinError)
							{
								currentMinError = tempError;
								finalTriangleIndex = tempTriangleIndex;
								distance = tempDistance;
								error = tempError;
								localSurfaceCoords[0] = newSurfaceCoords[0];
								localSurfaceCoords[1] = newSurfaceCoords[1];
							}


						}
					}



				}

				if(printDebugOutput)
				{
					std::cout<<"Recursion closest point, current distance "<<distance<<"\n";
				}

				if(printDebugOutput)
				{
					std::cout<<"Returning to upper call with triangleID: "<<finalTriangleIndex<<" and with error "<<error<<"\n";
				}



//						sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
//
//						sofa::defaulttype::Vector3  tempDifferenceVector;
//
//						//first optimize on given starttriangleid with given local coords
//						FindBestNormalFitOnSurfaceTriangle(startTriangleId, referencePosition,  in, localSurfaceCoords,  distance, error,  angle,tempDifferenceVector);
//						finalTriangleIndex = startTriangleId;
//
//						if(printDebugOutput)
//						{
//							sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
//
//							sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(startTriangleId);
//
//							double nodesAndNormals[36];
//
//							for(int iteri=0; iteri<3; iteri++)
//							{
//								for(int p=0; p<6; p++)
//								{
//									nodesAndNormals[6*iteri+p] = (in[currentTriangle[p]])[iteri];
//								}
//							}
//
//							for(int iteri=0; iteri<3; iteri++)
//							{
//								for(int p=0; p<6; p++)
//								{
//									nodesAndNormals[18+6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
//								}
//							}
//
//							double currentVTKPoint[3];
//							double currentVTKNormal[3];
//
//							CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKPoint[0],3,3,&nodesAndNormals[0]);
//							CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKNormal[0],3,3,&nodesAndNormals[18]);
//
//
//							vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
//							m_VectorsOptiDebug->InsertNextTuple3(currentVTKNormal[0],currentVTKNormal[1],currentVTKNormal[2]);
//							m_ScalarsOptiDebug->InsertNextTuple1(pIndex);
//
//							vtkIdType nodeIndices[6];
//							vtkIdType nodeIndices2[3];
//
//
//							for(unsigned int itera=0; itera<6; itera++)
//							{
//								currentVTKPoint[0] = (in[ currentTriangle[itera] ])[0];
//								currentVTKPoint[1] = (in[ currentTriangle[itera] ])[1];
//								currentVTKPoint[2] = (in[ currentTriangle[itera] ])[2];
//								unsigned int currentPointId2 = m_PointContainerDebugTriangles->InsertNextPoint( currentVTKPoint);
//								nodeIndices[itera] = currentPointId2;
//							}
//
//							nodeIndices2[0] = nodeIndices[0];
//							nodeIndices2[1] = nodeIndices[1];
//							nodeIndices2[2] = nodeIndices[2];
//
//							m_TriangleContainerDebugTriangles->InsertNextCell(3, nodeIndices2);
//
//							m_ScalarsDebugTriangles->InsertNextTuple1(pIndex);
//
//							m_DebugCounter++;
//						}
//
//
//						//if given error is smaller than current error, update#
//						double currentDifferenceToPerfectAngle = 1- angle;
//
//
//
//
//						//get all faces along current optimization front (1-2)
//						sofa::helper::vector< unsigned int >  currentEdges;
//						sofa::helper::vector< double>  currentEdgeCoordVec;
//
//
//
//						std::set<unsigned int>::iterator itt;
//
//						if(localSurfaceCoords[0] < TOLERANCE)
//						{
//							currentEdges.push_back(0);
//							currentEdgeCoordVec.push_back(localSurfaceCoords[1]);
//
//						}
//
//						if(localSurfaceCoords[1] < TOLERANCE)
//						{
//							currentEdges.push_back(1);
//							currentEdgeCoordVec.push_back(localSurfaceCoords[0]);
//						}
//
//						if((1-localSurfaceCoords[0]-localSurfaceCoords[1]) < TOLERANCE)
//						{
//							currentEdges.push_back(2);
//							currentEdgeCoordVec.push_back(localSurfaceCoords[1]);
//						}
//
//						unsigned int currentEdgeNr = currentEdges.size();
//
//						if(printDebugOutput)
//							std::cout<<"DebugInformation: triangle no:"<<startTriangleId<<", startTriangle:"<<theTopology->getQuadraticTriangleVTK(startTriangleId)<<", local coords:"<<localSurfaceCoords<<", current angle error"<<currentDifferenceToPerfectAngle<<", current difference vector"<<tempDifferenceVector<<", currentEdges:"<<currentEdges<<"\n";
//
//
//
//						if(angle < 0)
//						{
//							if(printDebugOutput)
//								std::cout<<"angle between normal and difference vector < 0, this should not happen, abort recursion \n";
//							return;
//						}
//
//						//if angle error is below tolerance: return
//						//if angle error is significantly greater than given current min error: return
//						//if current point is not on edge: return
//						if((currentDifferenceToPerfectAngle<TOLERANCE)||(currentDifferenceToPerfectAngle>(currentMinError+TOLERANCE))||(!currentEdgeNr))
//						//if((currentDifferenceToPerfectAngle<TOLERANCE)||(!currentEdgeNr))
//						{
//
//							return;
//						}
//
//						//only go on if angle error gets smaller, in this case
//						currentMinError = currentDifferenceToPerfectAngle;
//						//std::cout<<"Recursion for triangle "<<startTriangleId<<" successful, current angle error is:"<<currentMinError<<" \n ";
//
//						unsigned int currentTriangleId;
//						unsigned int tempTriangleIndex;
//						int currentOrientation, startTriangleEdgeOrientation;
//						sofa::defaulttype::Vector3 newSurfaceCoords;
//						newSurfaceCoords[2] =1;
//						double tempDistance, tempError, tempAngle;
//
//						double currentEdgeCoord;
//
//
//						for(unsigned int edgeIter = 0; edgeIter<currentEdgeNr; ++edgeIter)
//						{
//							unsigned int currentEdgeGlobalId = theTopology->getQuadraticEdgesInQuadraticTriangle(startTriangleId)[currentEdges[edgeIter] ];
//
//							//std::cout<<"Start recursion on active edge:"<< currentEdgeGlobalId <<", with nodes: "<<theTopology->getQuadraticEdge(currentEdgeGlobalId)<<"\n";
//
//							//get surface quadratic triangles around edge
//							sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdge(currentEdgeGlobalId );
//
//							if(triangles.size() != 2)
//							{
//								std::cout<<"Error: getSurfaceQuadraticTrianglesAroundQuadraticEdge must be wrong, triangles size is:"<<triangles.size()<<", edge Id is:"<<currentEdgeGlobalId<<", triangleId is:"<<startTriangleId<<", tetraId is:"<<theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId)[0]<<" , triangles are:\n";
//								for(unsigned int testIter = 0; testIter<triangles.size(); ++testIter)
//								{
//										std::cout<<"Triangle no "<<testIter<<": "<<triangles[testIter]<<" nodes:"<<theTopology->getQuadraticTriangleVTK(triangles[testIter])<<", number of tetras around triangle"<< theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[testIter]).size()<<"\n";
//								}
//							}
//
//							//one triangle is current triangle -> neglect
//							if(triangles[0] == startTriangleId)
//							{
//								currentTriangleId = triangles[1];
////								currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentEdgeGlobalId)[1];
////								startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(TriangleID i);
//							}
//							else
//							{
//								currentTriangleId = triangles[0];
////								currentOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[0];
////								startTriangleEdgeOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[1];
//							}
//
//							//std::cout<<"Testing if triangle is in set\n";
//							//if neighbour triangle is not in set: add to set
//							itt=trianglesVisited.find(currentTriangleId);
//
//							if(itt == trianglesVisited.end() )
//							{
//								//std::cout<<"Triangle is in set\n";
//								trianglesVisited.insert(currentTriangleId);
//
//								//calculate new start local coords
//								const sofa::component::topology::QuadraticMeshTopology::EdgesInTriangle&  currentEdgesInTriangle = theTopology->getQuadraticEdgesInQuadraticTriangle(currentTriangleId);
//								int currentLocalEdgeNumber = theTopology->getQuadraticEdgeIndexInTriangle(currentEdgesInTriangle,currentEdgeGlobalId);
//								if(currentLocalEdgeNumber == -1)
//									std::cout<<"Error, current local edge number wrong \n";
//								//get edge coordinate on startTriangleId (adjust for orientation)
//								startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(startTriangleId)[currentEdges[edgeIter]];
//								LocalSurfaceCoordsToLocalEdgeCoord(localSurfaceCoords.ptr(), currentEdges[edgeIter], &currentEdgeCoord);
//								if(startTriangleEdgeOrientation = -1)
//									currentEdgeCoord = 1-currentEdgeCoord;
//
//								//adjust orientation on currentTriangleId
//
//								//currentEdgeCoord = startTriangleEdgeOrientation*currentEdgeCoord;
//
//								//calculate surface coord on currentTriangleId
//								currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentTriangleId)[currentLocalEdgeNumber];
//								if(currentOrientation = -1)
//									currentEdgeCoord = 1-currentEdgeCoord;
//								LocalEdgeCoordToLocalSurfaceCoords(currentEdgeCoord, currentLocalEdgeNumber, newSurfaceCoords.ptr());
//
//								if(printDebugOutput)
//									std::cout<<"Nodes of neihbour triangle:"<<theTopology->getQuadraticTriangleVTK(currentTriangleId)<<", with local edge no:"<<currentLocalEdgeNumber<<"\n";
//
//								//recursively call method with updated startTriangleId, currentMinError
//								RecursiveFindBestNormalFitOnSurfaceTriangle(currentTriangleId, currentMinError, referencePosition, in, newSurfaceCoords, tempTriangleIndex, tempDistance, tempError, tempAngle, trianglesVisited,printDebugOutput);
//
//								currentDifferenceToPerfectAngle = 1- tempAngle;
//
//								//if given error is smaller than current error, update
//								if(currentDifferenceToPerfectAngle < currentMinError)
//								{
//									currentMinError = currentDifferenceToPerfectAngle;
//									finalTriangleIndex = tempTriangleIndex;
//									distance = tempDistance;
//									error = tempError;
//									angle = tempAngle;
//								}
//
//
//							}
//
//
//
//						}


					}

	template <class In, class Out>
					bool MapperQuadraticTetrahedronSetTopology<In,Out>::RecursiveClosestPoint(unsigned int startTetraId, double currentMinError, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localCoords, unsigned int & finalTetraIndex, double & distance,  std::set<unsigned int> &tetrasVisited, std::set<unsigned int> &trianglesVisited, bool printDebugOutput)
			{
					//std::cout<<"RecursiveClosesPoint, currentTetraId: "<<startTetraId<<"\n";
					//find coords on tetrahedron
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					finalTetraIndex = startTetraId;

					//first optimize on given starttriangleid with given local coords
					double currentDistance = FindClosestPointInTetrahedron(startTetraId, referencePosition, in, localCoords);



					//check if on surface
					sofa::defaulttype::Vector3 newSurfaceCoords;
					std::vector<unsigned int> faceNumbers;
					std::vector<sofa::defaulttype::Vector3> newSurfaceCoordsVec;
					bool isOnSurface = false;
					bool isOnFace = LocalVolumeCoordsToLocalSurfaceCoords(localCoords.ptr(), faceNumbers, newSurfaceCoordsVec);


					//if not on face or distance below tolerance: return
					if((!isOnFace)||(currentDistance<TOLERANCE))
					{
						distance = currentDistance;
						finalTetraIndex = startTetraId;
						//std::cout<<"RecursiveClosesPoint, finalTetraID not on face: "<<finalTetraIndex<<"\n";
						return false;
					}



					if(currentDistance<currentMinError)
					{
						currentMinError = currentDistance;
					}

					distance = currentDistance;
					bool isOnSurfaceTemp;

					for(unsigned int faceIter=0;faceIter<faceNumbers.size(); faceIter++)
					{

						isOnSurfaceTemp = false;
						unsigned int faceNumber = faceNumbers[faceIter];
						const sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron triangles = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(startTetraId);
						unsigned int startTriangleId = triangles[faceNumber];
						if((theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[faceNumber])).size() == 1)
						{
							isOnSurface = true;
							isOnSurfaceTemp = true;
						}

						//if on surface face: start surface recursion
						if(isOnSurfaceTemp)
						{
							unsigned int tempTriangleIndex;
							double tempDistance;

							//std::set<unsigned int> trianglesVisited;
							//std::cout<<"RecursiveClosesPoint, startTriangleId isOnSurface: "<<startTriangleId<<", final tetra index"<<finalTetraIndex<<"\n";

							RecursiveClosestPointOnSurfaceTriangle(startTriangleId, currentMinError, referencePosition, in, newSurfaceCoordsVec[faceIter], tempTriangleIndex, tempDistance, trianglesVisited,printDebugOutput);

							//std::cout<<"RecursiveClosesPointSurfaceCalled, old distance:"<<distance<<", new distance:"<<tempDistance<<"\n";

							if(tempDistance < currentMinError)
							{
								//convert  to volume coords
								//std::cout<<"RecursiveClosesPoint, finalTetraIndexBeforesufaceUpdate: "<<finalTetraIndex<<"\n";
								finalTetraIndex= theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(tempTriangleIndex)[0];
								//std::cout<<"RecursiveClosesPoint, finalTetraIndexAftersufaceUpdate: "<<finalTetraIndex<<"\n";
								const sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(finalTetraIndex);
								int localfaceNr = theTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, tempTriangleIndex);
								if(localfaceNr == -1)
									std::cout<<"Error, local face nr wrong"<<"\n";
								LocalSurfaceCoordsToLocalVolumeCoords( newSurfaceCoordsVec[faceIter].ptr(), localfaceNr, localCoords.ptr());
								distance = tempDistance;
								currentMinError = tempDistance;
							//	isOnSurface = true;
							}




							//std::cout<<"RecursiveClosesPoint, finalTetraID isOnSurface: "<<finalTetraIndex<<"\n";


						}
						//if no: hop to neihbour tetras
						else
						{
								//get neighbour tetras
								unsigned int nextTetraId;
								const sofa::component::topology::QuadraticMeshTopology::TetrahedraAroundTriangle candidateTetras =	theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId);

								if(candidateTetras.size() != 2)
									std::cout<<"Error, there must be two tetras around triangle!!\n";

								if(candidateTetras[0] == startTetraId)
								{
									nextTetraId = candidateTetras[1];
								}
								else
									nextTetraId = candidateTetras[0];

								std::set<unsigned int>::iterator itt=tetrasVisited.find(nextTetraId);

								if(itt == tetrasVisited.end() )
								{
									//std::cout<<"Triangle is in set\n";
									tetrasVisited.insert(nextTetraId);
									sofa::defaulttype::Vector3 newLocalCoords;
									newLocalCoords[0] = 0.3;
									newLocalCoords[1] = 0.3;
									newLocalCoords[2] = 0.3;
									unsigned int tempTetraIndex;
									double tempDistance;
									//std::cout<<"RecursiveClosesPoint, nextTetraId inside: "<<nextTetraId<<"\n";
									isOnSurfaceTemp = RecursiveClosestPoint(nextTetraId, currentMinError, referencePosition, in, newLocalCoords, tempTetraIndex, tempDistance,  tetrasVisited, trianglesVisited, printDebugOutput);

									//std::cout<<"RecursiveClosesPointCalled, old distance:"<<distance<<", new distance:"<<tempDistance<<"\n";
									if(tempDistance < currentMinError)
									{
										//convert  to volume coords
										localCoords[0]= newLocalCoords[0];
										localCoords[1]= newLocalCoords[1];
										localCoords[2]= newLocalCoords[2];
										distance = tempDistance;
										//std::cout<<"RecursiveClosesPoint, finalTetraIndexBeforeVolumeUpdate: "<<finalTetraIndex<<"\n";
										finalTetraIndex = tempTetraIndex;
										currentMinError = tempDistance;
										isOnSurface = isOnSurfaceTemp;
										//std::cout<<"RecursiveClosesPoint, finalTetraIndexAfterVolumeUpdate: "<<finalTetraIndex<<"\n";
									}


									//std::cout<<"RecursiveClosesPoint, finalTetraID inside: "<<finalTetraIndex<<"\n";


								}

						}
					}

					return isOnSurface;

			}


			template <class In, class Out>
							void MapperQuadraticTetrahedronSetTopology<In,Out>::RecursiveClosestPointOnSurfaceTriangle(unsigned int startTriangleId, double currentMinError, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, unsigned int & finalTriangleIndex, double & distance,  std::set<unsigned int> &trianglesVisited, bool printDebugOutput)
			{
				sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				//sanity check: is startTriangleId a surface Triangle?
				unsigned int noTetras = theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId).size();
				if(noTetras != 1)
					std::cout<<"Error, triangle is no surface triangle!!!!\n";

				//first optimize on given starttriangleid with given local coords
				double currentDistance = FindClosesPointOnSurfaceTriangle(startTriangleId, referencePosition,  in, localSurfaceCoords);


				finalTriangleIndex = startTriangleId;

				if(printDebugOutput)
				{
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(startTriangleId);


					double nodesAndNormals[36];

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodesAndNormals[6*iteri+p] = (in[currentTriangle[p]])[iteri];
						}
					}

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodesAndNormals[18+6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
						}
					}

					double currentVTKPoint[3];
					double currentVTKNormal[3];

					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKPoint[0],3,3,&nodesAndNormals[0]);
					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), &currentVTKNormal[0],3,3,&nodesAndNormals[18]);

					std::cout<<"Writing triangle no "<<startTriangleId<<" with nodes "<<currentTriangle<<" to vtk file, local coords are"<< localSurfaceCoords<<", current point is"<<currentVTKPoint[0]<<" "<<currentVTKPoint[1]<<" "<<currentVTKPoint[2]<<" "<<"\n";

					vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
					m_VectorsOptiDebug->InsertNextTuple3(currentVTKNormal[0],currentVTKNormal[1],currentVTKNormal[2]);
					m_ScalarsOptiDebug->InsertNextTuple1(pIndex);

					vtkIdType nodeIndices[6];
					vtkIdType nodeIndices2[3];


					for(unsigned int itera=0; itera<6; itera++)
					{
						currentVTKPoint[0] = (in[ currentTriangle[itera] ])[0];
						currentVTKPoint[1] = (in[ currentTriangle[itera] ])[1];
						currentVTKPoint[2] = (in[ currentTriangle[itera] ])[2];
						unsigned int currentPointId2 = m_PointContainerDebugTriangles->InsertNextPoint( currentVTKPoint);
						nodeIndices[itera] = currentPointId2;
					}

					//not necessary -> already vtk!!!!!
					nodeIndices2[0] = nodeIndices[0];
					nodeIndices2[1] = nodeIndices[1];
					nodeIndices2[2] = nodeIndices[2];

					m_TriangleContainerDebugTriangles->InsertNextCell(3, nodeIndices2);

					m_ScalarsDebugTriangles->InsertNextTuple1(pIndex);

					m_DebugCounter++;
				}



				//get all faces along current optimization front (1-2)
				sofa::helper::vector< unsigned int >  currentEdges;
				sofa::helper::vector< double>  currentEdgeCoordVec;



				std::set<unsigned int>::iterator itt;

				if(localSurfaceCoords[0] < TOLERANCE)
				{
					currentEdges.push_back(0);
					currentEdgeCoordVec.push_back(localSurfaceCoords[1]);

				}

				if(localSurfaceCoords[1] < TOLERANCE)
				{
					currentEdges.push_back(1);
					currentEdgeCoordVec.push_back(localSurfaceCoords[0]);
				}

				if((1-localSurfaceCoords[0]-localSurfaceCoords[1]) < TOLERANCE)
				{
					currentEdges.push_back(2);
					currentEdgeCoordVec.push_back(localSurfaceCoords[1]);
				}

				unsigned int currentEdgeNr = currentEdges.size();

				if(printDebugOutput)
					std::cout<<"DebugInformation: triangle no:"<<startTriangleId<<", startTriangle:"<<theTopology->getQuadraticTriangleVTK(startTriangleId)<<", local coords:"<<localSurfaceCoords<<", currentEdges:"<<currentEdges<<", currentDistance"<<currentDistance<<"\n";



				//if angle error is below tolerance: return
				//if angle error is significantly greater than given current min error: return
				//if current point is not on edge: return
				//if((currentDistance<TOLERANCE)||(currentDistance>(currentMinError+TOLERANCE))||(!currentEdgeNr))
				if((currentDistance<TOLERANCE)||(!currentEdgeNr))
				{
					finalTriangleIndex = startTriangleId;
					distance = currentDistance;
					return;
				}

				//only go on if angle error gets smaller, in this case
				if(currentDistance<currentMinError)
					currentMinError = currentDistance;

				finalTriangleIndex = startTriangleId;
				distance = currentDistance;
				//std::cout<<"Recursion for triangle "<<startTriangleId<<" successful, current angle error is:"<<currentMinError<<" \n ";

				unsigned int currentTriangleId;
				unsigned int tempTriangleIndex;
				int currentOrientation, startTriangleEdgeOrientation;
				sofa::defaulttype::Vector3 newSurfaceCoords;
				newSurfaceCoords[2] =1;
				double tempDistance;

				double currentEdgeCoord;


				for(unsigned int edgeIter = 0; edgeIter<currentEdgeNr; ++edgeIter)
				{
					unsigned int currentEdgeGlobalId = theTopology->getQuadraticEdgesInQuadraticTriangle(startTriangleId)[currentEdges[edgeIter] ];


					//std::cout<<"Start recursion on active edge:"<< currentEdgeGlobalId <<", with nodes: "<<theTopology->getQuadraticEdge(currentEdgeGlobalId)<<"\n";

					//get surface quadratic triangles around edge
					sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdge(currentEdgeGlobalId );
					sofa::helper::vector<unsigned int> currentTriangleIdVec;

					if(triangles.size() != 2) //this is indeed possible!!!!!
					{
						for(unsigned int iterTriangles=0; iterTriangles<triangles.size();iterTriangles++)
						{
							if(triangles[iterTriangles] != startTriangleId)
								currentTriangleIdVec.push_back(triangles[iterTriangles]);
						}

//						std::cout<<"Error: getSurfaceQuadraticTrianglesAroundQuadraticEdge must be wrong, triangles size is:"<<triangles.size()<<", edge Id is:"<<currentEdgeGlobalId<<", triangleId is:"<<startTriangleId<<", tetraId is:"<<theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId)[0]<<" , triangles are:\n";
//						for(unsigned int testIter = 0; testIter<triangles.size(); ++testIter)
//						{
//								std::cout<<"Triangle no "<<testIter<<": "<<triangles[testIter]<<" nodes:"<<theTopology->getQuadraticTriangleVTK(triangles[testIter])<<", number of tetras around triangle"<< theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[testIter]).size()<<"\n";
//						}
					}

					else
					{
						//one triangle is current triangle -> neglect
						if(triangles[0] == startTriangleId)
						{
							currentTriangleIdVec.push_back(triangles[1]);
	//								currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentEdgeGlobalId)[1];
	//								startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(TriangleID i);
						}
						else
						{
							currentTriangleIdVec.push_back(triangles[0]);
	//								currentOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[0];
	//								startTriangleEdgeOrientation = theTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(currentEdgeGlobalId)[1];
						}
					}

					if(printDebugOutput)
					{
						std::cout<<"\n"<<"CurrentGlobalEdgeId: "<<currentEdgeGlobalId<<" EdgesInStartTriangle:"<<theTopology->getQuadraticEdgesInQuadraticTriangle(startTriangleId)<<", triangles around edge "<<triangles;
					}

					for(unsigned int iterVec=0; iterVec<currentTriangleIdVec.size();iterVec++)
					{
						//std::cout<<"Testing if triangle is in set\n";
						//if neighbour triangle is not in set: add to set
						currentTriangleId = currentTriangleIdVec[iterVec];
						itt=trianglesVisited.find(currentTriangleId);

						if(itt == trianglesVisited.end() )
						{
							//std::cout<<"Triangle is in set\n";
							trianglesVisited.insert(currentTriangleId);

							//calculate new start local coords
							const sofa::component::topology::QuadraticMeshTopology::EdgesInTriangle&  currentEdgesInTriangle = theTopology->getQuadraticEdgesInQuadraticTriangle(currentTriangleId);
							int currentLocalEdgeNumber = theTopology->getQuadraticEdgeIndexInTriangle(currentEdgesInTriangle,currentEdgeGlobalId);
							if(currentLocalEdgeNumber == -1)
								std::cout<<"Error, current local edge number wrong \n";
							//get edge coordinate on startTriangleId (adjust for orientation)
							startTriangleEdgeOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(startTriangleId)[currentEdges[edgeIter]];
							LocalSurfaceCoordsToLocalEdgeCoord(localSurfaceCoords.ptr(), currentEdges[edgeIter], &currentEdgeCoord);
							if(startTriangleEdgeOrientation = -1)
								currentEdgeCoord = 1-currentEdgeCoord;
							//Error, there must be two triangles around edge, number of triangles
							//adjust orientation on currentTriangleId

							//currentEdgeCoord = startTriangleEdgeOrientation*currentEdgeCoord;

							//calculate surface coord on currentTriangleId
							currentOrientation = theTopology->getQuadraticEdgesInQuadraticTriangleOrientation(currentTriangleId)[currentLocalEdgeNumber];
							if(currentOrientation = -1)
								currentEdgeCoord = 1-currentEdgeCoord;
							LocalEdgeCoordToLocalSurfaceCoords(currentEdgeCoord, currentLocalEdgeNumber, newSurfaceCoords.ptr());

							if(printDebugOutput)
								std::cout<<"Edges in current Triangle:"<<currentEdgesInTriangle<<"\n";
							if(printDebugOutput)
								std::cout<<"Nodes of neighbour triangle:"<<theTopology->getQuadraticTriangleVTK(currentTriangleId)<<", with local edge no:"<<currentLocalEdgeNumber<<"\n";

							//recursively call method with updated startTriangleId, currentMinError
							RecursiveClosestPointOnSurfaceTriangle(currentTriangleId, currentMinError, referencePosition, in, newSurfaceCoords, tempTriangleIndex, tempDistance, trianglesVisited,printDebugOutput);

							//currentDifferenceToPerfectAngle = 1- tempAngle;

							//if given error is smaller than current error, update
							if(tempDistance < currentMinError)
							{
								currentMinError = tempDistance;
								finalTriangleIndex = tempTriangleIndex;
								distance = tempDistance;
								localSurfaceCoords[0] = newSurfaceCoords[0];
								localSurfaceCoords[1] = newSurfaceCoords[1];
							}


						}
					}



				}

				if(printDebugOutput)
				{
					std::cout<<"Recursion closest point, current distance "<<distance<<"\n";
				}



			}

			template <class In, class Out>
				void MapperQuadraticTetrahedronSetTopology<In,Out>::CalculateSurfaceGlobalCoords(unsigned int triangleId, sofa::defaulttype::Vector3& localSurfaceCoords, const typename In::VecCoord& in, sofa::defaulttype::Vector3 & position)
				{
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
					sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(triangleId);

					double nodes[18];

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodes[6*iteri+p] = (in[currentTriangle[p]])[iteri];
						}
					}


					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), position.ptr(),3,3,&nodes[0]);

				}

			template <class In, class Out>
				void MapperQuadraticTetrahedronSetTopology<In,Out>::CalculateSurfaceNormal(unsigned int triangleId, sofa::defaulttype::Vector3& localSurfaceCoords, sofa::defaulttype::Vector3 & normal)
				{
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
					sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(triangleId);

					double normals[18];

					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							normals[6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
						}
					}


					CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), normal.ptr(),3,3,&normals[0]);
					normal.normalize();
				}

			template <class In, class Out>
				void MapperQuadraticTetrahedronSetTopology<In,Out>::AddNeighboursToRecursion(RecursiveMappingData tempData,  std::map<unsigned int, RecursiveMappingData> & elementsToMap,  std::map<unsigned int, RecursiveMappingData> & elementsAlreadyMapped)
			{
				//start recursion on neighbour triangles:
				//unsigned int currentEdgeIndex;
				sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));


				const sofa::component::topology::TriangleSetTopologyContainer::EdgesInTriangle currentEdges = outTopology->getEdgesInTriangle(tempData.startOutElement);
				unsigned int nextOutElementIndex;
				typedef typename std::map<unsigned int,RecursiveMappingData>::iterator IteratorType ;
				IteratorType mapIterator;


				for(unsigned int edgeIter = 0; edgeIter<3; edgeIter++)
				{
					unsigned int currentEdgeGlobalId = currentEdges[edgeIter];
					//get surface quadratic triangles around edge
					sofa::component::topology::MeshTopology::TrianglesAroundEdge triangles = outTopology->getTrianglesAroundEdge(currentEdgeGlobalId );
					sofa::helper::vector<unsigned int> currentTriangleIdVec;

					if(triangles.size() != 2) //this is indeed possible!!!!!
					{
						for(unsigned int iterTriangles=0; iterTriangles<triangles.size();iterTriangles++)
						{
							if(triangles[iterTriangles] != tempData.startOutElement)
								currentTriangleIdVec.push_back(triangles[iterTriangles]);
						}
					}

					else
					{
						//one triangle is current triangle -> neglect
						if(triangles[0] == tempData.startOutElement)
						{
							currentTriangleIdVec.push_back(triangles[1]);
						}
						else
						{
							currentTriangleIdVec.push_back(triangles[0]);
						}
					}

					for(unsigned int iterVec=0; iterVec<currentTriangleIdVec.size();iterVec++)
					{
						nextOutElementIndex = currentTriangleIdVec[iterVec];
						mapIterator=elementsAlreadyMapped.find(nextOutElementIndex);

						if(mapIterator == elementsAlreadyMapped.end() )
						{
							mapIterator=elementsToMap.find(nextOutElementIndex);

							if(mapIterator == elementsToMap.end())
							{
								RecursiveMappingData currentTempData;
								currentTempData.startCoefs = tempData.startCoefs;
								currentTempData.startInElement = tempData.startInElement;
								currentTempData.startOutElement = nextOutElementIndex;
								currentTempData.distance = tempData.distance;
								currentTempData.error = tempData.error;
								elementsToMap[nextOutElementIndex]=currentTempData;
//								if(tempData.startOutElement == 9927)
//									std::cout<<"Element 9927 has been added To pingmap \n \n \n";
							}
							else
							{
								RecursiveMappingData currentTempData;
								currentTempData = mapIterator->second;

								if(currentTempData.distance>tempData.distance)
								{
									currentTempData.startCoefs = tempData.startCoefs;
									currentTempData.startInElement = tempData.startInElement;
									currentTempData.startOutElement = nextOutElementIndex;
									currentTempData.distance = tempData.distance;
									currentTempData.error = tempData.error;
									elementsToMap[nextOutElementIndex]=currentTempData;
//									if(tempData.startOutElement == 9927)
//										std::cout<<"Element 9927 has been changed in pingmap \n \n \n";
								}
							}

						}
					}
				}

			}

			template <class In, class Out>
				bool MapperQuadraticTetrahedronSetTopology<In,Out>::MapElement(unsigned int startOutElement, unsigned int &currentInElement, double errorThreshold, double distanceThreshold, sofa::defaulttype::Vector3 &currentCoefs, const typename In::VecCoord& in, const typename Out::VecCoord& out, std::set<unsigned int>  &trianglesMapped, std::map<unsigned int,unsigned int> &pointsMapped, double & currentError, double & currentDistance,unsigned int printFlag, unsigned int pointIndexDebugInformation)
			{
				//just for debug
//				Vector3 testOutPos;
//				testOutPos = out[3938];//Out::getCPos(out[currentNodeId]);
//				std::cout<<"FIND BUG current out pos "<<testOutPos<<", currentoutsize"<<out.size()<<"\n";

				double currentAngle;

				std::set<unsigned int>::iterator iteratorTriangles;
				iteratorTriangles=trianglesMapped.find(startOutElement);

				if(!(iteratorTriangles == trianglesMapped.end()))
					return false;
				//
				//bool printDebugOutput = false;
				//

				sofa::component::topology::QuadraticMeshTopology* inTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
				sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));

				//loop over all nodes in triangle
				sofa::component::topology::TriangleSetTopologyContainer::Triangle currentTriangle = outTopology->getTriangle(startOutElement);
				unsigned int currentNodeId;
				std::map<unsigned int,unsigned int>::iterator itt;
				Vector3 currentOutPos;

				//Vector3 currentInCoefs;
				unsigned int startInElement = currentInElement;
				//double currentDistance;
				//unsigned int tetraId;
				sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron  currentTrianglesInTetrahedron;
				//Vector3 currentVolumeCoefs;

				vtkIdType currentVTKTriangle[3];

				//inTopology->

				//double distance, error, angle;
				Vector3 differenceVector;
				bool printDebugOutputRecursion;
				bool saveDebugOutputRecursion = false;
				bool entireTriangleMapped = true;

				for(int i=0;i<3;i++)
				{
					currentNodeId = currentTriangle[i];
					itt=pointsMapped.find(currentNodeId);

//					if(currentNodeId == 190)
//							std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
//
//					if(startOutElement == 394)
//						std::cout<<"Element number 394 is "<<currentTriangle<<"\n";

					if(itt == pointsMapped.end() )
					{

						//for each node: recursively find currentInElement, currentInCoefs
						currentOutPos = out[currentNodeId];//Out::getCPos(out[currentNodeId]);
//						std::cout<<"current out pos "<<currentOutPos<<", currentoutsize"<<out.size()<<", currentNodeId"<<currentNodeId<<"\n";
						std::set<unsigned int> tetrasVisited;
						std::set<unsigned int> trianglesVisited;
//						std::cout<<"Start finding closest point on surface with element "<<startInElement<<", current out pos "<<currentOutPos;
//						std::cout<<", currentCoefs:"<<currentCoefs<<"\n";
//						if(currentNodeId == 190)
//							std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
						if(pointIndexDebugInformation == currentNodeId)
							std::cout<<"CurrentNodeId "<<currentNodeId<<", pointIndex"<<pointIndexDebugInformation<<"\n";

						if((pointIndexDebugInformation != 0) &&(currentNodeId == pointIndexDebugInformation))
						{
							std::cout<<"\n Print Debug Output \n";
							printDebugOutputRecursion = true;
							saveDebugOutputRecursion = true;

							double currentVTKPoint[3];
							currentVTKPoint[0] = currentOutPos[0];
							currentVTKPoint[1] = currentOutPos[1];
							currentVTKPoint[2] = currentOutPos[2];

							vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
							m_VectorsOptiDebug->InsertNextTuple3(0,0,0);
							m_ScalarsOptiDebug->InsertNextTuple1(pIndex);

							//std::cout<<"Current node id: "<<currentNodeId<<" ,current out id:"<<i<<", current distance:"<<distance<<"\n";


						}
						else
							printDebugOutputRecursion = false;


						currentCoefs[0] = 0.3;
						currentCoefs[1] = 0.3;
						currentCoefs[2] = 0.3;
						//std::cout<<"Current startInElement:"<<startInElement<<"\n";
						//std::cout<<"Start closes point recursion!\n";
						bool isOnSurface = RecursiveClosestPoint(startInElement, 1e10, currentOutPos, in, currentCoefs, currentInElement, currentDistance,  tetrasVisited, trianglesVisited, 0);//printDebugOutputRecursion);

//						bool isOnSurface = true;
//						if(inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(tempTriangleId).size() != 1)
//						{
//							isOnSurface = false;
//						}

						//std::cout<<"Current in element after closest point iteration:"<<currentInElement<<"\n";


						//RecursiveClosestPointOnSurfaceTriangle(startInElement, 1e10, currentOutPos, in, currentCoefs, currentInElement, currentDistance,  trianglesVisited, printDebugOutputRecursion);

//						if((!isOnSurface) && (currentDistance > TOLERANCE))
//						{
//							std::cout<<"Error, point not on surface, but distance is "<<currentDistance<<", try to correct it";
//
//							//assume that point is edge, test this!
//							int edgeId = inTopology->getQuadraticEdgeInInQuadraticTetrahedron(currentInElement, currentCoefs[0],currentCoefs[1],currentCoefs[2]);
//							if (edgeId == -1)
//								std::cout<<"Coords not on edge, but should be be, pointId: "<<currentNodeId<<" currentInElement "<<currentInElement <<" currentCoefs "<<currentCoefs<< " currentTetra "<<inTopology->getQuadTetrahedron(currentInElement)<< "!!\n";
//
//							const sofa::component::topology::QuadraticMeshTopology::QuadraticEdge theEdge = inTopology->getQuadraticEdge(edgeId);
//							//get the two surfaces around edge, grab first one
//							const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex surfaceTriangles = inTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdge(edgeId);
//							if(surfaceTriangles.size() == 0)
//								std::cout<<"\n No surface triangles around edge, but should be, pointId: "<<currentNodeId<<" currentInElement "<<currentInElement <<" currentEdge "<<theEdge<<" currentCoefs "<<currentCoefs<< " currentTetra "<<inTopology->getQuadTetrahedron(currentInElement)<< "!!\n";
//
//							unsigned int surfaceTriId = surfaceTriangles[0];
//
//							//set the coords to 0.3, 0.3
//							std::set<unsigned int> trianglesVisited3;
//							unsigned int currentInTriangle;
//
//							Vector3 currentTriangleCoefs;
//							currentTriangleCoefs[0] = 0.3;
//							currentTriangleCoefs[1] = 0.3;
//
//							//start another surface recursion
//							RecursiveClosestPointOnSurfaceTriangle(surfaceTriId, 1e10, currentOutPos, in, currentTriangleCoefs, currentInTriangle, currentDistance,  trianglesVisited3, 0);
//
//							currentInElement = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInTriangle)[0];
//							currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement);
//							int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInTriangle);
//							//std::cout<<"Face nr "<<faceNr<<"\n";
//							LocalSurfaceCoordsToLocalVolumeCoords(currentTriangleCoefs.ptr(), faceNr, currentCoefs.ptr());
//
//
//
//							std::cout<<", corrected distance is "<<currentDistance<<"\n";
//						}
						//check if outside
						//calculate normal
						Vector3 normal, position, direction;
//						CalculateSurfaceNormal(currentInElement, currentCoefs,normal);
//						CalculateSurfaceGlobalCoords(currentInElement, currentCoefs,in,position);
//
//						direction = currentOutPos-position;
//
//						if((pointIndexDebugInformation != 0) &&(currentNodeId == pointIndexDebugInformation))
//						{
//								std::cout<<"---------------------------------------------------------\n";
//								std::cout<<"Closest point found on inElement"<<currentInElement<<", currentDistance"<<currentDistance<<", localcoords "<<currentCoefs<< "\n";
//								std::cout<<"OutPos "<<position<<"\n";
//						}


						bool isOutside = false;

						bool verbose = false;

						if(currentDistance < distanceThreshold)
						{
//
//							if((direction*normal) < 0)
//							{
//								isOutside = false;
//
//								//get attached quadratic tetrahedron
//								tetraId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInElement)[0];
//								currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//								int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInElement);
//								LocalSurfaceCoordsToLocalVolumeCoords(currentCoefs.ptr(), faceNr, currentVolumeCoefs.ptr());
//
//								FindClosestPointInTetrahedron(tetraId, currentOutPos,  in, currentVolumeCoefs);
//
//							}
//							else
//							{
							double theFinalError = currentDistance;

								if(currentDistance>TOLERANCE)
								{
									isOutside = true;
									//optimize normals
									std::vector<unsigned int>  localFaceNumbers;
									std::vector<sofa::defaulttype::Vector3> localSurfaceCoordsVec;
									Vector3 localSurfaceCoords;
									bool isOnSurface = LocalVolumeCoordsToLocalSurfaceCoords(currentCoefs.ptr(), localFaceNumbers, localSurfaceCoordsVec);

//									if(!isOnSurface)
//									{
////										std::cout<<"Error, distance too high, but closest point not on surface\n";
////										std::cout<<"CurrentInElementId: "<<currentInElement<<", currentInElement:"<<inTopology->getQuadTetrahedra(currentInElement)<<", currentCoefs "<<currentCoefs<<", currentFaceNumbers"<<localFaceNumbers[0]<<" "<<localFaceNumbers[1]<<" "<<localFaceNumbers[2]<<"\n";
//
//
//								}



									unsigned int nextStepInElement;
									Vector3 currentTriangleCoefs;

									bool surfaceTriangleFound = false;
									if(localFaceNumbers.size() == 1)
									{
										nextStepInElement = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement) [localFaceNumbers[0]];
										//std::cout<<"nextstepInelement "<<nextStepInElement<<"\n";
										currentTriangleCoefs = localSurfaceCoordsVec[localFaceNumbers[0]];
										surfaceTriangleFound = true;
									}
									else
									{

										for(unsigned int faceIter = 0; faceIter<localFaceNumbers.size(); faceIter++)
										{
											unsigned int tempTriangleId = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement) [localFaceNumbers[faceIter]];
											if(inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(tempTriangleId).size() == 1)
											{
												nextStepInElement = tempTriangleId;
												currentTriangleCoefs = localSurfaceCoordsVec[localFaceNumbers[faceIter]];
												surfaceTriangleFound = true;
											//	std::cout<<"nextstepInelement "<<nextStepInElement<<"\n";

											}
										}

									}

									if(!surfaceTriangleFound)
									{
										//std::cout<<"TriangleNotSurfaceTriangle "<<"facenumbers "<<localFaceNumbers[0]<< " "<<localFaceNumbers[1]<<" "<<localFaceNumbers[2]<<" "<<"nextstepInelement "<<nextStepInElement<<"sizes "<<inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement) [localFaceNumbers[0]]).size()<< " "<< inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement) [localFaceNumbers[1]]).size()<<" distance "<< currentDistance<<"\n";
										//std::cout<<"Error, point not on surface, but distance is "<<currentDistance<<", try to correct it";

										//assume that point is edge, test this!
										int edgeId = inTopology->getSurfaceQuadraticEdgeInInQuadraticTetrahedron(currentInElement, currentCoefs[0],currentCoefs[1],currentCoefs[2]);
										unsigned int surfaceTriId;


										if (edgeId == -1)
										{
//											std::cout<<"Coords not on edge, but should be be, pointId: "<<currentNodeId<<" currentInElement "<<currentInElement <<" currentCoefs "<<currentCoefs<< " currentTetra "<<inTopology->getQuadTetrahedron(currentInElement)<< "!!\n";
											//coords are on a node -> check this
											int nodeId = inTopology->getVertexIndexInTetrahedron(currentCoefs[0],currentCoefs[1],currentCoefs[2]);

											//find surface triangles around node
											if(nodeId == -1)
												std::cout<<"Coords neither on edge nor on surface node, but should be be, pointId: "<<currentNodeId<<" currentInElement "<<currentInElement <<" currentCoefs "<<currentCoefs<< " currentTetra "<<inTopology->getQuadTetrahedron(currentInElement)<< "!!\n";

											//check if there is a surface triangle
											int globalNodeId = inTopology->getQuadTetrahedron(currentInElement) [nodeId];
//											std::cout<<"Looking for surface triangles around node "<<globalNodeId<<"\n";

											surfaceTriId = inTopology->getSurfaceQuadraticTrianglesAroundNode(globalNodeId) [0];

											currentTriangleCoefs[0] = 0.3;
											currentTriangleCoefs[1] = 0.3;

											std::set<unsigned int> trianglesVisited3;
											unsigned int currentInTriangle;

											//start another surface recursion
											RecursiveClosestPointOnSurfaceTriangle(surfaceTriId, 1e10, currentOutPos, in, currentTriangleCoefs, currentInTriangle, currentDistance,  trianglesVisited3, 0);

											nextStepInElement = currentInTriangle;

											currentInElement = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInTriangle)[0];
											currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement);
											int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInTriangle);
											//std::cout<<"Face nr "<<faceNr<<"\n";
											LocalSurfaceCoordsToLocalVolumeCoords(currentTriangleCoefs.ptr(), faceNr, currentCoefs.ptr());

											//std::cout<<", corrected distance is "<<currentDistance<<"\n";


										}
										else
										{
											const sofa::component::topology::QuadraticMeshTopology::QuadraticEdge theEdge = inTopology->getQuadraticEdge(edgeId);
											//get the two surfaces around edge, grab first one
											const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex surfaceTriangles = inTopology->getSurfaceQuadraticTrianglesAroundQuadraticEdge(edgeId);
											if(surfaceTriangles.size() == 0)
												std::cout<<"\n No surface triangles around edge, but should be, pointId: "<<currentNodeId<<" currentInElement "<<currentInElement <<" currentEdge "<<theEdge<<" currentCoefs "<<currentCoefs<< " currentTetra "<<inTopology->getQuadTetrahedron(currentInElement)<< "!!\n";

											surfaceTriId = surfaceTriangles[0];

											//Vector3 currentTriangleCoefs;
											currentTriangleCoefs[0] = 0.3;
											currentTriangleCoefs[1] = 0.3;

											//set the coords to 0.3, 0.3
											std::set<unsigned int> trianglesVisited3;
											unsigned int currentInTriangle;

											//start another surface recursion
											RecursiveClosestPointOnSurfaceTriangle(surfaceTriId, 1e10, currentOutPos, in, currentTriangleCoefs, currentInTriangle, currentDistance,  trianglesVisited3, 0);

											nextStepInElement = currentInTriangle;

											currentInElement = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInTriangle)[0];
											currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement);
											int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInTriangle);
											//std::cout<<"Face nr "<<faceNr<<"\n";
											LocalSurfaceCoordsToLocalVolumeCoords(currentTriangleCoefs.ptr(), faceNr, currentCoefs.ptr());

											//std::cout<<", corrected distance is "<<currentDistance<<"\n";

										}



									}

									std::set<unsigned int> trianglesVisited2;
									unsigned int currentInTriangle;

									//calculate currenterror
									Vector3 currentErrorNormal, currentErrorSurfacePos, currentErrorPos;
									CalculateSurfaceNormal(nextStepInElement, currentTriangleCoefs, currentErrorNormal);
									CalculateSurfaceGlobalCoords(nextStepInElement, currentTriangleCoefs, in, currentErrorSurfacePos);

									currentErrorPos = currentErrorSurfacePos + currentDistance * currentErrorNormal;

									double theError = (currentOutPos-currentErrorPos).norm();



									currentTriangleCoefs[0] = 0.3;
									currentTriangleCoefs[1] = 0.3;
									if(printDebugOutputRecursion)
										std::cout<<"\n Correcting normal for pointid:"<<currentNodeId<<", currently mapped to tetra "<<currentInElement<<" with error: "<<theError<<"\n";

									RecursiveFindBestNormalFitOnSurfaceTriangle(nextStepInElement, currentDistance, currentOutPos, in, currentTriangleCoefs, currentInTriangle, currentDistance, currentError, currentAngle,  trianglesVisited2, printDebugOutputRecursion);


									CalculateSurfaceNormal(currentInTriangle, currentTriangleCoefs, currentErrorNormal);
									CalculateSurfaceNormal(currentInTriangle, currentTriangleCoefs, normal);
									CalculateSurfaceGlobalCoords(currentInTriangle, currentTriangleCoefs, in, currentErrorSurfacePos);
									CalculateSurfaceGlobalCoords(currentInTriangle, currentTriangleCoefs, in, position);

									currentErrorPos = currentErrorSurfacePos + currentDistance * currentErrorNormal;

									theFinalError = (currentOutPos-currentErrorPos).norm();


//									if(theFinalError > 5)
//									{
//										verbose = true;
//										std::cout<<"Current error is "<<theError<<", corrected Error is "<<theFinalError<<", former coefs: "<<currentCoefs<<", corrected coefs";
//									}

									currentInElement = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInTriangle)[0];
									currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentInElement);
									int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInTriangle);
									//std::cout<<"Face nr "<<faceNr<<"\n";
									LocalSurfaceCoordsToLocalVolumeCoords(currentTriangleCoefs.ptr(), faceNr, currentCoefs.ptr());
									//FindBestNormalFitOnSurfaceTriangle(currentInElement, currentOutPos,  in, currentCoefs, distance,  error, angle,  differenceVector);

									if(printDebugOutputRecursion)
									{
											std::cout<<"\n Normal for pointid:"<<currentNodeId<<"corrected, finally mapped to tetra "<<currentInElement<<"with the final error: "<<theFinalError<<"\n";
											std::cout<<"FinalPos on surface: "<<currentErrorSurfacePos<<" with current normal "<<currentErrorNormal<<" and current distance "<<currentDistance<<"\n";
											const sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle printTri = inTopology->getQuadraticTriangleVTK(currentInTriangle);
											std::cout<<"current points in triangle "<< printTri <<", current tri coords, "<<currentTriangleCoefs<<"\n";
											std::cout<<"normals in tetra:\n";
											for(int p=0; p<6; p++)
											{
												std::cout<< this->m_NodeNormals[printTri[p]]<<"\n";
											}
									}

//									if(theFinalError > 5)
//										std::cout<<currentCoefs<<", currentTetra is "<<currentInElement<<" for current point nr. "<<currentNodeId<<" \n";

								}
								else
									isOutside = false;

								//std::cout<<"Current in element after additional surface recursion:"<<currentInElement<<"\n";

								//get attached quadratic tetrahedron


//							}

							if(theFinalError<errorThreshold)
							{

//							if(theFinalError>0.006)
//								std::cout<<"Final error is too large:"<< theFinalError<<" ,coefs "<<currentCoefs<<", currentInElementID "<<currentInElement<<", currentOutPtId: " <<currentNodeId <<" \n";

//							tetraId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInElement)[0];
//							currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//							int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInElement);
//							LocalSurfaceCoordsToLocalVolumeCoords(currentCoefs.ptr(), faceNr, currentVolumeCoefs.ptr());

							if(currentInElement == -1)
								std::cout<<"CurrentInElement is -1, should not happen!!\n \n \n";

							if(printDebugOutputRecursion)
								std::cout<<"Addd point "<<currentNodeId<<" to element "<<currentInElement<<" with error "<<currentError<<" and distance "<<currentDistance<<" and isOutside: "<<isOutside<<"\n";


							normal.normalize();

							addPointInQuadTetra(currentNodeId, currentInElement, currentCoefs.ptr(), currentDistance, isOutside, normal.ptr() ,position.ptr(), currentOutPos.ptr());

							if(verbose)
								std::cout<<"Saved coefs "<<currentCoefs<<", currentInElementID "<<currentInElement<<", currentOutPtId: " <<currentNodeId <<" \n";

							//add node to points mapped
							pointsMapped[currentNodeId] = m_MappingPointCounter;
							m_MappingPointCounter++;

							if(printFlag)
							{
								const sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra quadraticTetrahedra = inTopology->getQuadTetrahedra();
								const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[currentInElement];
								//construct nodes
								double nodes[30];

								for(int iteri=0; iteri<3; iteri++)
								{
									for(int p=0; p<10; p++)
									{
										nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
									}
								}

								Vector3 currentPos;
								CalculateGlobalCoords(currentCoefs.ptr(), currentPos.ptr(), 3, 3, &nodes[0]);

								if((pointIndexDebugInformation != 0) &&(currentNodeId == pointIndexDebugInformation))
								{
										std::cout<<"\n ReferencePos "<<currentOutPos<<",OutPos on Surface "<<currentPos;
								}

								if(isOutside)
								{
									//construct normals
									double normals[30];

									for(int iteri=0; iteri<3; iteri++)
									{
//										if((pointIndexDebugInformation != 0) &&(currentNodeId == pointIndexDebugInformation))
//										{
//											normals[10*iteri+0] = (this->m_NodeNormals[quadTetra[0]])[iteri];
//											normals[10*iteri+1] = (this->m_NodeNormals[quadTetra[1]])[iteri];
//											normals[10*iteri+2] = 0;
//											normals[10*iteri+3] = (this->m_NodeNormals[quadTetra[3]])[iteri];
//											normals[10*iteri+4] = (this->m_NodeNormals[quadTetra[4]])[iteri];
//											normals[10*iteri+5] = 0;
//											normals[10*iteri+6] = 0;
//											normals[10*iteri+7] = (this->m_NodeNormals[quadTetra[7]])[iteri];
//											normals[10*iteri+8] = (this->m_NodeNormals[quadTetra[8]])[iteri];
//											normals[10*iteri+9] = 0;
//
//										}
//										else
//										{
											for(int p=0; p<10; p++)
											{
												normals[10*iteri+p] = (this->m_NodeNormals[quadTetra[p]])[iteri];
											}
//										}
									}
									Vector3 currentNormal;
									CalculateGlobalCoords(currentCoefs.ptr(), currentNormal.ptr(), 3, 3, &normals[0]);
									currentNormal.normalize();
									currentPos +=  currentNormal*currentDistance;

									if((pointIndexDebugInformation != 0) &&(currentNodeId == pointIndexDebugInformation))
									{
											std::cout<<" normal on surface "<<currentNormal<<" currentDistance "<<currentDistance<<" and current error "<<(currentOutPos-currentPos).norm()<< "\n";
											std::cout<<"current points in tetra "<<quadTetra <<", current coords, "<<currentCoefs<<"\n";
											std::cout<<"normals in tetra:\n";
											for(int p=0; p<10; p++)
											{
												std::cout<< this->m_NodeNormals[quadTetra[p]]<<"\n";
											}
									}
								}





								currentVTKTriangle[i] = m_PointContainerRecursiveMapping->InsertNextPoint(currentPos[0], currentPos[1], currentPos[2] );

							}
							}
							else
							{
								entireTriangleMapped = false;
							}
						}
						else
							entireTriangleMapped = false;

					}
					else
					{
						if(printFlag)
							currentVTKTriangle[i] =(*itt).second;
					}
				}

				if(entireTriangleMapped)
				{
					trianglesMapped.insert(startOutElement);
					//add to triangle container
					this->m_MappingPrintCounter = m_TriangleContainerRecursiveMapping->InsertNextCell(3,currentVTKTriangle);

					//print debug output if necessary
					if(printFlag)
					{
						if((this->m_MappingPrintCounter % printFlag) == 1)
						{
							std::cout<<"Write debug data number "<< this->m_MappingPrintCounter<<"\n";
							std::string filenameRecursiveMappingDebug("/opt/Ergebnisse/tmp/RecursiveMappingDebugData");
							std::ostringstream oss;
							oss << "_"<< this->m_MappingPrintCounter;
							filenameRecursiveMappingDebug += oss.str() + ".vtk";

							vtkUnstructuredGrid* myGrid = vtkUnstructuredGrid::New();
							myGrid->SetPoints(m_PointContainerRecursiveMapping);
							myGrid->SetCells(VTK_TRIANGLE, m_TriangleContainerRecursiveMapping);

							vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
							writer->SetInput(myGrid);
							writer->SetFileName(filenameRecursiveMappingDebug.c_str());
							writer->Write();

							myGrid->Delete();
							writer->Delete();

						}

					}

					if(saveDebugOutputRecursion)
					{
							std::cout<<"Saving Debug information ... \n";
							//save the vtk debug output
							const char* filenamePointDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/PointDataDebug.vtk";
							const char* filenameTriangleDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/TriangleDataDebug.vtk";

							//std::cout<<"PointNumber: "<<i<<", Updated distance:"<<distance<<", final tetraId: "<<index-c1<<" ,coefs:"<<coefs<<"Final tetra:"<<theTopology->getQuadTetrahedra(index-c1)<<"\n";

							vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
							myGridTopoDebug->SetPoints(m_PointContainerDebugTriangles);
							myGridTopoDebug->SetCells(VTK_TRIANGLE, m_TriangleContainerDebugTriangles);
							myGridTopoDebug->GetCellData()->SetScalars(m_ScalarsDebugTriangles);

							vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();
							writerTopoDebug->SetInput(myGridTopoDebug);
							writerTopoDebug->SetFileName(filenameTriangleDebug);
							writerTopoDebug->Write();

							myGridTopoDebug->Delete();
							writerTopoDebug->Delete();

							vtkPolyData* myGrid2 = vtkPolyData::New();
							myGrid2->SetPoints(m_PointContainerDebugPoints);
							myGrid2->GetPointData()->SetVectors(m_VectorsOptiDebug);
							myGrid2->GetPointData()->SetScalars(m_ScalarsOptiDebug);

							vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
							vtkSmartPointer<vtkVertexGlyphFilter>::New();
							vertexGlyphFilter->AddInput(myGrid2);
							vertexGlyphFilter->Update();

							vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();

							writer2->SetInput(vertexGlyphFilter->GetOutput());
							writer2->SetFileName(filenamePointDebug);
							writer2->Write();

							myGrid2->Delete();
							writer2->Delete();


					}
					return true;
				}
				else
					return false;




			}

			template <class In, class Out>
				void MapperQuadraticTetrahedronSetTopology<In,Out>::RecursivelyMapElement(unsigned int startOutElement, unsigned int startInElement, sofa::defaulttype::Vector3 currentCoefs, const typename In::VecCoord& in, const typename Out::VecCoord& out, std::set<unsigned int> &trianglesMapped, std::map<unsigned int,unsigned int> &pointsMapped, unsigned int printFlag)
			{
				//just for debug
				Vector3 testOutPos;
				testOutPos = out[3938];//Out::getCPos(out[currentNodeId]);
				std::cout<<"FIND BUG current out pos "<<testOutPos<<", currentoutsize"<<out.size()<<"\n";

				std::set<unsigned int>::iterator iteratorTriangles;
				iteratorTriangles=trianglesMapped.find(startOutElement);

				if(!(iteratorTriangles == trianglesMapped.end()))
					return;
				//
				bool printDebugOutput = false;
				//

				sofa::component::topology::QuadraticMeshTopology* inTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
				sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));

				//loop over all nodes in triangle
				sofa::component::topology::TriangleSetTopologyContainer::Triangle currentTriangle = outTopology->getTriangle(startOutElement);
				unsigned int currentNodeId;
				std::map<unsigned int,unsigned int>::iterator itt;
				Vector3 currentOutPos;

				//Vector3 currentInCoefs;
				unsigned int currentInElement = startInElement;
				double currentDistance;
				unsigned int tetraId;
				sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron  currentTrianglesInTetrahedron;
				Vector3 currentVolumeCoefs;

				vtkIdType currentVTKTriangle[3];

				//inTopology->

				//double distance, error, angle;
				Vector3 differenceVector;

				for(int i=0;i<3;i++)
				{
					currentNodeId = currentTriangle[i];
					itt=pointsMapped.find(currentNodeId);

					if(itt == pointsMapped.end() )
					{
						//for each node: recursively find currentInElement, currentInCoefs
						currentOutPos = out[currentNodeId];//Out::getCPos(out[currentNodeId]);
						std::cout<<"current out pos "<<currentOutPos<<", currentoutsize"<<out.size()<<", currentNodeId"<<currentNodeId<<"\n";
						std::set<unsigned int> trianglesVisited;
						std::cout<<"Start finding closest point on surface with element "<<startInElement<<", current out pos "<<currentOutPos;
						std::cout<<", currentCoefs:"<<currentCoefs<<"\n";
						RecursiveClosestPointOnSurfaceTriangle(startInElement, 1e10, currentOutPos, in, currentCoefs, currentInElement, currentDistance,  trianglesVisited, printDebugOutput);
						//std::cout<<"Closest point found on inElement"<<currentInElement<<"\n";

						//check if outside
						//calculate normal
						Vector3 normal, position, direction;
						CalculateSurfaceNormal(currentInElement, currentCoefs,normal);
						CalculateSurfaceGlobalCoords(currentInElement, currentCoefs,in,position);

						direction = currentOutPos-position;

						bool isOutside = false;
//						if((direction*normal) < 0)
//						{
//							isOutside = false;
//
//							//get attached quadratic tetrahedron
//							tetraId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInElement)[0];
//							currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//							int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInElement);
//							LocalSurfaceCoordsToLocalVolumeCoords(currentCoefs.ptr(), faceNr, currentVolumeCoefs.ptr());
//
//							FindClosestPointInTetrahedron(tetraId, currentOutPos,  in, currentVolumeCoefs);
//
//						}
//						else
//						{
//							if(currentDistance>TOLERANCE)
//							{
//								isOutside = true;
//								//optimize normals
//								FindBestNormalFitOnSurfaceTriangle(currentInElement, currentOutPos,  in, currentCoefs, distance,  error, angle,  differenceVector);
//							}
//							else
//								isOutside = false;
//
//							//get attached quadratic tetrahedron
//							tetraId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInElement)[0];
//							currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//							int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInElement);
//							LocalSurfaceCoordsToLocalVolumeCoords(currentCoefs.ptr(), faceNr, currentVolumeCoefs.ptr());
//
//						}

						tetraId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(currentInElement)[0];
						currentTrianglesInTetrahedron = inTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
						int faceNr = inTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, currentInElement);
						LocalSurfaceCoordsToLocalVolumeCoords(currentCoefs.ptr(), faceNr, currentVolumeCoefs.ptr());

						Vector3 normalCorrection;
//						addPointInQuadTetra( currentNodeId, tetraId, currentVolumeCoefs.ptr(), currentDistance, isOutside, normalCorrection.ptr() );

						//add node to points mapped
						pointsMapped[currentNodeId] = m_MappingPointCounter;
						m_MappingPointCounter++;

						if(printFlag)
						{
							const sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra quadraticTetrahedra = inTopology->getQuadTetrahedra();
							const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[tetraId];
							//construct nodes
							double nodes[30];

							for(int iteri=0; iteri<3; iteri++)
							{
								for(int p=0; p<10; p++)
								{
									nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
								}
							}

							Vector3 currentPos;
							CalculateGlobalCoords(currentVolumeCoefs.ptr(), currentPos.ptr(), 3, 3, &nodes[0]);

							if(isOutside)
							{
								//construct normals
								double normals[30];

								for(int iteri=0; iteri<3; iteri++)
								{
									for(int p=0; p<10; p++)
									{
										normals[10*iteri+p] = (this->m_NodeNormals[quadTetra[p]])[iteri];
									}
								}
								Vector3 currentNormal;
								CalculateGlobalCoords(currentVolumeCoefs.ptr(), currentNormal.ptr(), 3, 3, &normals[0]);
								currentPos +=  currentNormal*currentDistance;
							}

							currentVTKTriangle[i] = m_PointContainerRecursiveMapping->InsertNextPoint(currentPos[0], currentPos[1], currentPos[2] );

						}

					}
					else
					{
						if(printFlag)
							currentVTKTriangle[i] =(*itt).second;
					}
				}

				trianglesMapped.insert(startOutElement);
				//add to triangle container
				this->m_MappingPrintCounter = m_TriangleContainerRecursiveMapping->InsertNextCell(3,currentVTKTriangle);

				//print debug output if necessary
				if(printFlag)
				{
					if((this->m_MappingPrintCounter % printFlag) == 1)
					{
						std::cout<<"Write debug data number "<< this->m_MappingPrintCounter<<"\n";
						std::string filenameRecursiveMappingDebug("/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/DebugInformation/RecursiveMappingDebugData");
						std::ostringstream oss;
						oss << "_"<< this->m_MappingPrintCounter;
						filenameRecursiveMappingDebug += oss.str() + ".vtk";

						vtkUnstructuredGrid* myGrid = vtkUnstructuredGrid::New();
						myGrid->SetPoints(m_PointContainerRecursiveMapping);
						myGrid->SetCells(VTK_TRIANGLE, m_TriangleContainerRecursiveMapping);

						vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
						writer->SetInput(myGrid);
						writer->SetFileName(filenameRecursiveMappingDebug.c_str());
						writer->Write();

						myGrid->Delete();
						writer->Delete();

					}

				}

				//start recursion on neighbour triangles:
				//unsigned int currentEdgeIndex;
				const sofa::component::topology::TriangleSetTopologyContainer::EdgesInTriangle currentEdges = outTopology->getEdgesInTriangle(startOutElement);
				unsigned int nextOutElementIndex;

				for(unsigned int edgeIter = 0; edgeIter<3; edgeIter++)
				{
					sofa::component::topology::TriangleSetTopologyContainer::TrianglesAroundVertex triangles = outTopology->getTrianglesAroundEdge(currentEdges[edgeIter]);
					if(triangles.size() != 2)
						std::cout<<"Error, there must be two triangles around edge! \n";
					if(triangles[0] == startOutElement)
						nextOutElementIndex = triangles[1];
					else
						nextOutElementIndex = triangles[0];

					Vector3 newCoefs;
					newCoefs[0]=0.3;
					newCoefs[1]=0.3;
					newCoefs[2]=1;
					std::cout<<"Start another recursion with outElement = "<<nextOutElementIndex;
					std::cout<<", in Element="<<currentInElement;
					//std::cout<<", currentCoeffs="<<newCoefs;
					std::cout<<", derived from triangles:"<<triangles<<"\n";
					RecursivelyMapElement(nextOutElementIndex, currentInElement, newCoefs, in,  out, trianglesMapped, pointsMapped, printFlag);

				}




			}

			template <class In, class Out>
							double MapperQuadraticTetrahedronSetTopology<In,Out>::FindClosestPointInTetrahedron(unsigned int tetraId, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localVolumeCoords)
			{

				sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				const sofa::component::topology::QuadraticMeshTopology::QuadTetra  currentTetra = theTopology->getQuadTetrahedra(tetraId);

				double nodes[30];

				for(int iteri=0; iteri<3; iteri++)
				{
					for(int p=0; p<10; p++)
					{
						nodes[10*iteri+p] = (in[currentTetra[p]])[iteri];
					}
				}


				double lb[3];
				lb[0]=0.0;
				lb[1]=0.0;
				lb[2]=0.0;

				double ub[3];
				ub[0]=1.0;
				ub[1]=1.0;
				ub[2]=1.0;

				double C[3];
				C[0]=-1.0;
				C[1]=-1.0;
				C[2]=-1.0;

				double d[1];
				d[0] = -1.0;

				double info[LM_INFO_SZ];

				dlevmar_bleic_der(CalculateGlobalCoords, CalculateJacobian, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);

				Vector3 testPos;

				CalculateGlobalCoords(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);

				double distanceTemp = (referencePosition - testPos).norm();

				return distanceTemp;

			}

			template <class In, class Out>
							double MapperQuadraticTetrahedronSetTopology<In,Out>::FindClosesPointOnSurfaceTriangle(unsigned int currentTriId, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords)
			{
				sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(currentTriId);

				double nodesAndNormalsAndPos[18];

				for(int iteri=0; iteri<3; iteri++)
				{
					for(int p=0; p<6; p++)
					{
						nodesAndNormalsAndPos[6*iteri+p] = (in[currentTriangle[p]])[iteri];
					}
				}

				//v[2] is used as a flag as to if initial estimate should be used
				if(localSurfaceCoords[2] == 0)
				{
					localSurfaceCoords[0]= 0.3;
					localSurfaceCoords[1]= 0.3;
					localSurfaceCoords[2]= 0.0;
				}


				double lb[2];
				lb[0]=0.0;
				lb[1]=0.0;

				double ub[2];
				ub[0]=1.0;
				ub[1]=1.0;

				double C[2];
				C[0]=-1.0;
				C[1]=-1.0;


				double d[1];
				d[0] = -1.0;

				double info[LM_INFO_SZ];

				dlevmar_bleic_dif(CalculateGlobalCoordsSurface,  localSurfaceCoords.ptr(), referencePosition.ptr(), 2, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodesAndNormalsAndPos[0]);

				Vector3 testPos;

				CalculateGlobalCoordsSurface(localSurfaceCoords.ptr(), testPos.ptr(),3,3,&nodesAndNormalsAndPos[0]);

				double distanceTemp = (referencePosition - testPos).norm();

				//debug
				//std::cout<<"Current triangle id:"<<currentTriId<<", current triangle:"<<currentTriangle<<", current surface coords: "<<localSurfaceCoords<<", current distance:"<<distanceTemp<<"\n";
				return distanceTemp;



			}

			template <class In, class Out>
							bool MapperQuadraticTetrahedronSetTopology<In,Out>::LocalVolumeCoordsToLocalSurfaceCoords(double* localVolumeCoords, std::vector<unsigned int>&  localFaceNumbers, std::vector<sofa::defaulttype::Vector3>& localSurfaceCoordsVec)
			{
				int localFaceNr2;
				localFaceNumbers.clear();
				localSurfaceCoordsVec.clear();
				bool onSurface = false;

				if(localVolumeCoords[0]<=TOLERANCE)
				{
					localFaceNr2 = 0;
					localFaceNumbers.push_back(0);
					onSurface = true;

				}
				if (localVolumeCoords[1]<=TOLERANCE)
				{
					localFaceNr2 = 1;
					localFaceNumbers.push_back(1);
					onSurface = true;
				}
				if ( localVolumeCoords[2]<=TOLERANCE)
				{
					localFaceNr2 = 3;
					localFaceNumbers.push_back(3);
					onSurface = true;
				}
				if ((localVolumeCoords[0]+localVolumeCoords[1]+localVolumeCoords[2])>=(1-TOLERANCE))
				{
					localFaceNr2 = 2;
					localFaceNumbers.push_back(2);
					onSurface = true;
				}

				if(!onSurface)
				{
					//localFaceNr =4;
					return false;
				}

				Vector3 localSurfaceCoords;

				for(unsigned int faceIter=0; faceIter<localFaceNumbers.size(); faceIter++)
				{
					//localFaceNr = localFaceNr2;
					localFaceNr2 = localFaceNumbers[faceIter];

					//std::cout<<"local face no."<<localFaceNr2<<"\n";

					switch (localFaceNr2)
					{
					case 0:
						localSurfaceCoords[0] = localVolumeCoords[2];
						localSurfaceCoords[1] = localVolumeCoords[1];
						break;
					case 1:
						localSurfaceCoords[0] = localVolumeCoords[0];
						localSurfaceCoords[1] = localVolumeCoords[2];
						break;
					case 2:
						localSurfaceCoords[0] = localVolumeCoords[0];
						localSurfaceCoords[1] = localVolumeCoords[1];
						break;
					case 3:
						localSurfaceCoords[0] = localVolumeCoords[1];
						localSurfaceCoords[1] = localVolumeCoords[0];
						break;
					default:
						std::cout<<"Error, faceNr wrong\n";
						break;
					}

					localSurfaceCoordsVec.push_back(localSurfaceCoords);
				}

				return true;



			}

			template <class In, class Out>
							void MapperQuadraticTetrahedronSetTopology<In,Out>::LocalSurfaceCoordsToLocalEdgeCoord(double* localSurfaceCoords, int edgeNumber, double*  localEdgeCoord)
			{
//				int localEdgeNr2;
//
//				if(localSurfaceCoords[0]<=TOLERANCE)
//					localEdgeNr2 = 0;
//				else if (localSurfaceCoords[1]<=TOLERANCE)
//					localEdgeNr2 = 1;
//				else if ((localSurfaceCoords[0]+localSurfaceCoords[1])>=(1-TOLERANCE))
//					localEdgeNr2 = 2;
//				else
//					std::cout<<"Error: Local coords are not on edge";

				switch (edgeNumber)
				{
				case 0:
					localEdgeCoord[0] = localSurfaceCoords[0];
					break;
				case 1:
					localEdgeCoord[0] = 1-localSurfaceCoords[1];
					break;
				case 2:
					localEdgeCoord[0] = localSurfaceCoords[1];
					break;
				default:
					std::cout<<"Error, edgeNr wrong\n";
					break;
				}

			}

			template <class In, class Out>
							void MapperQuadraticTetrahedronSetTopology<In,Out>::LocalEdgeCoordToLocalSurfaceCoords(double localEdgeCoord, int  edgeNumber, double * localSurfaceCoords)
			{
				switch (edgeNumber)
				{
				case 0:
					localSurfaceCoords[0] = localEdgeCoord;
					localSurfaceCoords[1] = 0;
					break;
				case 1:
					localSurfaceCoords[0] = 0;
					localSurfaceCoords[1] = 1-localEdgeCoord;
					break;
				case 2:
					localSurfaceCoords[0] = 1-localEdgeCoord;
					localSurfaceCoords[1] = localEdgeCoord;
					break;
				default:
					std::cout<<"Error, edgeNr wrong\n";
					break;
				}
			}

			template <class In, class Out>
				double MapperQuadraticTetrahedronSetTopology<In,Out>::CalculateDistanceAndClosestPoint(double* localCoords, double* pos,double* lambda2,  double* referencePosition, double* nodes)
			{
				//first calculate globalCoords
					double fx = localCoords[0];
					double fy = localCoords[1];
					double fz = localCoords[2];

					double tempExp = 1-fx-fy-fz;
					sofa::defaulttype::Vector3 globalCoords;

					for(int i=0;i<3;i++)
					{
						globalCoords[i] = 0;

						globalCoords[i] += ((double*)nodes)[10*i+0]*(2*fx-1)*fx;
						globalCoords[i] += ((double*)nodes)[10*i+1]*(2*fy-1)*fy;
						globalCoords[i] += ((double*)nodes)[10*i+2]*(2*tempExp-1)*tempExp;
						globalCoords[i] += ((double*)nodes)[10*i+3]*(2*fz-1)*fz;
						globalCoords[i] += ((double*)nodes)[10*i+4]*4*fx*fy;
						globalCoords[i] += ((double*)nodes)[10*i+5]*4*tempExp*fy;
						globalCoords[i] += ((double*)nodes)[10*i+6]*4*tempExp*fx;
						globalCoords[i] += ((double*)nodes)[10*i+7]*4*fx*fz;
						globalCoords[i] += ((double*)nodes)[10*i+8]*4*fy*fz;
						globalCoords[i] += ((double*)nodes)[10*i+9]*4*tempExp*fz;

						//globalCoords[i]=10;
					}

					sofa::defaulttype::Vector3 normal;
					//then determine normal vector
					for(int i=0;i<3;i++)
					{
						normal[i] = 0;

						normal[i] += ((double*)nodes)[30+10*i+0]*(2*fx-1)*fx;
						normal[i] += ((double*)nodes)[30+10*i+1]*(2*fy-1)*fy;
						normal[i] += ((double*)nodes)[30+10*i+2]*(2*tempExp-1)*tempExp;
						normal[i] += ((double*)nodes)[30+10*i+3]*(2*fz-1)*fz;
						normal[i] += ((double*)nodes)[30+10*i+4]*4*fx*fy;
						normal[i] += ((double*)nodes)[30+10*i+5]*4*tempExp*fy;
						normal[i] += ((double*)nodes)[30+10*i+6]*4*tempExp*fx;
						normal[i] += ((double*)nodes)[30+10*i+7]*4*fx*fz;
						normal[i] += ((double*)nodes)[30+10*i+8]*4*fy*fz;
						normal[i] += ((double*)nodes)[30+10*i+9]*4*tempExp*fz;
					}



					normal.normalize();

					//calculate distance between straight line and point d=|(p-a) x u0|
					sofa::defaulttype::Vector3 posVec;
					posVec[0] = referencePosition[0];
					posVec[1] = referencePosition[1];
					posVec[2] = referencePosition[2];
					sofa::defaulttype::Vector3 tempVec;
					tempVec = posVec-globalCoords;

					tempVec = tempVec.cross(normal);

					double d = (tempVec).norm();

					tempVec = posVec-globalCoords;
					double lambda = tempVec*normal;

					double testD = (posVec-(globalCoords+normal*lambda)).norm();

					if(d-testD>1e-6)
						std::cout<<"Calculation of lambda is wrong \n";

					pos[0] = globalCoords[0]+lambda*normal[0];
					pos[1] = globalCoords[1]+lambda*normal[1];
					pos[2] = globalCoords[2]+lambda*normal[2];

//					std::cout<<"Distance method1: "<<d<<"\n";
//
//					double d2 = ((referencePosition[0]-globalCoords[0])*normal[1]- (referencePosition[1]-globalCoords[1])*normal[0])*((referencePosition[0]-globalCoords[0])*normal[1]- (referencePosition[1]-globalCoords[1])*normal[0]);
//					d2 +=((referencePosition[1]-globalCoords[1])*normal[2]- (referencePosition[2]-globalCoords[2])*normal[1])*((referencePosition[1]-globalCoords[1])*normal[2]- (referencePosition[2]-globalCoords[2])*normal[1]);
//					d2 +=((referencePosition[2]-globalCoords[2])*normal[0]- (referencePosition[0]-globalCoords[0])*normal[2])*((referencePosition[2]-globalCoords[2])*normal[0]- (referencePosition[0]-globalCoords[0])*normal[2]);
//					d2= d2/(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
//					d2 = helper::rsqrt(d2);
//
//					std::cout<<"Distance method2: "<<d2<<"\n";
					lambda2[0] = lambda;

					return d;
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::CalculateSurfaceNormal(int triangleNode, sofa::defaulttype::Vector3 & normal, double* nodes)
			{
				Vector3 dFdU, dFdV;

				GetQuadraticTriangleShapeFunctionDeriv(dFdU.ptr(), 0, triangleNode, nodes);
				GetQuadraticTriangleShapeFunctionDeriv(dFdV.ptr(), 1, triangleNode, nodes);

				//std::cout<<"Triangle surface DFdU "<< dFdU<<" dFdV "<< dFdV <<"\n";

				normal = dFdU.cross(dFdV);
				normal.normalize();
			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::CalculateSurfaceNormal(double* localCoords, sofa::defaulttype::Vector3 & normal, double* nodes, int faceNumber)
			{
				Mat<3,3,double> jacobian;
				CalculateJacobian(localCoords,jacobian.ptr(),3,3,nodes);

				double r = localCoords[0];
				double s = localCoords[1];
				double t = localCoords[2];

				Vector3 dU, dV;

				if(faceNumber==0)//(r<TOLERANCE) && (r<s) && (r<t))
				{
					dU[0] = 0;
					dU[1] = 0;
					dU[2] = 1;

					dV[0] = 0;
					dV[1] = 1;
					dV[2] = 0;
				}
				else if((faceNumber==1))//(s<TOLERANCE) && (s<r) && (s<t))
				{
					dU[0] = 1;
					dU[1] = 0;
					dU[2] = 0;

					dV[0] = 0;
					dV[1] = 0;
					dV[2] = 1;
				}
				else if((faceNumber==2))//(1-r-s-t)<TOLERANCE)
				{
					dU[0] = 1;
					dU[1] = 0;
					dU[2] = -1;

					dV[0] = 0;
					dV[1] = 1;
					dV[2] = -1;
				}
				else if((faceNumber==3))//(t<TOLERANCE) && (t<s) && (t<r))
				{
					dU[0] = 0;
					dU[1] = 1;
					dU[2] = 0;

					dV[0] = 1;
					dV[1] = 0;
					dV[2] = 0;
				}
				else
				{
					std::cout<<"Point is not on surface, (R,s,t)="<<r<<","<<s<<","<<t<<"\n";
				}

				Vector3 dFdU, dFdV;

				dFdU = jacobian*dU;
				dFdV = jacobian*dV;

				//std::cout<<"Tetra surface DFdU "<< dFdU<<" dFdV "<< dFdV <<"\n";

				normal = dFdU.cross(dFdV);
				normal.normalize();

			}

			template <class In, class Out>
			void MapperQuadraticTetrahedronSetTopology<In,Out>::init ( const typename Out::VecCoord& out, const typename In::VecCoord& in )
			{

				helper::system::thread::CTime* _timer = new helper::system::thread::CTime();
				double t0 = (double)_timer->getTime() ;

				sofa::helper::AdvancedTimer::stepBegin("QuadraticMappingStep");
				sofa::helper::AdvancedTimer::begin("QuadraticMapping");
		;


				int outside = 0;
//				updateJ = true;

				const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
				sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra dummyQuadTetras;
				sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra& quadraticTetrahedra = dummyQuadTetras;
				if((dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology)))
				{
					quadraticTetrahedra = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology))->getQuadTetrahedra();
				}

				std::cout<<"Number Of Quad Tetras: "<<quadraticTetrahedra.size()<<"\n";

#ifdef SOFA_NEW_HEXA
				const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
#else
				const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
#endif
				const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
				const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
				sofa::helper::vector<Matrix3> bases;
				sofa::helper::vector<Vector3> centers;
				//sofa::helper::vector<Vec<9,double> > boundingBoxes;

				if ( tetrahedra.empty() && cubes.empty() && quadraticTetrahedra.empty())
				{
//					if ( triangles.empty() && quads.empty() )
//					{
//						//no 3D elements, nor 2D elements -> map on 1D elements
//
//						const sofa::core::topology::BaseMeshTopology::SeqEdges& edges = this->fromTopology->getEdges();
//						if ( edges.empty() ) return;
//
//						clear1d ( out.size() );
//
//						sofa::helper::vector< SReal > lengthEdges;
//						sofa::helper::vector< Vector3 > unitaryVectors;
//
//						unsigned int e;
//						for ( e=0;e<edges.size(); e++ )
//						{
//							lengthEdges.push_back ( ( in[edges[e][1]]-in[edges[e][0]] ).norm() );
//
//							Vector3 V12 = ( in[edges[e][1]]-in[edges[e][0]] ); V12.normalize();
//							unitaryVectors.push_back ( V12 );
//						}
//
//						for ( unsigned int i=0;i<out.size();i++ )
//						{
//							SReal coef=0;
//							for ( e=0;e<edges.size(); e++ )
//							{
//								SReal lengthEdge = lengthEdges[e];
//								Vector3 V12 =unitaryVectors[e];
//
//								coef = ( V12 ) *Vector3 ( Out::getCPos(out[i])-in[edges[e][0]] ) /lengthEdge;
//								if ( coef >= 0 && coef <= 1 ) {addPointInLine ( e,&coef ); break;}
//
//							}
//							//If no good coefficient has been found, we add to the last element
//							if ( e == edges.size() ) addPointInLine ( edges.size()-1,&coef );
//
//						}
//					}
//					else
//					{
//						// no 3D elements -> map on 2D elements
//						clear2d ( out.size() ); // reserve space for 2D mapping
//						int c0 = triangles.size();
//						bases.resize ( triangles.size() +quads.size() );
//						centers.resize ( triangles.size() +quads.size() );
//						for ( unsigned int t = 0; t < triangles.size(); t++ )
//						{
//							Mat3x3d m,mt;
//							m[0] = in[triangles[t][1]]-in[triangles[t][0]];
//							m[1] = in[triangles[t][2]]-in[triangles[t][0]];
//							m[2] = cross ( m[0],m[1] );
//							mt.transpose ( m );
//							bases[t].invert ( mt );
//							centers[t] = ( in[triangles[t][0]]+in[triangles[t][1]]+in[triangles[t][2]] ) /3;
//						}
//						for ( unsigned int c = 0; c < quads.size(); c++ )
//						{
//							Mat3x3d m,mt;
//							m[0] = in[quads[c][1]]-in[quads[c][0]];
//							m[1] = in[quads[c][3]]-in[quads[c][0]];
//							m[2] = cross ( m[0],m[1] );
//							mt.transpose ( m );
//							bases[c0+c].invert ( mt );
//							centers[c0+c] = ( in[quads[c][0]]+in[quads[c][1]]+in[quads[c][2]]+in[quads[c][3]] ) *0.25;
//						}
//						for ( unsigned int i=0;i<out.size();i++ )
//						{
//							Vector3 pos = Out::getCPos(out[i]);
//							Vector3 coefs;
//							int index = -1;
//							double distance = 1e10;
//							for ( unsigned int t = 0; t < triangles.size(); t++ )
//							{
//								Vec3d v = bases[t] * ( pos - in[triangles[t][0]] );
//								double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( ( v[2]<0?-v[2]:v[2] )-0.01,v[0]+v[1]-1 ) );
//								if ( d>0 ) d = ( pos-centers[t] ).norm2();
//								if ( d<distance ) {coefs = v; distance = d; index = t;}
//							}
//							for ( unsigned int c = 0; c < quads.size(); c++ )
//							{
//								Vec3d v = bases[c0+c] * ( pos - in[quads[c][0]] );
//								double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( std::max ( v[1]-1,v[0]-1 ),std::max ( v[2]-0.01,-v[2]-0.01 ) ) );
//								if ( d>0 ) d = ( pos-centers[c0+c] ).norm2();
//								if ( d<distance ) {coefs = v; distance = d; index = c0+c;}
//							}
//							if ( distance>0 )
//							{
//								++outside;
//							}
//							if ( index < c0 )
//							addPointInTriangle ( index, coefs.ptr() );
//							else
//							addPointInQuad ( index-c0, coefs.ptr() );
//						}
//					}
				}
				else
				{
					clear3d ( out.size() ); // reserve space for 3D mapping
					int c0 = tetrahedra.size() + quadraticTetrahedra.size();
					int c1 = tetrahedra.size();
					bases.resize ( tetrahedra.size() +cubes.size()+quadraticTetrahedra.size() );
					centers.resize ( tetrahedra.size() +cubes.size() +quadraticTetrahedra.size());
					for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
					{
						Mat3x3d m,mt;
						m[0] = in[tetrahedra[t][1]]-in[tetrahedra[t][0]];
						m[1] = in[tetrahedra[t][2]]-in[tetrahedra[t][0]];
						m[2] = in[tetrahedra[t][3]]-in[tetrahedra[t][0]];
						mt.transpose ( m );
						bases[t].invert ( mt );
						centers[t] = ( in[tetrahedra[t][0]]+in[tetrahedra[t][1]]+in[tetrahedra[t][2]]+in[tetrahedra[t][3]] ) *0.25;
						//sout << "Tetra "<<t<<" center="<<centers[t]<<" base="<<m<<sendl;
					}

					//if there are quad tetras, initialize normals
					if(quadraticTetrahedra.size())
					{

						sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

						//get surface points
						vector<sofa::component::topology::QuadraticMeshTopology::PointID>  surfaceNodes = theTopology->getSurfaceNodes();
						sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles;
						unsigned int currentTetraId;
						unsigned int localNodeId;

						unsigned int currentGlobalNodeId;
						unsigned int currentDebugGlobalNodeId;
						double nodes[30];
						double triNodes[18];
						double currentLocalCoords[3];
						Vector3 tempNormal;
						//Vector3 tempNormalTri;
						Vector3 averagedNormal;
						Vector3 averagedNormalME;
						int faceNumber;
						sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron  currentTrianglesInTetrahedron;

						////////////////////////7just 4 debug ////////////////
						//just for debug purposes: write to vtk file
//						const char* filename2 = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/NormalTest.vtk";
//						//save as vtk unstructured grid
//						vtkPoints* pointContainer2 = vtkPoints::New();
//						pointContainer2->SetDataTypeToDouble();
//
//
//						vtkFloatArray *vectors= vtkFloatArray::New();
//						vectors->SetNumberOfComponents(3);
//						vectors->SetName("normals");
//
//						double currentVTKPoint[3];

						this->m_NodeNormals.clear();
						this->m_NodeNormals.resize(theTopology->getNbPoints());//surfaceNodes.size()); //nb nodes
						//std::cout<<"NodeNormalSize: "<<surfaceNodes.size()<<"\n";

						///////////////////////////////////////////////////

						//for each point:
						for ( unsigned int nodeIter = 0; nodeIter < surfaceNodes.size(); nodeIter++ )
						{
							//get triangles around node
							//triangles = theTopology->getSurfaceQuadraticTrianglesAroundSurfaceNode(nodeIter);
							currentGlobalNodeId = surfaceNodes[nodeIter];
							triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(currentGlobalNodeId );
							averagedNormal = Vector3(0.0,0.0,0.0);
							averagedNormalME = Vector3(0.0,0.0,0.0);

							bool averageVerbose = false;
							if(currentGlobalNodeId == 3804)
								averageVerbose = 1;

							//for each triangle:
							for ( unsigned int triIter = 0; triIter < triangles.size(); triIter++ )
							{
								//try it triangle based

								//get triangle
								unsigned int currentTriangleId = triangles[triIter];
								const sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle &quadTriangle = theTopology->getQuadraticTriangleVTK(currentTriangleId);

								//get local vertex id in triangle
								unsigned int localNodeId = theTopology->getVertexIndexInQuadraticTriangle(quadTriangle, currentGlobalNodeId);

								//get nodes
								for(int iteri=0; iteri<3; iteri++)
								{
									for(int p=0; p<6; p++)
									{
										triNodes[6*iteri+p] = (in[quadTriangle[p]])[iteri];
									}
								}

								//get normal
								CalculateSurfaceNormal(localNodeId, tempNormal, triNodes);

								//get weight
								double currentWeight = 1;

								//only use weighted average if node is on triangle edge
								if(localNodeId <3)
								{
									Vector3 edge1, edge2;

									unsigned int localNodeIdEdge1;
									unsigned int localNodeIdEdge2;

									if(localNodeId == 0)
									{
										localNodeIdEdge1 = 1;
										localNodeIdEdge2 = 2;
									}

									if(localNodeId == 1)
									{
										localNodeIdEdge1 = 2;
										localNodeIdEdge2 = 0;
									}

									if(localNodeId == 2)
									{
										localNodeIdEdge1 = 0;
										localNodeIdEdge2 = 1;
									}

									Vector3 pointEdge1 = (in[quadTriangle[localNodeIdEdge1]]);
									Vector3 pointEdge2 = (in[quadTriangle[localNodeIdEdge2]]);
									Vector3 currentPoint =  (in[currentGlobalNodeId]);

									edge1= pointEdge1-currentPoint;
									edge2= pointEdge2-currentPoint;

									double alpha = edge1*edge2;//(edge1.cross(edge2)).norm();
									alpha = alpha / (edge1.norm() * edge2.norm());
									alpha = acos(alpha)*180.0/3.14159;
//									if(alpha<0)
//										alpha = alpha * (-1);


									currentWeight = alpha;


									//std::cout<<"Edge1 "<<edge1<<", edge2"<<edge2<<" alpha="<<alpha<<"\n";


								}



//								//get tetra number
//								currentTetraId = (theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[triIter]))[0];
//
//								//get tetrahedron
//								const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra &quadTetra = quadraticTetrahedra[currentTetraId];
//
//								//get local node number in tetra
//								localNodeId = theTopology->getVertexIndexInTetrahedron(quadTetra, currentGlobalNodeId);
//
//								//get local face number
//								currentTrianglesInTetrahedron = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentTetraId);
//								faceNumber = theTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, triangles[triIter]);
//
//								//switch statement to find local coords from node number
//								switch (localNodeId)
//								{
//								case 0:
//									currentLocalCoords[0] = 1; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//									break;
//								case 1:
//									currentLocalCoords[0] = 0; currentLocalCoords[1] = 1; currentLocalCoords[2] = 0;
//									break;
//								case 2:
//									currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//									break;
//								case 3:
//									currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 1;
//									break;
//								case 4:
//									currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0;
//									break;
//								case 5:
//									currentLocalCoords[0] = 0; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0;
//									break;
//								case 6:
//									currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//									break;
//								case 7:
//									currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0.5;
//									break;
//								case 8:
//									currentLocalCoords[0] = 0.0; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0.5;
//									break;
//								case 9:
//									currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0.5;
//									break;
//								default:
//									std::cout<<"LocalNOdeID: "<< localNodeId<<" Error, cant compute QuadTriangles\n";
//									break;
//								}
//
//								//get node coords
//								for(int iteri=0; iteri<3; iteri++)
//								{
//									for(int p=0; p<10; p++)
//									{
//										nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
//									}
//								}
//
//								CalculateSurfaceNormal(currentLocalCoords, tempNormal, nodes, faceNumber);
//
//								double differenceBetweenNormals = (tempNormal-tempNormalTri).norm();
//								if(differenceBetweenNormals>1e-3)
//									std::cout<<"Error, normals don't match, tetra normal is "<< tempNormal<<" tri normal is "<< tempNormalTri <<"\n";

								//calculate normal, add to completeNormal
								tempNormal.normalize();
//								if(surfaceNodes[nodeIter] ==0)//triIter == 1 && )
//								{
//									const sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle &quadTriangle = theTopology->getQuadraticTriangle(triangles[triIter]);
//
//									//Debug output
//									std::cout<<"Surface node id: "<<nodeIter<<", global node Id:"<<surfaceNodes[nodeIter]
//									   <<", currentTetraId="<<currentTetraId<<", localNodeId: "<<localNodeId<<"Local coords: "<<currentLocalCoords[0] <<", "<<currentLocalCoords[1]<<"," <<currentLocalCoords[2]<< " \n";
//									std::cout<<"Current triangle: " << quadTriangle<<", faceNumber"<<faceNumber<<"\n";
//
//									for(int tempIter=0; tempIter<6; tempIter++)
//									{
//										currentDebugGlobalNodeId = quadTriangle[tempIter];//surfaceNodes[nodeIter];
//										currentVTKPoint[0] = (in[currentDebugGlobalNodeId])[0];
//										currentVTKPoint[1] = (in[currentDebugGlobalNodeId])[1];
//										currentVTKPoint[2] = (in[currentDebugGlobalNodeId])[2];
//										pointContainer2->InsertNextPoint( currentVTKPoint);
//										vectors->InsertNextTuple3( tempNormal[0], tempNormal[1],  tempNormal[2]);
//									}
//								}
								if(averageVerbose)
									std::cout<<"alpha"<<triIter<<"="<<currentWeight<<" associated normal "<<tempNormal;
								if(averageVerbose)
										std::cout<<"\n";
								averagedNormal += currentWeight*tempNormal;
								averagedNormalME += tempNormal;
							}

							averagedNormal.normalize();
							averagedNormalME.normalize();

							if(averageVerbose)
									std::cout<<"MA average normal is "<<averagedNormal<<", ME average normal is "<<averagedNormalME<<"\n";

							this->m_NodeNormals[currentGlobalNodeId] = averagedNormal;



//							if(surfaceNodes[nodeIter] ==0)
//							{
//								currentDebugGlobalNodeId = surfaceNodes[nodeIter];
//								currentVTKPoint[0] = (in[currentDebugGlobalNodeId])[0];
//								currentVTKPoint[1] = (in[currentDebugGlobalNodeId])[1];
//								currentVTKPoint[2] = (in[currentDebugGlobalNodeId])[2];
//								pointContainer2->InsertNextPoint( currentVTKPoint);
//								vectors->InsertNextTuple3( this->m_NodeNormals[currentGlobalNodeId][0], this->m_NodeNormals[currentGlobalNodeId][1],  this->m_NodeNormals[currentGlobalNodeId][2]);
//							}


						}


						//just for debug purposes: write to vtk file
						const char* filename2 = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/NormalTest.vtk";
						//save as vtk unstructured grid
						vtkPoints* pointContainer2 = vtkPoints::New();
						pointContainer2->SetDataTypeToDouble();


						vtkFloatArray *vectors= vtkFloatArray::New();
						vectors->SetNumberOfComponents(3);
						vectors->SetName("normals");

						double currentVTKPoint[3];


						for ( unsigned int nodeIter = 0; nodeIter < surfaceNodes.size(); nodeIter++ )
						{
							currentGlobalNodeId = surfaceNodes[nodeIter];
							currentVTKPoint[0] = (in[currentGlobalNodeId])[0];
							currentVTKPoint[1] = (in[currentGlobalNodeId])[1];
							currentVTKPoint[2] = (in[currentGlobalNodeId])[2];
							pointContainer2->InsertNextPoint( currentVTKPoint);

							vectors->InsertNextTuple3( this->m_NodeNormals[currentGlobalNodeId][0], this->m_NodeNormals[currentGlobalNodeId][1],  this->m_NodeNormals[currentGlobalNodeId][2]);
						}


						vtkPolyData* myGrid2 = vtkPolyData::New();
						myGrid2->SetPoints(pointContainer2);

						myGrid2->GetPointData()->SetVectors(vectors);

						  vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
							vtkSmartPointer<vtkVertexGlyphFilter>::New();
						  vertexGlyphFilter->AddInput(myGrid2);
						  vertexGlyphFilter->Update();

						vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();

						writer2->SetInput(vertexGlyphFilter->GetOutput());
						writer2->SetFileName(filename2);
						writer2->Write();

						pointContainer2->Delete();
						myGrid2->Delete();
						writer2->Delete();
						vectors->Delete();

						std::cout<<"Debug normal data saved..\n";




					}
//					for ( unsigned int t = 0; t < quadraticTetrahedra.size(); t++ )
//					{
//						Mat3x3d m,mt;
//						m[0] = in[quadraticTetrahedra[t][0]]-in[quadraticTetrahedra[t][2]];
//						m[1] = in[quadraticTetrahedra[t][1]]-in[quadraticTetrahedra[t][2]];
//						m[2] = in[quadraticTetrahedra[t][3]]-in[quadraticTetrahedra[t][2]];
//						mt.transpose ( m );
//						bases[c1+t].invert ( mt );
//						centers[c1+t] = ( in[quadraticTetrahedra[t][0]]+in[quadraticTetrahedra[t][1]]+in[quadraticTetrahedra[t][2]]+in[quadraticTetrahedra[t][3]] ) *0.25;
//
//						//calculate bounding boxes
//						Vec<9,double> tempBB;
//						tempBB[0]=in[quadraticTetrahedra[t][0]][0];
//						tempBB[1]=in[quadraticTetrahedra[t][0]][0];
//						tempBB[2]=in[quadraticTetrahedra[t][0]][1];
//						tempBB[3]=in[quadraticTetrahedra[t][0]][1];
//						tempBB[4]=in[quadraticTetrahedra[t][0]][2];
//						tempBB[5]=in[quadraticTetrahedra[t][0]][2];
//
//						typename In::Coord currentCoord;
//
//						for(int bbiter=0; bbiter<10; bbiter++)
//						{
//							currentCoord = in[quadraticTetrahedra[t][bbiter]];
//							if(currentCoord[0]<tempBB[0])
//							tempBB[0] = currentCoord[0];
//							if(currentCoord[0]>tempBB[1])
//							tempBB[1] = currentCoord[0];
//							if(currentCoord[1]<tempBB[2])
//							tempBB[2] = currentCoord[1];
//							if(currentCoord[1]>tempBB[3])
//							tempBB[3] = currentCoord[1];
//							if(currentCoord[2]<tempBB[4])
//							tempBB[4] = currentCoord[2];
//							if(currentCoord[2]>tempBB[5])
//							tempBB[5] = currentCoord[2];
//
//						}
//
//						tempBB[0]-=TOLERANCE;
//						tempBB[1]+=TOLERANCE;
//						tempBB[2]-=TOLERANCE;
//						tempBB[3]+=TOLERANCE;
//						tempBB[4]-=TOLERANCE;
//						tempBB[5]+=TOLERANCE;
//
//						tempBB[6] = (tempBB[1]-tempBB[0])/2.0;
//						tempBB[7] = (tempBB[3]-tempBB[2])/2.0;
//						tempBB[8] = (tempBB[5]-tempBB[4])/2.0;
//
//						boundingBoxes.push_back(tempBB);
//
//						//sout << "Tetra "<<t<<" center="<<centers[t]<<" base="<<m<<sendl;
//					}
					for ( unsigned int c = 0; c < cubes.size(); c++ )
					{
						Mat3x3d m,mt;
						m[0] = in[cubes[c][1]]-in[cubes[c][0]];
#ifdef SOFA_NEW_HEXA
						m[1] = in[cubes[c][3]]-in[cubes[c][0]];
#else
						m[1] = in[cubes[c][2]]-in[cubes[c][0]];
#endif
						m[2] = in[cubes[c][4]]-in[cubes[c][0]];
						mt.transpose ( m );
						bases[c0+c].invert ( mt );
						centers[c0+c] = ( in[cubes[c][0]]+in[cubes[c][1]]+in[cubes[c][2]]+in[cubes[c][3]]+in[cubes[c][4]]+in[cubes[c][5]]+in[cubes[c][6]]+in[cubes[c][7]] ) *0.125;
					}

					//debug info
					//just for debug purposes: write to vtk file
//					const char* filenameOptiDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfos.vtk";
//
//					vtkPoints* pointContainerOptiDebug = vtkPoints::New();
//					pointContainerOptiDebug->SetDataTypeToDouble();
//
//
//					vtkFloatArray *vectorsOptiDebug= vtkFloatArray::New();
//					vectorsOptiDebug->SetNumberOfComponents(3);
//					vectorsOptiDebug->SetName("normals");
//
//					vtkFloatArray *scalarsOptiDebug= vtkFloatArray::New();
//					scalarsOptiDebug->SetNumberOfComponents(1);
//					scalarsOptiDebug->SetName("scalars");
//
//					const char* filenameOptiDebug2 = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfosTopology.vtk";
//
//					vtkPoints* pointContainerOptiDebug2 = vtkPoints::New();
//					pointContainerOptiDebug2->SetDataTypeToDouble();
//					vtkCellArray* triangleContainerOptiDebug2 = vtkCellArray::New();
//					vtkFloatArray *vectorsOptiDebug2= vtkFloatArray::New();
//					vectorsOptiDebug2->SetNumberOfComponents(3);
//					vectorsOptiDebug2->SetName("normals");
//
//					vtkIdType* nodeIndices;
//					nodeIndices = new vtkIdType[6];
//					vtkIdType* nodeIndicesTmp;
//					nodeIndicesTmp = new vtkIdType[6];
//					vtkIdType currentPointId;
//
//
//
//
					double currentVTKPointArray[3];
					double currentVTKNormalArray[3];

					const char* filenameDebugPoints = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfos.vtk";
					const char* filenameDebugTriangles = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfosTopology.vtk";

					m_PointContainerDebugPoints = vtkPoints::New();
					m_PointContainerDebugPoints->SetDataTypeToDouble();

					m_VectorsOptiDebug = vtkFloatArray::New();
					m_VectorsOptiDebug->SetNumberOfComponents(3);
					m_VectorsOptiDebug->SetName("normals");

					m_ScalarsOptiDebug = vtkFloatArray::New();
					m_ScalarsOptiDebug->SetNumberOfComponents(1);
					m_ScalarsOptiDebug->SetName("scalars");

					m_PointContainerDebugTriangles = vtkPoints::New();
					m_PointContainerDebugTriangles->SetDataTypeToDouble();

					m_TriangleContainerDebugTriangles = vtkCellArray::New();

					m_ScalarsDebugTriangles = vtkFloatArray::New();
					m_ScalarsDebugTriangles->SetNumberOfComponents(1);
					m_ScalarsDebugTriangles->SetName("scalars");
					//m_CellDataDebugTriangles->InsertNextTuple()

					m_PointContainerRecursiveMapping = vtkPoints::New();
					m_PointContainerRecursiveMapping->SetDataTypeToDouble();
					//m_PointContainerRecursiveMapping->SetNumberOfPoints(out.size());

					m_TriangleContainerRecursiveMapping = vtkCellArray::New();

					m_MappingPrintCounter = 0;
					m_MappingPointCounter = 0;



					////////big big debug output to test topology/////////////////////////
					{
						const char* filenameTopoDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/TopologyDebug.vtk";

						vtkPoints* pointContainerTopoDebug = vtkPoints::New();
						pointContainerTopoDebug->SetDataTypeToDouble();
						vtkCellArray* triangleContainerTopoDebug = vtkCellArray::New();

						vtkIdType* nodeIndices2;
						nodeIndices2 = new vtkIdType[6];
						vtkIdType currentPointId2;

						vtkIdType* nodeIndices3;
						nodeIndices3 = new vtkIdType[3];


						sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
						//int noTriangles = theTopology->getNbQuadraticTriangles();

						int noTriangles = theTopology->getNbQuadraticTriangles();

						for(unsigned int iterTriangle=0; iterTriangle<noTriangles; iterTriangle++)
						{


							const sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle currentTriangle = theTopology->getQuadraticTriangle(iterTriangle);


							for(unsigned int itera=0; itera<6; itera++)
							{
								currentVTKPointArray[0] = (in[ currentTriangle[itera] ])[0];
								currentVTKPointArray[1] = (in[ currentTriangle[itera] ])[1];
								currentVTKPointArray[2] = (in[ currentTriangle[itera] ])[2];
								currentPointId2 = pointContainerTopoDebug->InsertNextPoint( currentVTKPointArray);
								nodeIndices2[itera] = currentPointId2;
							}



							nodeIndices3[0] = nodeIndices2[0];
							nodeIndices3[1] = nodeIndices2[2];
							nodeIndices3[2] = nodeIndices2[4];

							triangleContainerTopoDebug->InsertNextCell(3, nodeIndices3);


						}

						vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
						myGridTopoDebug->SetPoints(pointContainerTopoDebug);
						myGridTopoDebug->SetCells(VTK_TRIANGLE, triangleContainerTopoDebug);


						vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();

						writerTopoDebug->SetInput(myGridTopoDebug);
						writerTopoDebug->SetFileName(filenameTopoDebug);
						writerTopoDebug->Write();

						pointContainerTopoDebug->Delete();
						triangleContainerTopoDebug->Delete();
						myGridTopoDebug->Delete();
						writerTopoDebug->Delete();

					}







					bool printOptiOutput = false;

					//////////////////////////////////////////////////////////////////////////////
					////////////////new linear time version//////////////////////////////////////////




					//initialize point (closest point to point on coarse mesh)
					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
					const vector<sofa::component::topology::QuadraticMeshTopology::PointID> & surfaceNodes = theTopology->getSurfaceNodes();
					//change this for different path
//					const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(surfaceNodes[100]);
					const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(surfaceNodes[0]);
					unsigned int  startInElementTriangle = triangles[0];
					unsigned int  startInElementTetra = theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startInElementTriangle)[0];
					sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle theTriangle = theTopology->getQuadraticTriangleVTK(startInElementTriangle);

					//display the nodes in starting triangle
					std::cout<<"Starting triangle"<<theTriangle<<"\n";


					double nodes[18];
					for(int iteri=0; iteri<3; iteri++)
					{
						for(int p=0; p<6; p++)
						{
							nodes[6*iteri+p] = (in[theTriangle[p]])[iteri];
						}
					}

					Vector3 startPos, startCoefs;
					startCoefs[0] = 0.5;
					startCoefs[1] = 0.5;
					startCoefs[2] = 1.0;


					CalculateGlobalCoordsSurface(startCoefs.ptr(), startPos.ptr(),3,3,&nodes[0]);

					double startDistance = 1e10;
					Vector3 currentStartPos;
					double tempError;
					unsigned int startIndex;
					for ( unsigned int i=0;i<out.size();i++ )
					{
						currentStartPos = Out::getCPos(out[i]);
						tempError = (currentStartPos - startPos).norm();
						if(tempError< startDistance)
						{
							startDistance = tempError;
							startIndex = i;
						}

					}

					//find element on out mesh
					sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));

					//resize map3d!!!!!!!!!!!!!
					map3d.resize(outTopology->getNbPoints());



					if(!outTopology)
						std::cout<<"Error, out topology has the wrong type!!!\n";
					unsigned int startOutElement = outTopology->getTrianglesAroundVertex(startIndex)[0];
					std::cout<<"Starting mapping recursion with startOutElement="<<startOutElement<<", startInElementIndex="<<startInElementTetra<<",startInElement="<<theTopology->getQuadTetrahedron(startInElementTetra)<<", startInCoefs="<<startCoefs<<", start distance="<<startDistance<<"\n";
					//recursively map mesh
					std::map<unsigned int, unsigned int> pointsMapped;
					std::set<unsigned int> trianglesMapped;
					//RecursivelyMapElement(unsigned int startOutElement, unsigned int startInElement, Vector3 startInCoefs, std::set<unsigned int> &pointsMapped);

					typedef typename std::map<unsigned int,RecursiveMappingData>::reverse_iterator ReverseIteratorType ;
					typedef typename std::map<unsigned int,RecursiveMappingData>::iterator IteratorType ;
					IteratorType mapIterator;
					IteratorType mapIteratorPong;
					std::map<unsigned int, RecursiveMappingData> elementsToMapPing;
					std::map<unsigned int, RecursiveMappingData> elementsToMapPong;
					std::map<unsigned int, RecursiveMappingData> elementsAlreadyMapped;

					ReverseIteratorType ritMappingMap;
//					std::map<unsigned int, RecursiveMappingData>::iterator ritMappingMap;
//					unsigned int noOutElements = outTopology->getNbTriangles();
//					mappingVec.resize(noOutElements);

					RecursiveMappingData tempData;
					tempData.startCoefs = startCoefs;
					tempData.startInElement = startInElementTetra;
					tempData.startOutElement = startOutElement;

					unsigned int currentMapSize = elementsToMapPing.size();
					elementsToMapPing[startOutElement] = tempData;
					//mappingVec[currentMapSize]=tempData;


					std::cout<<"Number of Triangle Elements="<<theTopology->getQuadraticTriangles().size()<<"\n";

					unsigned int printDebugInformation = 0;
					int debugPointId;

//					sofa::component::topology::MeshTopology::Triangle currentTri = outTopology->getTriangle(32181);
//					printDebugInformation = currentTri[0];

					//start with low thresholds
					//get relative coords
					double characteristicLength = (in[theTopology->getQuadraticTriangle(0)[0]]-in[theTopology->getQuadraticTriangle(0)[1]]).norm();
					double distanceThresholdStart = characteristicLength * 1e-5;//10.0;
					double errorThresholdStart = characteristicLength * 1e-5;//10.0;
//					theTopology->get
					double distanceThreshold = distanceThresholdStart;//10.0;
					double errorThreshold = errorThresholdStart;//10.0;

					double currentError;
					double currentDistance;

					std::map<unsigned int, RecursiveMappingData>* pingMapPtr;
					std::map<unsigned int, RecursiveMappingData>* pongMapPtr;

					bool ping = true;
					bool elementsToMapCurrentGroup = true;

					///debug output
					unsigned int printFlag = f_Debug_Output.getValue();//PRINTDEBUGOUTPUT;

					//copy all triangles to map in set
					std::set<unsigned int> trianglesToMap;
					{
						sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));

						unsigned int noTriangles = outTopology->getNbTriangles();

						std::cout<<"NoTriangles: "<<noTriangles<<"\n";

						for(unsigned int i=0; i<noTriangles;i++ )
						{
							trianglesToMap.insert(i);
						}
					}

					bool nextGroup = false;

					std::cout<<"Start mapping with distance threshold: "<<distanceThreshold<<" , error threshold "<<errorThreshold<<"\n";

					while(trianglesToMap.size() != 0)
					{
//						if(nextGroup)
//							break;

						if(!elementsToMapCurrentGroup)
						{
							sofa::component::topology::QuadraticMeshTopology* inTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

							//grab triangle from trianglestomap
							typedef typename std::set<unsigned int>::iterator SetIteratorType;
							SetIteratorType setIterator;
							setIterator = trianglesToMap.begin();
							unsigned int startTriangleIdNewGroup =(* setIterator);

							//grab point
							sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));

							unsigned int startPointIdNewGroup = outTopology->getTriangle(startTriangleIdNewGroup)[0];
							Vector3 startPointNewGroup = out[startPointIdNewGroup];

							//look for nearest point in elements already mapped
							double tempDistance = 1e20;
							Vector3 currentCompPoint;
							double finalStartDistance = 1e20;
							unsigned int finalStartInPointId = 0;
							double currentCompDistance;
							RecursiveMappingData finalTempData;
							std::cout<<" Number of points in intopology"<<inTopology->getNbPoints()<<"\n";

							for(unsigned int iterPoint=0; iterPoint<(inTopology->getNbPoints()); iterPoint++)
							{
								currentCompPoint = in[iterPoint ];

								currentCompDistance = (currentCompPoint-startPointNewGroup ).norm();
								if(currentCompDistance<finalStartDistance)
								{
									finalStartDistance = currentCompDistance;
									 finalStartInPointId = iterPoint;
								}
							}
//
							finalTempData.startOutElement = startTriangleIdNewGroup ;
							//std::cout<<"StartInPointID "<<finalStartInPointId<<" with tetrahedra "<<inTopology->getQuadTetrahedraAroundVertex(finalStartInPointId).size()<<"\n";
							unsigned int startInTriangleId = inTopology->getQuadraticTrianglesAroundVertex(finalStartInPointId)[0];
							unsigned int startInElementId = inTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startInTriangleId)[0];
							finalTempData.startInElement = startInElementId;
							std::cout<<"StartInElement "<<finalTempData.startInElement<<"\n";
							finalTempData.startCoefs = Vector3(0.3,0.3,0.3);
							finalTempData.distance = 1e10;
							finalTempData.error = finalStartDistance;
//
//							for(mapIterator = elementsAlreadyMapped.begin(); mapIterator != elementsAlreadyMapped.end(); ++mapIterator)
//							{
//
//								const sofa::component::topology::TriangleSetTopologyContainer::Triangle currentCompTriangle = outTopology->getTriangle(mapIterator->first);
//								for(unsigned int iterTri = 0; iterTri< 3; iterTri++)
//								{
//									currentCompPoint = out[ currentCompTriangle[iterTri] ];
//									currentCompDistance = (currentCompPoint-startPointNewGroup ).norm();
//									if(currentCompDistance<finalStartDistance)
//									{
//										finalStartDistance = currentCompDistance;
//										finalTempData= mapIterator->second;
//										finalTempData.distance = 1e10;
//										finalTempData.error = finalStartDistance;
//
//									}
//								}
//							}

							//add to ping map
							elementsToMapPing[finalTempData.startOutElement] = finalTempData;

							//set bool
							ping = true;
							elementsToMapCurrentGroup = true;
							distanceThreshold = distanceThresholdStart;
							errorThreshold = errorThresholdStart;

							nextGroup = true;

							std::cout<<"Initializing new mapping group with start element: "<< finalTempData.startOutElement <<" mapped to "<<finalTempData.startInElement<<" with start distance: "<<finalTempData.error<<"\n";
						}


						while( elementsToMapCurrentGroup)
						{
							if(ping)
							{
								pingMapPtr = &elementsToMapPing;
								pongMapPtr = &elementsToMapPong;
							}
							else
							{
								pingMapPtr = &elementsToMapPong;
								pongMapPtr = &elementsToMapPing;
							}

							pongMapPtr->clear();

							while((!pingMapPtr->empty()))
							{
		//						if(i>=mappingMap.size())
		//						{
		//							std::cout<<"loop iteration bigger than map size. error!!!!\n";
		//							std::cout<<"MapSize:"<<mappingMap.size()<<", i="<<i<<"\n";
		//							break;
		//						}
		//
		//						tempData = mappingVec[i];
		//
		//						if(i==3048)
		//						{
		//							std::cout<<"Debugging recursion at i=3048, currenTriId="<< tempData.startOutElement<<", currentTri "<<outTopology->getTriangle(tempData.startOutElement)<<"\n";
		//						}

								mapIterator = pingMapPtr->begin();
								tempData = mapIterator->second;

								//tochange
								printDebugInformation = PRINTDEBUGPOINT;//9380;//15364;//428;

								bool wasMapped ;

								if(nextGroup)
									wasMapped = MapElement(tempData.startOutElement, tempData.startInElement, errorThreshold, distanceThreshold, tempData.startCoefs,in, out, trianglesMapped, pointsMapped, currentError, currentDistance, printFlag, printDebugInformation);
								else
									wasMapped = MapElement(tempData.startOutElement, tempData.startInElement, errorThreshold, distanceThreshold, tempData.startCoefs,in, out, trianglesMapped, pointsMapped, currentError, currentDistance, printFlag, printDebugInformation);


								//MapElement(unsigned int startOutElement, unsigned int &currentInElement, double errorThreshold, double distanceThreshold, sofa::defaulttype::Vector3 &currentCoefs, const typename In::VecCoord& in, const typename Out::VecCoord& out, std::set<unsigned int>  &trianglesMapped, std::map<unsigned int,unsigned int> &pointsMapped, double & currentError, double & currentDistance,unsigned int printFlag, unsigned int pointIndexDebugInformation);

								//std::cout<<"Element Nr. :"<<tempData.startOutElement<<" mapped\n";

								//std::cout<<"Looking for Neighbours for triangle "<<tempData.startOutElement<<"\n";


//								if(nextGroup)
//									std::cout<<"Trying to map "<<tempData.startOutElement<<" to "<<tempData.startInElement<<" with distance "<<currentDistance<<"\n";

								if(wasMapped)
								{
									AddNeighboursToRecursion(tempData,  *pingMapPtr, elementsAlreadyMapped);

									//std::cout<<"Element "<<ritMappingMap->first "was mapped, deleting it from map...";
									//mapIterator = mappingMap.find(ritMappingMap->first);
									pingMapPtr->erase(mapIterator);
									elementsAlreadyMapped[tempData.startOutElement] = tempData;
	//								int currentTestInt = tempData.startOutElement;
	//								int nextTestInt = 9927;
	//								std::cout<<"Mapping element no "<<currentTestInt <<" nextint "<<nextTestInt<< "\n";

									pongMapPtr->erase(tempData.startOutElement);

	//								if(tempData.startOutElement == 9927)
	//								{
	//									std::cout<<"Element 9927 has been added To alreadymapped and removed from pingmap \n \n";
	//
	//									//check if element is in pingmap
	//									IteratorType mapIteratorTemp;
	//									mapIteratorTemp = pingMapPtr->find(9927);
	//									if(mapIteratorTemp != pingMapPtr->end())
	//										std::cout<<"Still in pingmap!!! \n";
	//
	//									mapIteratorTemp = pongMapPtr->find(9927);
	//									if(mapIteratorTemp != pongMapPtr->end())
	//										std::cout<<"Still in pongmap!!! \n";
	//								}

									//check if

									trianglesToMap.erase(tempData.startOutElement);


		//							++ritMappingMap;
		//						    mappingMap.erase(ritMappingMap.base());

								}
								else
								{
									//mapIterator = mappingMap.find(ritMappingMap->first);
									pingMapPtr->erase(mapIterator);

									(*pongMapPtr)[tempData.startOutElement] = tempData;



									//elementsToMapPong[tempData.startOutElement] = tempData;

								}

							}

							if(!pongMapPtr->empty())
							{
								if(ping)
									ping = false;
								else
									ping = true;


								if(distanceThreshold > (1000000*distanceThresholdStart))
								{
									errorThreshold = errorThreshold * 2.0;
									distanceThreshold = distanceThreshold * 2.0;
//									elementsToMapCurrentGroup = false;
//									std::cout<<"element not mapped:"<<pongMapPtr->begin()->first<<"\n";
								}
								else
									distanceThreshold = distanceThreshold * 2.0;

//								if(errorThreshold > (1000*errorThresholdStart))
//								{
//									elementsToMapCurrentGroup = false;
////									std::cout<<"element not mapped:"<<pongMapPtr->begin()->first<<"\n";
//								}

								std::cout<<pongMapPtr->size()<<" elements were not mappred, raise distance threshold to"<< distanceThreshold<<" and error threshold to "<<errorThreshold<<"\n";

	//							printFlag = 50;
	//
	//							if(distanceThreshold > 100)
	//							{
	//								printFlag = 5;
	//
	//								std::set<unsigned int>::iterator setIterator;
	//								typedef typename std::map<unsigned int, RecursiveMappingData>::iterator PongIteratorType;
	//								PongIteratorType pongMapIterator;
	//
	//								std::string filenameElementsToMapDebug("/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/DebugInformation/ElementsToMapDebugData");
	//								std::ostringstream oss;
	//								oss << "_"<< this->m_MappingPrintCounter;
	//								filenameElementsToMapDebug += oss.str() + ".vtk";
	//
	//								vtkPoints* pointContainerTopoDebug = vtkPoints::New();
	//								pointContainerTopoDebug->SetDataTypeToDouble();
	//								vtkCellArray* triangleContainerTopoDebug = vtkCellArray::New();
	//
	//								vtkIdType currentPointId;
	//								vtkIdType* nodeIndices;
	//								nodeIndices = new vtkIdType[3];
	//
	//
	//								sofa::component::topology::TriangleSetTopologyContainer* outTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));
	//								//int noTriangles = theTopology->getNbQuadraticTriangles();
	//
	//								int noTriangles = outTopology->getNbTriangles();
	//								unsigned int currentTriangleId;
	//
	//								//for(setIterator = trianglesToMap.begin(); setIterator != trianglesToMap.end(); ++setIterator)
	//								for(pongMapIterator = pongMapPtr->begin(); pongMapIterator != pongMapPtr->end(); ++pongMapIterator)
	//								{
	//
	//									//currentTriangleId = (*setIterator);
	//									currentTriangleId = pongMapIterator->first;
	//
	//									const sofa::component::topology::Triangle currentTriangle = outTopology->getTriangle(currentTriangleId);
	//
	//
	//									for(unsigned int itera=0; itera<3; itera++)
	//									{
	//										currentVTKPointArray[0] = (out[ currentTriangle[itera] ])[0];
	//										currentVTKPointArray[1] = (out[ currentTriangle[itera] ])[1];
	//										currentVTKPointArray[2] = (out[ currentTriangle[itera] ])[2];
	//										currentPointId = pointContainerTopoDebug->InsertNextPoint( currentVTKPointArray);
	//										nodeIndices[itera] = currentPointId;
	//									}
	//
	//
	//									triangleContainerTopoDebug->InsertNextCell(3, nodeIndices);
	//
	//								}
	//
	//								vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
	//								myGridTopoDebug->SetPoints(pointContainerTopoDebug);
	//								myGridTopoDebug->SetCells(VTK_TRIANGLE, triangleContainerTopoDebug);
	//
	//
	//								vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();
	//
	//								writerTopoDebug->SetInput(myGridTopoDebug);
	//								writerTopoDebug->SetFileName(filenameElementsToMapDebug.c_str());
	//								writerTopoDebug->Write();
	//
	//								pointContainerTopoDebug->Delete();
	//								triangleContainerTopoDebug->Delete();
	//								myGridTopoDebug->Delete();
	//								writerTopoDebug->Delete();
	//
	//								delete [] nodeIndices;
	//
	//							}
	//
	//							if(distanceThreshold > 135)
	//								printFlag = 1;
							}
							else
								elementsToMapCurrentGroup = false;
						}
					}

					if(printFlag)
					{
						std::cout<<"Write debug data number "<< this->m_MappingPrintCounter<<"\n";
						std::string filenameRecursiveMappingDebug("/opt/Ergebnisse/tmp/RecursiveMappingDebugData");
						std::ostringstream oss;
						oss << "_"<< this->m_MappingPrintCounter;
						filenameRecursiveMappingDebug += oss.str() + ".vtk";

						vtkUnstructuredGrid* myGrid = vtkUnstructuredGrid::New();
						myGrid->SetPoints(m_PointContainerRecursiveMapping);
						myGrid->SetCells(VTK_TRIANGLE, m_TriangleContainerRecursiveMapping);

						vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
						writer->SetInput(myGrid);
						writer->SetFileName(filenameRecursiveMappingDebug.c_str());
						writer->Write();

						myGrid->Delete();
						writer->Delete();

					}

					std::cout<<"Mapping complete, number of triangles not mapped: "<<trianglesToMap.size()<<" \n";


					std::cout<<"Sanity check for mapping...";

					for ( unsigned int i=0;i<map3d.size();i++ )
					{
						int index = map3d[i].in_index;
						if(index == -1)
							std::cout<<"Wrong index (-1) at point "<<i<<"\n";
					}

//					for(unsigned int i=0;i<map3d.size();i++)
//						{
//
//						const Real fx = map3d[i].baryCoords[0];
//						const Real fy = map3d[i].baryCoords[1];
//						const Real fz = map3d[i].baryCoords[2];
//						int index = map3d[i].in_index;
//						if ( index<c1 )
//						{
//							const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
//							Out::setCPos(out[i+i0] , in[tetra[0]] * ( 1-fx-fy-fz )
//									+ in[tetra[1]] * fx
//									+ in[tetra[2]] * fy
//									+ in[tetra[3]] * fz );
//						}
//						else
//						{
//							if(index < c0)
//							{
//								const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[index-c1];
//	//
//	//							//construct nodes
//								double nodes[30];
//
//								for(int iteri=0; iteri<3; iteri++)
//								{
//									for(int p=0; p<10; p++)
//									{
//										nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
//									}
//								}
//
//								Vector3 currentPos;
//								double localCoords[3];
//								localCoords[0] = fx;
//								localCoords[1] = fy;
//								localCoords[2] = fz;
//								CalculateGlobalCoords(localCoords, currentPos.ptr(), 3, 3, &nodes[0]);
//
//
//								typename Out::Coord outPos =  currentPos;
//
//	//							if(map3d[i].isOutside)
//	//							{
//	//								//construct normals
//	//								double normals[30];
//	//
//	//								for(int iteri=0; iteri<3; iteri++)
//	//								{
//	//									for(int p=0; p<10; p++)
//	//									{
//	//										normals[10*iteri+p] = (this->m_NodeNormals[quadTetra[p]])[iteri];
//	//									}
//	//								}
//	//								Vector3 currentNormal;
//	//								CalculateGlobalCoords(localCoords, currentNormal.ptr(), 3, 3, &normals[0]);
//	//								outPos +=  currentNormal*map3d[i].distance;
//	//
//	//								//std::cout<<"OUtside should not happen\n";
//	//
//	//							}
//
//
//								Out::setCPos(out[i+i0], outPos);
//
//							//currentTriangleId = (*setIterator);
//							currentTriangleId = pongMapIterator->first;
//
//							const sofa::component::topology::Triangle currentTriangle = outTopology->getTriangle(currentTriangleId);
//
//
//							for(unsigned int itera=0; itera<3; itera++)
//							{
//								currentVTKPointArray[0] = (out[ currentTriangle[itera] ])[0];
//								currentVTKPointArray[1] = (out[ currentTriangle[itera] ])[1];
//								currentVTKPointArray[2] = (out[ currentTriangle[itera] ])[2];
//								currentPointId = pointContainerTopoDebug->InsertNextPoint( currentVTKPointArray);
//								nodeIndices[itera] = currentPointId;
//							}
//
//
//							triangleContainerTopoDebug->InsertNextCell(3, nodeIndices);
//
//						}
//
//						vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
//						myGridTopoDebug->SetPoints(pointContainerTopoDebug);
//						myGridTopoDebug->SetCells(VTK_TRIANGLE, triangleContainerTopoDebug);
//
//
//						vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();
//
//						writerTopoDebug->SetInput(myGridTopoDebug);
//						writerTopoDebug->SetFileName(filenameElementsToMapDebug.c_str());
//						writerTopoDebug->Write();
//
//						pointContainerTopoDebug->Delete();
//						triangleContainerTopoDebug->Delete();
//						myGridTopoDebug->Delete();
//						writerTopoDebug->Delete();
//
//						delete [] nodeIndices;
//
//					}



					sofa::helper::AdvancedTimer::end("QuadraticMapping");
					sofa::helper::AdvancedTimer::stepEnd("QuadraticMappingStep");

					double timeScale = 1.0 / (double)helper::system::thread::CTime::getTicksPerSec() * 1000;
					double t1 = (double)_timer->getTime() ;

					std::cout<<"mapping succesfull, time="<<(t1-t0)*timeScale <<" ms \n";


					////////////////////////////////////////////////////////////////////////////////
//
//
//
//					for ( unsigned int i=0;i<out.size();i++ )
//					{
//						if(i==15632)//7643)//1457)//261)
//							printOptiOutput = true;
//						else
//							printOptiOutput = false;
//
//						Vector3 pos = Out::getCPos(out[i]);
//						Vector3 coefs;
//						int index = -1;
//						double distance = 1e10;
//						bool isOutside = true;
//						bool outsideBoundingBox = true;
//						double error = 1e10;
//						//double finalNormalCorrection[3];
//						for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//						{
//							Vector3 v = bases[t] * ( pos - in[tetrahedra[t][0]] );
//							double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( -v[2],v[0]+v[1]+v[2]-1 ) );
//							if ( d>0 ) d = ( pos-centers[t] ).norm2();
//							if ( d<distance ) {coefs = v; distance = d; index = t;}
//						}
//
//						if(quadraticTetrahedra.size())
//						{
//
//
//
//
//							//boundingBoxes.size();
//							//first try to find interpolation if node is inside the mesh
//							for ( unsigned int t = 0; t < quadraticTetrahedra.size(); t++ )
//							{
//
//								sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
//
//
//								if((pos[0]>=boundingBoxes[t][0])&&(pos[0]<=boundingBoxes[t][1])&&(pos[1]>=boundingBoxes[t][2])&&
//										(pos[1]<=boundingBoxes[t][3])&&(pos[2]>=boundingBoxes[t][4])&&(pos[2]<=boundingBoxes[t][5]))
//								{
//
//									//construct nodes
//									//std::cout<<"Construct nodes \n";
//									const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[t];
//									double nodes[30];
//
//									for(int iteri=0; iteri<3; iteri++)
//									{
//										for(int p=0; p<10; p++)
//										{
//											nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
//										}
//									}
//
//									Vector3 v;
//									v[0]= 0.3;
//									v[1]= 0.3;
//									v[2]= 0.3;
//
//
//									double lb[3];
//									lb[0]=0.0;
//									lb[1]=0.0;
//									lb[2]=0.0;
//
//									double ub[3];
//									ub[0]=1.0;
//									ub[1]=1.0;
//									ub[2]=1.0;
//
//									double C[3];
//									C[0]=-1.0;
//									C[1]=-1.0;
//									C[2]=-1.0;
//
//									double d[1];
//									d[0] = -1.0;
//
//									double info[LM_INFO_SZ];
//
//
//									dlevmar_bleic_der(CalculateGlobalCoords, CalculateJacobian, v.ptr(), pos.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
//
//
//									//if local coords
//									if( (v[0]>=-TOLERANCE) && (v[1]>=-TOLERANCE) &&( v[2]>=-TOLERANCE) && ((v[0]+v[1]+v[2])<=(1+TOLERANCE)) )
//									{
//										Vector3 testPos;
//										CalculateGlobalCoords(v.ptr(), testPos.ptr(),3,3,&nodes[0]);
//										double tempError = (pos - testPos).norm();
//
//										if(tempError<distance)
//										{
//											if(tempError<TOLERANCE)
//											{
//												isOutside = false;
//												coefs = v;
//												index = c1+t;
//												distance = tempError;
//												outsideBoundingBox = false;
//												break;
//											}
//											else
//											{
//												//test if point is on surface face
//												int faceNumber;
//												unsigned int triangleId = theTopology->getSurfaceQuadraticTriangleInQuadraticTetrahedron(t,v[0], v[1], v[2], faceNumber);
//												if(triangleId != -1)
//												{
//													coefs = v;
//													index = c1+t;
//													distance = tempError;
//													outsideBoundingBox = false;
//												}
//
//											}
//	//												finalNormalCorrection[0] = normalCorrection[0];
//	//												finalNormalCorrection[1] = normalCorrection[1];
//	//												finalNormalCorrection[2] = normalCorrection[2];
//										}
//									}
//									else
//									{
//										std::cout<<"Wrong result of optimization\n";
//									}
//
//								}
//								//
//							}
//
//							//if not inside a single bounding box -> use barycentric coords as seed
//							if((outsideBoundingBox)||(isOutside))
//							{
//								sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
//
//								const vector<sofa::component::topology::QuadraticMeshTopology::PointID> & surfaceNodes = theTopology->getSurfaceNodes();
//								Vector3 currentNode;
//								unsigned int currentNodeId;
//
//								//std::cout<<"Reference posiion:"<<pos<<"\n";
//
//								distance = 1e10;
//								for ( unsigned int nodeIter = 0; nodeIter < surfaceNodes.size(); nodeIter++ )
//								{
//									currentNodeId = surfaceNodes[nodeIter];
//									currentNode = in[currentNodeId ];
//									double d = (pos-currentNode).norm();
//									if ( d<distance ) {distance = d; index = currentNodeId;}
//								}
//
//								if(printOptiOutput)
//								{
//									double currentVTKPoint[3];
//									currentNode = in[index ];
//									currentVTKPoint[0] = currentNode[0];
//									currentVTKPoint[1] = currentNode[1];
//									currentVTKPoint[2] = currentNode[2];
//
//									vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
//									m_VectorsOptiDebug->InsertNextTuple3(0,0,0);
//									m_ScalarsOptiDebug->InsertNextTuple1(pIndex);
//
//									std::cout<<"Current node id: "<<index<<" ,current out id:"<<i<<", current distance:"<<distance<<"\n";
//
//								}
//								//std::cout<<"Current node id: "<<index<<" ,current out id:"<<i<<", current distance:"<<distance<<"\n";
//								//for each surface triangle around node index: perform optimization, choose best result
//								const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(index-c1);
//
//								distance = 1e10;
//								double tempDistance;
//								unsigned int currentTriId;
//								sofa::core::topology::BaseMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron;
//								unsigned int tetraId;
//
//								 std::set<unsigned int> trianglesVisited;
//
//								Vector3 tempPosition = pos;//in[index-c1];
//								Vector3 differenceVector;
//								//std::cout<<"nearest node position:"<<in[index-c1]<<" referenc posiion:"<<pos<<"\n";
//
//								double  tempAngle, tempError;
//								unsigned int finalTriangleIndex;
//								Vector3 currentLocalCoords;
//
//
//								for ( unsigned int triIter = 0; triIter < triangles.size(); triIter++ )
//								{
//									currentLocalCoords[0] = 0;
//									currentLocalCoords[1] = 0;
//									currentLocalCoords[2] = 0;
//
//									currentTriId = triangles[triIter];//0];
//									//std::cout<<"Current triangle:"<<theTopology->getQuadraticTriangleVTK(currentTriId)<<"\n";
//									//tempDistance = FindClosesPointOnSurfaceTriangle(currentTriId, tempPosition, in, currentLocalCoords);
//									//tempDistance = FindClosesPointOnSurfaceTriangle(currentTriId, tempPosition, in, currentLocalCoords);
////									std::cout<<"\nRecursive fit called on triangle: "<<currentTriId<<"\n";
////									RecursiveFindBestNormalFitOnSurfaceTriangle(currentTriId, 1e10, tempPosition, in, currentLocalCoords, finalTriangleIndex, tempDistance, tempError, tempAngle,  trianglesVisited);
//									//RecursiveFindBestNormalFitOnSurfaceTriangle(unsigned int startTriangleId, double currentMinError, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, unsigned int & finalTriangleIndex, double & distance, double & error, double & angle,  std::set<unsigned int> &trianglesVisited);
//
//									//FindBestNormalFitOnSurfaceTriangle(unsigned int triangleId, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, double & distance, double & error, double & angle);
//									FindBestNormalFitOnSurfaceTriangle(currentTriId, tempPosition, in, currentLocalCoords, tempDistance, tempError, tempAngle, differenceVector);
//									finalTriangleIndex = currentTriId;
//
//									if(tempDistance<distance)
//									{
//										tetraId = theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(finalTriangleIndex)[0];
//										currentTrianglesInTetrahedron = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//										int faceNr = theTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, finalTriangleIndex);
//										LocalSurfaceCoordsToLocalVolumeCoords(currentLocalCoords.ptr(), faceNr, coefs.ptr());
//										distance = tempDistance;
//										index = c1+tetraId;
//									}
//								}
//
//								//start recursion
//								//std::cout<<"\nRecursive fit called on triangle: "<<currentTriId<<"\n";
////								if(printOptiOutput)
////								{
////									//add first point and first triangle to vtk debug output
////									vtkIdType* nodeIndices;
////									nodeIndices = new vtkIdType[6];
////									double currentVTKPointArray[3];
////									double currentVTKNormalArray[3];
////
////									sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle  currentTriangle = theTopology->getQuadraticTriangleVTK(currentTriId);
////
////									double nodesAndNormals[36];
////
////
////									for(int iteri=0; iteri<3; iteri++)
////									{
////										for(int p=0; p<6; p++)
////										{
////											nodesAndNormals[6*iteri+p] = (in[currentTriangle[p]])[iteri];
////										}
////									}
////
////									for(int iteri=0; iteri<3; iteri++)
////									{
////										for(int p=0; p<6; p++)
////										{
////											nodesAndNormals[18+6*iteri+p] = (this->m_NodeNormals[currentTriangle[p]])[iteri];
////										}
////									}
////
////
////									CalculateGlobalCoords(coefs.ptr(), &currentVTKPointArray[0],3,3,&nodesAndNormals[0]);
////
////									CalculateGlobalCoords(coefs.ptr(), &currentVTKNormalArray[0],3,3,&nodesAndNormals[18]);
////
////									m_PointContainerDebugPoints->InsertNextPoint( currentVTKPointArray);
////									m_VectorsOptiDebug->InsertNextTuple3( currentVTKNormalArray[0], currentVTKNormalArray[1], currentVTKNormalArray[2]);
////									m_ScalarsOptiDebug->InsertNextTuple1(0);
////
////									vtkIdType nodeIndicesTri6[6];
////									vtkIdType nodeIndicesTri3[3];
////
////									for(unsigned int itera=0; itera<6; itera++)
////									{
////										currentVTKPointArray[0] = (in[ currentTriangle[itera] ])[0];
////										currentVTKPointArray[1] = (in[ currentTriangle[itera] ])[1];
////										currentVTKPointArray[2] = (in[ currentTriangle[itera] ])[2];
////										vtkIdType currentPointId2 = m_PointContainerDebugTriangles->InsertNextPoint( currentVTKPointArray);
////										nodeIndicesTri6[itera] = currentPointId2;
////									}
////									nodeIndicesTri3[0] = nodeIndicesTri6[0];
////									nodeIndicesTri3[1] = nodeIndicesTri6[2];
////									nodeIndicesTri3[2] = nodeIndicesTri6[4];
////
////									vtkIdType triNo = m_TriangleContainerDebugTriangles->InsertNextCell(3, &nodeIndicesTri3[0]);
////
////								//	m_CellDataDebugTriangles->O;
////
////								}
//
//								if(printOptiOutput)
//								{
//									double currentVTKPoint[3];
//									currentVTKPoint[0] = pos[0];
//									currentVTKPoint[1] = pos[1];
//									currentVTKPoint[2] = pos[2];
//
//									vtkIdType pIndex = m_PointContainerDebugPoints->InsertNextPoint(&currentVTKPoint[0]);
//									m_VectorsOptiDebug->InsertNextTuple3(0,0,0);
//									m_ScalarsOptiDebug->InsertNextTuple1(pIndex);
//
//
//								}
//
//
//								RecursiveClosestPointOnSurfaceTriangle(currentTriId, 1e10, tempPosition, in, currentLocalCoords, finalTriangleIndex, tempDistance, trianglesVisited,printOptiOutput);
//								FindBestNormalFitOnSurfaceTriangle(finalTriangleIndex, tempPosition, in, currentLocalCoords, tempDistance, tempError, tempAngle, differenceVector);
//								if(printOptiOutput)
//								{
//									std::cout<<"final distance"<<tempDistance<<"\n";
//								}
//								//RecursiveFindBestNormalFitOnSurfaceTriangle(currentTriId, 1e10, tempPosition, in, currentLocalCoords, finalTriangleIndex, tempDistance, tempError, tempAngle,  trianglesVisited,printOptiOutput);
//
//								//unsigned int startTriangleId, double currentMinError, sofa::defaulttype::Vector3 & referencePosition, const typename In::VecCoord& in, sofa::defaulttype::Vector3& localSurfaceCoords, unsigned int & finalTriangleIndex, double & distance,  std::set<unsigned int> &trianglesVisited, bool printDebugOutput);
//
//								//if(tempDistance<distance)
//								{
//									tetraId = theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(finalTriangleIndex)[0];
//									currentTrianglesInTetrahedron = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(tetraId);
//									int faceNr = theTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, finalTriangleIndex);
//									LocalSurfaceCoordsToLocalVolumeCoords(currentLocalCoords.ptr(), faceNr, coefs.ptr());
//									distance = tempDistance;
//									index = c1+tetraId;
//								}


								if(printOptiOutput)
								{
									//save the vtk debug output
									const char* filenamePointDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/PointDataDebug.vtk";
									const char* filenameTriangleDebug = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/TriangleDataDebug.vtk";

									//std::cout<<"PointNumber: "<<i<<", Updated distance:"<<distance<<", final tetraId: "<<index-c1<<" ,coefs:"<<coefs<<"Final tetra:"<<theTopology->getQuadTetrahedra(index-c1)<<"\n";

									vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
									myGridTopoDebug->SetPoints(m_PointContainerDebugTriangles);
									myGridTopoDebug->SetCells(VTK_TRIANGLE, m_TriangleContainerDebugTriangles);
									myGridTopoDebug->GetCellData()->SetScalars(m_ScalarsDebugTriangles);

									vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();
									writerTopoDebug->SetInput(myGridTopoDebug);
									writerTopoDebug->SetFileName(filenameTriangleDebug);
									writerTopoDebug->Write();

									myGridTopoDebug->Delete();
									writerTopoDebug->Delete();

									vtkPolyData* myGrid2 = vtkPolyData::New();
									myGrid2->SetPoints(m_PointContainerDebugPoints);
									myGrid2->GetPointData()->SetVectors(m_VectorsOptiDebug);
									myGrid2->GetPointData()->SetScalars(m_ScalarsOptiDebug);

									vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
									vtkSmartPointer<vtkVertexGlyphFilter>::New();
									vertexGlyphFilter->AddInput(myGrid2);
									vertexGlyphFilter->Update();

									vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();

									writer2->SetInput(vertexGlyphFilter->GetOutput());
									writer2->SetFileName(filenamePointDebug);
									writer2->Write();

									myGrid2->Delete();
									writer2->Delete();




								}




//
//							} //loop bounding box





						//} //if quadtetra

//					for ( unsigned int i=0;i<out.size();i++ )
//					{
//						if(i==15632)//7643)//1457)//261)
//							printOptiOutput = true;
//						else
//							printOptiOutput = false;
//
//						Vector3 pos = Out::getCPos(out[i]);
//						Vector3 coefs;
//						int index = -1;
//						double distance = 1e10;
//						bool isOutside = true;
//						bool outsideBoundingBox = true;
//						double error = 1e10;
//						//double finalNormalCorrection[3];
//						for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//						{
//							Vector3 v = bases[t] * ( pos - in[tetrahedra[t][0]] );
//							double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( -v[2],v[0]+v[1]+v[2]-1 ) );
//							if ( d>0 ) d = ( pos-centers[t] ).norm2();
//							if ( d<distance ) {coefs = v; distance = d; index = t;}
//						}
//
//						for ( unsigned int c = 0; c < cubes.size(); c++ )
//						{
//							Vector3 v = bases[c0+c] * ( pos - in[cubes[c][0]] );
//							double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( std::max ( -v[2],v[0]-1 ),std::max ( v[1]-1,v[2]-1 ) ) );
//							if ( d>0 ) d = ( pos-centers[c0+c] ).norm2();
//							if ( d<distance ) {coefs = v; distance = d; index = c0+c;}
//						}
//						if ( distance>0 )
//						{
//							++outside;
//						}
//						if ( index < c1 )
//						addPointInTetra ( index, coefs.ptr() );
//						else
//						{
//							if(index < c0)
//							{
//								Vector3 normalCorrection;
//								addPointInQuadTetra( index-c1, coefs.ptr(), distance, isOutside, normalCorrection.ptr() );
//
//
//						}
//
//						//}
//						else
//						addPointInCube ( index-c0, coefs.ptr() );
//					}
//				} //loop over in points


			}
		}

		template <class In, class Out>
		int MapperQuadraticTetrahedronSetTopology<In,Out>::addContactPointFromInputMapping(const typename In::VecDeriv& in, const sofa::defaulttype::Vector3& _pos, std::vector< std::pair<int, double> > & /*baryCoords*/)
		{
//			updateJ = true;
			int retValue = 0;

			const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
#ifdef SOFA_NEW_HEXA
			const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
#else
			const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
#endif
			const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();

			sofa::helper::vector<Matrix3> bases;
			sofa::helper::vector<Vector3> centers;

			if ( tetrahedra.empty() && cubes.empty() )
			{
//				if ( triangles.empty() && quads.empty() )
//				{
//					//no 3D elements, nor 2D elements -> map on 1D elements
//
//					const sofa::core::topology::BaseMeshTopology::SeqEdges& edges = this->fromTopology->getEdges();
//					if ( edges.empty() )
//					return retValue;
//
//					sofa::helper::vector< SReal > lengthEdges;
//					sofa::helper::vector< Vector3 > unitaryVectors;
//
//					unsigned int e;
//					for ( e=0;e<edges.size(); e++ )
//					{
//						lengthEdges.push_back ( ( in[edges[e][1]]-in[edges[e][0]] ).norm() );
//
//						Vector3 V12 = ( in[edges[e][1]]-in[edges[e][0]] );
//						V12.normalize();
//						unitaryVectors.push_back ( V12 );
//					}
//
//					SReal coef=0;
//					for ( e=0;e<edges.size(); e++ )
//					{
//						SReal lengthEdge = lengthEdges[e];
//						Vector3 V12 = unitaryVectors[e];
//
//						coef = ( V12 ) *Vector3 ( _pos - in[edges[e][0]] ) / lengthEdge;
//						if ( coef >= 0 && coef <= 1 )
//						{
//							retValue = addPointInLine ( e,&coef );
//							break;
//						}
//					}
//					//If no good coefficient has been found, we add to the last element
//					if ( e == edges.size() )
//					retValue = addPointInLine ( edges.size()-1,&coef );
//				}
//				else
//				{
//					// no 3D elements -> map on 2D elements
//					int c0 = triangles.size();
//					bases.resize ( triangles.size() + quads.size() );
//					centers.resize ( triangles.size() + quads.size() );
//
//					for ( unsigned int t = 0; t < triangles.size(); t++ )
//					{
//						Mat3x3d m,mt;
//						m[0] = in[triangles[t][1]]-in[triangles[t][0]];
//						m[1] = in[triangles[t][2]]-in[triangles[t][0]];
//						m[2] = cross ( m[0],m[1] );
//						mt.transpose ( m );
//						bases[t].invert ( mt );
//						centers[t] = ( in[triangles[t][0]]+in[triangles[t][1]]+in[triangles[t][2]] ) /3;
//					}
//
//					for ( unsigned int c = 0; c < quads.size(); c++ )
//					{
//						Mat3x3d m,mt;
//						m[0] = in[quads[c][1]]-in[quads[c][0]];
//						m[1] = in[quads[c][3]]-in[quads[c][0]];
//						m[2] = cross ( m[0],m[1] );
//						mt.transpose ( m );
//						bases[c0+c].invert ( mt );
//						centers[c0+c] = ( in[quads[c][0]]+in[quads[c][1]]+in[quads[c][2]]+in[quads[c][3]] ) *0.25;
//					}
//
//					Vector3 coefs;
//					int index = -1;
//					double distance = 1e10;
//
//					for ( unsigned int t = 0; t < triangles.size(); t++ )
//					{
//						Vec3d v = bases[t] * ( _pos - in[triangles[t][0]] );
//						double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( ( v[2]<0?-v[2]:v[2] )-0.01,v[0]+v[1]-1 ) );
//						if ( d>0 ) d = ( _pos-centers[t] ).norm2();
//						if ( d<distance ) {coefs = v; distance = d; index = t;}
//					}
//
//					for ( unsigned int c = 0; c < quads.size(); c++ )
//					{
//						Vec3d v = bases[c0+c] * ( _pos - in[quads[c][0]] );
//						double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( std::max ( v[1]-1,v[0]-1 ),std::max ( v[2]-0.01,-v[2]-0.01 ) ) );
//						if ( d>0 ) d = ( _pos-centers[c0+c] ).norm2();
//						if ( d<distance ) {coefs = v; distance = d; index = c0+c;}
//					}
//
//					if ( index < c0 )
//					retValue = addPointInTriangle ( index, coefs.ptr() );
//					else
//					retValue = addPointInQuad ( index-c0, coefs.ptr() );
//				}
			}
			else
			{
				int c0 = tetrahedra.size();
				bases.resize ( tetrahedra.size() +cubes.size() );
				centers.resize ( tetrahedra.size() +cubes.size() );

				for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
				{
					Mat3x3d m,mt;
					m[0] = in[tetrahedra[t][1]]-in[tetrahedra[t][0]];
					m[1] = in[tetrahedra[t][2]]-in[tetrahedra[t][0]];
					m[2] = in[tetrahedra[t][3]]-in[tetrahedra[t][0]];
					mt.transpose ( m );
					bases[t].invert ( mt );
					centers[t] = ( in[tetrahedra[t][0]]+in[tetrahedra[t][1]]+in[tetrahedra[t][2]]+in[tetrahedra[t][3]] ) *0.25;
				}

//				for ( unsigned int c = 0; c < cubes.size(); c++ )
//				{
//					Mat3x3d m,mt;
//					m[0] = in[cubes[c][1]]-in[cubes[c][0]];
//#ifdef SOFA_NEW_HEXA
//					m[1] = in[cubes[c][3]]-in[cubes[c][0]];
//#else
//					m[1] = in[cubes[c][2]]-in[cubes[c][0]];
//#endif
//					m[2] = in[cubes[c][4]]-in[cubes[c][0]];
//					mt.transpose ( m );
//					bases[c0+c].invert ( mt );
//					centers[c0+c] = ( in[cubes[c][0]]+in[cubes[c][1]]+in[cubes[c][2]]+in[cubes[c][3]]+in[cubes[c][4]]+in[cubes[c][5]]+in[cubes[c][6]]+in[cubes[c][7]] ) *0.125;
//				}

				Vector3 coefs;
				int index = -1;
				double distance = 1e10;

				for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
				{
					Vector3 v = bases[t] * ( _pos - in[tetrahedra[t][0]] );
					double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( -v[2],v[0]+v[1]+v[2]-1 ) );
					if ( d>0 )
					d = ( _pos-centers[t] ).norm2();
					if ( d<distance )
					{
						coefs = v;
						distance = d;
						index = t;
					}
				}

//				for ( unsigned int c = 0; c < cubes.size(); c++ )
//				{
//					Vector3 v = bases[c0+c] * ( _pos - in[cubes[c][0]] );
//					double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( std::max ( -v[2],v[0]-1 ),std::max ( v[1]-1,v[2]-1 ) ) );
//					if ( d>0 )
//					d = ( _pos-centers[c0+c] ).norm2();
//					if ( d<distance )
//					{
//						coefs = v;
//						distance = d;
//						index = c0+c;
//					}
//				}

				if ( index < c0 )
				retValue = addPointInTetra ( index, coefs.ptr() );
//				else
//				retValue = addPointInCube ( index-c0, coefs.ptr() );
			}

			return retValue;
		}

//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::clear ( int reserve )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.clear(); if ( reserve>0 ) vectorData.reserve ( reserve );
//			map.endEdit();
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperEdgeSetTopology<In,Out>::addPointInLine ( const int edgeIndex, const SReal* baryCoords )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.resize ( map.getValue().size() +1 );
//			map.endEdit();
//			MappingData& data = *vectorData.rbegin();
//			data.in_index = edgeIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			return map.getValue().size()-1;
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperEdgeSetTopology<In,Out>::createPointInLine ( const typename Out::Coord& p, int edgeIndex, const typename In::VecCoord* points )
//		{
//			SReal baryCoords[1];
//			const topology::Edge& elem = this->fromTopology->getEdge ( edgeIndex );
//			const typename In::Coord p0 = ( *points ) [elem[0]];
//			const typename In::Coord pA = ( *points ) [elem[1]] - p0;
//			typename In::Coord pos = Out::getCPos(p) - p0;
//			baryCoords[0] = dot ( pA,pos ) /dot ( pA,pA );
//			return this->addPointInLine ( edgeIndex, baryCoords );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::init ( const typename Out::VecCoord& /*out*/, const typename In::VecCoord& /*in*/)
//		{
//			_fromContainer->getContext()->get ( _fromGeomAlgo );
//			if (this->toTopology)
//			{
//				map.createTopologicalEngine(this->toTopology);
//				map.registerTopologicalData();
//			}
//			//  int outside = 0;
//			//  const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//			//TODO: implementation of BarycentricMapperEdgeSetTopology::init
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::clear ( int reserve )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.clear(); if ( reserve>0 ) vectorData.reserve ( reserve );
//			map.endEdit();
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperTriangleSetTopology<In,Out>::addPointInTriangle ( const int triangleIndex, const SReal* baryCoords )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.resize ( map.getValue().size() +1 );
//			MappingData& data = *vectorData.rbegin();
//			map.endEdit();
//			data.in_index = triangleIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			data.baryCoords[1] = ( Real ) baryCoords[1];
//			return map.getValue().size()-1;
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperTriangleSetTopology<In,Out>::createPointInTriangle ( const typename Out::Coord& p, int triangleIndex, const typename In::VecCoord* points )
//		{
//			SReal baryCoords[2];
//			const topology::Triangle& elem = this->fromTopology->getTriangle ( triangleIndex );
//			const typename In::Coord p0 = ( *points ) [elem[0]];
//			const typename In::Coord pA = ( *points ) [elem[1]] - p0;
//			const typename In::Coord pB = ( *points ) [elem[2]] - p0;
//			typename In::Coord pos = Out::getCPos(p) - p0;
//			// First project to plane
//			typename In::Coord normal = cross ( pA, pB );
//			Real norm2 = normal.norm2();
//			pos -= normal* ( ( pos*normal ) /norm2 );
//			baryCoords[0] = ( Real ) sqrt ( cross ( pB, pos ).norm2() / norm2 );
//			baryCoords[1] = ( Real ) sqrt ( cross ( pA, pos ).norm2() / norm2 );
//			return this->addPointInTriangle ( triangleIndex, baryCoords );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::init ( const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			_fromContainer->getContext()->get ( _fromGeomAlgo );
//			if (this->toTopology)
//			{
//				map.createTopologicalEngine(this->toTopology);
//				map.registerTopologicalData();
//			}
//
//			int outside = 0;
//
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//			sofa::helper::vector<Mat3x3d> bases;
//			sofa::helper::vector<Vector3> centers;
//
//			// no 3D elements -> map on 2D elements
//			clear ( out.size() ); // reserve space for 2D mapping
//			bases.resize ( triangles.size() );
//			centers.resize ( triangles.size() );
//
//			for ( unsigned int t = 0; t < triangles.size(); t++ )
//			{
//				Mat3x3d m,mt;
//				m[0] = in[triangles[t][1]]-in[triangles[t][0]];
//				m[1] = in[triangles[t][2]]-in[triangles[t][0]];
//				m[2] = cross ( m[0],m[1] );
//				mt.transpose ( m );
//				bases[t].invert ( mt );
//				centers[t] = ( in[triangles[t][0]]+in[triangles[t][1]]+in[triangles[t][2]] ) /3;
//			}
//
//			for ( unsigned int i=0;i<out.size();i++ )
//			{
//				Vec3d pos = Out::getCPos(out[i]);
//				Vector3 coefs;
//				int index = -1;
//				double distance = 1e10;
//				for ( unsigned int t = 0; t < triangles.size(); t++ )
//				{
//					Vec3d v = bases[t] * ( pos - in[triangles[t][0]] );
//					double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( ( v[2]<0?-v[2]:v[2] )-0.01,v[0]+v[1]-1 ) );
//					if ( d>0 ) d = ( pos-centers[t] ).norm2();
//					if ( d<distance ) {coefs = v; distance = d; index = t;}
//				}
//				if ( distance>0 )
//				{
//					++outside;
//				}
//				addPointInTriangle ( index, coefs.ptr() );
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::clear ( int reserve )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.clear(); if ( reserve>0 ) vectorData.reserve ( reserve );
//			map.beginEdit();
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperQuadSetTopology<In,Out>::addPointInQuad ( const int quadIndex, const SReal* baryCoords )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.resize ( map.getValue().size() +1 );
//			MappingData& data = *vectorData.rbegin();
//			map.endEdit();
//			data.in_index = quadIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			data.baryCoords[1] = ( Real ) baryCoords[1];
//			return map.getValue().size()-1;
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperQuadSetTopology<In,Out>::createPointInQuad ( const typename Out::Coord& p, int quadIndex, const typename In::VecCoord* points )
//		{
//			SReal baryCoords[2];
//			const topology::Quad& elem = this->fromTopology->getQuad ( quadIndex );
//			const typename In::Coord p0 = ( *points ) [elem[0]];
//			const typename In::Coord pA = ( *points ) [elem[1]] - p0;
//			const typename In::Coord pB = ( *points ) [elem[3]] - p0;
//			typename In::Coord pos = Out::getCPos(p) - p0;
//			Mat<3,3,typename In::Real> m,mt,base;
//			m[0] = pA;
//			m[1] = pB;
//			m[2] = cross ( pA, pB );
//			mt.transpose ( m );
//			base.invert ( mt );
//			const typename In::Coord base0 = base[0];
//			const typename In::Coord base1 = base[1];
//			baryCoords[0] = base0 * pos;
//			baryCoords[1] = base1 * pos;
//			return this->addPointInQuad ( quadIndex, baryCoords );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::init ( const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			_fromContainer->getContext()->get ( _fromGeomAlgo );
//			if (this->toTopology)
//			{
//				map.createTopologicalEngine(this->toTopology);
//				map.registerTopologicalData();
//			}
//
//			int outside = 0;
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//
//			sofa::helper::vector<Matrix3> bases;
//			sofa::helper::vector<Vector3> centers;
//
//			clear ( out.size() );
//			bases.resize ( quads.size() );
//			centers.resize ( quads.size() );
//
//			for ( unsigned int c = 0; c < quads.size(); c++ )
//			{
//				Mat3x3d m,mt;
//				m[0] = in[quads[c][1]]-in[quads[c][0]];
//				m[1] = in[quads[c][3]]-in[quads[c][0]];
//				m[2] = cross ( m[0],m[1] );
//				mt.transpose ( m );
//				bases[c].invert ( mt );
//				centers[c] = ( in[quads[c][0]]+in[quads[c][1]]+in[quads[c][2]]+in[quads[c][3]] ) *0.25;
//			}
//
//			for ( unsigned int i=0;i<out.size();i++ )
//			{
//				Vec3d pos = Out::getCPos(out[i]);
//				Vector3 coefs;
//				int index = -1;
//				double distance = 1e10;
//				for ( unsigned int c = 0; c < quads.size(); c++ )
//				{
//					Vec3d v = bases[c] * ( pos - in[quads[c][0]] );
//					double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( std::max ( v[1]-1,v[0]-1 ),std::max ( v[2]-0.01,-v[2]-0.01 ) ) );
//					if ( d>0 ) d = ( pos-centers[c] ).norm2();
//					if ( d<distance ) {coefs = v; distance = d; index = c;}
//				}
//				if ( distance>0 )
//				{
//					++outside;
//				}
//				addPointInQuad ( index, coefs.ptr() );
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::clear ( int reserve )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.clear(); if ( reserve>0 ) vectorData.reserve ( reserve );
//			map.endEdit();
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperTetrahedronSetTopology<In,Out>::addPointInTetra ( const int tetraIndex, const SReal* baryCoords )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.resize ( map.getValue().size() +1 );
//			MappingData& data = *vectorData.rbegin();
//			map.endEdit();
//			data.in_index = tetraIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			data.baryCoords[1] = ( Real ) baryCoords[1];
//			data.baryCoords[2] = ( Real ) baryCoords[2];
//			return map.getValue().size()-1;
//		}
//
//		//template <class In, class Out>
//		//int BarycentricMapperTetrahedronSetTopology<In,Out>::createPointInTetra(const typename Out::Coord& p, int index, const typename In::VecCoord* points)
//		//{
//		//  //TODO: add implementation
//		//}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::init ( const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			_fromContainer->getContext()->get ( _fromGeomAlgo );
//			if (this->toTopology)
//			{
//				map.createTopologicalEngine(this->toTopology);
//				map.registerTopologicalData();
//			}
//
//			int outside = 0;
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			sofa::helper::vector<Matrix3> bases;
//			sofa::helper::vector<Vector3> centers;
//
//			clear ( out.size() );
//			bases.resize ( tetrahedra.size() );
//			centers.resize ( tetrahedra.size() );
//			for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//			{
//				Mat3x3d m,mt;
//				m[0] = in[tetrahedra[t][1]]-in[tetrahedra[t][0]];
//				m[1] = in[tetrahedra[t][2]]-in[tetrahedra[t][0]];
//				m[2] = in[tetrahedra[t][3]]-in[tetrahedra[t][0]];
//				mt.transpose ( m );
//				bases[t].invert ( mt );
//				centers[t] = ( in[tetrahedra[t][0]]+in[tetrahedra[t][1]]+in[tetrahedra[t][2]]+in[tetrahedra[t][3]] ) *0.25;
//			}
//
//			for ( unsigned int i=0;i<out.size();i++ )
//			{
//				Vec3d pos = Out::getCPos(out[i]);
//				Vector3 coefs;
//				int index = -1;
//				double distance = 1e10;
//				for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//				{
//					Vec3d v = bases[t] * ( pos - in[tetrahedra[t][0]] );
//					double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( -v[2],v[0]+v[1]+v[2]-1 ) );
//					if ( d>0 ) d = ( pos-centers[t] ).norm2();
//					if ( d<distance ) {coefs = v; distance = d; index = t;}
//				}
//				if ( distance>0 )
//				{
//					++outside;
//				}
//				addPointInTetra ( index, coefs.ptr() );
//			}
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperTetrahedronSetTopology<In,Out>::addContactPointFromInputMapping(const typename In::VecDeriv& in, const sofa::defaulttype::Vector3& pos
//				, std::vector< std::pair<int, double> > & /*baryCoords*/)
//		{
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			sofa::helper::vector<Matrix3> bases;
//			sofa::helper::vector<Vector3> centers;
//
//			bases.resize ( tetrahedra.size() );
//			centers.resize ( tetrahedra.size() );
//			for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//			{
//				Mat3x3d m,mt;
//				m[0] = in[tetrahedra[t][1]]-in[tetrahedra[t][0]];
//				m[1] = in[tetrahedra[t][2]]-in[tetrahedra[t][0]];
//				m[2] = in[tetrahedra[t][3]]-in[tetrahedra[t][0]];
//				mt.transpose ( m );
//				bases[t].invert ( mt );
//				centers[t] = ( in[tetrahedra[t][0]]+in[tetrahedra[t][1]]+in[tetrahedra[t][2]]+in[tetrahedra[t][3]] ) *0.25;
//			}
//
//			Vector3 coefs;
//			int index = -1;
//			double distance = 1e10;
//			for ( unsigned int t = 0; t < tetrahedra.size(); t++ )
//			{
//				Vec3d v = bases[t] * ( pos - in[tetrahedra[t][0]] );
//				double d = std::max ( std::max ( -v[0],-v[1] ),std::max ( -v[2],v[0]+v[1]+v[2]-1 ) );
//
//				if (d > 0)
//				d = (pos - centers[t]).norm2();
//
//				if (d < distance)
//				{
//					coefs = v;
//					distance = d;
//					index = t;
//				}
//			}
//
//			addPointInTetra (index, coefs.ptr() );
//
//			return map.getValue().size() - 1;
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::clear ( int reserve )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.clear();
//			if ( reserve>0 ) vectorData.reserve ( reserve );
//			map.endEdit();
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperHexahedronSetTopology<In,Out>::addPointInCube ( const int cubeIndex, const SReal* baryCoords )
//		{
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			vectorData.resize ( map.getValue().size() +1 );
//			MappingData& data = *vectorData.rbegin();
//			map.endEdit();
//			data.in_index = cubeIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			data.baryCoords[1] = ( Real ) baryCoords[1];
//			data.baryCoords[2] = ( Real ) baryCoords[2];
//			return map.getValue().size()-1;
//		}
//
//		template <class In, class Out>
//		int BarycentricMapperHexahedronSetTopology<In,Out>::setPointInCube ( const int pointIndex,
//				const int cubeIndex,
//				const SReal* baryCoords )
//		{
//			if ( pointIndex >= ( int ) map.getValue().size() )
//			return -1;
//
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//			MappingData& data = vectorData[pointIndex];
//			data.in_index = cubeIndex;
//			data.baryCoords[0] = ( Real ) baryCoords[0];
//			data.baryCoords[1] = ( Real ) baryCoords[1];
//			data.baryCoords[2] = ( Real ) baryCoords[2];
//			map.endEdit();
//
//			if(cubeIndex == -1)
//			_invalidIndex.insert(pointIndex);
//			else
//			_invalidIndex.erase(pointIndex);
//
//			return pointIndex;
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::init ( const typename Out::VecCoord& out,
//				const typename In::VecCoord& /*in*/)
//		{
//			_fromContainer->getContext()->get ( _fromGeomAlgo );
//			if (this->toTopology)
//			{
//				map.createTopologicalEngine(this->toTopology);
//				map.registerTopologicalData();
//			}
//
//			if ( _fromGeomAlgo == NULL )
//			{
//				std::cerr << "Error [BarycentricMapperHexahedronSetTopology::init] cannot find GeometryAlgorithms component." << std::endl;
//			}
//
//			if ( !map.getValue().empty() ) return;
//
//			clear ( out.size() );
//
//			typename In::VecCoord coord;
//			helper::vector<int> elements ( out.size() );
//			helper::vector<Vector3> coefs ( out.size() );
//			helper::vector<Real> distances ( out.size() );
//
//			//coord.assign(out.begin(), out.end());
//			coord.resize ( out.size() );
//			for ( unsigned int i=0;i<out.size();++i ) coord[i] = Out::getCPos(out[i]);
//
//			_fromGeomAlgo->findNearestElementsInRestPos ( coord, elements, coefs, distances );
//
//			for ( unsigned int i=0; i<elements.size(); ++i )
//			{
//				if ( elements[i] != -1 )
//				addPointInCube ( elements[i], coefs[i].ptr() );
//				else
//				std::cerr << "Error [BarycentricMapperHexahedronSetTopology::init] cannot find a cell for barycentric mapping." << std::endl;
//			}
//		}

//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::createMapperFromTopology ( BaseMeshTopology * topology )
//		{
//			using sofa::core::behavior::BaseMechanicalState;
//
//			mapper = NULL;
//
//			topology::PointSetTopologyContainer* toTopoCont;
//			this->toModel->getContext()->get(toTopoCont);
//
//			core::topology::TopologyContainer* fromTopoCont;
//			this->fromModel->getContext()->get(fromTopoCont);
//
//			BaseMechanicalState *dofFrom = static_cast< simulation::Node* >(this->fromModel->getContext())->mechanicalState;
//			BaseMechanicalState *dofTo = static_cast< simulation::Node* >(this->toModel->getContext())->mechanicalState;
//			helper::ParticleMask *maskFrom = &dofFrom->forceMask;
//			helper::ParticleMask *maskTo = NULL;
//
//			if (dofTo)
//			maskTo = &dofTo->forceMask;
//
////			if (fromTopoCont != NULL)
////			{
////				topology::HexahedronSetTopologyContainer* t1 = dynamic_cast< topology::HexahedronSetTopologyContainer* >(fromTopoCont);
////				if (t1 != NULL)
////				{
////					typedef BarycentricMapperHexahedronSetTopology<InDataTypes, OutDataTypes> HexahedronSetMapper;
////					mapper = sofa::core::objectmodel::New<HexahedronSetMapper>(t1, toTopoCont, maskFrom, maskTo);
////				}
////				else
////				{
////					topology::TetrahedronSetTopologyContainer* t2 = dynamic_cast<topology::TetrahedronSetTopologyContainer*>(fromTopoCont);
////					if (t2 != NULL)
////					{
////						typedef BarycentricMapperTetrahedronSetTopology<InDataTypes, OutDataTypes> TetrahedronSetMapper;
////						mapper = sofa::core::objectmodel::New<TetrahedronSetMapper>(t2, toTopoCont, maskFrom, maskTo);
////					}
////					else
////					{
////						topology::QuadSetTopologyContainer* t3 = dynamic_cast<topology::QuadSetTopologyContainer*>(fromTopoCont);
////						if (t3 != NULL)
////						{
////							typedef BarycentricMapperQuadSetTopology<InDataTypes, OutDataTypes> QuadSetMapper;
////							mapper = sofa::core::objectmodel::New<QuadSetMapper>(t3, toTopoCont, maskFrom, maskTo);
////						}
////						else
////						{
////							topology::TriangleSetTopologyContainer* t4 = dynamic_cast<topology::TriangleSetTopologyContainer*>(fromTopoCont);
////							if (t4 != NULL)
////							{
////								typedef BarycentricMapperTriangleSetTopology<InDataTypes, OutDataTypes> TriangleSetMapper;
////								mapper = sofa::core::objectmodel::New<TriangleSetMapper>(t4, toTopoCont, maskFrom, maskTo);
////							}
////							else
////							{
////								topology::EdgeSetTopologyContainer* t5 = dynamic_cast<topology::EdgeSetTopologyContainer*>(fromTopoCont);
////								if ( t5 != NULL )
////								{
////									typedef BarycentricMapperEdgeSetTopology<InDataTypes, OutDataTypes> EdgeSetMapper;
////									mapper = sofa::core::objectmodel::New<EdgeSetMapper>(t5, toTopoCont, maskFrom, maskTo);
////								}
////							}
////						}
////					}
////				}
////			}
////			else
////			{
////				using sofa::component::topology::RegularGridTopology;
////
////				RegularGridTopology* rgt = dynamic_cast< RegularGridTopology* >(topology);
////
////				if (rgt != NULL && rgt->isVolume())
////				{
////					typedef BarycentricMapperRegularGridTopology< InDataTypes, OutDataTypes > RegularGridMapper;
////
////					mapper = sofa::core::objectmodel::New<RegularGridMapper>(rgt, toTopoCont, maskFrom, maskTo);
////				}
////				else
////				{
////					using sofa::component::topology::SparseGridTopology;
////
////					SparseGridTopology* sgt = dynamic_cast< SparseGridTopology* >(topology);
////					if (sgt != NULL && sgt->isVolume())
////					{
////						typedef BarycentricMapperSparseGridTopology< InDataTypes, OutDataTypes > SparseGridMapper;
////						mapper = sofa::core::objectmodel::New<SparseGridMapper>(sgt, toTopoCont, maskFrom, maskTo);
////					}
////					else // generic MeshTopology
////					{
//						using sofa::core::topology::BaseMeshTopology;
//
//						typedef MapperQuadraticTetrahedronSetTopology< InDataTypes, OutDataTypes > MeshMapper;
//						BaseMeshTopology* bmt = dynamic_cast< BaseMeshTopology* >(topology);
//						mapper = sofa::core::objectmodel::New<MeshMapper>(bmt, toTopoCont, maskFrom, maskTo);
////					}
////				}
////			}
//			if (mapper)
//			{
//				mapper->setName("mapper");
//				this->addSlave(mapper.get());
//			}
//		}
//
//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::init()
//		{
//			topology_from = this->fromModel->getContext()->getMeshTopology();
//			topology_to = this->toModel->getContext()->getMeshTopology();
//
//			//IPB
//			//core::objectmodel::BaseContext* context = this->fromModel->getContext();
//			//->get(tetForceField);
//			//serr << "!!!!!!!!!!!! getDT = " <<  this->fromModel->getContext()->getDt() << sendl;
//			//IPE
//
//			if ( mapper == NULL ) // try to create a mapper according to the topology of the In model
//			{
//				if ( topology_from!=NULL )
//				{
//					createMapperFromTopology ( topology_from );
//				}
//			}
//
//			if ( mapper != NULL )
//			{
//				if (useRestPosition.getValue())
//				mapper->init ( *((const core::State<Out> *)this->toModel)->getX0(), *((const core::State<In> *)this->fromModel)->getX0() );
//				else
//				mapper->init ( *((const core::State<Out> *)this->toModel)->getX(), *((const core::State<In> *)this->fromModel)->getX() );
//			}
//			else
//			{
//				serr << "ERROR: Barycentric mapping does not understand topology."<<sendl;
//			}
//
//			Inherit::init();
//		}
//
//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::reinit()
//		{
//			if ( mapper != NULL )
//			{
//				mapper->clear();
//				mapper->init ( *((const core::State<Out> *)this->toModel)->getX(), *((const core::State<In> *)this->fromModel)->getX() );
//			}
//		}
//
//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::apply(const core::MechanicalParams * /*mparams*//* PARAMS FIRST */, Data< typename Out::VecCoord >& out, const Data< typename In::VecCoord >& in)
//		{
//			if (
//					mapper != NULL)
//			{
//				mapper->apply(*out.beginEdit(), in.getValue());
//				out.endEdit();
//			}
//		}

		//static void CalculateGlobalCoords(double* localCoords, double* globalCoords, int m, int n, void* nodes)
		//{
		//	double fx = localCoords[0];
		//	double fy = localCoords[1];
		//	double fz = localCoords[2];
		//
		//	double tempExp = 1-fx-fy-fz;
		//
		//	for(int i=0;i<3;i++)
		//	{
		//		globalCoords[i] = 0;
		//
		//		globalCoords[i] += ((double*)nodes)[10*i+0]*(2*fx-1)*fx;
		//		globalCoords[i] += ((double*)nodes)[10*i+1]*(2*fy-1)*fy;
		//		globalCoords[i] += ((double*)nodes)[10*i+2]*(2*tempExp-1)*tempExp;
		//		globalCoords[i] += ((double*)nodes)[10*i+3]*(2*fz-1)*fz;
		//		globalCoords[i] += ((double*)nodes)[10*i+4]*4*fx*fy;
		//		globalCoords[i] += ((double*)nodes)[10*i+5]*4*tempExp*fy;
		//		globalCoords[i] += ((double*)nodes)[10*i+6]*4*tempExp*fx;
		//		globalCoords[i] += ((double*)nodes)[10*i+7]*4*fx*fz;
		//		globalCoords[i] += ((double*)nodes)[10*i+8]*4*fy*fz;
		//		globalCoords[i] += ((double*)nodes)[10*i+9]*4*tempExp*fz;
		//
		//		//globalCoords[i]=10;
		//	}
		//
		//
		//}

		template <class In, class Out>
		void MapperQuadraticTetrahedronSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
		{


			out.resize (map3d.size() );
			const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
			sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra dummyQuadTetras;
			sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra& quadraticTetrahedra = dummyQuadTetras;
			if((dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology)))
			{
				quadraticTetrahedra = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology))->getQuadTetrahedra();
			}

#ifdef SOFA_NEW_HEXA
			const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
#else
			const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
#endif
			// 1D elements
//			{
//				for ( unsigned int i=0;i<map1d.size();i++ )
//				{
//					const Real fx = map1d[i].baryCoords[0];
//					int index = map1d[i].in_index;
//					{
//						const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
//						Out::setCPos(out[i] , in[line[0]] * ( 1-fx )
//								+ in[line[1]] * fx );
//					}
//				}
//			}
			// 2D elements
//			{
//				const int i0 = map1d.size();
//				const int c0 = triangles.size();
//				for ( unsigned int i=0;i<map2d.size();i++ )
//				{
//					const Real fx = map2d[i].baryCoords[0];
//					const Real fy = map2d[i].baryCoords[1];
//					int index = map2d[i].in_index;
//					if ( index<c0 )
//					{
//						const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
//						Out::setCPos(out[i+i0] , in[triangle[0]] * ( 1-fx-fy )
//								+ in[triangle[1]] * fx
//								+ in[triangle[2]] * fy );
//					}
//					else
//					{
//						const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
//						Out::setCPos(out[i+i0] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
//								+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
//								+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
//								+ in[quad[2]] * ( ( fx ) * ( fy ) ) );
//					}
//				}
//			}
			// 3D elements
			{
				const int i0 = 0;//map1d.size() + map2d.size();
				const int c0 = tetrahedra.size() + quadraticTetrahedra.size();
				const int c1 = tetrahedra.size();

				if(quadraticTetrahedra.size())
				{

					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					//get surface points
					vector<sofa::component::topology::QuadraticMeshTopology::PointID>  surfaceNodes = theTopology->getSurfaceNodes();
					sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles;
					unsigned int currentTetraId;
					unsigned int localNodeId;

					unsigned int currentGlobalNodeId;
					unsigned int currentDebugGlobalNodeId;
					double nodes[30];
					double triNodes[18];
					double currentLocalCoords[3];
					Vector3 tempNormal;
					Vector3 averagedNormal;
					Vector3 averagedNormalME;
					int faceNumber;
					sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron  currentTrianglesInTetrahedron;



					this->m_NodeNormals.clear();
					this->m_NodeNormals.resize(theTopology->getNbPoints());//surfaceNodes.size()); //nb nodes
					//std::cout<<"NodeNormalSize: "<<surfaceNodes.size()<<"\n";

					///////////////////////////////////////////////////

					//for each point:
					for ( unsigned int nodeIter = 0; nodeIter < surfaceNodes.size(); nodeIter++ )
					{
						//get triangles around node
						//triangles = theTopology->getSurfaceQuadraticTrianglesAroundSurfaceNode(nodeIter);
						currentGlobalNodeId = surfaceNodes[nodeIter];
						triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(currentGlobalNodeId );
						averagedNormal = Vector3(0.0,0.0,0.0);
						averagedNormalME = Vector3(0.0,0.0,0.0);

						bool averageVerbose = false;

						//for each triangle:
						for ( unsigned int triIter = 0; triIter < triangles.size(); triIter++ )
						{
							//try it triangle based

							//get triangle
							unsigned int currentTriangleId = triangles[triIter];
							const sofa::component::topology::QuadraticMeshTopology::QuadraticTriangle &quadTriangle = theTopology->getQuadraticTriangleVTK(currentTriangleId);

							//get local vertex id in triangle
							unsigned int localNodeId = theTopology->getVertexIndexInQuadraticTriangle(quadTriangle, currentGlobalNodeId);

							//get nodes
							for(int iteri=0; iteri<3; iteri++)
							{
								for(int p=0; p<6; p++)
								{
									triNodes[6*iteri+p] = (in[quadTriangle[p]])[iteri];
								}
							}

							//get normal
							CalculateSurfaceNormal(localNodeId, tempNormal, triNodes);

							//get weight
							double currentWeight = 1;

							//only use weighted average if node is on triangle edge
							if(localNodeId <3)
							{
								Vector3 edge1, edge2;

								unsigned int localNodeIdEdge1;
								unsigned int localNodeIdEdge2;

								if(localNodeId == 0)
								{
									localNodeIdEdge1 = 1;
									localNodeIdEdge2 = 2;
								}

								if(localNodeId == 1)
								{
									localNodeIdEdge1 = 2;
									localNodeIdEdge2 = 0;
								}

								if(localNodeId == 2)
								{
									localNodeIdEdge1 = 0;
									localNodeIdEdge2 = 1;
								}

								Vector3 pointEdge1 = (in[quadTriangle[localNodeIdEdge1]]);
								Vector3 pointEdge2 = (in[quadTriangle[localNodeIdEdge2]]);
								Vector3 currentPoint =  (in[currentGlobalNodeId]);

								edge1= pointEdge1-currentPoint;
								edge2= pointEdge2-currentPoint;

								double alpha = edge1*edge2;//(edge1.cross(edge2)).norm();
								alpha = alpha / (edge1.norm() * edge2.norm());
								alpha = acos(alpha)*180.0/3.14159;

								currentWeight = alpha;


								//std::cout<<"Edge1 "<<edge1<<", edge2"<<edge2<<" alpha="<<alpha<<"\n";


							}


							tempNormal.normalize();

	//						if(averageVerbose)
	//							std::cout<<"alpha"<<triIter<<"="<<currentWeight<<" associated normal "<<tempNormal;
	//						if(averageVerbose)
	//								std::cout<<"\n";
							averagedNormal += currentWeight*tempNormal;
	//						averagedNormalME += tempNormal;
						}

						averagedNormal.normalize();
	//					averagedNormalME.normalize();

	//					if(averageVerbose)
	//							std::cout<<"MA average normal is "<<averagedNormal<<", ME average normal is "<<averagedNormalME<<"\n";

						this->m_NodeNormals[currentGlobalNodeId] = averagedNormal;


					}
				}


//				//calculate normals if quadratic tetrahedra are present
//				if(quadraticTetrahedra.size())
//				{
//					sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
////					this->m_NodeNormals.clear();
////					this->m_NodeNormals.resize(theTopology->getNbPoints()); //nb nodes
//					//get surface points
//					vector<sofa::component::topology::QuadraticMeshTopology::PointID>  surfaceNodes = theTopology->getSurfaceNodes();
//					sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex triangles;
//					unsigned int currentTetraId;
//					unsigned int localNodeId;
//
//					unsigned int currentGlobalNodeId;
//					unsigned int currentDebugGlobalNodeId;
//					double nodes[30];
//					double currentLocalCoords[3];
//					Vector3 tempNormal;
//					Vector3 averagedNormal;
//					int faceNumber;
//					sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron  currentTrianglesInTetrahedron;
//
//
//					//for each point:
//					for ( unsigned int nodeIter = 0; nodeIter < surfaceNodes.size(); nodeIter++ )
//					{
//						//get triangles around node
//						//triangles = theTopology->getSurfaceQuadraticTrianglesAroundSurfaceNode(nodeIter);
//						currentGlobalNodeId = surfaceNodes[nodeIter];
//						triangles = theTopology->getSurfaceQuadraticTrianglesAroundNode(currentGlobalNodeId );
//						averagedNormal = Vector3(0.0,0.0,0.0);
//						//for each triangle:
//						for ( unsigned int triIter = 0; triIter < triangles.size(); triIter++ )
//						{
//							//get tetra number
//							currentTetraId = (theTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[triIter]))[0];
//
//							//get tetrahedron
//							const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra &quadTetra = quadraticTetrahedra[currentTetraId];
//
//							//get local node number in tetra
//							localNodeId = theTopology->getVertexIndexInTetrahedron(quadTetra, currentGlobalNodeId);
//
//							//get local face number
//							currentTrianglesInTetrahedron = theTopology->getQuadraticTrianglesInQuadraticTetrahedron(currentTetraId);
//							faceNumber = theTopology->getQuadraticTriangleIndexInTetrahedron(currentTrianglesInTetrahedron, triangles[triIter]);
//
//							//switch statement to find local coords from node number
//							switch (localNodeId)
//							{
//							case 0:
//								currentLocalCoords[0] = 1; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//								break;
//							case 1:
//								currentLocalCoords[0] = 0; currentLocalCoords[1] = 1; currentLocalCoords[2] = 0;
//								break;
//							case 2:
//								currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//								break;
//							case 3:
//								currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 1;
//								break;
//							case 4:
//								currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0;
//								break;
//							case 5:
//								currentLocalCoords[0] = 0; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0;
//								break;
//							case 6:
//								currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0;
//								break;
//							case 7:
//								currentLocalCoords[0] = 0.5; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0.5;
//								break;
//							case 8:
//								currentLocalCoords[0] = 0.0; currentLocalCoords[1] = 0.5; currentLocalCoords[2] = 0.5;
//								break;
//							case 9:
//								currentLocalCoords[0] = 0; currentLocalCoords[1] = 0; currentLocalCoords[2] = 0.5;
//								break;
//							default:
//								std::cout<<"LocalNOdeID: "<< localNodeId<<" Error, cant compute QuadTriangles\n";
//								break;
//							}
//
//							//get node coords
//							for(int iteri=0; iteri<3; iteri++)
//							{
//								for(int p=0; p<10; p++)
//								{
//									nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
//								}
//							}
//
//							CalculateSurfaceNormal(currentLocalCoords, tempNormal, nodes, faceNumber);
//
//							//calculate normal, add to completeNormal
//							tempNormal.normalize();
//
//							averagedNormal += tempNormal;
//						}
//						averagedNormal.normalize();
//						this->m_NodeNormals[currentGlobalNodeId] = averagedNormal;
//					}
//
//				}


//				for ( unsigned int i=0;i<map3d.size();i++ )
//				{
//					int index = map3d[i].in_index;
//					if(index == -1)
//						std::cout<<"Wrong index (-1) at point "<<i<<"\n";
//				}

				for ( unsigned int i=0;i<map3d.size();i++ )
				{
					const Real fx = map3d[i].baryCoords[0];
					const Real fy = map3d[i].baryCoords[1];
					const Real fz = map3d[i].baryCoords[2];
					int index = map3d[i].in_index;
					if ( index<c1 )
					{
						const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
						Out::setCPos(out[i+i0] , in[tetra[0]] * ( 1-fx-fy-fz )
								+ in[tetra[1]] * fx
								+ in[tetra[2]] * fy
								+ in[tetra[3]] * fz );
					}
					else
					{
						if(index < c0)
						{
							const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[index-c1];
//
//							//construct nodes
							double nodes[30];

							for(int iteri=0; iteri<3; iteri++)
							{
								for(int p=0; p<10; p++)
								{
									nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
								}
							}

							Vector3 currentPos;
							double localCoords[3];
							localCoords[0] = fx;
							localCoords[1] = fy;
							localCoords[2] = fz;
							CalculateGlobalCoords(localCoords, currentPos.ptr(), 3, 3, &nodes[0]);


							typename Out::Coord outPos =  currentPos;

							if(map3d[i].isOutside)
							{
								if(this->m_DisplacementBased.getValue())
								{

									double normals[30];

									for(int iteri=0; iteri<3; iteri++)
									{
										for(int p=0; p<10; p++)
										{
											normals[10*iteri+p] = (this->m_NodeNormals[quadTetra[p]])[iteri];
										}
									}
									Vector3 currentNormal;
									CalculateGlobalCoords(localCoords, currentNormal.ptr(), 3, 3, &normals[0]);
									currentNormal.normalize();

									Vector3 differenceNormal = (currentNormal-map3d[i].normal0);
									Vector3 differencePos = (outPos- map3d[i].pos0);
									outPos =  map3d[i].outPos0 + differencePos + differenceNormal*map3d[i].distance;
								}
								else
								{
									//construct normals
									double normals[30];

									for(int iteri=0; iteri<3; iteri++)
									{
										for(int p=0; p<10; p++)
										{
											normals[10*iteri+p] = (this->m_NodeNormals[quadTetra[p]])[iteri];
										}
									}
									Vector3 currentNormal;
									CalculateGlobalCoords(localCoords, currentNormal.ptr(), 3, 3, &normals[0]);
									currentNormal.normalize();
									outPos +=  currentNormal*map3d[i].distance;

									//std::cout<<"OUtside should not happen\n";
								}

							}


							Out::setCPos(out[i+i0], outPos);


//							//TODO
//							//					Out::setCPos(out[i+i0] , in[quadTetra[0]] * ( 1-fx-fy-fz )
//							//						+ in[quadTetra[1]] * fx
//							//						+ in[quadTetra[2]] * fy
//							//						+ in[quadTetra[3]] * fz );
//
//							double tempExp = 1-fx-fy-fz;
//
//							typename Out::Coord outPos = in[quadTetra[0]] * (2*fx-1)*fx
//							+ in[quadTetra[1]] * (2*fy-1)*fy
//							+ in[quadTetra[2]] * (2*tempExp-1)*tempExp
//							+ in[quadTetra[3]] * (2*fz-1)*fz
//							+ in[quadTetra[4]] * 4*fx*fy
//							+ in[quadTetra[5]] * 4*tempExp*fy
//							+ in[quadTetra[6]] * 4*tempExp*fx
//							+ in[quadTetra[7]] * 4*fx*fz
//							+ in[quadTetra[8]] * 4*fy*fz
//							+ in[quadTetra[9]] * 4*tempExp*fz;
							//
//															Out::setCPos(out[i+i0] , in[quadTetra[0]] * (2*fx-1)*fx
//																		+ in[quadTetra[1]] * (2*fy-1)*fy
//																		+ in[quadTetra[2]] * (2*tempExp-1)*tempExp
//																		+ in[quadTetra[3]] * (2*fz-1)*fz
//																		+ in[quadTetra[4]] * 4*fx*fy
//																		+ in[quadTetra[5]] * 4*tempExp*fy
//																		+ in[quadTetra[6]] * 4*tempExp*fx
//																		+ in[quadTetra[7]] * 4*fx*fz
//																		+ in[quadTetra[8]] * 4*fy*fz
//																		+ in[quadTetra[9]] * 4*tempExp*fz
//																);
//
//
//							if(map3d[i].isOutside)
//							{
//								//construct nodes
//								double nodes[30];
//
//								//std::cout<<"Nodes: \n";
//								for(int iteri=0; iteri<3; iteri++)
//								{
//									for(int p=0; p<10; p++)
//									{
//										nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
//									}
//								}
//
//								Vector3 normal, normalCorrection;
//								double localCoords[3];
//								localCoords[0] = fx;
//								localCoords[1] = fy;
//								localCoords[2] = fz;
//
//								//CalculateSurfaceNormal(localCoords, normal, nodes);
//								normal[0] += map3d[i].normalCorrection[0];
//								normal[1] += map3d[i].normalCorrection[1];
//								normal[2] += map3d[i].normalCorrection[2];
//
//
//								outPos+= normal*map3d[i].distance;
//
//
//							}
//
//							Out::setCPos(out[i+i0], outPos);

							//check outside points, test normal calculation
							//									const sofa::core::topology::QuadraticBaseMeshTopology::QuadTetra& quadTetra = quadraticTetrahedra[index];
							//
							//									//construct nodes
							//									double nodes[30];
							//
							//									//std::cout<<"Nodes: \n";
							//									for(int iteri=0; iteri<3; iteri++)
							//									{
							//										for(int p=0; p<10; p++)
							//										{
							//											nodes[10*iteri+p] = (in[quadTetra[p]])[iteri];
							//										}
							//									}
							//
							//									Vector3 normal, outPos;
							//
							//									CalculateSurfaceNormal(coefs, normal, nodes);
							//									CalculateGlobalCoords(coefs.ptr(), outPos.ptr(), 3, 3, &nodes[0]);
							//
							//									Vector3 pseudoNormal = pos-outPos;
							//									pseudoNormal.normalize();

							//								else
							//								{
							//									Out::setCPos(out[i+i0] , in[quadTetra[0]] * fx
							//											+ in[quadTetra[1]] * fy
							//											+ in[quadTetra[2]] * tempExp
							//											+ in[quadTetra[3]] * fz
							//									);
							//								}


						}
						else
						{
#ifdef SOFA_NEW_HEXA
							const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
#else
							const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
#endif
							Out::setCPos(out[i+i0] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
									+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
#ifdef SOFA_NEW_HEXA
									+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
									+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
#else
									+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
									+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
#endif
									+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
									+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
#ifdef SOFA_NEW_HEXA
									+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
									+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
#else
							+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
							+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
#endif
						}
					}
				}
			}
		}

//		template <class In, class Out>
//		void BarycentricMapperRegularGridTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.size() );
//			for ( unsigned int i=0;i<map.size();i++ )
//			{
//#ifdef SOFA_NEW_HEXA
//				const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[i].in_index );
//#else
//				const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[i].in_index );
//#endif
//				const Real fx = map[i].baryCoords[0];
//				const Real fy = map[i].baryCoords[1];
//				const Real fz = map[i].baryCoords[2];
//				Out::setCPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//				+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//				+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//				+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//				+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//				+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//				+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//				+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//				+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//				+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperSparseGridTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.size() );
//
//			typedef sofa::helper::vector< CubeData > CubeDataVector;
//			typedef typename CubeDataVector::const_iterator CubeDataVectorIt;
//
//			CubeDataVectorIt it = map.begin();
//			CubeDataVectorIt itEnd = map.end();
//
//			unsigned int i = 0;
//
//			while (it != itEnd)
//			{
//#ifdef SOFA_NEW_HEXA
//				const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron( it->in_index );
//#else
//				const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( it->in_index );
//#endif
//				const Real fx = it->baryCoords[0];
//				const Real fy = it->baryCoords[1];
//				const Real fz = it->baryCoords[2];
//
//				Out::setCPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//				+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//				+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//				+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//				+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//				+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//				+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//				+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//				+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//				+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//				++it;
//				++i;
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//			// 2D elements
//			helper::vector<MappingData>& vectorData = *(map.beginEdit());
//
//			for ( unsigned int i=0;i<map.getValue().size();i++ )
//			{
//				const Real fx = vectorData[i].baryCoords[0];
//				int index = vectorData[i].in_index;
//				const topology::Edge& edge = edges[index];
//				Out::setCPos(out[i] , in[edge[0]] * ( 1-fx )
//				+ in[edge[1]] * fx );
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//			for ( unsigned int i=0;i<map.getValue().size();i++ )
//			{
//				const Real fx = map.getValue()[i].baryCoords[0];
//				const Real fy = map.getValue()[i].baryCoords[1];
//				int index = map.getValue()[i].in_index;
//				const topology::Triangle& triangle = triangles[index];
//				Out::setCPos(out[i] , in[triangle[0]] * ( 1-fx-fy )
//				+ in[triangle[1]] * fx
//				+ in[triangle[2]] * fy );
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//			for ( unsigned int i=0;i<map.getValue().size();i++ )
//			{
//				const Real fx = map.getValue()[i].baryCoords[0];
//				const Real fy = map.getValue()[i].baryCoords[1];
//				int index = map.getValue()[i].in_index;
//				const topology::Quad& quad = quads[index];
//				Out::setCPos(out[i] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
//				+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
//				+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
//				+ in[quad[2]] * ( ( fx ) * ( fy ) ) );
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//			for ( unsigned int i=0;i<map.getValue().size();i++ )
//			{
//				const Real fx = map.getValue()[i].baryCoords[0];
//				const Real fy = map.getValue()[i].baryCoords[1];
//				const Real fz = map.getValue()[i].baryCoords[2];
//				int index = map.getValue()[i].in_index;
//				const topology::Tetrahedron& tetra = tetrahedra[index];
//				Out::setCPos(out[i] , in[tetra[0]] * ( 1-fx-fy-fz )
//				+ in[tetra[1]] * fx
//				+ in[tetra[2]] * fy
//				+ in[tetra[3]] * fz );
//			}
//			//serr<<"BarycentricMapperTetrahedronSetTopology<In,Out>::apply, in = "<<in<<sendl;
//			//serr<<"BarycentricMapperTetrahedronSetTopology<In,Out>::apply, out = "<<out<<sendl;
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::apply ( typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//			for ( unsigned int i=0;i<map.getValue().size();i++ )
//			{
//				const Real fx = map.getValue()[i].baryCoords[0];
//				const Real fy = map.getValue()[i].baryCoords[1];
//				const Real fz = map.getValue()[i].baryCoords[2];
//				int index = map.getValue()[i].in_index;
//				const topology::Hexahedron& cube = cubes[index];
//				Out::setCPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//				+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//				+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//				+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//				+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//				+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//				+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//			}
//		}
//
//		//-- test mapping partiel
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::applyOnePoint( const unsigned int& hexaPointId,typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			//out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//			const Real fx = map.getValue()[hexaPointId].baryCoords[0];
//			const Real fy = map.getValue()[hexaPointId].baryCoords[1];
//			const Real fz = map.getValue()[hexaPointId].baryCoords[2];
//			int index = map.getValue()[hexaPointId].in_index;
//			const topology::Hexahedron& cube = cubes[index];
//			Out::setCPos(out[hexaPointId] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//			+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//			+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//			+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//			+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//			+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//			+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//			+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//		}
		//--

//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::applyJ (const core::MechanicalParams * /*mparams*//* PARAMS FIRST */, Data< typename Out::VecDeriv >& _out, const Data< typename In::VecDeriv >& in)
//		{
//			typename Out::VecDeriv* out = _out.beginEdit();
//			out->resize(this->toModel->getX()->size());
//			if (mapper != NULL)
//			{
//				mapper->applyJ(*out, in.getValue());
//			}
//			_out.endEdit();
//		}

		template <class In, class Out>
		void MapperQuadraticTetrahedronSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
		{

			out.resize (map3d.size() );
			//	const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			//	const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			//	const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			//	const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
			//#ifdef SOFA_NEW_HEXA
			//	const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
			//#else
			//	const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
			//#endif
			//
			//	if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
			//	{
			//		//std::cout << "BarycentricMapper: applyJ without masks" << std::endl;
			//		// 1D elements
			//		{
			//			for ( unsigned int i=0;i<map1d.size();i++ )
			//			{
			//				const Real fx = map1d[i].baryCoords[0];
			//				int index = map1d[i].in_index;
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
			//					Out::setDPos(out[i] , in[line[0]] * ( 1-fx )
			//						+ in[line[1]] * fx );
			//				}
			//			}
			//		}
			//		// 2D elements
			//		{
			//			const int i0 = map1d.size();
			//			const int c0 = triangles.size();
			//			for ( unsigned int i=0;i<map2d.size();i++ )
			//			{
			//				const Real fx = map2d[i].baryCoords[0];
			//				const Real fy = map2d[i].baryCoords[1];
			//				int index = map2d[i].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
			//					Out::setDPos(out[i+i0] , in[triangle[0]] * ( 1-fx-fy )
			//						+ in[triangle[1]] * fx
			//						+ in[triangle[2]] * fy );
			//				}
			//				else
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
			//					Out::setDPos(out[i+i0] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
			//						+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
			//						+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
			//						+ in[quad[2]] * ( ( fx ) * ( fy ) ) );
			//				}
			//			}
			//		}
			//		// 3D elements
			//		{
			//			const int i0 = map1d.size() + map2d.size();
			//			const int c0 = tetrahedra.size();
			//			for ( unsigned int i=0;i<map3d.size();i++ )
			//			{
			//				const Real fx = map3d[i].baryCoords[0];
			//				const Real fy = map3d[i].baryCoords[1];
			//				const Real fz = map3d[i].baryCoords[2];
			//				int index = map3d[i].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
			//					Out::setDPos(out[i+i0] , in[tetra[0]] * ( 1-fx-fy-fz )
			//						+ in[tetra[1]] * fx
			//						+ in[tetra[2]] * fy
			//						+ in[tetra[3]] * fz );
			//				}
			//				else
			//				{
			//#ifdef SOFA_NEW_HEXA
			//					const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
			//#else
			//					const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
			//#endif
			//					Out::setDPos(out[i+i0] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
			//						+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
			//#ifdef SOFA_NEW_HEXA
			//						+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
			//						+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
			//#else
			//						+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
			//						+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
			//#endif
			//						+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
			//						+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
			//#ifdef SOFA_NEW_HEXA
			//						+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
			//						+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
			//#else
			//						+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
			//						+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
			//#endif
			//				}
			//			}
			//		}
			//	}
			//	else
			//	{
			//		//std::cout << "BarycentricMapper: applyJ with masks" << std::endl;
			//		typedef helper::ParticleMask ParticleMask;
			//		const ParticleMask::InternalStorage &indices=maskTo->getEntries();
			//
			//		const unsigned int sizeMap1d=map1d.size();
			//		const unsigned int sizeMap2d=map2d.size();
			//		const unsigned int sizeMap3d=map3d.size();
			//
			//		const unsigned int idxStart1=sizeMap1d;
			//		const unsigned int idxStart2=sizeMap1d+sizeMap2d;
			//		const unsigned int idxStart3=sizeMap1d+sizeMap2d+sizeMap3d;
			//
			//		ParticleMask::InternalStorage::const_iterator it;
			//		for (it=indices.begin();it!=indices.end();it++)
			//		{
			//			const unsigned int i=(*it);
			//			// 1D elements
			//			if (i < idxStart1)
			//			{
			//				const Real fx = map1d[i].baryCoords[0];
			//				int index = map1d[i].in_index;
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
			//					Out::setDPos(out[i] , in[line[0]] * ( 1-fx )
			//						+ in[line[1]] * fx );
			//				}
			//			}
			//			// 2D elements
			//			else if (i < idxStart2)
			//			{
			//				const int i0 = idxStart1;
			//				const int c0 = triangles.size();
			//
			//				const Real fx = map2d[i-i0].baryCoords[0];
			//				const Real fy = map2d[i-i0].baryCoords[1];
			//				int index = map2d[i-i0].in_index;
			//
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
			//					Out::setDPos(out[i] , in[triangle[0]] * ( 1-fx-fy )
			//						+ in[triangle[1]] * fx
			//						+ in[triangle[2]] * fy );
			//				}
			//				else
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
			//					Out::setDPos(out[i] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
			//						+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
			//						+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
			//						+ in[quad[2]] * ( ( fx ) * ( fy ) ) );
			//				}
			//			}
			//			// 3D elements
			//			else if (i < idxStart3)
			//			{
			//				const int i0 = idxStart2;
			//				const int c0 = tetrahedra.size();
			//				const Real fx = map3d[i-i0].baryCoords[0];
			//				const Real fy = map3d[i-i0].baryCoords[1];
			//				const Real fz = map3d[i-i0].baryCoords[2];
			//				int index = map3d[i-i0].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
			//					Out::setDPos(out[i] , in[tetra[0]] * ( 1-fx-fy-fz )
			//						+ in[tetra[1]] * fx
			//						+ in[tetra[2]] * fy
			//						+ in[tetra[3]] * fz );
			//				}
			//				else
			//				{
			//#ifdef SOFA_NEW_HEXA
			//					const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
			//#else
			//					const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
			//#endif
			//					Out::setDPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
			//						+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
			//#ifdef SOFA_NEW_HEXA
			//						+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
			//						+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
			//#else
			//						+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
			//						+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
			//#endif
			//						+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
			//						+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
			//#ifdef SOFA_NEW_HEXA
			//						+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
			//						+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
			//#else
			//						+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
			//						+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
			//#endif
			//				}
			//			}
			//		}
			//	}
		}

//		template <class In, class Out>
//		void BarycentricMapperRegularGridTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.size();i++ )
//				{
//#ifdef SOFA_NEW_HEXA
//					const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[i].in_index );
//#else
//					const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[i].in_index );
//#endif
//					const Real fx = map[i].baryCoords[0];
//					const Real fy = map[i].baryCoords[1];
//					const Real fz = map[i].baryCoords[2];
//					Out::setDPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//					+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//					+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const unsigned int index=(*it);
//#ifdef SOFA_NEW_HEXA
//					const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[index].in_index );
//#else
//					const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[index].in_index );
//#endif
//					const Real fx = map[index].baryCoords[0];
//					const Real fy = map[index].baryCoords[1];
//					const Real fz = map[index].baryCoords[2];
//					Out::setDPos(out[index] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//					+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//					+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperSparseGridTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.size();i++ )
//				{
//#ifdef SOFA_NEW_HEXA
//					const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[i].in_index );
//#else
//					const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[i].in_index );
//#endif
//					const Real fx = map[i].baryCoords[0];
//					const Real fy = map[i].baryCoords[1];
//					const Real fz = map[i].baryCoords[2];
//					Out::setDPos(out[i] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//					+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//					+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const unsigned int index=(*it);
//
//#ifdef SOFA_NEW_HEXA
//					const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[index].in_index );
//#else
//					const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[index].in_index );
//#endif
//					const Real fx = map[index].baryCoords[0];
//					const Real fy = map[index].baryCoords[1];
//					const Real fz = map[index].baryCoords[2];
//					Out::setDPos(out[index] , in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#else
//					+ in[cube[2]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//#endif
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//#ifdef SOFA_NEW_HEXA
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#else
//					+ in[cube[6]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//#endif
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					int index = map.getValue()[i].in_index;
//					const topology::Edge& edge = edges[index];
//					Out::setDPos(out[i] , in[edge[0]] * ( 1-fx )
//					+ in[edge[1]] * fx );
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const Real fx = map.getValue()[i].baryCoords[0];
//					int index = map.getValue()[i].in_index;
//					const topology::Edge& edge = edges[index];
//					Out::setDPos(out[i] , in[edge[0]] * ( 1-fx )
//					+ in[edge[1]] * fx);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					Out::setDPos(out[i] , in[triangle[0]] * ( 1-fx-fy )
//					+ in[triangle[1]] * fx
//					+ in[triangle[2]] * fy);
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					Out::setDPos(out[i] , in[triangle[0]] * ( 1-fx-fy )
//					+ in[triangle[1]] * fx
//					+ in[triangle[2]] * fy);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Quad& quad = quads[index];
//					Out::setDPos(out[i] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
//					+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
//					+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
//					+ in[quad[2]] * ( ( fx ) * ( fy ) ));
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Quad& quad = quads[index];
//					Out::setDPos(out[i] , in[quad[0]] * ( ( 1-fx ) * ( 1-fy ) )
//					+ in[quad[1]] * ( ( fx ) * ( 1-fy ) )
//					+ in[quad[3]] * ( ( 1-fx ) * ( fy ) )
//					+ in[quad[2]] * ( ( fx ) * ( fy ) ) );
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//					Out::setDPos(out[i] , in[tetra[0]] * ( 1-fx-fy-fz )
//					+ in[tetra[1]] * fx
//					+ in[tetra[2]] * fy
//					+ in[tetra[3]] * fz );
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//					Out::setDPos(out[i] , in[tetra[0]] * ( 1-fx-fy-fz )
//					+ in[tetra[1]] * fx
//					+ in[tetra[2]] * fy
//					+ in[tetra[3]] * fz );
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::applyJ ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
//		{
//			out.resize ( map.getValue().size() );
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//					Out::setDPos(out[i] ,
//					in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//					Out::setDPos(out[i] ,
//					in[cube[0]] * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[1]] * ( ( fx ) * ( 1-fy ) * ( 1-fz ) )
//					+ in[cube[3]] * ( ( 1-fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[2]] * ( ( fx ) * ( fy ) * ( 1-fz ) )
//					+ in[cube[4]] * ( ( 1-fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[5]] * ( ( fx ) * ( 1-fy ) * ( fz ) )
//					+ in[cube[7]] * ( ( 1-fx ) * ( fy ) * ( fz ) )
//					+ in[cube[6]] * ( ( fx ) * ( fy ) * ( fz ) ) );
//				}
//			}
//		}

//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::applyJT (const core::MechanicalParams * /*mparams*//* PARAMS FIRST */, Data< typename In::VecDeriv >& out, const Data< typename Out::VecDeriv >& in)
//		{
//			if (
//			mapper != NULL)
//			{
//				mapper->applyJT(*out.beginEdit(), in.getValue());
//				out.endEdit();
//			}
//		}

		template <class In, class Out>
		void MapperQuadraticTetrahedronSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
		{
			//	const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			//	const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			//	const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			//	const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
			//#ifdef SOFA_NEW_HEXA
			//	const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
			//#else
			//	const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
			//#endif
			//
			//	//         std::cerr << "MapperQuadraticTetrahedronSetTopology<In,Out>::applyJT " << maskTo->isInUse() << "\n";
			//
			//	if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
			//	{
			//		maskFrom->setInUse(false);
			//		// 1D elements
			//		{
			//			for ( unsigned int i=0;i<map1d.size();i++ )
			//			{
			//				const typename Out::DPos v = Out::getDPos(in[i]);
			//				const OutReal fx = ( OutReal ) map1d[i].baryCoords[0];
			//				int index = map1d[i].in_index;
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
			//					out[line[0]] += v * ( 1-fx );
			//					out[line[1]] += v * fx;
			//				}
			//			}
			//		}
			//		// 2D elements
			//		{
			//			const int i0 = map1d.size();
			//			const int c0 = triangles.size();
			//			for ( unsigned int i=0;i<map2d.size();i++ )
			//			{
			//				const typename Out::DPos v = Out::getDPos(in[i+i0]);
			//				const OutReal fx = ( OutReal ) map2d[i].baryCoords[0];
			//				const OutReal fy = ( OutReal ) map2d[i].baryCoords[1];
			//				int index = map2d[i].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
			//					out[triangle[0]] += v * ( 1-fx-fy );
			//					out[triangle[1]] += v * fx;
			//					out[triangle[2]] += v * fy;
			//				}
			//				else
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
			//					out[quad[0]] += v * ( ( 1-fx ) * ( 1-fy ) );
			//					out[quad[1]] += v * ( ( fx ) * ( 1-fy ) );
			//					out[quad[3]] += v * ( ( 1-fx ) * ( fy ) );
			//					out[quad[2]] += v * ( ( fx ) * ( fy ) );
			//				}
			//			}
			//		}
			//		// 3D elements
			//		{
			//			const int i0 = map1d.size() + map2d.size();
			//			const int c0 = tetrahedra.size();
			//			for ( unsigned int i=0;i<map3d.size();i++ )
			//			{
			//				const typename Out::DPos v = Out::getDPos(in[i+i0]);
			//				const OutReal fx = ( OutReal ) map3d[i].baryCoords[0];
			//				const OutReal fy = ( OutReal ) map3d[i].baryCoords[1];
			//				const OutReal fz = ( OutReal ) map3d[i].baryCoords[2];
			//				int index = map3d[i].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
			//					out[tetra[0]] += v * ( 1-fx-fy-fz );
			//					out[tetra[1]] += v * fx;
			//					out[tetra[2]] += v * fy;
			//					out[tetra[3]] += v * fz;
			//				}
			//				else
			//				{
			//#ifdef SOFA_NEW_HEXA
			//					const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
			//#else
			//					const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
			//#endif
			//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
			//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
			//#ifdef SOFA_NEW_HEXA
			//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
			//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
			//#else
			//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
			//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
			//#endif
			//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
			//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
			//#ifdef SOFA_NEW_HEXA
			//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
			//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
			//#else
			//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
			//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
			//#endif
			//				}
			//			}
			//		}
			//	}
			//	else
			//	{
			//		typedef helper::ParticleMask ParticleMask;
			//		const ParticleMask::InternalStorage &indices=maskTo->getEntries();
			//
			//
			//		const int i1d = map1d.size();
			//		const int i2d = map2d.size();
			//		const int i3d = map3d.size();
			//
			//
			//		ParticleMask::InternalStorage::const_iterator it;
			//		for (it=indices.begin();it!=indices.end();it++)
			//		{
			//			const int i=(*it);
			//			// 1D elements
			//			if (i < i1d)
			//			{
			//				const typename Out::DPos v = Out::getDPos(in[i]);
			//				const OutReal fx = ( OutReal ) map1d[i].baryCoords[0];
			//				int index = map1d[i].in_index;
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
			//					out[line[0]] += v * ( 1-fx );
			//					out[line[1]] += v * fx;
			//					maskFrom->insertEntry(line[0]);
			//					maskFrom->insertEntry(line[1]);
			//				}
			//			}
			//			// 2D elements
			//			else if (i < i1d+i2d)
			//			{
			//				const int i0 = map1d.size();
			//				const int c0 = triangles.size();
			//				const typename Out::DPos v = Out::getDPos(in[i]);
			//				const OutReal fx = ( OutReal ) map2d[i-i0].baryCoords[0];
			//				const OutReal fy = ( OutReal ) map2d[i-i0].baryCoords[1];
			//				int index = map2d[i-i0].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
			//					out[triangle[0]] += v * ( 1-fx-fy );
			//					out[triangle[1]] += v * fx;
			//					out[triangle[2]] += v * fy;
			//					maskFrom->insertEntry(triangle[0]);
			//					maskFrom->insertEntry(triangle[1]);
			//					maskFrom->insertEntry(triangle[2]);
			//				}
			//				else
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
			//					out[quad[0]] += v * ( ( 1-fx ) * ( 1-fy ) );
			//					out[quad[1]] += v * ( ( fx ) * ( 1-fy ) );
			//					out[quad[3]] += v * ( ( 1-fx ) * ( fy ) );
			//					out[quad[2]] += v * ( ( fx ) * ( fy ) );
			//					maskFrom->insertEntry(quad[0]);
			//					maskFrom->insertEntry(quad[1]);
			//					maskFrom->insertEntry(quad[2]);
			//					maskFrom->insertEntry(quad[3]);
			//				}
			//			}
			//			// 3D elements
			//			else if (i < i1d+i2d+i3d)
			//			{
			//				const int i0 = map1d.size() + map2d.size();
			//				const int c0 = tetrahedra.size();
			//				const typename Out::DPos v = Out::getDPos(in[i]);
			//				const OutReal fx = ( OutReal ) map3d[i-i0].baryCoords[0];
			//				const OutReal fy = ( OutReal ) map3d[i-i0].baryCoords[1];
			//				const OutReal fz = ( OutReal ) map3d[i-i0].baryCoords[2];
			//				int index = map3d[i-i0].in_index;
			//				if ( index<c0 )
			//				{
			//					const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
			//					out[tetra[0]] += v * ( 1-fx-fy-fz );
			//					out[tetra[1]] += v * fx;
			//					out[tetra[2]] += v * fy;
			//					out[tetra[3]] += v * fz;
			//					maskFrom->insertEntry(tetra[0]);
			//					maskFrom->insertEntry(tetra[1]);
			//					maskFrom->insertEntry(tetra[2]);
			//					maskFrom->insertEntry(tetra[3]);
			//				}
			//				else
			//				{
			//#ifdef SOFA_NEW_HEXA
			//					const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
			//#else
			//					const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
			//#endif
			//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
			//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
			//#ifdef SOFA_NEW_HEXA
			//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
			//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
			//#else
			//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
			//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
			//#endif
			//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
			//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
			//#ifdef SOFA_NEW_HEXA
			//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
			//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
			//#else
			//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
			//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
			//#endif
			//
			//					maskFrom->insertEntry(cube[0]);
			//					maskFrom->insertEntry(cube[1]);
			//					maskFrom->insertEntry(cube[2]);
			//					maskFrom->insertEntry(cube[3]);
			//					maskFrom->insertEntry(cube[4]);
			//					maskFrom->insertEntry(cube[5]);
			//					maskFrom->insertEntry(cube[6]);
			//					maskFrom->insertEntry(cube[7]);
			//				}
			//			}
			//		}
			//	}
		}

//		template <class In, class Out>
//		void BarycentricMapperRegularGridTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//#ifdef SOFA_NEW_HEXA
//					const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[i].in_index );
//#else
//					const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[i].in_index );
//#endif
//					const OutReal fx = ( OutReal ) map[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map[i].baryCoords[2];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#else
//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#endif
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#else
//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#endif
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const unsigned int index=(*it);
//					const typename Out::DPos v = Out::getDPos(in[index]);
//#ifdef SOFA_NEW_HEXA
//					const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[index].in_index );
//#else
//					const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[index].in_index );
//#endif
//					const OutReal fx = ( OutReal ) map[index].baryCoords[0];
//					const OutReal fy = ( OutReal ) map[index].baryCoords[1];
//					const OutReal fz = ( OutReal ) map[index].baryCoords[2];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#else
//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#endif
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#else
//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#endif
//					maskFrom->insertEntry(cube[0]);
//					maskFrom->insertEntry(cube[1]);
//					maskFrom->insertEntry(cube[2]);
//					maskFrom->insertEntry(cube[3]);
//					maskFrom->insertEntry(cube[4]);
//					maskFrom->insertEntry(cube[5]);
//					maskFrom->insertEntry(cube[6]);
//					maskFrom->insertEntry(cube[7]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperSparseGridTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//#ifdef SOFA_NEW_HEXA
//					const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[i].in_index );
//#else
//					const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[i].in_index );
//#endif
//					const OutReal fx = ( OutReal ) map[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map[i].baryCoords[2];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#else
//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#endif
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#else
//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#endif
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const unsigned int index=(*it);
//					const typename Out::DPos v = Out::getDPos(in[index]);
//#ifdef SOFA_NEW_HEXA
//					const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[index].in_index );
//#else
//					const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[index].in_index );
//#endif
//					const OutReal fx = ( OutReal ) map[index].baryCoords[0];
//					const OutReal fy = ( OutReal ) map[index].baryCoords[1];
//					const OutReal fz = ( OutReal ) map[index].baryCoords[2];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#else
//					out[cube[2]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//#endif
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//#ifdef SOFA_NEW_HEXA
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#else
//					out[cube[6]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//#endif
//
//					maskFrom->insertEntry(cube[0]);
//					maskFrom->insertEntry(cube[1]);
//					maskFrom->insertEntry(cube[2]);
//					maskFrom->insertEntry(cube[3]);
//					maskFrom->insertEntry(cube[4]);
//					maskFrom->insertEntry(cube[5]);
//					maskFrom->insertEntry(cube[6]);
//					maskFrom->insertEntry(cube[7]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					int index = map.getValue()[i].in_index;
//					const topology::Edge& edge = edges[index];
//					out[edge[0]] += v * ( 1-fx );
//					out[edge[1]] += v * fx;
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					int index = map.getValue()[i].in_index;
//					const topology::Edge& edge = edges[index];
//					out[edge[0]] += v * ( 1-fx );
//					out[edge[1]] += v * fx;
//
//					maskFrom->insertEntry(edge[0]);
//					maskFrom->insertEntry(edge[1]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					out[triangle[0]] += v * ( 1-fx-fy );
//					out[triangle[1]] += v * fx;
//					out[triangle[2]] += v * fy;
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					out[triangle[0]] += v * ( 1-fx-fy );
//					out[triangle[1]] += v * fx;
//					out[triangle[2]] += v * fy;
//					maskFrom->insertEntry(triangle[0]);
//					maskFrom->insertEntry(triangle[1]);
//					maskFrom->insertEntry(triangle[2]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Quad& quad = quads[index];
//					out[quad[0]] += v * ( ( 1-fx ) * ( 1-fy ) );
//					out[quad[1]] += v * ( ( fx ) * ( 1-fy ) );
//					out[quad[3]] += v * ( ( 1-fx ) * ( fy ) );
//					out[quad[2]] += v * ( ( fx ) * ( fy ) );
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Quad& quad = quads[index];
//					out[quad[0]] += v * ( ( 1-fx ) * ( 1-fy ) );
//					out[quad[1]] += v * ( ( fx ) * ( 1-fy ) );
//					out[quad[3]] += v * ( ( 1-fx ) * ( fy ) );
//					out[quad[2]] += v * ( ( fx ) * ( fy ) );
//					maskFrom->insertEntry(quad[0]);
//					maskFrom->insertEntry(quad[1]);
//					maskFrom->insertEntry(quad[2]);
//					maskFrom->insertEntry(quad[3]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//					out[tetra[0]] += v * ( 1-fx-fy-fz );
//					out[tetra[1]] += v * fx;
//					out[tetra[2]] += v * fy;
//					out[tetra[3]] += v * fz;
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//					out[tetra[0]] += v * ( 1-fx-fy-fz );
//					out[tetra[1]] += v * fx;
//					out[tetra[2]] += v * fy;
//					out[tetra[3]] += v * fz;
//					maskFrom->insertEntry(tetra[0]);
//					maskFrom->insertEntry(tetra[1]);
//					maskFrom->insertEntry(tetra[2]);
//					maskFrom->insertEntry(tetra[3]);
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::applyJT ( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
//		{
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//
//			//////////////  DEBUG  /////////////
//			// unsigned int mapSize = map.size();
//			// std::cout << "Map size: " << mapSize << std::endl;
//			// for(unsigned int i=0;i<map.size();i++)
//			// {
//			//   std::cout << "index " << map[i].in_index << ", baryCoord ( " << (OutReal)map[i].baryCoords[0] << ", " << (OutReal)map[i].baryCoords[1] << ", " << (OutReal)map[i].baryCoords[2] << ")." << std::endl;
//			// }
//			////////////////////////////////////
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				maskFrom->setInUse(false);
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int i=(int)(*it);
//					const typename Out::DPos v = Out::getDPos(in[i]);
//					const OutReal fx = ( OutReal ) map.getValue()[i].baryCoords[0];
//					const OutReal fy = ( OutReal ) map.getValue()[i].baryCoords[1];
//					const OutReal fz = ( OutReal ) map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//					out[cube[0]] += v * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[1]] += v * ( ( fx ) * ( 1-fy ) * ( 1-fz ) );
//					out[cube[3]] += v * ( ( 1-fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[2]] += v * ( ( fx ) * ( fy ) * ( 1-fz ) );
//					out[cube[4]] += v * ( ( 1-fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[5]] += v * ( ( fx ) * ( 1-fy ) * ( fz ) );
//					out[cube[7]] += v * ( ( 1-fx ) * ( fy ) * ( fz ) );
//					out[cube[6]] += v * ( ( fx ) * ( fy ) * ( fz ) );
//
//					maskFrom->insertEntry(cube[0]);
//					maskFrom->insertEntry(cube[1]);
//					maskFrom->insertEntry(cube[2]);
//					maskFrom->insertEntry(cube[3]);
//					maskFrom->insertEntry(cube[4]);
//					maskFrom->insertEntry(cube[5]);
//					maskFrom->insertEntry(cube[6]);
//					maskFrom->insertEntry(cube[7]);
//				}
//			}
//		}

//		template <class TIn, class TOut>
//		const sofa::defaulttype::BaseMatrix* ExtendedBarycentricMapping<TIn, TOut>::getJ()
//		{
//			if (
//			mapper!=NULL )
//			{
//				const unsigned int outStateSize = this->toModel->getX()->size();
//				const unsigned int inStateSize = this->fromModel->getX()->size();
//				return mapper->getJ(outStateSize, inStateSize);
//			}
//			else
//			return NULL;
//		}

		template <class In, class Out>
		const sofa::defaulttype::BaseMatrix* MapperQuadraticTetrahedronSetTopology<In,Out>::getJ(int outSize, int inSize)
		{

			//	if (matrixJ && !updateJ && matrixJ->rowBSize() == (unsigned)outSize && matrixJ->colBSize() == (unsigned)inSize)
			//		return matrixJ;
			//	if (outSize > 0 && map1d.size()+map2d.size()+map3d.size() == 0)
			//		return NULL; // error: maps not yet created ?
			//	std::cout << "MapperQuadraticTetrahedronSetTopology: creating " << outSize << "x" << inSize << " " << NOut << "x" << NIn << " J matrix" << std::endl;
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//
//			return matrixJ;
			//
			//	const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			//	const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			//	const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			//	const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
			//#ifdef SOFA_NEW_HEXA
			//	const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
			//#else
			//	const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
			//#endif
			//
			//	//         std::cerr << "MapperQuadraticTetrahedronSetTopology<In,Out>::getJ() \n";
			//
			//	// 1D elements
			//	{
			//		for ( unsigned int i=0;i<map1d.size();i++ )
			//		{
			//			const int out = i;
			//			const Real fx = ( Real ) map1d[i].baryCoords[0];
			//			int index = map1d[i].in_index;
			//			{
			//				const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
			//				addMatrixContrib(matrixJ, out, line[0],  ( 1-fx ));
			//				addMatrixContrib(matrixJ, out, line[1],  fx);
			//			}
			//		}
			//	}
			//	// 2D elements
			//	{
			//		const int i0 = map1d.size();
			//		const int c0 = triangles.size();
			//		for ( unsigned int i=0;i<map2d.size();i++ )
			//		{
			//			const int out = i+i0;
			//			const OutReal fx = ( OutReal ) map2d[i].baryCoords[0];
			//			const OutReal fy = ( OutReal ) map2d[i].baryCoords[1];
			//			int index = map2d[i].in_index;
			//			if ( index<c0 )
			//			{
			//				const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
			//				addMatrixContrib(matrixJ, out, triangle[0],  ( 1-fx-fy ));
			//				addMatrixContrib(matrixJ, out, triangle[1],  fx);
			//				addMatrixContrib(matrixJ, out, triangle[2],  fy);
			//			}
			//			else
			//			{
			//				const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
			//				addMatrixContrib(matrixJ, out, quad[0],  ( ( 1-fx ) * ( 1-fy ) ));
			//				addMatrixContrib(matrixJ, out, quad[1],  ( ( fx ) * ( 1-fy ) ));
			//				addMatrixContrib(matrixJ, out, quad[3],  ( ( 1-fx ) * ( fy ) ));
			//				addMatrixContrib(matrixJ, out, quad[2],  ( ( fx ) * ( fy ) ));
			//			}
			//		}
			//	}
			//	// 3D elements
			//	{
			//		const int i0 = map1d.size() + map2d.size();
			//		const int c0 = tetrahedra.size();
			//		for ( unsigned int i=0;i<map3d.size();i++ )
			//		{
			//			const int out = i+i0;
			//			const OutReal fx = ( OutReal ) map3d[i].baryCoords[0];
			//			const OutReal fy = ( OutReal ) map3d[i].baryCoords[1];
			//			const OutReal fz = ( OutReal ) map3d[i].baryCoords[2];
			//			int index = map3d[i].in_index;
			//			if ( index<c0 )
			//			{
			//				const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
			//				addMatrixContrib(matrixJ, out, tetra[0],  ( 1-fx-fy-fz ));
			//				addMatrixContrib(matrixJ, out, tetra[1],  fx);
			//				addMatrixContrib(matrixJ, out, tetra[2],  fy);
			//				addMatrixContrib(matrixJ, out, tetra[3],  fz);
			//			}
			//			else
			//			{
			//#ifdef SOFA_NEW_HEXA
			//				const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
			//#else
			//				const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
			//#endif
			//				addMatrixContrib(matrixJ, out, cube[0],  ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[1],  ( ( fx ) * ( 1-fy ) * ( 1-fz ) ));
			//#ifdef SOFA_NEW_HEXA
			//				addMatrixContrib(matrixJ, out, cube[3],  ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[2],  ( ( fx ) * ( fy ) * ( 1-fz ) ));
			//#else
			//				addMatrixContrib(matrixJ, out, cube[2],  ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[3],  ( ( fx ) * ( fy ) * ( 1-fz ) ));
			//#endif
			//				addMatrixContrib(matrixJ, out, cube[4],  ( ( 1-fx ) * ( 1-fy ) * ( fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[5],  ( ( fx ) * ( 1-fy ) * ( fz ) ));
			//#ifdef SOFA_NEW_HEXA
			//				addMatrixContrib(matrixJ, out, cube[7],  ( ( 1-fx ) * ( fy ) * ( fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[6],  ( ( fx ) * ( fy ) * ( fz ) ));
			//#else
			//				addMatrixContrib(matrixJ, out, cube[6],  ( ( 1-fx ) * ( fy ) * ( fz ) ));
			//				addMatrixContrib(matrixJ, out, cube[7],  ( ( fx ) * ( fy ) * ( fz ) ));
			//#endif
			//			}
			//		}
			//	}
//			matrixJ->compress();
//			//	std::cout << "J = " << *matrixJ << std::endl;
//			updateJ = false;
			return 0;//matrixJ;
		}

//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperRegularGridTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperRegularGridTopology<In,Out>::getJ() \n";
//
//			for ( unsigned int i=0;i<map.size();i++ )
//			{
//				const int out = i;
//#ifdef SOFA_NEW_HEXA
//				const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[i].in_index );
//#else
//				const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[i].in_index );
//#endif
//				const OutReal fx = ( OutReal ) map[i].baryCoords[0];
//				const OutReal fy = ( OutReal ) map[i].baryCoords[1];
//				const OutReal fz = ( OutReal ) map[i].baryCoords[2];
//				addMatrixContrib(matrixJ, out, cube[0], ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[1], ( ( fx ) * ( 1-fy ) * ( 1-fz ) ));
//#ifdef SOFA_NEW_HEXA
//				addMatrixContrib(matrixJ, out, cube[3], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[2], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//#else
//				addMatrixContrib(matrixJ, out, cube[2], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[3], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//#endif
//				addMatrixContrib(matrixJ, out, cube[4], ( ( 1-fx ) * ( 1-fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[5], ( ( fx ) * ( 1-fy ) * ( fz ) ));
//#ifdef SOFA_NEW_HEXA
//				addMatrixContrib(matrixJ, out, cube[7], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[6], ( ( fx ) * ( fy ) * ( fz ) ));
//#else
//				addMatrixContrib(matrixJ, out, cube[6], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[7], ( ( fx ) * ( fy ) * ( fz ) ));
//#endif
//			}
//			updateJ = false;
//			return matrixJ;
//		}
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperSparseGridTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperSparseGridTopology<In,Out>::getJ() \n";
//
//			for ( unsigned int i=0;i<map.size();i++ )
//			{
//				const int out = i;
//#ifdef SOFA_NEW_HEXA
//				const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[i].in_index );
//#else
//				const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[i].in_index );
//#endif
//				const OutReal fx = ( OutReal ) map[i].baryCoords[0];
//				const OutReal fy = ( OutReal ) map[i].baryCoords[1];
//				const OutReal fz = ( OutReal ) map[i].baryCoords[2];
//				addMatrixContrib(matrixJ, out, cube[0], ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[1], ( ( fx ) * ( 1-fy ) * ( 1-fz ) ));
//#ifdef SOFA_NEW_HEXA
//				addMatrixContrib(matrixJ, out, cube[3], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[2], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//#else
//				addMatrixContrib(matrixJ, out, cube[2], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//				addMatrixContrib(matrixJ, out, cube[3], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//#endif
//				addMatrixContrib(matrixJ, out, cube[4], ( ( 1-fx ) * ( 1-fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[5], ( ( fx ) * ( 1-fy ) * ( fz ) ));
//#ifdef SOFA_NEW_HEXA
//				addMatrixContrib(matrixJ, out, cube[7], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[6], ( ( fx ) * ( fy ) * ( fz ) ));
//#else
//				addMatrixContrib(matrixJ, out, cube[6], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				addMatrixContrib(matrixJ, out, cube[7], ( ( fx ) * ( fy ) * ( fz ) ));
//#endif
//			}
//			//matrixJ->compress();
//			//	std::cout << "J = " << *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}
//
//		///////////////////////////////////////////////////////////////////////////////////////////////
//		///////////////////////////////////////////////////////////////////////////////////////////////
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperEdgeSetTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperEdgeSetTopology<In,Out>::getJ() \n";
//
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int outId=0;outId<map.getValue().size();outId++ )
//				{
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					int index = map.getValue()[outId].in_index;
//					const topology::Edge& edge = edges[index];
//
//					addMatrixContrib(matrixJ, outId, edge[0], ( 1-fx));
//					addMatrixContrib(matrixJ, outId, edge[1], ( fx));
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int outId=(int)(*it);
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					int index = map.getValue()[outId].in_index;
//					const topology::Edge& edge = edges[index];
//
//					addMatrixContrib(matrixJ, outId, edge[0], ( 1-fx));
//					addMatrixContrib(matrixJ, outId, edge[1], ( fx));
//				}
//			}
//			//matrixJ->compress();
//			//	std::cout << "BarycentricMapperEdgeSetTopology  J = " << *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperTriangleSetTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperTriangleSetTopology<In,Out>::getJ() \n";
//
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int outId=0;outId<map.getValue().size();outId++ )
//				{
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					int index = map.getValue()[outId].in_index;
//					const topology::Triangle& triangle = triangles[index];
//
//					addMatrixContrib(matrixJ, outId, triangle[0], ( 1-fx-fy ));
//					addMatrixContrib(matrixJ, outId, triangle[1], ( fx ));
//					addMatrixContrib(matrixJ, outId, triangle[2], ( fy ));
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int outId=(int)(*it);
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					int index = map.getValue()[outId].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					addMatrixContrib(matrixJ, outId, triangle[0], ( 1-fx-fy ));
//					addMatrixContrib(matrixJ, outId, triangle[1], ( fx ));
//					addMatrixContrib(matrixJ, outId, triangle[2], ( fy ));
//				}
//			}
//
//			//matrixJ->compress();
//			//	std::cout << "BarycentricMapperTriangleSetTopology  J = " << *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperQuadSetTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperQuadSetTopology<In,Out>::getJ() \n";
//
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int outId=0;outId<map.getValue().size();outId++ )
//				{
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					int index = map.getValue()[outId].in_index;
//					const topology::Quad& quad = quads[index];
//
//					addMatrixContrib(matrixJ, outId, quad[0], ( ( 1-fx ) * ( 1-fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[1], ( ( fx ) * ( 1-fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[2], ( ( fx ) * ( fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[3], ( ( 1-fx ) * ( fy ) ));
//				}
//			}
//			else
//			{
//
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int outId=(int)(*it);
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					int index = map.getValue()[outId].in_index;
//					const topology::Quad& quad = quads[index];
//
//					addMatrixContrib(matrixJ, outId, quad[0], ( ( 1-fx ) * ( 1-fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[1], ( ( fx ) * ( 1-fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[2], ( ( fx ) * ( fy ) ));
//					addMatrixContrib(matrixJ, outId, quad[3], ( ( 1-fx ) * ( fy ) ));
//				}
//			}
//			//matrixJ->compress();
//			//	std::cout << "BarycentricMapperQuadSetTopology  J = " << std::endl<< *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperTetrahedronSetTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperTetrahedronSetTopology<In,Out>::getJ() \n";
//
//
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int outId=0;outId<map.getValue().size();outId++ )
//				{
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					const Real fz = map.getValue()[outId].baryCoords[2];
//					int index = map.getValue()[outId].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//
//					addMatrixContrib(matrixJ, outId, tetra[0], ( 1-fx-fy-fz ));
//					addMatrixContrib(matrixJ, outId, tetra[1], ( fx ));
//					addMatrixContrib(matrixJ, outId, tetra[2], ( fy ));
//					addMatrixContrib(matrixJ, outId, tetra[3], ( fz ));
//
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int outId=(int)(*it);
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					const Real fz = map.getValue()[outId].baryCoords[2];
//					int index = map.getValue()[outId].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//
//					addMatrixContrib(matrixJ, outId, tetra[0], ( 1-fx-fy-fz ));
//					addMatrixContrib(matrixJ, outId, tetra[1], ( fx ));
//					addMatrixContrib(matrixJ, outId, tetra[2], ( fy ));
//					addMatrixContrib(matrixJ, outId, tetra[3], ( fz ));
//				}
//			}
//			//matrixJ->compress();
//			//	std::cout << "BarycentricMapperTetrahedronSetTopology  J = " << std::endl << *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}
//
//		template <class In, class Out>
//		const sofa::defaulttype::BaseMatrix* BarycentricMapperHexahedronSetTopology<In,Out>::getJ(int outSize, int inSize)
//		{
//			if (matrixJ && !updateJ)
//			return matrixJ;
//
//			if (!matrixJ) matrixJ = new MatrixType;
//			if (matrixJ->rowBSize() != (unsigned)outSize || matrixJ->colBSize() != (unsigned)inSize)
//			matrixJ->resize(outSize*NOut, inSize*NIn);
//			else
//			matrixJ->clear();
//			//         std::cerr << "BarycentricMapperHexahedronSetTopology<In,Out>::getJ() \n";
//
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//
//			if ((!maskTo)||(maskTo&& !(maskTo->isInUse())) )
//			{
//				for ( unsigned int outId=0;outId<map.getValue().size();outId++ )
//				{
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					const Real fz = map.getValue()[outId].baryCoords[2];
//					int index = map.getValue()[outId].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//
//					addMatrixContrib(matrixJ, outId, cube[0], ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[1], ( ( fx ) * ( 1-fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[2], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[3], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[4], ( ( 1-fx ) * ( 1-fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[5], ( ( fx ) * ( 1-fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[6], ( ( fx ) * ( fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[7], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				}
//			}
//			else
//			{
//				typedef helper::ParticleMask ParticleMask;
//				const ParticleMask::InternalStorage &indices=maskTo->getEntries();
//
//				ParticleMask::InternalStorage::const_iterator it;
//				for (it=indices.begin();it!=indices.end();it++)
//				{
//					const int outId = (int)(*it);
//					const Real fx = map.getValue()[outId].baryCoords[0];
//					const Real fy = map.getValue()[outId].baryCoords[1];
//					const Real fz = map.getValue()[outId].baryCoords[2];
//					int index = map.getValue()[outId].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//
//					addMatrixContrib(matrixJ, outId, cube[0], ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[1], ( ( fx ) * ( 1-fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[2], ( ( fx ) * ( fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[3], ( ( 1-fx ) * ( fy ) * ( 1-fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[4], ( ( 1-fx ) * ( 1-fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[5], ( ( fx ) * ( 1-fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[6], ( ( fx ) * ( fy ) * ( fz ) ));
//					addMatrixContrib(matrixJ, outId, cube[7], ( ( 1-fx ) * ( fy ) * ( fz ) ));
//				}
//			}
//			//matrixJ->compress();
//			//	std::cout << "BarycentricMapperHexahedronSetTopology  J = " << std::endl << *matrixJ << std::endl;
//			updateJ = false;
//			return matrixJ;
//		}

		///////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////


//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::draw(const core::visual::VisualParams* vparams)
//		{
//			if ( !vparams->displayFlags().getShowMappings() ) return;
//
//			const OutVecCoord& out = *this->toModel->getX();
//			std::vector< Vector3 > points;
//			for ( unsigned int i=0; i<out.size(); i++ )
//			{
//				points.push_back ( OutDataTypes::getCPos(out[i]) );
//			}
//			//	glEnd();
//			const InVecCoord& in = *this->fromModel->getX();
//			if ( mapper!=NULL ) mapper->draw(vparams,out, in );
//
//			vparams->drawTool()->drawPoints ( points, 7, Vec<4,float> ( 1,1,0,1 ) );
//		}

		template <class In, class Out>
		void MapperQuadraticTetrahedronSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
		{
			const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
#ifdef SOFA_NEW_HEXA
			const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
#else
			const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
#endif
			std::vector< Vector3 > points;
			// 1D elements
//			{
//				const int i0 = 0;
//				for ( unsigned int i=0;i<map1d.size();i++ )
//				{
//					const Real fx = map1d[i].baryCoords[0];
//					int index = map1d[i].in_index;
//					{
//						const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
//						Real f[2];
//						f[0] = ( 1-fx );
//						f[1] = fx;
//						for ( int j=0;j<2;j++ )
//						{
//							if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//							{
//								//                         glColor3f((float)f[j],1,(float)f[j]);
//								points.push_back ( Out::getCPos(out[i+i0]) );
//								points.push_back ( in[line[j]] );
//							}
//						}
//					}
//				}
//			}
//			// 2D elements
//			{
//				const int i0 = map1d.size();
//				const int c0 = triangles.size();
//				for ( unsigned int i=0;i<map2d.size();i++ )
//				{
//					const Real fx = map2d[i].baryCoords[0];
//					const Real fy = map2d[i].baryCoords[1];
//					int index = map2d[i].in_index;
//					if ( index<c0 )
//					{
//						const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
//						Real f[3];
//						f[0] = ( 1-fx-fy );
//						f[1] = fx;
//						f[2] = fy;
//						for ( int j=0;j<3;j++ )
//						{
//							if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//							{
//								//                         glColor3f((float)f[j],1,(float)f[j]);
//								points.push_back ( Out::getCPos(out[i+i0]) );
//								points.push_back ( in[triangle[j]] );
//							}
//						}
//					}
//					else
//					{
//						const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index-c0];
//						Real f[4];
//						f[0] = ( ( 1-fx ) * ( 1-fy ) );
//						f[1] = ( ( fx ) * ( 1-fy ) );
//						f[3] = ( ( 1-fx ) * ( fy ) );
//						f[2] = ( ( fx ) * ( fy ) );
//						for ( int j=0;j<4;j++ )
//						{
//							if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//							{
//								//                         glColor3f((float)f[j],1,(float)f[j]);
//								points.push_back ( Out::getCPos(out[i+i0]) );
//								points.push_back ( in[quad[j]] );
//							}
//						}
//					}
//				}
//			}
			// 3D elements
			{
				const int i0 = 0;//map1d.size() +map2d.size();
				const int c0 = tetrahedra.size();
				for ( unsigned int i=0; i<map3d.size(); i++ )
				{
					const Real fx = map3d[i].baryCoords[0];
					const Real fy = map3d[i].baryCoords[1];
					const Real fz = map3d[i].baryCoords[2];
					int index = map3d[i].in_index;
					if ( index<c0 )
					{
						const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
						Real f[4];
						f[0] = ( 1-fx-fy-fz );
						f[1] = fx;
						f[2] = fy;
						f[3] = fz;
						for ( int j=0;j<4;j++ )
						{
							if ( f[j]<=-0.0001 || f[j]>=0.0001 )
							{
								//                         glColor3f((float)f[j],1,(float)f[j]);
								points.push_back ( Out::getCPos(out[i+i0]) );
								points.push_back ( in[tetra[j]] );
							}
						}
					}
					else
					{
#ifdef SOFA_NEW_HEXA
						const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-c0];
#else
						const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-c0];
#endif
						Real f[8];
						f[0] = ( 1-fx ) * ( 1-fy ) * ( 1-fz );
						f[1] = ( fx ) * ( 1-fy ) * ( 1-fz );
#ifdef SOFA_NEW_HEXA
						f[3] = ( 1-fx ) * ( fy ) * ( 1-fz );
						f[2] = ( fx ) * ( fy ) * ( 1-fz );
#else
						f[2] = ( 1-fx ) * ( fy ) * ( 1-fz );
						f[3] = ( fx ) * ( fy ) * ( 1-fz );
#endif
						f[4] = ( 1-fx ) * ( 1-fy ) * ( fz );
						f[5] = ( fx ) * ( 1-fy ) * ( fz );
#ifdef SOFA_NEW_HEXA
						f[7] = ( 1-fx ) * ( fy ) * ( fz );
						f[6] = ( fx ) * ( fy ) * ( fz );
#else
						f[6] = ( 1-fx ) * ( fy ) * ( fz );
						f[7] = ( fx ) * ( fy ) * ( fz );
#endif
						for ( int j=0;j<8;j++ )
						{
							if ( f[j]<=-0.0001 || f[j]>=0.0001 )
							{
								//                         glColor3f((float)f[j],1,1);
								points.push_back ( Out::getCPos(out[i+i0]) );
								points.push_back ( in[cube[j]] );
							}
						}
					}
				}
			}
			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
		}

//		template <class In, class Out>
//		void BarycentricMapperRegularGridTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			std::vector< Vector3 > points;
//
//			for ( unsigned int i=0; i<map.size(); i++ )
//			{
//#ifdef SOFA_NEW_HEXA
//				const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[i].in_index );
//#else
//				const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[i].in_index );
//#endif
//				const Real fx = map[i].baryCoords[0];
//				const Real fy = map[i].baryCoords[1];
//				const Real fz = map[i].baryCoords[2];
//				Real f[8];
//				f[0] = ( 1-fx ) * ( 1-fy ) * ( 1-fz );
//				f[1] = ( fx ) * ( 1-fy ) * ( 1-fz );
//#ifdef SOFA_NEW_HEXA
//				f[3] = ( 1-fx ) * ( fy ) * ( 1-fz );
//				f[2] = ( fx ) * ( fy ) * ( 1-fz );
//#else
//				f[2] = ( 1-fx ) * ( fy ) * ( 1-fz );
//				f[3] = ( fx ) * ( fy ) * ( 1-fz );
//#endif
//				f[4] = ( 1-fx ) * ( 1-fy ) * ( fz );
//				f[5] = ( fx ) * ( 1-fy ) * ( fz );
//#ifdef SOFA_NEW_HEXA
//				f[7] = ( 1-fx ) * ( fy ) * ( fz );
//				f[6] = ( fx ) * ( fy ) * ( fz );
//#else
//				f[6] = ( 1-fx ) * ( fy ) * ( fz );
//				f[7] = ( fx ) * ( fy ) * ( fz );
//#endif
//				for ( int j=0;j<8;j++ )
//				{
//					if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//					{
//						//glColor3f((float)f[j],(float)f[j],1);
//						points.push_back ( Out::getCPos(out[i]) );
//						points.push_back ( in[cube[j]] );
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,0,1,1 ) );
//
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperSparseGridTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			std::vector< Vector3 > points;
//			for ( unsigned int i=0; i<map.size(); i++ )
//			{
//#ifdef SOFA_NEW_HEXA
//				const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[i].in_index );
//#else
//				const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[i].in_index );
//#endif
//				const Real fx = map[i].baryCoords[0];
//				const Real fy = map[i].baryCoords[1];
//				const Real fz = map[i].baryCoords[2];
//				Real f[8];
//				f[0] = ( 1-fx ) * ( 1-fy ) * ( 1-fz );
//				f[1] = ( fx ) * ( 1-fy ) * ( 1-fz );
//#ifdef SOFA_NEW_HEXA
//				f[3] = ( 1-fx ) * ( fy ) * ( 1-fz );
//				f[2] = ( fx ) * ( fy ) * ( 1-fz );
//#else
//				f[2] = ( 1-fx ) * ( fy ) * ( 1-fz );
//				f[3] = ( fx ) * ( fy ) * ( 1-fz );
//#endif
//				f[4] = ( 1-fx ) * ( 1-fy ) * ( fz );
//				f[5] = ( fx ) * ( 1-fy ) * ( fz );
//#ifdef SOFA_NEW_HEXA
//				f[7] = ( 1-fx ) * ( fy ) * ( fz );
//				f[6] = ( fx ) * ( fy ) * ( fz );
//#else
//				f[6] = ( 1-fx ) * ( fy ) * ( fz );
//				f[7] = ( fx ) * ( fy ) * ( fz );
//#endif
//				for ( int j=0;j<8;j++ )
//				{
//					if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//					{
//						//glColor3f((float)f[j],(float)f[j],1);
//						points.push_back ( Out::getCPos(out[i]) );
//						points.push_back ( in[cube[j]] );
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,0,1,1 ) );
//
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//
//			std::vector< Vector3 > points;
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					int index = map.getValue()[i].in_index;
//					const topology::Edge& edge = edges[index];
//					{
//						const Real f = Real ( 1.0 )-fx;
//						if ( f<=-0.0001 || f>=0.0001 )
//						{
//							//                     glColor3f((float)f,1,(float)f);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[edge[0]] );
//						}
//					}
//					{
//						const Real f = fx;
//						if ( f<=-0.0001 || f>=0.0001 )
//						{
//							//                     glColor3f((float)f,1,(float)f);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[edge[1]] );
//						}
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//
//			std::vector< Vector3 > points;
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Triangle& triangle = triangles[index];
//					Real f[3];
//					f[0] = ( 1-fx-fy );
//					f[1] = fx;
//					f[2] = fy;
//					for ( int j=0;j<3;j++ )
//					{
//						if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//						{
//							//                     glColor3f((float)f[j],1,(float)f[j]);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[triangle[j]] );
//						}
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//			std::vector< Vector3 > points;
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					int index = map.getValue()[i].in_index;
//					const topology::Quad& quad = quads[index];
//					Real f[4];
//					f[0] = ( ( 1-fx ) * ( 1-fy ) );
//					f[1] = ( ( fx ) * ( 1-fy ) );
//					f[3] = ( ( 1-fx ) * ( fy ) );
//					f[2] = ( ( fx ) * ( fy ) );
//					for ( int j=0;j<4;j++ )
//					{
//						if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//						{
//							//                     glColor3f((float)f[j],1,(float)f[j]);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[quad[j]] );
//						}
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			std::vector< Vector3 > points;
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Tetrahedron& tetra = tetrahedra[index];
//					Real f[4];
//					f[0] = ( 1-fx-fy-fz );
//					f[1] = fx;
//					f[2] = fy;
//					f[3] = fz;
//					for ( int j=0;j<4;j++ )
//					{
//						if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//						{
//							//                     glColor3f((float)f[j],1,(float)f[j]);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[tetra[j]] );
//						}
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::draw (const core::visual::VisualParams* vparams,const typename Out::VecCoord& out, const typename In::VecCoord& in )
//		{
//			const sofa::helper::vector<topology::Hexahedron>& cubes = this->fromTopology->getHexahedra();
//
//			std::vector< Vector3 > points;
//			{
//				for ( unsigned int i=0;i<map.getValue().size();i++ )
//				{
//					const Real fx = map.getValue()[i].baryCoords[0];
//					const Real fy = map.getValue()[i].baryCoords[1];
//					const Real fz = map.getValue()[i].baryCoords[2];
//					int index = map.getValue()[i].in_index;
//					const topology::Hexahedron& cube = cubes[index];
//					Real f[8];
//					f[0] = ( 1-fx ) * ( 1-fy ) * ( 1-fz );
//					f[1] = ( fx ) * ( 1-fy ) * ( 1-fz );
//					f[3] = ( 1-fx ) * ( fy ) * ( 1-fz );
//					f[2] = ( fx ) * ( fy ) * ( 1-fz );
//					f[4] = ( 1-fx ) * ( 1-fy ) * ( fz );
//					f[5] = ( fx ) * ( 1-fy ) * ( fz );
//					f[7] = ( 1-fx ) * ( fy ) * ( fz );
//					f[6] = ( fx ) * ( fy ) * ( fz );
//					for ( int j=0;j<8;j++ )
//					{
//						if ( f[j]<=-0.0001 || f[j]>=0.0001 )
//						{
//							//                     glColor3f((float)f[j],1,1);
//							points.push_back ( Out::getCPos(out[i]) );
//							points.push_back ( in[cube[j]] );
//						}
//					}
//				}
//			}
//			vparams->drawTool()->drawLines ( points, 1, Vec<4,float> ( 0,1,0,1 ) );
//		}

		/************************************* PropagateConstraint ***********************************/

//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::applyJT(const core::ConstraintParams * /*cparams*//* PARAMS FIRST */, Data< typename In::MatrixDeriv >& out, const Data< typename Out::MatrixDeriv >& in)
//		{
//			if (
//			mapper!=NULL )
//			{
//				mapper->applyJT(*out.beginEdit(), in.getValue());
//				out.endEdit();
//			}
//		}

		/// @TODO Optimization
		template <class In, class Out>
		void MapperQuadraticTetrahedronSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
		{
			const sofa::core::topology::BaseMeshTopology::SeqLines& lines = this->fromTopology->getLines();
			const sofa::core::topology::BaseMeshTopology::SeqTriangles& triangles = this->fromTopology->getTriangles();
			const sofa::core::topology::BaseMeshTopology::SeqQuads& quads = this->fromTopology->getQuads();
			const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();
#ifdef SOFA_NEW_HEXA
			const sofa::core::topology::BaseMeshTopology::SeqHexahedra& cubes = this->fromTopology->getHexahedra();
#else
			const sofa::core::topology::BaseMeshTopology::SeqCubes& cubes = this->fromTopology->getCubes();
#endif
			//const int iLine = lines.size();
			const int iTri = triangles.size();
			//const int iQuad = quads.size();
			const int iTetra= tetrahedra.size();
			//const int iCube = cubes.size();

			const int i1d = 0;//map1d.size();
			const int i2d = 0;//map2d.size();
			const int i3d = map3d.size();

			int indexIn;

			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();

			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
			{
				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();

				if (colIt != colItEnd)
				{
					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());

					for (; colIt != colItEnd; ++colIt)
					{
						indexIn = colIt.index();
						InDeriv data = (InDeriv) Out::getDPos(colIt.val());

						// 1D elements
//						if ( indexIn < i1d )
//						{
//							const OutReal fx = ( OutReal ) map1d[indexIn].baryCoords[0];
//							int index = map1d[indexIn].in_index;
//							{
//								const sofa::core::topology::BaseMeshTopology::Line& line = lines[index];
//								o.addCol( line[0], data * ( 1-fx ) );
//								o.addCol( line[1], data * fx );
//							}
//						}
//						// 2D elements : triangle or quad
//						else if ( indexIn < i2d )
//						{
//							const OutReal fx = ( OutReal ) map2d[indexIn].baryCoords[0];
//							const OutReal fy = ( OutReal ) map2d[indexIn].baryCoords[1];
//							int index = map2d[indexIn].in_index;
//							if ( index < iTri ) // triangle
//							{
//								const sofa::core::topology::BaseMeshTopology::Triangle& triangle = triangles[index];
//								o.addCol( triangle[0], data * ( 1-fx-fy ) );
//								o.addCol( triangle[1], data * fx );
//								o.addCol( triangle[2], data * fy );
//							}
//							else // 2D element : Quad
//							{
//								const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[index - iTri];
//								o.addCol( quad[0], data * ( ( 1-fx ) * ( 1-fy ) ) );
//								o.addCol( quad[1], data * ( ( fx ) * ( 1-fy ) ) );
//								o.addCol( quad[3], data * ( ( 1-fx ) * ( fy ) ) );
//								o.addCol( quad[2], data * ( ( fx ) * ( fy ) ) );
//							}
//						}
						// 3D elements
						if ( indexIn < i3d )
						{
							const OutReal fx = ( OutReal ) map3d[indexIn].baryCoords[0];
							const OutReal fy = ( OutReal ) map3d[indexIn].baryCoords[1];
							const OutReal fz = ( OutReal ) map3d[indexIn].baryCoords[2];
							int index = map3d[indexIn].in_index;
							if ( index < iTetra ) // tetra
							{
								const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];
								o.addCol ( tetra[0], data * ( 1-fx-fy-fz ) );
								o.addCol ( tetra[1], data * fx );
								o.addCol ( tetra[2], data * fy );
								o.addCol ( tetra[3], data * fz );
							}
							else // cube
							{
#ifdef SOFA_NEW_HEXA
								const sofa::core::topology::BaseMeshTopology::Hexa& cube = cubes[index-iTetra];
#else
								const sofa::core::topology::BaseMeshTopology::Cube& cube = cubes[index-iTetra];
#endif
								o.addCol ( cube[0],data * ( ( 1-fx ) * ( 1-fy ) * ( 1-fz ) ) );
								o.addCol ( cube[1],data * ( ( fx ) * ( 1-fy ) * ( 1-fz ) ) );
#ifdef SOFA_NEW_HEXA
								o.addCol ( cube[3],data * ( ( 1-fx ) * ( fy ) * ( 1-fz ) ) );
								o.addCol ( cube[2],data * ( ( fx ) * ( fy ) * ( 1-fz ) ) );
#else
								o.addCol ( cube[2],data * ( ( 1-fx ) * ( fy ) * ( 1-fz ) ) );
								o.addCol ( cube[3],data * ( ( fx ) * ( fy ) * ( 1-fz ) ) );
#endif
								o.addCol ( cube[4],data * ( ( 1-fx ) * ( 1-fy ) * ( fz ) ) );
								o.addCol ( cube[5],data * ( ( fx ) * ( 1-fy ) * ( fz ) ) );
#ifdef SOFA_NEW_HEXA
								o.addCol ( cube[7],data * ( ( 1-fx ) * ( fy ) * ( fz ) ) );
								o.addCol ( cube[6],data * ( ( fx ) * ( fy ) * ( fz ) ) );
#else
								o.addCol ( cube[6],data * ( ( 1-fx ) * ( fy ) * ( fz ) ) );
								o.addCol ( cube[7],data * ( ( fx ) * ( fy ) * ( fz ) ) );
#endif
							}
						}
					}
				}
			}
		}

//		template <class In, class Out>
//		void BarycentricMapperRegularGridTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned int indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//#ifdef SOFA_NEW_HEXA
//						const topology::RegularGridTopology::Hexa cube = this->fromTopology->getHexaCopy ( this->map[indexIn].in_index );
//#else
//						const topology::RegularGridTopology::Cube cube = this->fromTopology->getCubeCopy ( this->map[indexIn].in_index );
//#endif
//						const OutReal fx = (OutReal) map[indexIn].baryCoords[0];
//						const OutReal fy = (OutReal) map[indexIn].baryCoords[1];
//						const OutReal fz = (OutReal) map[indexIn].baryCoords[2];
//						const OutReal oneMinusFx = 1-fx;
//						const OutReal oneMinusFy = 1-fy;
//						const OutReal oneMinusFz = 1-fz;
//
//						o.addCol(cube[0], data * ((oneMinusFx) * (oneMinusFy) * (oneMinusFz)));
//						o.addCol(cube[1], data * ((fx) * (oneMinusFy) * (oneMinusFz)));
//#ifdef SOFA_NEW_HEXA
//						o.addCol(cube[3], data * ((oneMinusFx) * (fy) * (oneMinusFz)));
//						o.addCol(cube[2], data * ((fx) * (fy) * (oneMinusFz)));
//#else
//						o.addCol(cube[2], data * ((oneMinusFx) * (fy) * (oneMinusFz)));
//						o.addCol(cube[3], data * ((fx) * (fy) * (oneMinusFz)));
//#endif
//						o.addCol(cube[4], data * ((oneMinusFx) * (oneMinusFy) * (fz)));
//						o.addCol(cube[5], data * ((fx) * (oneMinusFy) * (fz)));
//#ifdef SOFA_NEW_HEXA
//						o.addCol(cube[7], data * ((oneMinusFx) * (fy) * (fz)));
//						o.addCol(cube[6], data * ((fx) * (fy) * (fz)));
//#else
//						o.addCol(cube[6], data * ((oneMinusFx) * (fy) * (fz)));
//						o.addCol(cube[7], data * ((fx) * (fy) * (fz)));
//#endif
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperSparseGridTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//#ifdef SOFA_NEW_HEXA
//						const topology::SparseGridTopology::Hexa cube = this->fromTopology->getHexahedron ( this->map[indexIn].in_index );
//#else
//						const topology::SparseGridTopology::Cube cube = this->fromTopology->getCube ( this->map[indexIn].in_index );
//#endif
//						const OutReal fx = ( OutReal ) map[indexIn].baryCoords[0];
//						const OutReal fy = ( OutReal ) map[indexIn].baryCoords[1];
//						const OutReal fz = ( OutReal ) map[indexIn].baryCoords[2];
//						const OutReal oneMinusFx = 1-fx;
//						const OutReal oneMinusFy = 1-fy;
//						const OutReal oneMinusFz = 1-fz;
//
//						OutReal f = ( oneMinusFx * oneMinusFy * oneMinusFz );
//						o.addCol ( cube[0], ( data * f ) );
//
//						f = ( ( fx ) * oneMinusFy * oneMinusFz );
//						o.addCol ( cube[1], ( data * f ) );
//
//#ifdef SOFA_NEW_HEXA
//						f = ( oneMinusFx * ( fy ) * oneMinusFz );
//						o.addCol ( cube[3], ( data * f ) );
//
//						f = ( ( fx ) * ( fy ) * oneMinusFz );
//						o.addCol ( cube[2], ( data * f ) );
//
//#else
//						f = ( oneMinusFx * ( fy ) * oneMinusFz );
//						o.addCol ( cube[2], ( data * f ) );
//
//						f = ( ( fx ) * ( fy ) * oneMinusFz );
//						o.addCol ( cube[3], ( data * f ) );
//
//#endif
//						f = ( oneMinusFx * oneMinusFy * ( fz ) );
//						o.addCol ( cube[4], ( data * f ) );
//
//						f = ( ( fx ) * oneMinusFy * ( fz ) );
//						o.addCol ( cube[5], ( data * f ) );
//
//#ifdef SOFA_NEW_HEXA
//						f = ( oneMinusFx * ( fy ) * ( fz ) );
//						o.addCol ( cube[7], ( data * f ) );
//
//						f = ( ( fx ) * ( fy ) * ( fz ) );
//						o.addCol ( cube[6], ( data * f ) );
//#else
//						f = ( oneMinusFx * ( fy ) * ( fz ) );
//						o.addCol ( cube[6], ( data * f ) );
//
//						f = ( ( fx ) * ( fy ) * ( fz ) );
//						o.addCol ( cube[7], ( data * f ) );
//#endif
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperEdgeSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//			const sofa::helper::vector<topology::Edge>& edges = this->fromTopology->getEdges();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//						const topology::Edge edge = edges[this->map.getValue()[indexIn].in_index];
//						const OutReal fx = ( OutReal ) map.getValue()[indexIn].baryCoords[0];
//
//						o.addCol ( edge[0], data * ( 1-fx ) );
//						o.addCol ( edge[1], data * ( fx ) );
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTriangleSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//			const sofa::helper::vector<topology::Triangle>& triangles = this->fromTopology->getTriangles();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//						const topology::Triangle triangle = triangles[this->map.getValue()[indexIn].in_index];
//						const OutReal fx = ( OutReal ) map.getValue()[indexIn].baryCoords[0];
//						const OutReal fy = ( OutReal ) map.getValue()[indexIn].baryCoords[1];
//
//						o.addCol (triangle[0],data * ( 1-fx-fy ) );
//						o.addCol (triangle[1],data * ( fx ) );
//						o.addCol (triangle[2],data * ( fy ) );
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperQuadSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//			const sofa::helper::vector<topology::Quad>& quads = this->fromTopology->getQuads();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//						const OutReal fx = ( OutReal ) map.getValue()[indexIn].baryCoords[0];
//						const OutReal fy = ( OutReal ) map.getValue()[indexIn].baryCoords[1];
//						const sofa::core::topology::BaseMeshTopology::Quad& quad = quads[map.getValue()[indexIn].in_index];
//
//						o.addCol (quad[0], data * ( ( 1-fx ) * ( 1-fy ) ) );
//						o.addCol (quad[1], data * ( ( fx ) * ( 1-fy ) ) );
//						o.addCol (quad[3], data * ( ( 1-fx ) * ( fy ) ) );
//						o.addCol (quad[2], data * ( ( fx ) * ( fy ) ) );
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperTetrahedronSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//			const sofa::helper::vector<topology::Tetrahedron>& tetrahedra = this->fromTopology->getTetrahedra();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//						const OutReal fx = ( OutReal ) map.getValue()[indexIn].baryCoords[0];
//						const OutReal fy = ( OutReal ) map.getValue()[indexIn].baryCoords[1];
//						const OutReal fz = ( OutReal ) map.getValue()[indexIn].baryCoords[2];
//						int index = map.getValue()[indexIn].in_index;
//						const topology::Tetrahedron& tetra = tetrahedra[index];
//
//						o.addCol (tetra[0], data * ( 1-fx-fy-fz ) );
//						o.addCol (tetra[1], data * fx );
//						o.addCol (tetra[2], data * fy );
//						o.addCol (tetra[3], data * fz );
//					}
//				}
//			}
//		}
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::applyJT ( typename In::MatrixDeriv& out, const typename Out::MatrixDeriv& in )
//		{
//			typename Out::MatrixDeriv::RowConstIterator rowItEnd = in.end();
//			const sofa::helper::vector< topology::Hexahedron >& cubes = this->fromTopology->getHexahedra();
//
//			for (typename Out::MatrixDeriv::RowConstIterator rowIt = in.begin(); rowIt != rowItEnd; ++rowIt)
//			{
//				typename Out::MatrixDeriv::ColConstIterator colItEnd = rowIt.end();
//				typename Out::MatrixDeriv::ColConstIterator colIt = rowIt.begin();
//
//				if (colIt != colItEnd)
//				{
//					typename In::MatrixDeriv::RowIterator o = out.writeLine(rowIt.index());
//
//					for (; colIt != colItEnd; ++colIt)
//					{
//						unsigned indexIn = colIt.index();
//						InDeriv data = (InDeriv) Out::getDPos(colIt.val());
//
//						const OutReal fx = ( OutReal ) map.getValue()[indexIn].baryCoords[0];
//						const OutReal fy = ( OutReal ) map.getValue()[indexIn].baryCoords[1];
//						const OutReal fz = ( OutReal ) map.getValue()[indexIn].baryCoords[2];
//						const OutReal oneMinusFx = 1-fx;
//						const OutReal oneMinusFy = 1-fy;
//						const OutReal oneMinusFz = 1-fz;
//
//						int index = map.getValue()[indexIn].in_index;
//						const topology::Hexahedron& cube = cubes[index];
//
//						o.addCol (cube[0], data * ( oneMinusFx * oneMinusFy * oneMinusFz ) );
//						o.addCol (cube[1], data * ( ( fx ) * oneMinusFy * oneMinusFz ) );
//						o.addCol (cube[3], data * ( oneMinusFx * ( fy ) * oneMinusFz ) );
//						o.addCol (cube[2], data * ( ( fx ) * ( fy ) * oneMinusFz ) );
//						o.addCol (cube[4], data * ( oneMinusFx * oneMinusFy * ( fz ) ) );
//						o.addCol (cube[5], data * ( ( fx ) * oneMinusFy * ( fz ) ) );
//						o.addCol (cube[7], data * ( oneMinusFx * ( fy ) * ( fz ) ) );
//						o.addCol (cube[6], data * ( ( fx ) * ( fy ) * ( fz ) ) );
//					}
//				}
//			}
//		}
//
//		/************************************* Topological Changes ***********************************/
//
//		template <class In, class Out>
//		void BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange(core::topology::Topology* t)
//		{
//			using sofa::core::behavior::MechanicalState;
//
//			if (t != this->fromTopology) return;
//
//			if ( this->fromTopology->beginChange() == this->fromTopology->endChange() )
//			return;
//
//			std::list<const core::topology::TopologyChange *>::const_iterator itBegin = this->fromTopology->beginChange();
//			std::list<const core::topology::TopologyChange *>::const_iterator itEnd = this->fromTopology->endChange();
//
//			for ( std::list<const core::topology::TopologyChange *>::const_iterator changeIt = itBegin;
//			changeIt != itEnd; ++changeIt )
//			{
//				const core::topology::TopologyChangeType changeType = ( *changeIt )->getChangeType();
//				switch ( changeType )
//				{
//					//TODO: implementation of BarycentricMapperHexahedronSetTopology<In,Out>::handleTopologyChange()
//					case core::topology::ENDING_EVENT: ///< To notify the end for the current sequence of topological change events
//					{
//						if(!_invalidIndex.empty())
//						{
//							helper::vector<MappingData>& mapData = *(map.beginEdit());
//
//							for ( std::set<int>::const_iterator iter = _invalidIndex.begin();
//							iter != _invalidIndex.end(); ++iter )
//							{
//								const int j = *iter;
//								if ( mapData[j].in_index == -1 ) // compute new mapping
//								{
//									//	std::cout << "BarycentricMapperHexahedronSetTopology : new mapping" << std::endl;
//									Vector3 coefs;
//									typename In::Coord pos;
//									pos[0] = mapData[j].baryCoords[0];
//									pos[1] = mapData[j].baryCoords[1];
//									pos[2] = mapData[j].baryCoords[2];
//
//									// find nearest cell and barycentric coords
//									Real distance = 1e10;
//
//									int index = -1;
//									// When smoothing a mesh, the element has to be found using the rest position of the point. Then, its position is set using this element.
//									if( this->toTopology)
//									{
//										typedef MechanicalState<Out> MechanicalStateT;
//										MechanicalStateT* mState;
//										this->toTopology->getContext()->get( mState);
//										if( !mState)
//										{
//											std::cerr << "Can not find mechanical state." << std::endl;
//										}
//										else
//										{
//											const typename MechanicalStateT::VecCoord& xto0 = *(mState->getX0());
//											index = _fromGeomAlgo->findNearestElementInRestPos ( xto0[j], coefs, distance );
//											//_fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );
//											coefs = _fromGeomAlgo->computeHexahedronRestBarycentricCoeficients(index, pos);
//										}
//									}
//									else
//									{
//										index = _fromGeomAlgo->findNearestElementInRestPos ( pos, coefs, distance );
//									}
//
//									if ( index != -1 )
//									{
//										mapData[j].baryCoords[0] = ( Real ) coefs[0];
//										mapData[j].baryCoords[1] = ( Real ) coefs[1];
//										mapData[j].baryCoords[2] = ( Real ) coefs[2];
//										mapData[j].in_index = index;
//									}
//								}
//							}
//
//							map.endEdit();
//							_invalidIndex.clear();
//						}
//					}
//					break;
//					case core::topology::POINTSINDICESSWAP: ///< For PointsIndicesSwap.
//					break;
//					case core::topology::POINTSADDED: ///< For PointsAdded.
//					break;
//					case core::topology::POINTSREMOVED: ///< For PointsRemoved.
//					break;
//					case core::topology::POINTSRENUMBERING: ///< For PointsRenumbering.
//					break;
//					case core::topology::TRIANGLESADDED: ///< For Triangles Added.
//					break;
//					case core::topology::TRIANGLESREMOVED: ///< For Triangles Removed.
//					break;
//					case core::topology::HEXAHEDRAADDED: ///< For HexahedraAdded.
//					{
//					}
//					break;
//					case core::topology::HEXAHEDRAREMOVED: ///< For HexahedraRemoved.
//					{
//						// std::cout << "BarycentricMapperHexahedronSetTopology() HEXAHEDRAREMOVED" << std::endl;
//						const unsigned int nbHexahedra = this->fromTopology->getNbHexahedra();
//
//						const sofa::helper::vector<unsigned int> &hexahedra = ( static_cast< const component::topology::HexahedraRemoved *> ( *changeIt ) )->getArray();
//						//        sofa::helper::vector<unsigned int> hexahedra(tab);
//
//						for ( unsigned int i=0; i<hexahedra.size(); ++i )
//						{
//							// remove all references to the removed cubes from the mapping data
//							unsigned int cubeId = hexahedra[i];
//							for ( unsigned int j=0; j<map.getValue().size(); ++j )
//							{
//								if ( map.getValue()[j].in_index == ( int ) cubeId ) // invalidate mapping
//								{
//									Vector3 coefs;
//									coefs[0] = map.getValue()[j].baryCoords[0];
//									coefs[1] = map.getValue()[j].baryCoords[1];
//									coefs[2] = map.getValue()[j].baryCoords[2];
//
//									typename In::Coord restPos = _fromGeomAlgo->getRestPointPositionInHexahedron ( cubeId, coefs );
//
//									helper::vector<MappingData>& vectorData = *(map.beginEdit());
//									vectorData[j].in_index = -1;
//									vectorData[j].baryCoords[0] = restPos[0];
//									vectorData[j].baryCoords[1] = restPos[1];
//									vectorData[j].baryCoords[2] = restPos[2];
//									map.endEdit();
//
//									_invalidIndex.insert(j);
//								}
//							}
//						}
//
//						// renumber
//						unsigned int lastCubeId = nbHexahedra-1;
//						for ( unsigned int i=0; i<hexahedra.size(); ++i, --lastCubeId )
//						{
//							unsigned int cubeId = hexahedra[i];
//							for ( unsigned int j=0; j<map.getValue().size(); ++j )
//							{
//								if ( map.getValue()[j].in_index == ( int ) lastCubeId )
//								{
//									helper::vector<MappingData>& vectorData = *(map.beginEdit());
//									vectorData[j].in_index = cubeId;
//									map.endEdit();
//								}
//							}
//						}
//					}
//					break;
//					case core::topology::HEXAHEDRARENUMBERING: ///< For HexahedraRenumbering.
//					break;
//					default:
//					break;
//				}
//			}
//		}

		// handle topology changes depending on the topology
//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::handleTopologyChange ( core::topology::Topology* t )
//		{
//			if (mapper)
//			mapper->handleTopologyChange(t);
//		}
//
//		template <class TIn, class TOut>
//		void ExtendedBarycentricMapping<TIn, TOut>::beginAddContactPoint()
//		{
//			if (!m_init)
//			{
//				if (this->mapper)
//				{
//					this->mapper->clear(0);
//				}
//
//				this->toModel->resize(0);
//
//				m_init = true;
//			}
//			else
//			m_init = false;
//		}

//		template <class TIn, class TOut>
//		int ExtendedBarycentricMapping<TIn, TOut>::addContactPointFromInputMapping(const sofa::defaulttype::Vector3& pos, std::vector< std::pair<int, double> > &baryCoords)
//		{
//			if (this->mapper)
//			{
//				const typename In::VecCoord& xfrom = *this->fromModel->getX();
//				int index = mapper->addContactPointFromInputMapping(xfrom, pos, baryCoords);
//				this->toModel->resize(index+1);
//				return index;
//			}
//
//			return 0;
//		}

	} // namespace mapping

} // namespace component

} // namespace sofa

#endif
