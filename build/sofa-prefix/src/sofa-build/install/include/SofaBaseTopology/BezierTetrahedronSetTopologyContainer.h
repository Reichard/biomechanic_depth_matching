/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
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
#ifndef SOFA_COMPONENT_TOPOLOGY_BEZIERTETRAHEDRONSETTOPOLOGYCONTAINER_H
#define SOFA_COMPONENT_TOPOLOGY_BEZIERTETRAHEDRONSETTOPOLOGYCONTAINER_H
#include "config.h"

#include <SofaBaseTopology/TetrahedronSetTopologyContainer.h>


namespace sofa
{

namespace component
{

namespace topology
{
class BezierTetrahedronSetTopologyModifier;

using core::topology::BaseMeshTopology;

typedef unsigned char BezierDegreeType;
typedef sofa::defaulttype::Vec<4,BezierDegreeType> TetrahedronBezierIndex;

/** a class that stores a set of Bezier tetrahedra and provides access with adjacent triangles, edges and vertices 
A Bezier Tetrahedron has exactly the same topology as a Tetrahedron but with additional (control) points on its edges, triangles and inside 
We use a Vec4D to number the control points inside  a Bezier tetrahedron */
class SOFA_BASE_TOPOLOGY_API BezierTetrahedronSetTopologyContainer : public TetrahedronSetTopologyContainer
{

public: 
    SOFA_CLASS(BezierTetrahedronSetTopologyContainer,TetrahedronSetTopologyContainer);

	typedef BaseMeshTopology::PointID			         PointID;
	typedef BaseMeshTopology::EdgeID			            EdgeID;
	typedef BaseMeshTopology::TriangleID		         TriangleID;
	typedef BaseMeshTopology::TetraID			         TetraID;
	typedef BaseMeshTopology::Edge				         Edge;
	typedef BaseMeshTopology::Triangle			         Triangle;
	typedef BaseMeshTopology::Tetra				         Tetra;
	typedef BaseMeshTopology::SeqTetrahedra			   SeqTetrahedra;
	typedef sofa::defaulttype::Vec<2,BezierDegreeType>		EdgeBezierIndex;
	typedef sofa::defaulttype::Vec<3,BezierDegreeType>		TriangleBezierIndex;



	typedef Tetra			Tetrahedron;
	typedef EdgesInTetrahedron		EdgesInTetrahedron;
	typedef TrianglesInTetrahedron	TrianglesInTetrahedron;
	typedef sofa::helper::vector<PointID>         VecPointID;
	typedef sofa::helper::vector<TetraID>         VecTetraID;
	typedef sofa::helper::vector<SReal> SeqWeights;
	typedef sofa::helper::vector<bool> SeqBools;
	typedef VecPointID  BezierDOFInTetrahedron;
	typedef sofa::helper::vector< VecPointID > SeqBezierDOFInTetrahedron;


	
	typedef sofa::defaulttype::Vec<4,int> ElementTetrahedronIndex;
	typedef sofa::defaulttype::Vec<4,size_t> LocalTetrahedronIndex;

	friend class BezierTetrahedronSetTopologyModifier;
	friend class Mesh2BezierTopologicalMapping;

public :
	// specifies where a Bezier Point can lies with respect to the underlying tetrahedral mesh
	enum BezierTetrahedronPointLocation
    {
        POINT = 0,
        EDGE =1 ,
        TRIANGLE = 2,
        TETRAHEDRON = 3
    };
	

	typedef std::pair<size_t,std::pair< BezierTetrahedronPointLocation,size_t> > ControlPointLocation;
protected:
    BezierTetrahedronSetTopologyContainer();

    virtual ~BezierTetrahedronSetTopologyContainer() {}
public:
    virtual void init();
	// build some maps specific of the degree of the tetrahedral elements.
	virtual void reinit();

    /// Bezier Specific Information Topology API
    /// @{
protected :
	/// the degree of the Bezier Tetrahedron 1=linear, 2=quadratic...
	Data <BezierDegreeType> d_degree;
	/// the number of control points corresponding to the vertices of the tetrahedra (different from the total number of points)
    Data<size_t> d_numberOfTetrahedralPoints;
	/// whether the Bezier tetrahedron are integral (false = classical Bezier splines) or rational splines (true)
	Data <SeqBools> d_isRationalSpline;
	/// the array of weights for rational splines
	Data <SeqWeights > d_weightArray;
public :

