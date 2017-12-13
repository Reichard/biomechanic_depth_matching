/******************************************************************************
 *
******************************************************************************/
#ifndef SOFA_COMPONENT_TOPOLOGY_QUADRATICMESHTOPOLOGY_H
#define SOFA_COMPONENT_TOPOLOGY_QUADRATICMESHTOPOLOGY_H
//use the library of OpenGL
//#define SOFA_HAVE_GLEW

#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <sofa/core/topology/Topology.h>
//#include <sofa/core/topology/BaseMeshTopology.h>
#include <sofa/helper/fixed_array.h>
#include <sofa/helper/vector.h>
#include <sofa/component/component.h>

#include "QuadraticBaseMeshTopology.h"

namespace sofa
{

namespace component
{

namespace container
{
class MeshLoader;
}

namespace topology
{

using namespace sofa::defaulttype;
using helper::vector;
using helper::fixed_array;




class SOFA_CORE_API QuadraticMeshTopology : public core::topology::QuadraticBaseMeshTopology
{
  public:
    SOFA_CLASS(QuadraticMeshTopology, core::topology::QuadraticBaseMeshTopology);

	//typedef int index_type;
	typedef unsigned int index_type;
	enum { InvalidID = (unsigned)-1 };
	typedef index_type	                	QuadraticTetraID;

	typedef fixed_array<PointID,10>              QuadraticTetra;
	//typedef fixed_array<PointID,6>              QuadraticTriangle;

	typedef vector<QuadraticTetra>		        SeqQuadraticTetra;
	typedef vector<QuadraticTetra>		        SeqQuadraticTetrahedra;

		/// fixed-size neighbors arrays
	/// @{
	typedef fixed_array<TriangleID,4>	TrianglesInQuadraticTetrahedron;
	typedef fixed_array<EdgeID,6>		EdgesInQuadraticTetrahedron;
	/// @}

	/// dynamic-size neighbors arrays
	/// @{
	typedef vector<QuadraticTetraID>		QuadraticTetrahedraAroundVertex;
	typedef vector<QuadraticTetraID>		QuadraticTetrahedraAroundEdge;
	typedef vector<TetraID>		QuadraticTetrahedraAroundTriangle;



	/// @}

	class QuadraticTriangle : public fixed_array<PointID,6>
	{
	public:
		QuadraticTriangle() {}
		QuadraticTriangle(PointID a, PointID b, PointID c, PointID d, PointID e, PointID f) : fixed_array<PointID,6>(a,b,c,d,e,f) {}
	};

	class QuadraticEdge : public fixed_array<PointID,3>
	{
	public:
		QuadraticEdge() {}
		QuadraticEdge(PointID a, PointID b, PointID c) : fixed_array<PointID,3>(a,b,c) {}
	};


	typedef vector<QuadraticTriangle>		        SeqQuadraticTriangles;

	typedef vector<QuadraticEdge>		        SeqQuadraticEdges;

	QuadraticMeshTopology();

	virtual void init();

	virtual int getNbPoints() const;

	virtual void setNbPoints(int n);

	// Complete sequence accessors

	virtual const SeqEdges& getEdges();
	virtual const SeqTriangles& getTriangles();
	virtual const SeqQuads& getQuads();
	virtual const SeqTetrahedra& getTetrahedra();
	virtual const SeqHexahedra& getHexahedra();
	//TODO getQuadTetrahedra()
	virtual const SeqQuadTetrahedra& getQuadTetrahedra();


	// Random accessors

	virtual int getNbEdges();
	virtual int getNbTriangles();
	virtual int getNbQuadraticTriangles();
	virtual int getNbQuads();
	virtual int getNbTetrahedra();
	virtual int getNbHexahedra();
	//TODO getNbQuadTetrahedra()
	virtual int getNbQuadTetrahedra();


	virtual const Edge getEdge(EdgeID i);
	virtual const Triangle getTriangle(TriangleID i);
	virtual const Quad getQuad(QuadID i);
	virtual const Tetra getTetrahedron(TetraID i);
	virtual const Hexa getHexahedron(HexaID i);
	//TODO getQuadTetrahedra(ID)
	virtual const QuadTetra getQuadTetrahedra(QuadTetraID i);
	virtual const QuadraticTriangle getQuadraticTriangle(TriangleID i);
	virtual const QuadraticTriangle getQuadraticTriangleVTK(TriangleID i);

