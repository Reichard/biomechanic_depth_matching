/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 RC 1        *
*                (c) 2006-2011 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
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
#ifndef SOFA_COMPONENT_MAPPING_MapperLinearTetrahedronSetTopology_H
#define SOFA_COMPONENT_MAPPING_MapperLinearTetrahedronSetTopology_H

#include <sofa/component/topology/TopologyData.h>
#include <sofa/component/topology/RegularGridTopology.h>
#include <sofa/component/topology/PointSetTopologyContainer.h>
#include <sofa/component/linearsolver/CompressedRowSparseMatrix.h>

#include <sofa/component/topology/TetrahedronSetTopologyContainer.h>

//#include <sofa/component/mapping/BarycentricMapping.h>

#include <sofa/core/Mapping.h>
#include <sofa/core/MechanicalParams.h>

#include <sofa/defaulttype/VecTypes.h>
#include <sofa/defaulttype/Quat.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>


#include <sofa/helper/vector.h>

//replace with forward declarations
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"
#include "vtkCellData.h"


// forward declarations
//namespace sofa {
//	namespace core {
//		namespace topology {
//		  class BaseMeshTopology;
//		}
//	}
//
//}

namespace sofa
{

namespace component
{

namespace mapping
{


/// Class allowing barycentric mapping computation on a MeshTopology
template <class TIn, class TOut>
class MapperLinearTetrahedronSetTopology : public core::Mapping<TIn, TOut>
{
public:
	SOFA_CLASS(SOFA_TEMPLATE2(MapperLinearTetrahedronSetTopology,TIn,TOut), SOFA_TEMPLATE2(core::Mapping,TIn,TOut));

    typedef core::Mapping<TIn, TOut> Inherit;
    typedef TIn In;
    typedef TOut Out;
	typedef In InDataTypes;
	typedef typename InDataTypes::VecCoord InVecCoord;
	typedef typename InDataTypes::VecDeriv InVecDeriv;
	typedef typename InDataTypes::Coord InCoord;
	typedef typename InDataTypes::Deriv InDeriv;
	typedef typename InDataTypes::Real Real;
	typedef Out OutDataTypes;
	typedef typename OutDataTypes::VecCoord OutVecCoord;
	typedef typename OutDataTypes::VecDeriv OutVecDeriv;
	typedef typename OutDataTypes::Coord OutCoord;
	typedef typename OutDataTypes::Deriv OutDeriv;
	typedef typename OutDataTypes::Real OutReal;

	typedef core::topology::BaseMeshTopology BaseMeshTopology;

	typedef sofa::helper::vector<unsigned int> IdVec;
	typedef sofa::helper::fixed_array<unsigned int,4> TrianglesInElement;


	MapperLinearTetrahedronSetTopology(): Inherit()
	{
		isQuadratic = false;

	}


	virtual ~MapperLinearTetrahedronSetTopology()
	{

	}

public:

//	void clear(int reserve=0);

	void init(const typename Out::VecCoord& out, const typename In::VecCoord& in);

	void init()
	{
		this->toModel->getContext()->get(toTopology);

		this->fromModel->getContext()->get(fromTopology);

//		std::cout<<"Using displacement based mapping: "<<this->m_DisplacementBased.getValue()<<"\n";

		init ( *((const core::State<Out> *)this->toModel)->getX(), *((const core::State<In> *)this->fromModel)->getX() );

//		std::cout<<"Now init inherited class...";

//		Inherit::init();


//
//		sofa::component::topology::TetrahedronSetTopologyContainer* inTetrahedronTopology = (dynamic_cast<sofa::component::topology::TetrahedronSetTopologyContainer* > (this->fromTopology));
//		const IdVec tempTetrahedra = inTetrahedronTopology->getTetrahedraAroundTriangle(1);

//		std::cout<<"finished\n";

	}

	void reinit()
	{
		init();
	}

	void apply( typename Out::VecCoord& out, const typename In::VecCoord& in );
	void applyJ( typename Out::VecDeriv& out, const typename In::VecDeriv& in );
	void applyJT( typename In::VecDeriv& out, const typename Out::VecDeriv& in );

	bool MapElement(unsigned int outElementIndex, unsigned int & currentInElement, InCoord &currentCoefs,double & currentDistance, const typename In::VecCoord& in,
			const typename Out::VecCoord& out, std::set<unsigned int>  &trianglesMapped, std::set<unsigned int> &pointsMapped, bool debugOutput);
	bool RecursivelyFindClosestPoint(unsigned int startTetraId, double currentMinError, InCoord referencePosition,const typename In::VecCoord& in, InCoord& localCoords,
			unsigned int & finalTetraIndex, double & distance,  std::set<unsigned int> &tetrasVisited, bool debugOutput);

	double FindClosestPointInTetrahedron(unsigned int tetraId, InCoord referencePosition, const typename In::VecCoord& in, InCoord& localVolumeCoords);
	static void CalculateGlobalCoordsLinearTetra(Real* localCoords, Real* globalCoords, int m, int n, void* nodes);
	static void CalculateJacobianLinearTetra(Real* localCoords, Real* jacobian, int m, int n, void* nodes);
	static void CalculateGlobalCoordsQuadraticTetra(Real* localCoords, Real* globalCoords, int m, int n, void* nodes);
	static void CalculateJacobianQuadraticTetra(Real* localCoords, Real* jacobian, int m, int n, void* nodes);



protected:

	void getElementsAroundElements(unsigned int rootElement, IdVec& elements);
	void getElementsAroundVertex(unsigned int vertex, IdVec& elements);
	void getPointsInElementToTopology(unsigned int element, IdVec& points);
	void getPointsInElementFromTopology(unsigned int elementId, IdVec& points);
	void getTrianglesInTetrahedron(unsigned int tetrahedron, TrianglesInElement& triangles);
//	void getTrianglesAroundTetrahedron(unsigned int tetrahedron, IdVec& triangles);
//	void getTetrahedraAroundTriangle(unsigned int triangle, IdVec& tetrahedra);
	bool LocalVolumeCoordsToLocalSurfaceCoords(Real* localVolumeCoords, std::vector<unsigned int>&  localFaceNumbers,InVecCoord& localSurfaceCoordsVec);
	unsigned int getNumberOfOutElements();

	struct MappingData
	{
		unsigned int inIndex;
		InCoord baryCoords;
	};

	struct ToMapData
	{
		unsigned int startInIndex;
		unsigned int outIndex;
		InCoord startLocalCoords;
	};


	core::topology::BaseMeshTopology* fromTopology;
	core::topology::BaseMeshTopology* toTopology;

	sofa::helper::vector< MappingData >  map3d;

	bool isQuadratic;

	//////////////////////////////DebugOutput//////////////////////////////
	vtkPoints* m_PointContainerDebugPoints;
	vtkFloatArray *m_VectorsOptiDebug;
	vtkFloatArray *m_ScalarsOptiDebug;


	vtkPoints* m_PointContainerDebugTriangles;
	vtkCellArray* m_TriangleContainerDebugTriangles;
	vtkFloatArray* m_ScalarsDebugTriangles;

	unsigned int m_DebugCounter;

	vtkPoints* m_PointContainerRecursiveMapping;
	vtkCellArray* m_TriangleContainerRecursiveMapping;
	unsigned int m_MappingPrintCounter;
	unsigned int m_MappingElementCounter;
	unsigned int m_MappingPointCounter;
	/////////////////////////////////////////////////////////////////////////


};




} // namespace mapping

} // namespace component

} // namespace sofa

#endif
