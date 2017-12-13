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
#ifndef SOFA_COMPONENT_MAPPING_MapperLinearTetrahedronSetTopology_INL
#define SOFA_COMPONENT_MAPPING_MapperLinearTetrahedronSetTopology_INL

#include <MapperLinearTetrahedronSetTopology.h>
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
#include <limits>

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



namespace sofa
{

	namespace component
	{

		namespace mapping
		{

			using namespace sofa::defaulttype;

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getElementsAroundElements(unsigned int rootElement, IdVec& elements)
			{
//				std::cout<<"getelementaroundElementCalled \n";
				sofa::component::topology::TriangleSetTopologyContainer* outTriangleTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::TetrahedronSetTopologyContainer* outTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::QuadraticMeshTopology* outQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->toTopology));


				if(outTriangleTopology && !outTetrahedronTopology)
				{
					const sofa::component::topology::TriangleSetTopologyContainer::EdgesInTriangle edges = outTriangleTopology->getEdgesInTriangle(rootElement);

					for(unsigned int i=0; i< 3; i++)
					{
						const IdVec triangles = outTriangleTopology->getTrianglesAroundEdge(edges[i]);
						for(unsigned int j=0; j< triangles.size(); j++)
						{
							unsigned int neighborId = triangles[j];
							if(neighborId != rootElement)
								elements.push_back(neighborId);
						}
					}
				}


				if(outTetrahedronTopology)
				{
					const sofa::component::topology::TetrahedronSetTopologyContainer::TrianglesInTetrahedron triangles = outTetrahedronTopology->getTrianglesInTetrahedron(rootElement);

					for(unsigned int i=0; i< 4; i++)
					{
						const IdVec tetras = outTetrahedronTopology->getTetrahedraAroundTriangle(triangles[i]);

						for(unsigned int j=0; j< tetras.size(); j++)
						{
							unsigned int neighborId = tetras[j];
							if(neighborId != rootElement)
							{
								elements.push_back(neighborId);
							}
						}
					}
				}


				if(outQuadTetraTopology)
				{
//					std::cout<<"Looking for elements around tet10 element\n";
					const sofa::component::topology::QuadraticMeshTopology::TrianglesInTetrahedron triangles =outQuadTetraTopology->getQuadraticTrianglesInQuadraticTetrahedron(rootElement);

					for(unsigned int i=0; i< 4; i++)
					{
						const IdVec tetras = outQuadTetraTopology->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[i]);


						for(unsigned int j=0; j< tetras.size(); j++)
						{
							unsigned int neighborId = tetras[j];
//							std::cout<<"Current neighbour id is "<<neighborId<<"\n";
							if(neighborId != rootElement)
								elements.push_back(neighborId);
						}
					}
				}



				if(!outTriangleTopology && !outTetrahedronTopology && !outQuadTetraTopology)
				{
					std::cout<<"Must use triangle, tetra or quadtetra topology as output topology \n";
				}

			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getElementsAroundVertex(unsigned int vertex, IdVec& elements)
			{
				sofa::component::topology::TriangleSetTopologyContainer* outTriangleTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::TetrahedronSetTopologyContainer* outTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::QuadraticMeshTopology* outQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->toTopology));

				if(outTriangleTopology && !outTetrahedronTopology)
				{
					const IdVec triangles  = outTriangleTopology->getTrianglesAroundVertex(vertex);


					for(unsigned int i=0; i< triangles.size(); i++)
					{
						unsigned int neighborId = triangles[i];
						elements.push_back(neighborId);
					}
				}

				if(outTetrahedronTopology)
				{

					const IdVec tetras  = outTetrahedronTopology->getTrianglesAroundVertex(vertex);


					for(unsigned int i=0; i< tetras.size(); i++)
					{
						unsigned int neighborId = tetras[i];
						elements.push_back(neighborId);
					}
				}


				if(outQuadTetraTopology)
				{

					const IdVec quadTetras  = outQuadTetraTopology->getQuadraticTrianglesAroundVertex(vertex);


					for(unsigned int i=0; i< quadTetras.size(); i++)
					{
						unsigned int neighborId = quadTetras[i];
						elements.push_back(neighborId);
					}
				}


				if(!outTriangleTopology && !outTetrahedronTopology && !outQuadTetraTopology)
				{
					std::cout<<"Must use triangle, tetra or quadtetra topology as output topology \n";
				}

			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getPointsInElementToTopology(unsigned int elementId, IdVec& points)
			{

				sofa::component::topology::TriangleSetTopologyContainer* outTriangleTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::TetrahedronSetTopologyContainer* outTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::QuadraticMeshTopology* outQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->toTopology));

				if(outTriangleTopology && !outTetrahedronTopology)
				{
					const sofa::component::topology::TriangleSetTopologyContainer::Triangle triangle  = outTriangleTopology->getTriangle(elementId);

					for(unsigned int i=0; i< 3; i++)
					{
						unsigned int pointId = triangle[i];
						points.push_back(pointId);
					}
				}


				if(outTetrahedronTopology)
				{

					sofa::component::topology::TetrahedronSetTopologyContainer::Tetra tetra = outTetrahedronTopology->getTetra(elementId);

					for(unsigned int i=0; i< 4; i++)
					{
						unsigned int pointId = tetra[i];
						points.push_back(pointId);
					}
				}


				if(outQuadTetraTopology)
				{
					sofa::component::topology::QuadraticMeshTopology::QuadTetra quadTetra = outQuadTetraTopology->getQuadTetrahedra(elementId);


					for(unsigned int i=0; i< 10; i++)
					{
						unsigned int pointId = quadTetra[i];
						points.push_back(pointId);
					}
				}



				if(!outTriangleTopology && !outTetrahedronTopology && !outQuadTetraTopology)
				{
					std::cout<<"Must use triangle, tetra or quadtetra topology as output topology \n";
				}
			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getPointsInElementFromTopology(unsigned int elementId, IdVec& points)
			{
				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
				sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				if(inTetrahedronTopology)
				{
					const sofa::component::topology::TetrahedronSetTopologyContainer::Tetrahedron tetra  = inTetrahedronTopology->getTetra(elementId);

					for(unsigned int i=0; i< 4; i++)
					{
						unsigned int pointId = tetra[i];
						points.push_back(pointId);
					}
				}

				if(inQuadTetraTopology)
				{
					const sofa::component::topology::QuadraticMeshTopology::QuadTetra quadTetra  = inQuadTetraTopology->getQuadTetrahedra(elementId);

					for(unsigned int i=0; i< 10; i++)
					{
						unsigned int pointId = quadTetra[i];
						points.push_back(pointId);
					}
				}


				if(!inQuadTetraTopology && !inTetrahedronTopology)
				{
					std::cout<<"Must use tetra  or quad tetra topology\n";
				}
			}


			template <class In, class Out> unsigned int MapperLinearTetrahedronSetTopology<In,Out>::getNumberOfOutElements()
			{

			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getTrianglesInTetrahedron(unsigned int element, TrianglesInElement& triangles)
			{

				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
				sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));


				if(inTetrahedronTopology)
				{
					const TrianglesInElement tempTriangles = inTetrahedronTopology->getTrianglesInTetrahedron(element);
					triangles = tempTriangles;

				}

				if(inQuadTetraTopology)
				{
					const TrianglesInElement tempTriangles = inQuadTetraTopology->getQuadraticTrianglesInQuadraticTetrahedron(element);
					triangles = tempTriangles;
				}


				if(!inQuadTetraTopology && !inTetrahedronTopology)
				{
					std::cout<<"Must use tetra  or quad tetra topology\n";
				}


			}