	virtual const QuadraticEdge getQuadraticEdge(EdgeID i);

	virtual const SeqQuadraticTriangles& getQuadraticTriangles();
	virtual const SeqQuadraticEdges& getQuadraticEdges();
	virtual const SeqQuadraticTriangles& getQuadraticTrianglesVTK();


//    /// @name neighbors queries
//    /// @{
//    /// Returns the set of edges adjacent to a given vertex.
//    virtual const EdgesAroundVertex &getEdgesAroundVertex(PointID i);
//    /// Returns the set of edges adjacent to a given triangle.
//    virtual const EdgesInTriangle &getEdgesInTriangle(TriangleID i);
//    /// Returns the set of edges adjacent to a given quad.
//    virtual const EdgesInQuad &getEdgesInQuad(QuadID i);
//    /// Returns the set of edges adjacent to a given tetrahedron.
//    virtual const EdgesInTetrahedron& getEdgesInTetrahedron(TetraID i);
//    /// Returns the set of edges adjacent to a given hexahedron.
//    virtual const EdgesInHexahedron& getEdgesInHexahedron(HexaID i);
//    /// Returns the set of triangle adjacent to a given vertex.
 //   virtual const TrianglesAroundVertex &getTrianglesAroundVertex(PointID i);
	virtual const TrianglesAroundVertex &getQuadraticTrianglesAroundVertex(PointID i);
	virtual const EdgesAroundVertex &getQuadraticEdgesAroundVertex(PointID i);
//    /// Returns the set of triangle adjacent to a given edge.
//    virtual const TrianglesAroundEdge &getTrianglesAroundEdge(EdgeID i);
//    /// Returns the set of triangles adjacent to a given tetrahedron.
    //virtual const TrianglesInTetrahedron& getTrianglesInTetrahedron(TetraID i);

	//virtual const TrianglesInTetrahedron& getTrianglesInTetrahedron(TetraID i);

	virtual const TrianglesInTetrahedron& getQuadraticTrianglesInQuadraticTetrahedron(TetraID i);
	virtual const EdgesInTriangle& getQuadraticEdgesInQuadraticTriangle(TriangleID i);
	virtual const EdgesInTriangle& getQuadraticEdgesInQuadraticTriangleOrientation(TriangleID i);

	virtual int getSurfaceQuadraticTriangleInQuadraticTetrahedron(TetraID i, double r, double s, double t, int & faceNumber);

	virtual int getSurfaceQuadraticEdgeInInQuadraticTetrahedron(TetraID i, double r, double s, double t);
	virtual int getQuadraticEdgeInQuadraticTriangle(TetraID i, double r, double s, int & edgeNumber);
    /// Returns the set of quad adjacent to a given vertex.
    virtual const QuadsAroundVertex &getQuadsAroundVertex(PointID i);
//    /// Returns the set of quad adjacent to a given edge.
//    virtual const QuadsAroundEdge &getQuadsAroundEdge(EdgeID i);
//    /// Returns the set of quads adjacent to a given hexahedron.
//    virtual const QuadsInHexahedron& getQuadsInHexahedron(HexaID i);
//    /// Returns the set of tetrahedra adjacent to a given vertex.
//    virtual const TetrahedraAroundVertex& getTetrahedraAroundVertex(PointID i);
//    /// Returns the set of tetrahedra adjacent to a given edge.
//    virtual const TetrahedraAroundEdge& getTetrahedraAroundEdge(EdgeID i);
//    /// Returns the set of tetrahedra adjacent to a given triangle.
//    virtual const TetrahedraAroundTriangle& getTetrahedraAroundTriangle(TriangleID i);
    virtual const TetrahedraAroundTriangle& getQuadraticTetrahedraAroundQuadraticTriangle(TriangleID i);
//    /// Returns the set of hexahedra adjacent to a given vertex.
//    virtual const HexahedraAroundVertex& getHexahedraAroundVertex(PointID i);
//    /// Returns the set of hexahedra adjacent to a given edge.
//    virtual const HexahedraAroundEdge& getHexahedraAroundEdge(EdgeID i);
//    /// Returns the set of hexahedra adjacent to a given quad.
//    virtual const HexahedraAroundQuad& getHexahedraAroundQuad(QuadID i);
//    /// @}

