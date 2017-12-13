/*
 * BaseMeshTopology for Quadratic Tetrahedra
 * The BaseMeshTopology hier is just a header file of the Class, the detail implement is defined in the SOFA_SVN
 * we don't need to define anything, except the new functions of Quadratic Tetrahedron.
 */

#ifndef SOFA_CORE_TOPOLOGY_QUADRATICBASEMESHTOPOLOGY_H
#define SOFA_CORE_TOPOLOGY_QUADRATICBASEMESHTOPOLOGY_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <sofa/core/topology/Topology.h>
#include <sofa/core/objectmodel/DataFileName.h>
#include <sofa/helper/fixed_array.h>
#include <sofa/helper/vector.h>
#include <sofa/helper/list.h>

#include <sofa/core/DataEngine.h>
#include <sofa/core/core.h>

#include <sofa/core/topology/BaseMeshTopology.h>


namespace sofa
{

namespace core
{

namespace topology
{

using namespace sofa::defaulttype;
using helper::vector;
using helper::fixed_array;

#define SOFA_NEW_HEXA

class SOFA_CORE_API QuadraticBaseMeshTopology : public sofa::core::topology::BaseMeshTopology
{
public:
	SOFA_CLASS(QuadraticBaseMeshTopology, sofa::core::topology::BaseMeshTopology);

//    typedef unsigned int index_type;
//    enum { InvalidID = (unsigned)-1 };

	typedef index_type                   QuadTetraID;

	typedef fixed_array<PointID, 10>     QuadTetra;

	typedef vector<QuadTetra>			 SeqQuadTetrahedra;

	typedef vector<QuadTetraID>		     QuadTetrahedraAroundVertex;
	typedef vector<QuadTetraID>		     QuadTetrahedraAroundEdge;
	typedef vector<QuadTetraID>		     QuadTetrahedraAroundTriangle;

	QuadraticBaseMeshTopology():BaseMeshTopology(){};
//	QuadraticBaseMeshTopology():fileTopology(initData(&fileTopology,"fileTopology","Filename of the mesh")){
//		addAlias(&fileTopology,"filename");
//	}

//	virtual bool load(const char* filename);
//	virtual std::string getFilename() const {return fileTopology.getValue();}


	virtual const SeqQuadTetrahedra& getQuadTetrahedra() = 0;

	virtual const int getVertexIndexInTetrahedron(const QuadTetra &t, PointID vertexIndex) =0;

	virtual int getNbQuadTetrahedra(){
		return getQuadTetrahedra().size();
	}
	virtual const QuadTetra getQuadTetrahedron(QuadTetraID i){
		return getQuadTetrahedra()[i];
	}

private:
	const TetrahedraAroundVertex tav;
	const TetrahedraAroundEdge tae;
	const TetrahedraAroundTriangle tat;

public:
	//the following 3 function are not necessary for Quad.Tetra.FEM
	/// Returns the set of tetrahedra adjacent to a given vertex.
	virtual const TetrahedraAroundVertex& getQuadTetrahedraAroundVertex(PointID i){return tav;};
    /// Returns the set of tetrahedra adjacent to a given edge.
	virtual const TetrahedraAroundEdge& getQuadTetrahedraAroundEdge(EdgeID i){return tae;};
	/// Returns the set of tetrahedra adjacent to a given triangle.
	virtual const TetrahedraAroundTriangle& getQuadTetrahedraAroundTriangle(TriangleID i){return tat;};

protected:

//	sofa::core::objectmodel::DataFileName fileTopology;

};

} // namespace topology

} // namespace core

} // namespace sofa

#endif