//			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getTrianglesAroundTetrahedron(unsigned int tetrahedron, IdVec& triangles)
//			{
//				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
//				if(inTetrahedronTopology)
//				{
//					const TrianglesInElement tempTriangles = inTetrahedronTopology->getTriangles
//					triangles = tempTriangles;
//
//				}
//				else
//					std::cout<<"Must use tetrahedron topology\n";
//
//			}

//			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::getTetrahedraAroundTriangle(unsigned int triangle, IdVec& tetrahedra)
//			{
//				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
//				sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));
//
//				if(inTetrahedronTopology)
//				{
////					std::cout<<"Get tetrahedra around triangle no "<<triangle<<"\n";
////					if(triangle == 1)
////						const IdVec tempTetrahedra = inTetrahedronTopology->getTetrahedraAroundTriangle(16);//triangle);
//					for(unsigned int i=0; i< 2;i++)//tempTetrahedra.size(); i++)
//					{
//						tetrahedra.push_back(i);//tempTetrahedra[i]);
//					}
//
//				}
//
//				if(inQuadTetraTopology)
//				{
//					const TrianglesInElement tempTriangles = inQuadTetraTopology->getQuadraticTrianglesInQuadraticTetrahedron(element);
//					triangles = tempTriangles;
//				}
//
//				if(!inQuadTetraTopology && !inTetrahedronTopology)
//				{
//					std::cout<<"Must use tetra  or quad tetra topology\n";
//				}
//
//
//			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::init(const typename Out::VecCoord& out, const typename In::VecCoord& in)
			{
				///////////////////////////////////////////DebugOutput ////////////////////////////////////////////////////////////
//				double currentVTKPointArray[3];
//				double currentVTKNormalArray[3];
//
//				const char* filenameDebugPoints = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfos.vtk";
//				const char* filenameDebugTriangles = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/VolumenModelle/Ergebnisse/Elephant/OptimizationInfosTopology.vtk";
//
//				m_PointContainerDebugPoints = vtkPoints::New();
//				m_PointContainerDebugPoints->SetDataTypeToDouble();
//
//				m_VectorsOptiDebug = vtkFloatArray::New();
//				m_VectorsOptiDebug->SetNumberOfComponents(3);
//				m_VectorsOptiDebug->SetName("normals");
//
//				m_ScalarsOptiDebug = vtkFloatArray::New();
//				m_ScalarsOptiDebug->SetNumberOfComponents(1);
//				m_ScalarsOptiDebug->SetName("scalars");
//
//				m_PointContainerDebugTriangles = vtkPoints::New();
//				m_PointContainerDebugTriangles->SetDataTypeToDouble();
//
//				m_TriangleContainerDebugTriangles = vtkCellArray::New();
//
//				m_ScalarsDebugTriangles = vtkFloatArray::New();
//				m_ScalarsDebugTriangles->SetNumberOfComponents(1);
//				m_ScalarsDebugTriangles->SetName("scalars");
//				//m_CellDataDebugTriangles->InsertNextTuple()
//
//				m_PointContainerRecursiveMapping = vtkPoints::New();
//				m_PointContainerRecursiveMapping->SetDataTypeToDouble();
//				//m_PointContainerRecursiveMapping->SetNumberOfPoints(out.size());
//
//				m_TriangleContainerRecursiveMapping = vtkCellArray::New();
//
//				m_MappingPrintCounter = 0;
//				m_MappingPointCounter = 0;
//				m_MappingElementCounter=0;


				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
//				unsigned int triangleSize = inTetrahedronTopology->getNbTriangles();
//				std::cout<<"triangle size is "<<triangleSize<<"\n";
//
//				for(unsigned int i=0;i<triangleSize;i++)
//				{
//					const IdVec tempTetrahedra = inTetrahedronTopology->getTetrahedraAroundTriangle(i);
//				}

				//////////////////////////////give some infos about mapped topologies//////////////////////


				unsigned int inDOF = in.size();
				unsigned int outDOF = out.size();

				unsigned int inElementNo, outElementNo;


				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
				sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				if(inTetrahedronTopology)
				{
					inElementNo = inTetrahedronTopology->getNbTetras();
				}

				if(inQuadTetraTopology)
				{
//						std::cout<<"Quadratic mesh topology found!\n";
					inElementNo = inQuadTetraTopology->getNbQuadTetrahedra();
				}

				if(!inTetrahedronTopology && !inQuadTetraTopology)
					std::cout<<"Error, must use tetra or quadratic tetra input topology\n";

				sofa::component::topology::TriangleSetTopologyContainer* outTriangleTopology = (dynamic_cast<sofa::component::topology::TriangleSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::TetrahedronSetTopologyContainer* outTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->toTopology));
				sofa::component::topology::QuadraticMeshTopology* outQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->toTopology));

				if(outTriangleTopology && !outTetrahedronTopology)
				{
					outElementNo =  outTriangleTopology->getNbTriangles();
				}


				if(outTetrahedronTopology)
				{

					outElementNo =  outTetrahedronTopology->getNbTetras();
				}


				if(outQuadTetraTopology)
				{
					outElementNo =  outQuadTetraTopology->getNbQuadTetrahedra();

				}


				if(!outTriangleTopology && !outTetrahedronTopology && !outQuadTetraTopology)
				{
					std::cout<<"Must use triangle, tetra or quadtetra topology as output topology \n";
				}

				std::cout<<"Initialize mapping: Mapping a mesh with "<< outElementNo << " elements and "<<outDOF<<" DOF to a mesh with "<< inElementNo<<" elements and "<<inDOF <<" DOF \n";



				///////////////////////////////////////////////////////////////////////////////////////////////////

				//resize outmap vectpor
				map3d.resize(out.size());

				//get first element of in topology
				unsigned int firstInIndex;
				bool isQuadratic = false;

				const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();

				if(tetrahedra.size() == 0)
				{
					sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra dummyQuadTetras;
					sofa::core::topology::QuadraticBaseMeshTopology::SeqQuadTetrahedra& quadraticTetrahedra = dummyQuadTetras;
					if((dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology)))
					{
						quadraticTetrahedra = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology))->getQuadTetrahedra();
					}
					else
					{
						std::cout<<"Error no tet4 or tet10 found as input topology\n";
						return;
					}
					firstInIndex = quadraticTetrahedra[0][0];

				}
				else
					firstInIndex = tetrahedra[0][0];


				// get nearest point to first point of first element -> add to map
				double currentDistance = std::numeric_limits<double>::max();
				unsigned int currentIndex = 0;
				InCoord referencePoint = in[firstInIndex];

				for(unsigned int i;i<out.size(); i++)
				{
					double tempDistance = (out[i]-referencePoint).norm();
					if(tempDistance < currentDistance)
					{
						currentIndex = i;
						currentDistance = tempDistance;
					}
				}

				//find element around currentIndex
				IdVec elementSet;
				getElementsAroundVertex(currentIndex, elementSet);
				unsigned int currentElement = elementSet[0];

				std::set<unsigned int> elementsMapped;
				std::set<unsigned int> pointsMapped;
				std::map<unsigned int, ToMapData> elementsToMap;