    virtual const vector<PointID> & getSurfaceNodes();
    virtual const TrianglesAroundVertex getSurfaceQuadraticTrianglesAroundSurfaceNode(unsigned int node);
    virtual const TrianglesAroundVertex getSurfaceQuadraticTrianglesAroundNode(unsigned int node);
    virtual const TrianglesAroundVertex getSurfaceQuadraticTrianglesAroundQuadraticTriangle(unsigned int triangle);
    virtual const TrianglesAroundVertex getSurfaceQuadraticTrianglesAroundQuadraticEdge(unsigned int edgeID);
    //virtual const TrianglesAroundVertex getSurfaceQuadraticTrianglesAroundQuadraticEdgeOrientation(unsigned int edgeID);


    /** returns the index (either 0, 1 ,2 or 3) of the vertex whose global index is vertexIndex. Returns -1 if none */
    virtual const int getVertexIndexInTetrahedron(const QuadraticTetra &t, PointID vertexIndex);
    virtual const int getVertexIndexInQuadraticTriangle (const QuadraticTriangle &t, PointID vertexIndex);
    virtual const int getVertexIndexInTetrahedron(double r, double s, double t);

//
//
//    /// Get information about connexity of the mesh
//    /// @{
//    /** \brief Checks if the topology has only one connected component
//      *
//      * @return true if only one connected component
//      */
//    virtual bool checkConnexity();
//
//    /// Returns the number of connected component.
//    virtual unsigned int getNumberOfConnectedComponent();
//
//    /// Returns the set of element indices connected to an input one (i.e. which can be reached by topological links)
//    virtual const sofa::helper::vector<unsigned int> getConnectedElement(unsigned int elem);
//
//    /// Returns the set of element indices adjacent to a given element (i.e. sharing a link)
//    virtual const sofa::helper::vector<unsigned int> getElementAroundElement(unsigned int elem);
//    /// Returns the set of element indices adjacent to a given list of elements (i.e. sharing a link)
//    virtual const sofa::helper::vector<unsigned int> getElementAroundElements(sofa::helper::vector<unsigned int> elems);
//    /// @}


    // Points accessors (not always available)

//    virtual bool hasPos() const;
//    virtual double getPX(int i) const;
//    virtual double getPY(int i) const;
//    virtual double getPZ(int i) const;

    // for procedural creation without file loader
    virtual void clear();
    void addPoint(double px, double py, double pz);
    void addEdge( int a, int b );
    void addTriangle( int a, int b, int c );
    void addQuad( int a, int b, int c, int d );
    void addTetra( int a, int b, int c, int d );
    void addHexa( int a, int b, int c, int d, int e, int f, int g, int h );
    //TODO addQuadTetra()
    void addQuadTetra(int a, int b, int c, int d, int e,
					  int f, int g, int h, int i, int j);

    /// get the current revision of this mesh (use to detect changes)
    int getRevision() const { return revision; }

    /// return true if the given cube is active, i.e. it contains or is surrounded by mapped points.
    /// @deprecated
    virtual bool isCubeActive(int /*index*/) { return true; }

    void draw(){};

    virtual bool hasVolume() { return ( ( getNbTetrahedra() + getNbHexahedra() ) > 0 ); }
    virtual bool hasSurface() { return ( ( getNbTriangles() + getNbQuads() ) > 0 ); }
    virtual bool hasLines() { return ( ( getNbLines() ) > 0 ); }

    virtual bool isVolume() { return hasVolume(); }
    virtual bool isSurface() { return !hasVolume() && hasSurface(); }
    virtual bool isLines() { return !hasVolume() && !hasSurface() && hasLines(); }


//    /// Returns the set of edges adjacent to a given vertex.
//    virtual const EdgesAroundVertex &getOrientedEdgesAroundVertex(PointID i);
//
//    /// Returns the set of oriented triangle adjacent to a given vertex.
//    virtual const TrianglesAroundVertex &getOrientedTrianglesAroundVertex(PointID i);
//
//    /// Returns the set of oriented quad adjacent to a given vertex.
//    virtual const QuadsAroundVertex &getOrientedQuadsAroundVertex(PointID i);
//
//
//    // test whether p0p1 has the same orientation as triangle t
//    // opposite dirction: return -1
//    // same direction: return 1
//    // otherwise: return 0
//    int computeRelativeOrientationInTri(const unsigned int ind_p0, const unsigned int ind_p1, const unsigned int ind_t);
//
//    // test whether p0p1 has the same orientation as triangle t
//    // opposite dirction: return -1
//    // same direction: return 1
//    // otherwise: return 0
//    int computeRelativeOrientationInQuad(const unsigned int ind_p0, const unsigned int ind_p1, const unsigned int ind_q);