	/// get the Degree of the Bezier Tetrahedron 
	BezierDegreeType getDegree() const;
	/// get the number of control points corresponding to the vertices of the tetrahedra 
	size_t getNumberOfTetrahedralPoints() const;
	/// get the global index of the Bezier  point associated with a given tetrahedron index and given its 4D Index   
	size_t getGlobalIndexOfBezierPoint(const TetraID tetrahedronIndex,const TetrahedronBezierIndex id) ;
	/// get the indices of all control points associated with a given tetrahedron
	const VecPointID &getGlobalIndexArrayOfBezierPoints(const TetraID tetrahedronIndex) const;
	/// return the Bezier index given the local index in a tetrahedron
	TetrahedronBezierIndex getTetrahedronBezierIndex(const size_t localIndex) const;
	/// get the Tetrahedron Bezier Index Array of degree d
	sofa::helper::vector<TetrahedronBezierIndex> getTetrahedronBezierIndexArray() const;
	/// get the Tetrahedron Bezier Index Array of a given degree 
	sofa::helper::vector<TetrahedronBezierIndex> getTetrahedronBezierIndexArrayOfGivenDegree(const BezierDegreeType deg) const;
	/** create an array which maps the local index of a Tetrahedron Bezier Index of degree d-1
	into a local index of a TBI of degree d by adding respectively (1,0,0,0), (0,1,0,0), (0,0,1,0), (0,0,0,1) **/
	sofa::helper::vector<LocalTetrahedronIndex> getMapOfTetrahedronBezierIndexArrayFromInferiorDegree() const;
	/// return the local index in a tetrahedron from a tetrahedron Bezier index (inverse of getTetrahedronBezierIndex())
	size_t getLocalIndexFromTetrahedronBezierIndex(const TetrahedronBezierIndex id) const;
	/// return the location, the element index and offset from the global index of a point
	void getLocationFromGlobalIndex(const size_t globalIndex, BezierTetrahedronPointLocation &location, 
		size_t &elementIndex, size_t &elementOffset) ;
		/// convert the edge offset into a EdgeBezierIndex
	void getEdgeBezierIndexFromEdgeOffset(size_t offset, EdgeBezierIndex &ebi);
	/// convert the triangle offset into a TriangleBezierIndex
	void getTriangleBezierIndexFromTriangleOffset(size_t offset, TriangleBezierIndex &tbi);
	/// convert the tetrahedron offset into a TriangleBezierIndex
	void getTetrahedronBezierIndexFromTetrahedronOffset(size_t offset, TetrahedronBezierIndex &tbi);
	/// check the Bezier Point Topology
	virtual bool checkBezierPointTopology();
	/** \brief Returns the weight coordinate of the ith DOF. */
	virtual SReal getWeight(int i) const;
	/// returns the array of weights
	const SeqWeights & getWeightArray() const;
	// if the Bezier tetrahedron is rational or integral
	bool isRationalSpline(int i) const;
	 /// @}

    inline friend std::ostream& operator<< (std::ostream& out, const BezierTetrahedronSetTopologyContainer& t)
    {
        helper::ReadAccessor< Data< sofa::helper::vector<Tetrahedron> > > m_tetrahedron = t.d_tetrahedron;
        out  << m_tetrahedron<< " "
                << t.m_edgesInTetrahedron<< " "
                << t.m_trianglesInTetrahedron;

        out << " "<< t.m_tetrahedraAroundVertex.size();
        for (unsigned int i=0; i<t.m_tetrahedraAroundVertex.size(); i++)
        {
            out << " " << t.m_tetrahedraAroundVertex[i];
        }
        out <<" "<< t.m_tetrahedraAroundEdge.size();
        for (unsigned int i=0; i<t.m_tetrahedraAroundEdge.size(); i++)
        {
            out << " " << t.m_tetrahedraAroundEdge[i];
        }
        out <<" "<< t.m_tetrahedraAroundTriangle.size();
        for (unsigned int i=0; i<t.m_tetrahedraAroundTriangle.size(); i++)
        {
            out << " " << t.m_tetrahedraAroundTriangle[i];
        }
		out << " " << t.d_degree.getValue();
		out << " " << t.getNbPoints();
        return out;
    }