//				unsigned int elementsToMapNo = getNumberOfOutElements();

				//add starting element to map set
				ToMapData startData;
				startData.outIndex = currentIndex;
				startData.startInIndex = firstInIndex;
				startData.startLocalCoords[0] = 0.3;
				startData.startLocalCoords[1] = 0.3;
				startData.startLocalCoords[2] = 0.3;
				elementsToMap[startData.outIndex]= (startData);

				//start mapping (while mapped size != in size)
				typedef typename std::set<unsigned int>::iterator MappedElementsSetIteratorType;
				typedef typename std::map<unsigned int, ToMapData>::iterator ToMapSetIteratorType;
//				elementsToMap::
				while(true)
				{
//					std::cout<<"Another recursion\n";
					if(elementsToMap.size() == 0)
						break;

					//currentElement
					ToMapSetIteratorType myItt;
					myItt = elementsToMap.begin();
//					mapIterator = pingMapPtr->begin();
//					tempData = mapIterator->second;
					ToMapData tempData = myItt->second;


					//map element
//					std::cout<<"Trying to map element "<<tempData.outIndex<<"...";
					double isMapped = MapElement(tempData.outIndex, tempData.startInIndex, tempData.startLocalCoords ,currentDistance, in, out, elementsMapped, pointsMapped, false);
//					currentDistance = 1;

//					std::cout<<"mapped with distance"<< currentDistance<<" to element "<< tempData.startInIndex<<"\n";

					if(!isMapped)
						std::cout<<"Element "<<tempData.outIndex<<"not mapped, this should not happen!!\n";




					//delete from to map set, add to elementsMapped set
					elementsToMap.erase(myItt);

					elementsMapped.insert(tempData.outIndex);


					//add neighbours to toMapMap (if not in elementsMapped)
					IdVec neighborIds;
					getElementsAroundElements(tempData.outIndex, neighborIds);
//					if(tempData.outIndex == 696)
//					{
//						for(unsigned int i=0;i<neighborIds.size();i++)
//						{
//							std::cout<<"Neighbour Ids:"<<neighborIds[i]<<"\n";
//						}
//					}

//					std::cout<<neighborIds.size()<<" neighbors found\n";

					for(unsigned int i=0;i<neighborIds.size();i++)
					{
						MappedElementsSetIteratorType itt;
						itt = elementsMapped.find(neighborIds[i]);


						if(itt == elementsMapped.end())
						{
							//first test if in elements to map
							ToMapSetIteratorType itt2;
							itt2 = elementsToMap.find(neighborIds[i]);

							if(itt2 == elementsToMap.end())
							{

//								if(neighborIds[i] == 712)
//									std::cout<<"neighbour 712 added from element "<<tempData.outIndex<<"\n";
	//							std::cout<<"Adding neighbor with id "<<neighborIds[i]<<"to toMapMap\n";
								ToMapData newElement;
								newElement.outIndex = neighborIds[i];
								newElement.startInIndex = tempData.startInIndex;
								newElement.startLocalCoords[0] = 0.3;
								newElement.startLocalCoords[1] = 0.3;
								newElement.startLocalCoords[2] = 0.3;
								elementsToMap[newElement.outIndex] = (newElement);
							}
						}
					}

//					std::cout<<"There are still "<<elementsToMap.size()<<" elements to map\n";


				}


				std::cout<<"Init complete\n";