    // functions returning border elements. To be moved in a mapping.
    //virtual const sofa::helper::vector <TriangleID>& getTrianglesOnBorder();

    //virtual const sofa::helper::vector <EdgeID>& getEdgesOnBorder();

    //virtual const sofa::helper::vector <PointID>& getPointsOnBorder();

 protected:
	int nbPoints;
	typedef vector<defaulttype::Vec<3, SReal > > SeqPoints;
	Data< SeqPoints > seqPoints;

	Data<SeqEdges> seqEdges;
	bool validEdges;

	//SeqTriangles   seqTriangles;
    Data<SeqTriangles> seqTriangles;
	bool         validTriangles;

	//SeqTriangles   seqTriangles;
	Data<SeqQuadraticTriangles> seqQuadraticTriangles;
	Data<SeqQuadraticEdges> seqQuadraticEdges;
	Data<SeqQuadraticTriangles> seqQuadraticTrianglesVTK;
	bool         validQuadraticTriangles;
	bool         validQuadraticEdges;

	//SeqQuads       seqQuads;
	Data<SeqQuads>       seqQuads;
	bool         validQuads;

	//SeqTetrahedra      seqTetrahedra;
	Data<SeqTetrahedra>      seqTetrahedra;
	bool         validTetrahedra;

	//TODO SeqQuadTetrahedra
	Data<SeqQuadTetrahedra>     seqQuadTetrahedra;
	bool		 validQuadTetrahedra;

#ifdef SOFA_NEW_HEXA
	//SeqHexahedra	   seqHexahedra;
	Data<SeqHexahedra>	   seqHexahedra;
#else
	Data<SeqCubes>       seqHexahedra;
#endif
	bool         validHexahedra;

	/** the array that stores the set of edge-vertex shells, ie for each vertex gives the set of adjacent edges */
	vector< EdgesAroundVertex > m_edgesAroundVertex;

	/** the array that stores the set of oriented edge-vertex shells, ie for each vertex gives the set of adjacent edges */
	vector< EdgesAroundVertex > m_orientedEdgesAroundVertex;

	/** the array that stores the set of edge-triangle shells, ie for each triangle gives the 3 adjacent edges */
	vector< EdgesInTriangle > m_edgesInTriangle;

	/// provides the 4 edges in each quad
	vector< EdgesInQuad > m_edgesInQuad;

	/// provides the set of edges for each tetrahedron
	vector< EdgesInTetrahedron > m_edgesInTetrahedron;

	/// provides the set of edges for each hexahedron
	vector< EdgesInHexahedron > m_edgesInHexahedron;

	/// for each vertex provides the set of triangles adjacent to that vertex
	vector< TrianglesAroundVertex > m_trianglesAroundVertex;

	vector< TrianglesAroundVertex > m_QuadraticTrianglesAroundVertex;

	vector< EdgesAroundVertex > m_QuadraticEdgesAroundVertex;

	/// for each vertex provides the set of oriented triangles adjacent to that vertex
	vector< TrianglesAroundVertex > m_orientedTrianglesAroundVertex;

	/// for each edge provides the set of triangles adjacent to that edge
	vector< TrianglesAroundEdge > m_trianglesAroundEdge;

	/// provides the set of triangles adjacent to each tetrahedron
	vector< TrianglesInTetrahedron > m_trianglesInTetrahedron;

	vector< TrianglesInTetrahedron > m_QuadraticTrianglesInQuadraticTetrahedron;

	vector< EdgesInTriangle > m_QuadraticEdgesInQuadraticTriangle;
	vector< EdgesInTriangle > m_QuadraticEdgesInQuadraticTriangleOrientation;

	vector< PointID> m_SurfaceNodes;
	vector<TrianglesAroundVertex> m_SurfaceQuadraticTrianglesAroundSurfaceNodes;
	vector<TrianglesAroundVertex> m_SurfaceQuadraticTrianglesAroundNodes;

	/// for each vertex provides the set of quads adjacent to that vertex
	vector< QuadsAroundVertex > m_quadsAroundVertex;

	/// for each vertex provides the set of oriented quads adjacent to that vertex
	vector< QuadsAroundVertex > m_orientedQuadsAroundVertex;