    inline friend std::istream& operator>>(std::istream& in, BezierTetrahedronSetTopologyContainer& t)
    {
        unsigned int s=0;
        sofa::helper::vector< unsigned int > value;
        helper::WriteAccessor< Data< sofa::helper::vector<Tetrahedron> > > m_tetrahedron = t.d_tetrahedron;

        in >> m_tetrahedron >> t.m_edgesInTetrahedron >> t.m_trianglesInTetrahedron;


        in >> s;
        for (unsigned int i=0; i<s; i++)
        {
            in >> value;
            t.m_tetrahedraAroundVertex.push_back(value);
        }
        in >> s;
        for (unsigned int i=0; i<s; i++)
        {
            in >> value;
            t.m_tetrahedraAroundEdge.push_back(value);
        }
        in >> s;
        for (unsigned int i=0; i<s; i++)
        {
            in >> value;
            t.m_tetrahedraAroundTriangle.push_back(value);
        }
        BezierDegreeType bdt=0;
		in >> bdt;
		t.d_degree.setValue(bdt);
		int nbp;
		in >> nbp;
		t.setNbPoints(nbp);
        return in;
    }
protected:
	/// array describing the global  index of the DOFs used in weightedDOFArray
	SeqBezierDOFInTetrahedron  tetrahedronDOFArray;
	/// Map which provides the location (point, edge, triangle, tetrahedron) of a control point given its tetrahedron Bezier index
	std::map<TetrahedronBezierIndex,ElementTetrahedronIndex> elementMap;
	/// Map which provides the offset in the DOF vector for a control point lying on an edge 
	std::map<TetrahedronBezierIndex,size_t> edgeOffsetMap;
	/// Map which provides the offset in the DOF vector for a control point lying on a triangle 
	std::map<TetrahedronBezierIndex,size_t> triangleOffsetMap;
	/// Map which provides the offset in the DOF vector for a control point lying on a tetrahedron
	std::map<TetrahedronBezierIndex,size_t> tetrahedronOffsetMap;
	/// Map which provides the rank in a control point from the array outputed by getGlobalIndexArrayOfBezierPointsInTetrahedron (consistent with bezierIndexArray) 
	std::map<TetrahedronBezierIndex,size_t> localIndexMap;
	/// array of the tetrahedron Bezier index outputed by the function getGlobalIndexArrayOfBezierPointsInTetrahedron()
	sofa::helper::vector<TetrahedronBezierIndex> bezierIndexArray;
	/// array of the tetrahedron Bezier index outputed by the function getGlobalIndexArrayOfBezierPointsInTetrahedron()
	sofa::helper::vector<TetrahedronBezierIndex> reducedDegreeBezierIndexArray;
	/// convert triangle offset into triangle bezier index
	sofa::helper::vector<TriangleBezierIndex> offsetToTriangleBezierIndexArray;
	/// convert triangle offset into triangle bezier index
	sofa::helper::vector<TetrahedronBezierIndex> offsetToTetrahedronBezierIndexArray;
		/** Map which provides the global index of a control point knowing its location (i.e. triangle index and its TriangleBezierIndex).
	This is empty by default since there is a default layout of control points based on edge and triangles indices */
	std::map<ControlPointLocation,size_t> locationToGlobalIndexMap;
	/** Map which provides the  location (i.e. triangle index and its TriangleBezierIndex) of a control point knowing its  global index.
	Note that the location may not be unique.
	This is empty by default since there is a default layout of control points based on edge and triangles indices */
	std::map<size_t,ControlPointLocation> globalIndexToLocationMap;

	void getGlobalIndexArrayOfBezierPointsInTetrahedron(const TetraID tetrahedronIndex, VecPointID & indexArray) ;

};

} // namespace topology

} // namespace component

} // namespace sofa

#endif