//				std::cout<<"Init really complete\n";


			}

			template <class In, class Out> bool MapperLinearTetrahedronSetTopology<In,Out>::MapElement(unsigned int outElementIndex, unsigned int & currentInElement, InCoord &currentCoefs,double & currentDistance, const typename In::VecCoord& in,
					const typename Out::VecCoord& out, std::set<unsigned int>  &elementsMapped, std::set<unsigned int> &pointsMapped, bool debugOutput)
			{

//				std::cout<<"!!!!!!!!!!!!!!!!!!Mapping element "<<outElementIndex<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
//				if(outElementIndex == 345)
//				{
//					debugOutput = true;
//					std::cout<<"Brea here\n";
//				}

				std::set<unsigned int>::iterator itt;
				itt = elementsMapped.find(outElementIndex);

				if(itt != elementsMapped.end())
				{
//					std::cout<<"Error, element was already mapped\n";
					return false;
				}

				//get all points in element
				IdVec pointsInElement;
				getPointsInElementToTopology(outElementIndex,  pointsInElement);

				bool isMapped = true;

//				std::cout<<"mapping points"<<pointsInElement<<"\n";

				//for each poit
				for(unsigned int i=0; i< pointsInElement.size(); i++)
				{
					//check if already mapped
					itt = pointsMapped.find(pointsInElement[i]);


					//if not, map the point
					if(itt == pointsMapped.end())
					{
						std::set<unsigned int> elementsVisited;
						double currentMinError =  std::numeric_limits<double>::max();
						OutCoord referencePosition = out[pointsInElement[i]];

						unsigned int finalIndex;
						double distance =  std::numeric_limits<double>::max();
						InCoord localCoords;
//						localCoords.clear();

//						std::cout<<"Mapping point "<<pointsInElement[i]<<"\n";

						/////////////////////////DebugOutput////////////

//						double currentVTKPointArray[3];
//
//						m_MappingPointCounter = 0;
//						m_MappingElementCounter = 1;
//						currentVTKPointArray[0] = referencePosition[0];
//						currentVTKPointArray[1] = referencePosition[1];
//						currentVTKPointArray[2] = referencePosition[2];
//
//						m_PointContainerDebugPoints->InsertNextPoint( currentVTKPointArray);
//						m_ScalarsOptiDebug->InsertNextTuple1(m_MappingPointCounter);
//						m_MappingPointCounter++;


						RecursivelyFindClosestPoint(currentInElement, currentMinError, referencePosition,in,  localCoords, finalIndex, distance, elementsVisited, debugOutput);
						currentDistance = distance;

//						std::cout<<"Point "<<pointsInElement[i]<<"mapped to element "<<finalIndex<<" with error "<<distance<<"\n";

						//Verbose output
//						if(distance > 1e-6)
//						{
//							std::cout<<"Error above threshold: "<<distance<<"\n";
//
//							std::cout<<"Point "<<pointsInElement[i]<<"mapped to element "<<finalIndex<<"\n";
//						}

						//add in_index and coords to map3d
						map3d[pointsInElement[i]].inIndex = finalIndex;
						map3d[pointsInElement[i]].baryCoords[0] = localCoords[0];
						map3d[pointsInElement[i]].baryCoords[1] = localCoords[1];
						map3d[pointsInElement[i]].baryCoords[2] = localCoords[2];


						pointsMapped.insert(pointsInElement[i]);

						currentInElement = finalIndex;

//						std::cout<<"Point "<< pointsInElement[i] <<" mapped to "<<finalIndex<<" with distance "<<distance<<"\n";
					}

//					////////////////////////////////////DebugOutput //////////////////////////////////////
//					if(isMapped)
//					{
//						trianglesMapped.insert(startOutElement);
//						//add to triangle container
//						this->m_MappingPrintCounter = m_TriangleContainerRecursiveMapping->InsertNextCell(3,currentVTKTriangle);

						//print debug output if necessary
	//					if(printFlag)
	//					{
	//						if((this->m_MappingPrintCounter % printFlag) == 1)
	//						{
	//							std::cout<<"Write debug data number "<< this->m_MappingPrintCounter<<"\n";
	//							std::string filenameRecursiveMappingDebug("/opt/Ergebnisse/tmp/RecursiveMappingDebugData");
	//							std::ostringstream oss;
	//							oss << "_"<< this->m_MappingPrintCounter;
	//							filenameRecursiveMappingDebug += oss.str() + ".vtk";
	//
	//							vtkUnstructuredGrid* myGrid = vtkUnstructuredGrid::New();
	//							myGrid->SetPoints(m_PointContainerRecursiveMapping);
	//							myGrid->SetCells(VTK_TRIANGLE, m_TriangleContainerRecursiveMapping);
	//
	//							vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();
	//							writer->SetInput(myGrid);
	//							writer->SetFileName(filenameRecursiveMappingDebug.c_str());
	//							writer->Write();
	//
	//							myGrid->Delete();
	//							writer->Delete();
	//
	//						}
	//
	//					}
//
//						if(debugOutput)
//						{
//								std::cout<<"Saving Debug information ... \n";
//								//save the vtk debug output
//								const char* filenamePointDebug = "/homes/staff/suwelack/NMBE/Temp/PointDataDebug.vtk";
//								const char* filenameTriangleDebug = "/homes/staff/suwelack/NMBE/Temp/TriangleDataDebug.vtk";
//
//								//std::cout<<"PointNumber: "<<i<<", Updated distance:"<<distance<<", final tetraId: "<<index-c1<<" ,coefs:"<<coefs<<"Final tetra:"<<theTopology->getQuadTetrahedra(index-c1)<<"\n";
//
//								vtkUnstructuredGrid* myGridTopoDebug = vtkUnstructuredGrid::New();
//								myGridTopoDebug->SetPoints(m_PointContainerDebugTriangles);
//								myGridTopoDebug->SetCells(VTK_TETRA, m_TriangleContainerDebugTriangles);
//								myGridTopoDebug->GetCellData()->SetScalars(m_ScalarsDebugTriangles);
//
//								vtkUnstructuredGridWriter* writerTopoDebug = vtkUnstructuredGridWriter::New();
//								writerTopoDebug->SetInput(myGridTopoDebug);
//								writerTopoDebug->SetFileName(filenameTriangleDebug);
//								writerTopoDebug->Write();
//
//								myGridTopoDebug->Delete();
//								writerTopoDebug->Delete();
//
//								vtkPolyData* myGrid2 = vtkPolyData::New();
//								myGrid2->SetPoints(m_PointContainerDebugPoints);
//	//							myGrid2->GetPointData()->SetVectors(m_VectorsOptiDebug);
//								myGrid2->GetPointData()->SetScalars(m_ScalarsOptiDebug);
//
//								vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
//								vtkSmartPointer<vtkVertexGlyphFilter>::New();
//								vertexGlyphFilter->AddInput(myGrid2);
//								vertexGlyphFilter->Update();
//
//								vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();
//
//								writer2->SetInput(vertexGlyphFilter->GetOutput());
//								writer2->SetFileName(filenamePointDebug);
//								writer2->Write();
//
//								myGrid2->Delete();
//								writer2->Delete();
//
//								debugOutput = false;
//
//						}
						////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				}



				return isMapped;




			}

			template <class In, class Out> bool MapperLinearTetrahedronSetTopology<In,Out>::RecursivelyFindClosestPoint(unsigned int startTetraId, double currentMinError, InCoord referencePosition,const typename In::VecCoord& in, InCoord& localCoords,
					unsigned int & finalElementIndex, double & distance,  std::set<unsigned int> &elementsVisited, bool debugOutput)
			{
				//std::cout<<"RecursiveClosesPoint, currentTetraId: "<<startTetraId<<"\n";
				//find coords on tetrahedron
//				sofa::component::topology::QuadraticMeshTopology* theTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				finalElementIndex = startTetraId;

				//first optimize on given starttriangleid with given local coords


				double currentDistance = FindClosestPointInTetrahedron(startTetraId, referencePosition, in, localCoords);

				//////////////////////////////////////////////DebugOutput//////////////////////////////////////
//				double currentVTKPointArray[3];
//
//				IdVec pointIds;
//				getPointsInElementFromTopology(startTetraId, pointIds);
//
//				double nodes[30];
//				unsigned int noPoints = pointIds.size();
//
//				std::cout<<"NO of points "<<noPoints<<"\n";
//
//				for(int iteri=0; iteri<3; iteri++)
//				{
//					for(int p=0; p<noPoints; p++)
//					{
//						nodes[noPoints*iteri+p] = (in[pointIds[p]])[iteri];
//					}
//				}
//
//				Vector3 testPos;
//
//				CalculateGlobalCoordsLinearTetra(localCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
//
//				currentVTKPointArray[0] = testPos[0];
//				currentVTKPointArray[1] = testPos[1];
//				currentVTKPointArray[2] = testPos[2];
//
//				m_PointContainerDebugPoints->InsertNextPoint( currentVTKPointArray);
//				m_ScalarsOptiDebug->InsertNextTuple1(m_MappingPointCounter);
//				m_MappingPointCounter++;
//
//				//add tetra
//				vtkIdType nodeIndices[4];
//				for(unsigned int p=0; p<4; p++)
//				{
//					currentVTKPointArray[0] = (in[pointIds[p]])[0];
//					currentVTKPointArray[1] = (in[pointIds[p]])[1];
//					currentVTKPointArray[2] = (in[pointIds[p]])[2];
//					nodeIndices[p] = m_PointContainerDebugTriangles->InsertNextPoint( currentVTKPointArray);
//				}
//
//				m_TriangleContainerDebugTriangles->InsertNextCell(4, nodeIndices);
//				m_ScalarsDebugTriangles->InsertNextTuple1(m_MappingElementCounter);
//				m_MappingElementCounter++;


				/////////////////////////////////////////////////////////////////////////////////////////////

				//check if on surface
//				InCoord newSurfaceCoords;
				std::vector<unsigned int> faceNumbers;
				InVecCoord newSurfaceCoordsVec;
				bool isOnFace = LocalVolumeCoordsToLocalSurfaceCoords(localCoords.ptr(), faceNumbers, newSurfaceCoordsVec);
				bool isOnSurface = false;

//				std::cout<<"This coord is on faces ";
//				for(unsigned int k; k<faceNumbers.size(); k++)
//				{
//					std::cout<<faceNumbers[k]<<", ";
//				}
//				std::cout<<"\n ";

				//if not on face or distance below tolerance: return
				if((!isOnFace)||(currentDistance<TOLERANCE))
				{
					distance = currentDistance;
					finalElementIndex = startTetraId;
					//std::cout<<"RecursiveClosesPoint, finalTetraID not on face: "<<finalTetraIndex<<"\n";
//					std::cout<<"Point "<<referencePosition<<" mapped to element "<<finalElementIndex<<" with distance "<<currentDistance<<"\n";

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

//					std::cout<<"Now testing face "<<faceNumber<<"\n";


					TrianglesInElement triangles;
					getTrianglesInTetrahedron(startTetraId, triangles);

					unsigned int startTriangleId = triangles[faceNumber];
//					IdVec candidateTetras;
//					getTetrahedraAroundTriangle(startTriangleId, candidateTetras);

					IdVec candidateTetras;

					sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
					sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					if(inTetrahedronTopology)
					{
						const IdVec candidateTetrasTemp = inTetrahedronTopology->getTetrahedraAroundTriangle(startTriangleId);//triangle);
						candidateTetras = candidateTetrasTemp ;
					}

					if(inQuadTetraTopology)
					{
//						std::cout<<"Quadratic mesh topology found!\n";
						const IdVec candidateTetrasTemp = inQuadTetraTopology->getQuadraticTetrahedraAroundQuadraticTriangle(startTriangleId);
						candidateTetras = candidateTetrasTemp ;
					}

					if(!inTetrahedronTopology && !inQuadTetraTopology)
						std::cout<<"Error, must use tetra or quadratic tetra input topology\n";

					if(candidateTetras.size() == 1)
					{
						isOnSurface = true;
						isOnSurfaceTemp = true;
					}
//
//					//if on surface face: start surface recursion
					if(!isOnSurfaceTemp)
					{
							//get neighbour tetras
							unsigned int nextTetraId;

							if(candidateTetras.size() != 2)
								std::cout<<"Error, there must be two tetras around triangle!!\n";

							if(candidateTetras[0] == startTetraId)
							{
								nextTetraId = candidateTetras[1];
							}
							else
								nextTetraId = candidateTetras[0];

							std::set<unsigned int>::iterator itt=elementsVisited.find(nextTetraId);

							if(itt == elementsVisited.end() )
							{
//								std::cout<<"Next tetra to be tested is "<<nextTetraId<<"\n";
								elementsVisited.insert(nextTetraId);
								InCoord newLocalCoords;
								newLocalCoords[0] = 0.3;
								newLocalCoords[1] = 0.3;
								newLocalCoords[2] = 0.3;
								unsigned int tempTetraIndex;
								double tempDistance = std::numeric_limits<double>::max();
								//std::cout<<"RecursiveClosesPoint, nextTetraId inside: "<<nextTetraId<<"\n";
								isOnSurfaceTemp = RecursivelyFindClosestPoint(nextTetraId, currentMinError, referencePosition, in, newLocalCoords, tempTetraIndex, tempDistance,  elementsVisited, debugOutput);

								//std::cout<<"RecursiveClosesPointCalled, old distance:"<<distance<<", new distance:"<<tempDistance<<"\n";
								if(tempDistance < currentMinError)
								{
									//convert  to volume coords
									localCoords[0]= newLocalCoords[0];
									localCoords[1]= newLocalCoords[1];
									localCoords[2]= newLocalCoords[2];
									distance = tempDistance;
									//std::cout<<"RecursiveClosesPoint, finalTetraIndexBeforeVolumeUpdate: "<<finalTetraIndex<<"\n";
									finalElementIndex = tempTetraIndex;
									currentMinError = tempDistance;
									isOnSurface = isOnSurfaceTemp;
									//std::cout<<"RecursiveClosesPoint, finalTetraIndexAfterVolumeUpdate: "<<finalTetraIndex<<"\n";
								}


								//std::cout<<"RecursiveClosesPoint, finalTetraID inside: "<<finalTetraIndex<<"\n";


							}
							else
							{
//								std::cout<<nextTetraId<<" has already been tested\n";
							}

					}
					else
					{
//						std::cout<<"!!! This is on outer surface, final element id is "<<finalElementIndex<<" with distance "<<distance<<"\n";
					}
				}

				return isOnSurface;

			}

			template <class In, class Out> bool MapperLinearTetrahedronSetTopology<In,Out>::LocalVolumeCoordsToLocalSurfaceCoords(Real* localVolumeCoords, std::vector<unsigned int>&  localFaceNumbers, InVecCoord& localSurfaceCoordsVec)
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

			template<>
			double MapperLinearTetrahedronSetTopology<Vec3fTypes, Vec3fTypes>::FindClosestPointInTetrahedron(unsigned int tetraId, InCoord referencePosition, const typename In::VecCoord& in, InCoord& localVolumeCoords)
			{
				IdVec pointIds;
								getPointsInElementFromTopology(tetraId, pointIds);

								Real nodes[30];
								unsigned int noPoints = pointIds.size();

								for(int iteri=0; iteri<3; iteri++)
								{
									for(int p=0; p<noPoints; p++)
									{
										nodes[noPoints*iteri+p] = (in[pointIds[p]])[iteri];
									}
								}


								float lb[3];
								lb[0]=0.0;
								lb[1]=0.0;
								lb[2]=0.0;

								float ub[3];
								ub[0]=1.0;
								ub[1]=1.0;
								ub[2]=1.0;

								float C[3];
								C[0]=-1.0;
								C[1]=-1.0;
								C[2]=-1.0;

								float d[1];
								d[0] = -1.0;

								float info[LM_INFO_SZ];

								InCoord testPos;

								if(noPoints ==4)
								{
									CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
									slevmar_bleic_der(CalculateGlobalCoordsLinearTetra, CalculateJacobianLinearTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
									CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
								}
								else if(noPoints ==10)
								{
									CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
									slevmar_bleic_der(CalculateGlobalCoordsQuadraticTetra, CalculateJacobianQuadraticTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
									CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
								}
								else
									std::cout<<"BIG terrible error\n";

				//				std::cout<<"Distance before optimization of pos "<<referencePosition<<" in tetra "<<tetraId<<" is "<<(referencePosition - testPos).norm()<<" with local coords "<<localVolumeCoords <<"\n";

				//				dlevmar_bleic_der(CalculateGlobalCoordsLinearTetra, CalculateJacobianLinearTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
				//
				//				if(noPoints ==4)
				//					CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
				//				else if(noPoints ==10)
				//					CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
				//				else
				//					std::cout<<"BIG terrible error\n";


								double distanceTemp = (referencePosition - testPos).norm();

				//				std::cout<<"Distance after optimization of pos "<<referencePosition<<" in tetra "<<tetraId<<" is "<<(referencePosition - testPos).norm()<<" with local coords "<<localVolumeCoords <<"\n";

								return distanceTemp;

			}


			template <class In, class Out> double MapperLinearTetrahedronSetTopology<In,Out>::FindClosestPointInTetrahedron(unsigned int tetraId, InCoord referencePosition, const typename In::VecCoord& in, InCoord& localVolumeCoords)
			{

				IdVec pointIds;
				getPointsInElementFromTopology(tetraId, pointIds);

				Real nodes[30];
				unsigned int noPoints = pointIds.size();

				for(int iteri=0; iteri<3; iteri++)
				{
					for(int p=0; p<noPoints; p++)
					{
						nodes[noPoints*iteri+p] = (in[pointIds[p]])[iteri];
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

				InCoord testPos;

				if(noPoints ==4)
				{
					CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
					dlevmar_bleic_der(CalculateGlobalCoordsLinearTetra, CalculateJacobianLinearTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
					CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
				}
				else if(noPoints ==10)
				{
					CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
					dlevmar_bleic_der(CalculateGlobalCoordsQuadraticTetra, CalculateJacobianQuadraticTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
					CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
				}
				else
					std::cout<<"BIG terrible error\n";

//				std::cout<<"Distance before optimization of pos "<<referencePosition<<" in tetra "<<tetraId<<" is "<<(referencePosition - testPos).norm()<<" with local coords "<<localVolumeCoords <<"\n";

//				dlevmar_bleic_der(CalculateGlobalCoordsLinearTetra, CalculateJacobianLinearTetra, localVolumeCoords.ptr(), referencePosition.ptr(), 3, 3,lb,NULL,NULL,NULL,0,C,d,1,100000,NULL, info, NULL, NULL, (void*)&nodes[0]);
//
//				if(noPoints ==4)
//					CalculateGlobalCoordsLinearTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
//				else if(noPoints ==10)
//					CalculateGlobalCoordsQuadraticTetra(localVolumeCoords.ptr(), testPos.ptr(),3,3,&nodes[0]);
//				else
//					std::cout<<"BIG terrible error\n";


				double distanceTemp = (referencePosition - testPos).norm();

//				std::cout<<"Distance after optimization of pos "<<referencePosition<<" in tetra "<<tetraId<<" is "<<(referencePosition - testPos).norm()<<" with local coords "<<localVolumeCoords <<"\n";

				return distanceTemp;

			}


			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::CalculateGlobalCoordsLinearTetra(Real* localCoords, Real* globalCoords, int m, int n, void* nodes)
			{
				Real fx = localCoords[0];
				Real fy = localCoords[1];
				Real fz = localCoords[2];

				Real tempExp = 1-fx-fy-fz;

				for(int i=0;i<3;i++)
				{
					globalCoords[i] = 0;

					globalCoords[i] += ((Real*)nodes)[4*i+0]*fx;
					globalCoords[i] += ((Real*)nodes)[4*i+1]*fy;
					globalCoords[i] += ((Real*)nodes)[4*i+2]*tempExp;
					globalCoords[i] += ((Real*)nodes)[4*i+3]*fz;

				}

//				for(int i=0;i<3;i++)
//				{
//					globalCoords[i] = 0;
//
//					globalCoords[i] += ((double*)nodes)[10*i+0]*(2*fx-1)*fx;
//					globalCoords[i] += ((double*)nodes)[10*i+1]*(2*fy-1)*fy;
//					globalCoords[i] += ((double*)nodes)[10*i+2]*(2*tempExp-1)*tempExp;
//					globalCoords[i] += ((double*)nodes)[10*i+3]*(2*fz-1)*fz;
//					globalCoords[i] += ((double*)nodes)[10*i+4]*4*fx*fy;
//					globalCoords[i] += ((double*)nodes)[10*i+5]*4*tempExp*fy;
//					globalCoords[i] += ((double*)nodes)[10*i+6]*4*tempExp*fx;
//					globalCoords[i] += ((double*)nodes)[10*i+7]*4*fx*fz;
//					globalCoords[i] += ((double*)nodes)[10*i+8]*4*fy*fz;
//					globalCoords[i] += ((double*)nodes)[10*i+9]*4*tempExp*fz;
//
//					//globalCoords[i]=10;
//				}

			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::CalculateJacobianLinearTetra(Real* localCoords, Real* jacobian, int m, int n, void* nodes)
			{
				Real r = localCoords[0];
				Real s = localCoords[1];
				Real t = localCoords[2];

				Real tempExp = 1-r-s-t;

				Real shapeFunctionDerivs[4][3];

				// Fill the vector and member variable directly in order to avoid pointer problems and unnecessary copy actions!
				shapeFunctionDerivs[0][0] = 1;
				shapeFunctionDerivs[0][1] = 0;
				shapeFunctionDerivs[0][2] = 0;

				shapeFunctionDerivs[1][0] = 0;
				shapeFunctionDerivs[1][1] = 1;
				shapeFunctionDerivs[1][2] = 0;

				shapeFunctionDerivs[2][0] = -1;
				shapeFunctionDerivs[2][1] = -1;
				shapeFunctionDerivs[2][2] = -1;

				shapeFunctionDerivs[3][0] = 0;
				shapeFunctionDerivs[3][1] = 0;
				shapeFunctionDerivs[3][2] = 1;


				for(int m=0;m<3;m++)
				{
					for(int n=0;n<3;n++)
					{
						//sum over all shape functions
						jacobian[3*m+n] = 0;
						for(int i=0;i<4;i++)
						{
							jacobian[3*m+n] += ((Real*)nodes)[4*m+i]* shapeFunctionDerivs[i][n];// ((coords[i])[m])*m_ShapeFunctionDerivatives[indexOfIntegrationPoint][i][n];

						}
					}
				}

			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::CalculateGlobalCoordsQuadraticTetra(Real* localCoords, Real* globalCoords, int m, int n, void* nodes)
			{
				Real fx = localCoords[0];
				Real fy = localCoords[1];
				Real fz = localCoords[2];

				Real tempExp = 1-fx-fy-fz;

				for(int i=0;i<3;i++)
				{
					globalCoords[i] = 0;

					globalCoords[i] += ((Real*)nodes)[10*i+0]*(2*fx-1)*fx;
					globalCoords[i] += ((Real*)nodes)[10*i+1]*(2*fy-1)*fy;
					globalCoords[i] += ((Real*)nodes)[10*i+2]*(2*tempExp-1)*tempExp;
					globalCoords[i] += ((Real*)nodes)[10*i+3]*(2*fz-1)*fz;
					globalCoords[i] += ((Real*)nodes)[10*i+4]*4*fx*fy;
					globalCoords[i] += ((Real*)nodes)[10*i+5]*4*tempExp*fy;
					globalCoords[i] += ((Real*)nodes)[10*i+6]*4*tempExp*fx;
					globalCoords[i] += ((Real*)nodes)[10*i+7]*4*fx*fz;
					globalCoords[i] += ((Real*)nodes)[10*i+8]*4*fy*fz;
					globalCoords[i] += ((Real*)nodes)[10*i+9]*4*tempExp*fz;

					//globalCoords[i]=10;
				}


			}

			template <class In, class Out> void MapperLinearTetrahedronSetTopology<In,Out>::CalculateJacobianQuadraticTetra(Real* localCoords, Real* jacobian, int m, int n, void* nodes)
			{
				Real r = localCoords[0];
					Real s = localCoords[1];
					Real t = localCoords[2];

					Real tempExp = 1-r-s-t;

					Real shapeFunctionDerivs[10][3];


					// Fill the vector and member variable directly in order to avoid pointer problems and unnecessary copy actions!
					shapeFunctionDerivs[0][0] = 4*r-1;
					shapeFunctionDerivs[0][1] = 0;
					shapeFunctionDerivs[0][2] = 0;

					shapeFunctionDerivs[1][0] = 0;
					shapeFunctionDerivs[1][1] = 4*s-1;
					shapeFunctionDerivs[1][2] = 0;

					shapeFunctionDerivs[2][0] = -4*tempExp+1;
					shapeFunctionDerivs[2][1] = -4*tempExp+1;
					shapeFunctionDerivs[2][2] = -4*tempExp+1;

					shapeFunctionDerivs[3][0] = 0;
					shapeFunctionDerivs[3][1] = 0;
					shapeFunctionDerivs[3][2] = 4*t-1;

					shapeFunctionDerivs[4][0] = 4*s;
					shapeFunctionDerivs[4][1] = 4*r;
					shapeFunctionDerivs[4][2] = 0;

					shapeFunctionDerivs[5][0] = -4*s;
					shapeFunctionDerivs[5][1] = 4*(tempExp-s);
					shapeFunctionDerivs[5][2] = -4*s;

					shapeFunctionDerivs[6][0] = 4*(tempExp-r);
					shapeFunctionDerivs[6][1] = -4*r;
					shapeFunctionDerivs[6][2] = -4*r;

					shapeFunctionDerivs[7][0] = 4*t;
					shapeFunctionDerivs[7][1] = 0;
					shapeFunctionDerivs[7][2] = 4*r;

					shapeFunctionDerivs[8][0] = 0;
					shapeFunctionDerivs[8][1] = 4*t;
					shapeFunctionDerivs[8][2] = 4*s;

					shapeFunctionDerivs[9][0] = -4*t;
					shapeFunctionDerivs[9][1] = -4*t;
					shapeFunctionDerivs[9][2] = 4*(tempExp-t);

					for(int m=0;m<3;m++)
					{
						for(int n=0;n<3;n++)
						{
							//sum over all shape functions
							jacobian[3*m+n] = 0;
							for(int i=0;i<10;i++)
							{
								jacobian[3*m+n] += ((Real*)nodes)[10*m+i]* shapeFunctionDerivs[i][n];// ((coords[i])[m])*m_ShapeFunctionDerivatives[indexOfIntegrationPoint][i][n];

							}
						}
					}

			}


			template <class In, class Out>
						void MapperLinearTetrahedronSetTopology<In,Out>::apply( typename Out::VecCoord& out, const typename In::VecCoord& in )
			{

//				std::cout<<"map3d size is "<<map3d.size()<<"\n";

				sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
				sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

				if(inTetrahedronTopology)
				{
					const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();

					for ( unsigned int i=0;i<map3d.size();i++ )
					{
						const Real fx = map3d[i].baryCoords[0];
						const Real fy = map3d[i].baryCoords[1];
						const Real fz = map3d[i].baryCoords[2];

	//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
						unsigned int index = map3d[i].inIndex;

						const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];

						{
							Out::setCPos(out[i] , in[tetra[0]] * fx	+
									in[tetra[1]] * fy	+
									in[tetra[2]] * (1-fx-fy-fz) +
									in[tetra[3]] * fz);

						}



					}
				}

				if(inQuadTetraTopology)
				{
					const sofa::component::topology::QuadraticMeshTopology::SeqQuadTetrahedra tetrahedra = inQuadTetraTopology->getQuadTetrahedra();

					for ( unsigned int i=0;i<map3d.size();i++ )
					{
						const Real fx = map3d[i].baryCoords[0];
						const Real fy = map3d[i].baryCoords[1];
						const Real fz = map3d[i].baryCoords[2];
						Real tempExp = 1-fx-fy-fz;
	//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
						unsigned int index = map3d[i].inIndex;

						const sofa::component::topology::QuadraticMeshTopology::QuadTetra& tetra = tetrahedra[index];

						{



							Out::setCPos(out[i] , in[tetra[0]]  *(2*fx-1)*fx	+
									in[tetra[1]] * (2*fy-1)*fy	+
									in[tetra[2]] * (2*tempExp-1)*tempExp +
									in[tetra[3]] * (2*fz-1)*fz +
									in[tetra[4]] * 4*fx*fy +
									in[tetra[5]] * 4*tempExp*fy +
									in[tetra[6]] * 4*tempExp*fx +
									in[tetra[7]] * 4*fx*fz +
									in[tetra[8]] * 4*fy*fz +
									in[tetra[9]] * 4*tempExp*fz);
						}


					}
				}

				if(!inTetrahedronTopology && !inQuadTetraTopology)
					std::cout<<"Error, must use tetra or quadratic tetra input topology\n";


			}

			template <class In, class Out>
						void MapperLinearTetrahedronSetTopology<In,Out>::applyJ( typename Out::VecDeriv& out, const typename In::VecDeriv& in )
			{
	//				std::cout<<"map3d size is "<<map3d.size()<<"\n";

					sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
					sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

					if(inTetrahedronTopology)
					{
						const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = this->fromTopology->getTetrahedra();

						for ( unsigned int i=0;i<map3d.size();i++ )
						{
							const Real fx = map3d[i].baryCoords[0];
							const Real fy = map3d[i].baryCoords[1];
							const Real fz = map3d[i].baryCoords[2];

		//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
							unsigned int index = map3d[i].inIndex;

							const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];

							{
								Out::setCPos(out[i] , in[tetra[0]] * fx	+
										in[tetra[1]] * fy	+
										in[tetra[2]] * (1-fx-fy-fz) +
										in[tetra[3]] * fz);

							}



						}
					}

					if(inQuadTetraTopology)
					{
						const sofa::component::topology::QuadraticMeshTopology::SeqQuadTetrahedra tetrahedra = inQuadTetraTopology->getQuadTetrahedra();

						for ( unsigned int i=0;i<map3d.size();i++ )
						{
							const Real fx = map3d[i].baryCoords[0];
							const Real fy = map3d[i].baryCoords[1];
							const Real fz = map3d[i].baryCoords[2];
							Real tempExp = 1-fx-fy-fz;
		//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
							unsigned int index = map3d[i].inIndex;

							const sofa::component::topology::QuadraticMeshTopology::QuadTetra& tetra = tetrahedra[index];

							{



								Out::setCPos(out[i] , in[tetra[0]]  *(2*fx-1)*fx	+
										in[tetra[1]] * (2*fy-1)*fy	+
										in[tetra[2]] * (2*tempExp-1)*tempExp +
										in[tetra[3]] * (2*fz-1)*fz +
										in[tetra[4]] * 4*fx*fy +
										in[tetra[5]] * 4*tempExp*fy +
										in[tetra[6]] * 4*tempExp*fx +
										in[tetra[7]] * 4*fx*fz +
										in[tetra[8]] * 4*fy*fz +
										in[tetra[9]] * 4*tempExp*fz);
							}


						}
					}

					if(!inTetrahedronTopology && !inQuadTetraTopology)
						std::cout<<"Error, must use tetra or quadratic tetra input topology\n";
			}

			template <class In, class Out>
						void MapperLinearTetrahedronSetTopology<In,Out>::applyJT( typename In::VecDeriv& out, const typename Out::VecDeriv& in )
			{
				//				std::cout<<"map3d size is "<<map3d.size()<<"\n";

								sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
								sofa::component::topology::QuadraticMeshTopology* inQuadTetraTopology = (dynamic_cast<sofa::component::topology::QuadraticMeshTopology* > (this->fromTopology));

								if(inTetrahedronTopology)
								{
									const sofa::core::topology::BaseMeshTopology::SeqTetrahedra& tetrahedra = inTetrahedronTopology->getTetrahedra();

									for ( unsigned int i=0;i<map3d.size();i++ )
									{
										const typename Out::DPos v = Out::getDPos(in[i]);
										const OutReal fx = (OutReal) map3d[i].baryCoords[0];
										const OutReal fy = (OutReal) map3d[i].baryCoords[1];
										const OutReal fz = (OutReal) map3d[i].baryCoords[2];

					//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
										unsigned int index = map3d[i].inIndex;

										const sofa::core::topology::BaseMeshTopology::Tetra& tetra = tetrahedra[index];

										{
											out[tetra[0]] += v*fx;
											out[tetra[1]] += v*fy;
											out[tetra[2]] += v*(1-fx-fy-fz);
											out[tetra[3]] += v*fz;

										}



									}
								}

								if(inQuadTetraTopology)
								{
									const sofa::component::topology::QuadraticMeshTopology::SeqQuadTetrahedra tetrahedra = inQuadTetraTopology->getQuadTetrahedra();

									for ( unsigned int i=0;i<map3d.size();i++ )
									{
										const typename Out::DPos v = Out::getDPos(in[i]);
										const Real fx = map3d[i].baryCoords[0];
										const Real fy = map3d[i].baryCoords[1];
										const Real fz = map3d[i].baryCoords[2];
										Real tempExp = 1-fx-fy-fz;
					//					std::cout<<"Index in is "<<map3d[i].inIndex<<"\n";
										unsigned int index = map3d[i].inIndex;

										const sofa::component::topology::QuadraticMeshTopology::QuadTetra& tetra = tetrahedra[index];

										{

											out[tetra[0]] += v*(2*fx-1)*fx;
											out[tetra[1]] += v*(2*fy-1)*fy;
											out[tetra[2]] += v*(2*tempExp-1)*tempExp;
											out[tetra[3]] += v*(2*fz-1)*fz;
											out[tetra[4]] += v*4*fx*fy;
											out[tetra[5]] += v*4*tempExp*fy;
											out[tetra[6]] += v*4*tempExp*fx;
											out[tetra[7]] += v*4*fx*fz;
											out[tetra[8]] += v*4*fy*fz;
											out[tetra[9]] += v*4*tempExp*fz;


										}


									}
								}

								if(!inTetrahedronTopology && !inQuadTetraTopology)
									std::cout<<"Error, must use tetra or quadratic tetra input topology\n";
			}


		} // namespace mapping

	} // namespace component

} // namespace sofa

#endif