	/// for each edge provides the set of quads adjacent to that edge
	vector< QuadsAroundEdge > m_quadsAroundEdge;

	/// provides the set of quads adjacents to each hexahedron
	vector< QuadsInHexahedron > m_quadsInHexahedron;

	/// provides the set of tetrahedrons adjacents to each vertex
	vector< TetrahedraAroundVertex> m_tetrahedraAroundVertex;

	/// for each edge provides the set of tetrahedra adjacent to that edge
	vector< TetrahedraAroundEdge > m_tetrahedraAroundEdge;

	/// for each triangle provides the set of tetrahedrons adjacent to that triangle
	vector< TetrahedraAroundTriangle > m_tetrahedraAroundTriangle;

	vector< TetrahedraAroundTriangle > m_QuadraticTetrahedraAroundTriangle;

	/// provides the set of hexahedrons for each vertex
	vector< HexahedraAroundVertex > m_hexahedraAroundVertex;

	/// for each edge provides the set of tetrahedra adjacent to that edge
	vector< HexahedraAroundEdge > m_hexahedraAroundEdge;

	/// for each quad provides the set of hexahedrons adjacent to that quad
	vector< HexahedraAroundQuad > m_hexahedraAroundQuad;

	vector<TrianglesAroundVertex> m_SurfaceQuadraticTrianglesAroundQuadraticTriangle;

	vector<TrianglesAroundVertex> m_SurfaceQuadraticTrianglesAroundQuadraticEdge;

//	/** \brief Creates the EdgeSetIndex.
//	 *
//	 * This function is only called if the EdgesAroundVertex member is required.
//	 * m_edgesAroundVertex[i] contains the indices of all edges having the ith DOF as
//	 * one of their ends.
//	 */
//	void createEdgesAroundVertexArray();
//
//	/** \brief Creates the array of edge indices for each triangle.
//	 *
//	 * This function is only called if the EdgesInTriangle array is required.
//	 * m_edgesInTriangle[i] contains the 3 indices of the 3 edges composing the ith triangle.
//	 */
//	void createEdgesInTriangleArray();
//
//	/** \brief Creates the array of edge indices for each quad.
//	 *
//	 * This function is only called if the EdgesInQuad array is required.
//	 * m_edgesInQuad[i] contains the 4 indices of the 4 edges composing the ith quad.
//	 */
//	void createEdgesInQuadArray();
//
//	/** \brief Creates the array of edge indices for each tetrahedron.
//	 *
//	 * This function is only called if the EdgesInTetrahedrone array is required.
//	 * m_edgesInTetrahedron[i] contains the 6 indices of the 6 edges of each tetrahedron
//	 The number of each edge is the following : edge 0 links vertex 0 and 1, edge 1 links vertex 0 and 2,
//	 edge 2 links vertex 0 and 3, edge 3 links vertex 1 and 2, edge 4 links vertex 1 and 3,
//	 edge 5 links vertex 2 and 3
//	*/
//	void createEdgesInTetrahedronArray();
//
//	/** \brief Creates the array of edge indices for each hexahedron.
//	 *
//	 * This function is only called if the EdgesInHexahedron array is required.
//	 * m_edgesInHexahedron[i] contains the 12 indices of the 12 edges of each hexahedron.
//	 */
//	void createEdgesInHexahedronArray();
//
//	/** \brief Creates the TrianglesAroundVertex Array.
//	 *
//	 * This function is only called if the TrianglesAroundVertex array is required.
//	 * m_trianglesAroundVertex[i] contains the indices of all triangles adjacent to the ith DOF.
//	 */
//	void createTrianglesAroundVertexArray();
	void createQuadraticTrianglesAroundVertexArray();
	void createQuadraticEdgesAroundVertexArray();

	void createSurfaceNodes();
	void createQuadraticTrianglesAroundSurfaceNodeArray();
	void createQuadraticTrianglesAroundNodeArray();
	void createSurfaceQuadraticTrianglesAroundQuadraticTriangleArray();
	void createSurfaceQuadraticTrianglesAroundQuadraticEdgeArray();
//
//	/** \brief Creates the oriented Triangle Vertex Shell Array
//	*
//	* This function is only called if the OrientedTrianglesAroundVertex array is required.
//	* m_orientedTrianglesAroundVertex[i] contains the indices of all triangles adjacent to the ith vertex
//	*/
//	void createOrientedTrianglesAroundVertexArray();
//
//	/** \brief Creates the TrianglesAroundEdge Array.
//	 *
//	 * This function is only called if the TrianglesAroundVertex array is required.
//	 * m_trianglesAroundEdge[i] contains the indices of all triangles adjacent to the ith edge.
//	 */
//	void createTrianglesAroundEdgeArray();
//
//	/** \brief Creates the array of triangle indices for each tetrahedron.
//	 *
//	 * This function is only called if the TrianglesInTetrahedron array is required.
//	 * m_trianglesInTetrahedron[i] contains the 4 indices of the 4 triangles composing the ith tetrahedron.
//	 */
//	void createTrianglesInTetrahedronArray ();

	void createQuadraticTrianglesInQuadraticTetrahedronArray ();

//
//	/** \brief Creates the QuadsAroundVertex Array.
//	 *
//	 * This function is only called if the QuadsAroundVertex array is required.
//	 * m_quadsAroundVertex[i] contains the indices of all quads adjacent to the ith vertex.
//	 */
//	void createQuadsAroundVertexArray ();
//
//	/** \brief Creates the Quad Vertex Shell Array
//	 *
//	 * This function is only called if the QuadsAroundVertex array is required.
//	 * m_quadsAroundVertex[i] contains the indices of all quads adjacent to the ith vertex
//	 */
//	void createOrientedQuadsAroundVertexArray ();
//
//	/** \brief Creates the quadsAroundEdge Array.
//	 *
//	 * This function is only called if the QuadsAroundVertex array is required.
//	 * m_quadsAroundEdge[i] contains the indices of all quads adjacent to the ith edge
//	 */
//	void createQuadsAroundEdgeArray();
//
//	/** \brief Creates the array of quad indices for each hexahedron.
//	 *
//	 * This function is only called if the QuadsInHexahedron array is required.
//	 * m_quadsInHexahedron[i] contains the 6 indices of the 6 quads of each hexahedron.
//	 */
//	void createQuadsInHexahedronArray ();
//
	/** \brief Creates the TetrahedraAroundVertex Array.
	 *
	 * This function is only called if the TetrahedraAroundVertex array is required.
	 * m_tetrahedraAroundVertex[i] contains the indices of all tetrahedra adjacent to the ith vertex.
	 */
	void createTetrahedraAroundVertexArray ();
//
//	/** \brief Creates the TetrahedraAroundEdge Array.
//	 *
//	 * This function is only called if the TetrahedraAroundEdge array is required.
//	 * m_tetrahedraAroundEdge[i] contains the indices of all tetrahedra adjacent to the ith edge.
//	 */
//	void createTetrahedraAroundEdgeArray();
//
//	/** \brief Creates the TetrahedraAroundTriangle Array.
//	 *
//	 * This function is only called if the TetrahedraAroundTriangle array is required.
//	 * m_tetrahedraAroundTriangle[i] contains the indices of all tetrahedra adjacent to the ith triangle.
//	 */
//	void createTetrahedraAroundTriangleArray();
	void createQuadraticTetrahedraAroundQuadraticTriangleArray();
//
//	/** \brief Creates the HexahedraAroundVertex Array.
//	 *
//	 * This function is only called if the HexahedraAroundVertex array is required.
//	 * m_hexahedraAroundVertex[i] contains the indices of all hexahedra adjacent to the ith vertex.
//	 */
//	void createHexahedraAroundVertexArray();
//
//	/** \brief Creates the HexahedraAroundEdge Array.
//	 *
//	 * This function is only called if the HexahedraAroundEdge array is required.
//	 * m_hexahedraAroundEdge[i] contains the indices of all hexahedra adjacent to the ith edge.
//	 */
//	void createHexahedraAroundEdgeArray ();
//
//	/** \brief Creates the HexahedraAroundQuad Array.
//	 *
//	 * This function is only called if the HexahedraAroundQuad array is required.
//	 * m_hexahedraAroundQuad[i] contains the indices of all hexahedra adjacent to the ith quad.
//	 */
//	void createHexahedraAroundQuadArray();
//
//
//
//
//	/** \brief Returns the EdgesInTriangle array (i.e. provide the 3 edge indices for each triangle). */
//	const sofa::helper::vector< EdgesInTriangle > &getEdgesInTriangleArray();
//
//	/** \brief Returns the TrianglesAroundVertex array (i.e. provide the triangles indices adjacent to each vertex). */
//	const sofa::helper::vector< TrianglesAroundVertex > &getTrianglesAroundVertexArray();
	const sofa::helper::vector< TrianglesAroundVertex > &getQuadraticTrianglesAroundVertexArray();

	const sofa::helper::vector< PointID > &getSurfaceNodeArray();
	const sofa::helper::vector< TrianglesAroundVertex > &getSurfaceQuadraticTrianglesAroundSurfaceNodeArray();
	const sofa::helper::vector< TrianglesAroundVertex > &getSurfaceQuadraticTrianglesAroundNodeArray();
//
//	/** \brief Returns the TrianglesAroundEdge array (i.e. provide the triangles indices adjacent to each edge). */
//	const sofa::helper::vector< TrianglesAroundEdge > &getTrianglesAroundEdgeArray();
//
//
//
//	/** \brief Returns the EdgesInQuadArray array (i.e. provide the 4 edge indices for each quad) */
//	const sofa::helper::vector< EdgesInQuad > &getEdgesInQuadArray();
//
//	/** \brief Returns the QuadsAroundVertex array (i.e. provide the quad indices adjacent to each vertex). */
//	const sofa::helper::vector< QuadsAroundVertex > &getQuadsAroundVertexArray();
//
//	/** \brief Returns the QuadsAroundEdge array (i.e. provide the quad indices adjacent to each edge). */
//	const sofa::helper::vector< QuadsAroundEdge > &getQuadsAroundEdgeArray();
//
//
//
//	/** \brief Returns the EdgesInHexahedron array (i.e. provide the 12 edge indices for each hexahedron).	*/
//	const sofa::helper::vector< EdgesInHexahedron > &getEdgesInHexahedronArray();
//
//	/** \brief Returns the QuadsInHexahedron array (i.e. provide the 8 quad indices for each hexahedron).	*/
//	const sofa::helper::vector< QuadsInHexahedron > &getQuadsInHexahedronArray();
//
//	/** \brief Returns the HexahedraAroundVertex array (i.e. provide the hexahedron indices adjacent to each vertex).*/
//	const sofa::helper::vector< HexahedraAroundVertex > &getHexahedraAroundVertexArray();
//
//	/** \brief Returns the HexahedraAroundEdge array (i.e. provide the hexahedron indices adjacent to each edge). */
//	const sofa::helper::vector< HexahedraAroundEdge > &getHexahedraAroundEdgeArray();
//
//	/** \brief Returns the HexahedraAroundQuad array (i.e. provide the hexahedron indices adjacent to each quad). */
//	const sofa::helper::vector< HexahedraAroundQuad > &getHexahedraAroundQuadArray();
//
//
//
//	/** \brief Returns the EdgesInTetrahedron array (i.e. provide the 6 edge indices for each tetrahedron). */
//	const sofa::helper::vector< EdgesInTetrahedron > &getEdgesInTetrahedronArray();
//
//	/** \brief Returns the TrianglesInTetrahedron array (i.e. provide the 4 triangle indices for each tetrahedron). */
//	const sofa::helper::vector< TrianglesInTetrahedron > &getTrianglesInTetrahedronArray();
	const sofa::helper::vector< TrianglesInTetrahedron > &getQuadraticTrianglesInQuadraticTetrahedronArray();
//
//	/** \brief Returns the TetrahedraAroundVertex array (i.e. provide the tetrahedron indices adjacent to each vertex). */
//	const sofa::helper::vector< TetrahedraAroundVertex > &getTetrahedraAroundVertexArray();
//
//	/** \brief Returns the TetrahedraAroundEdge array (i.e. provide the tetrahedron indices adjacent to each edge). */
//	const sofa::helper::vector< TetrahedraAroundEdge > &getTetrahedraAroundEdgeArray();
//
//	/** \brief Returns the TetrahedraAroundTriangle array (i.e. provide the tetrahedron indices adjacent to each triangle). */
//	const sofa::helper::vector< TetrahedraAroundTriangle > &getTetrahedraAroundTriangleArray();



public:
	/** \brief Returns the index of the edge joining vertex v1 and vertex v2; returns -1 if no edge exists
	 *
	 */
//	int getEdgeIndex(PointID v1, PointID v2);
//
//	/** Returns the indices of a triangle given three vertex indices : returns -1 if none */
//	int getTriangleIndex(PointID v1, PointID v2, PointID v3);
	int getQuadraticTriangleIndex(int* v);
//
//	/** \brief Returns the index of the quad joining vertex v1, v2, v3 and v4; returns -1 if no edge exists
//	 *
//	 */
//	int getQuadIndex(PointID v1, PointID v2, PointID v3,  PointID v4);
//
//	/** \brief Returns the index of the tetrahedron given four vertex indices; returns -1 if no edge exists
//	 *
//	 */
//	int getTetrahedronIndex(PointID v1, PointID v2, PointID v3, PointID v4);
//
//	/** \brief Returns the index of the hexahedron given eight vertex indices; returns -1 if no edge exists
//	 *
//	 */
//	int getHexahedronIndex(PointID v1, PointID v2, PointID v3, PointID v4, PointID v5, PointID v6, PointID v7, PointID v8);
//
//	/** \brief Returns the index (either 0, 1 ,2 or 3) of the vertex whose global index is vertexIndex. Returns -1 if none
//	*
//	*/
//
//	int getVertexIndexInTriangle(const Triangle &t, PointID vertexIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2) of the edge whose global index is edgeIndex. Returns -1 if none
//	*
//	*/
//	int getEdgeIndexInTriangle(const EdgesInTriangle &t, EdgeID edgeIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 or 3) of the vertex whose global index is vertexIndex. Returns -1 if none
//	*
//	*/
//	int getVertexIndexInQuad(const Quad &t, PointID vertexIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2, 3) of the edge whose global index is edgeIndex. Returns -1 if none
//	*
//	*/
//	int getEdgeIndexInQuad(const EdgesInQuad &t, EdgeID edgeIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 or 3) of the vertex whose global index is vertexIndex. Returns -1 if none
//	*
//	*/
//	int getVertexIndexInTetrahedron(const Tetra &t, PointID vertexIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 ,3, 4, 5) of the edge whose global index is edgeIndex. Returns -1 if none
//	*
//	*/
//	int getEdgeIndexInTetrahedron(const EdgesInTetrahedron &t, EdgeID edgeIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 ,3) of the triangle whose global index is triangleIndex. Returns -1 if none
//	*
//	*/
	//int getTriangleIndexInTetrahedron(const TrianglesInTetrahedron &t, TriangleID triangleIndex) const;

	int getQuadraticTriangleIndexInTetrahedron(const TrianglesInTetrahedron &t, TriangleID triangleIndex) const;
	int getQuadraticEdgeIndexInTriangle(const EdgesInTriangle &t, EdgeID edgeIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2, 3, 4, 5, 6, or 7) of the vertex whose global index is vertexIndex. Returns -1 if none
//	*
//	*/
//	int getVertexIndexInHexahedron(const Hexa &t, PointID vertexIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 ,3, 4, 5, 6, 7, 8, 9, 10, 11) of the edge whose global index is edgeIndex. Returns -1 if none
//	*
//	*/
//	int getEdgeIndexInHexahedron(const EdgesInHexahedron &t, EdgeID edgeIndex) const;
//
//	/** \brief Returns the index (either 0, 1 ,2 ,3, 4, 5) of the quad whose global index is quadIndex. Returns -1 if none
//	*
//	*/
//	int getQuadIndexInHexahedron(const QuadsInHexahedron &t, QuadID quadIndex) const;
//
//	/** \brief Returns for each index (between 0 and 5) the two vertex indices that are adjacent to that edge
//	*
//	*/
//	Edge getLocalEdgesInTetrahedron (const unsigned int i) const;
//
//	/** \brief Returns for each index (between 0 and 12) the two vertex indices that are adjacent to that edge */
//	Edge getLocalEdgesInHexahedron (const unsigned int i) const;


   int revision;

	Data< bool > _draw;

//	void invalidate();

	virtual void updateEdges();
	virtual void updateTriangles();
	virtual void updateQuadraticTriangles();
	virtual void updateQuadraticEdges();
	virtual void updateQuads();
	virtual void updateTetrahedra();
	virtual void updateHexahedra();
	//TODO updateQuadTetrahedra()
	virtual void updateQuadTetrahedra();

	void invalidate();

	protected:
//        virtual void loadFromMeshLoader(sofa::component::container::MeshLoader* loader);
        sofa::core::topology::TopologyObjectType UpperTopology;
};

} // namespace topology

} // namespace component

} // namespace sofa

#endif
