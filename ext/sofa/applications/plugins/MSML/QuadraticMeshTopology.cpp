/******************************************************************************

******************************************************************************/
#include <iostream>
//the head file of MyMeshTopology should be included at first
#include <QuadraticMeshTopology.h>
//#include <sofa/component/topology/MeshTopology.h>
#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/fixed_array.h>
//#include <sofa/component/container/MeshLoader.h>
#include <sofa/helper/system/gl.h>
#include <sofa/helper/gl/template.h>
#include <set>
#include <string.h>

#define TOLERANCE 1e-6

namespace sofa
{

namespace component
{

namespace topology
{

using namespace sofa::defaulttype;
using core::topology::QuadraticBaseMeshTopology;

SOFA_DECL_CLASS(QuadraticMeshTopology)

int QuadraticMeshTopologyClass = core::RegisterObject("Generic mesh topology")
.addAlias("QuadraticMesh")
.add< QuadraticMeshTopology >()
;

QuadraticMeshTopology::QuadraticMeshTopology()
: nbPoints(0)
, seqPoints(initData(&seqPoints,"position","List of point positions"))
, seqEdges(initData(&seqEdges,"edges","List of edge indices")), validEdges(false)
, seqTriangles(initData(&seqTriangles,"triangles","List of triangle indices")), validTriangles(false)
, seqQuads(initData(&seqQuads,"quads","List of quad indices")), validQuads(false)
, seqTetrahedra(initData(&seqTetrahedra,"tetrahedra","List of tetrahedron indices")), validTetrahedra(false)
//TODO seqQuadTetrahedra new added
, seqQuadTetrahedra(initData(&seqQuadTetrahedra, "Quadtetrahedra", "List of Quadratic Tetrahedra indices")), validQuadTetrahedra(false)

, seqHexahedra(initData(&seqHexahedra,"hexahedra","List of hexahedron indices")), validHexahedra(false)
, revision(0)
, _draw(initData(&_draw, false, "drawHexahedra","if true, draw the topology hexahedra"))
, UpperTopology(sofa::core::topology::EDGE)
{
    addAlias(&seqPoints,"points");
    addAlias(&seqEdges,"lines");
    addAlias(&seqTetrahedra,"tetras");
    //TODO new added
    addAlias(&seqQuadTetrahedra, "quadTetras");
    addAlias(&seqHexahedra,"hexas");

    validQuadraticTriangles = false;
    validQuadraticEdges = false;
}
//QuadraticMeshTopology::QuadraticMeshTopology(){};

void QuadraticMeshTopology::init()
{
	 BaseMeshTopology::init();
	    if (nbPoints==0)
	    {


	      // looking for upper topology
	      if (seqHexahedra.isSet())
	         UpperTopology = sofa::core::topology::HEXAHEDRON;
	      else if (seqTetrahedra.isSet())
	         UpperTopology = sofa::core::topology::TETRAHEDRON;
	      else if (seqQuads.isSet())
	         UpperTopology = sofa::core::topology::QUAD;
	      else if (seqTriangles.isSet())
	         UpperTopology = sofa::core::topology::TRIANGLE;
	      else
	         UpperTopology = sofa::core::topology::EDGE;
		}

	    // compute the number of points, if the topology is charged from the scene or if it was loaded from a MeshLoader without any points data.
	    if (nbPoints==0)
	    {
	        unsigned int n = 0;
	        for (unsigned int i=0; i<seqEdges.getValue().size();i++)
	        {
	            for (unsigned int j=0; j<seqEdges.getValue()[i].size();j++)
	            {
	                if (n <= seqEdges.getValue()[i][j])
	                    n = 1 + seqEdges.getValue()[i][j];
	            }
	        }
	        for (unsigned int i=0; i<seqTriangles.getValue().size();i++)
	        {
	            for (unsigned int j=0; j<seqTriangles.getValue()[i].size();j++)
	            {
	                if (n <= seqTriangles.getValue()[i][j])
	                    n = 1 + seqTriangles.getValue()[i][j];
	            }
	        }
	        for (unsigned int i=0; i<seqQuads.getValue().size();i++)
	        {
	            for (unsigned int j=0; j<seqQuads.getValue()[i].size();j++)
	            {
	                if (n <= seqQuads.getValue()[i][j])
	                    n = 1 + seqQuads.getValue()[i][j];
	            }
	        }
	        for (unsigned int i=0; i<seqTetrahedra.getValue().size();i++)
	        {
	            for (unsigned int j=0; j<seqTetrahedra.getValue()[i].size();j++)
	            {
	                if (n <= seqTetrahedra.getValue()[i][j])
	                    n = 1 + seqTetrahedra.getValue()[i][j];
	            }
	        }
	        for (unsigned int i=0; i<seqHexahedra.getValue().size();i++)
	        {
	            for (unsigned int j=0; j<seqHexahedra.getValue()[i].size();j++)
	            {
	                if (n <= seqHexahedra.getValue()[i][j])
	                    n = 1 + seqHexahedra.getValue()[i][j];
	            }
	        }

	        for (unsigned int i=0; i<seqQuadTetrahedra.getValue().size();i++)
			{
				for (unsigned int j=0; j<seqQuadTetrahedra.getValue()[i].size();j++)
				{
					if (n <= seqQuadTetrahedra.getValue()[i][j])
						n = 1 + seqQuadTetrahedra.getValue()[i][j];
				}
			}

	        nbPoints = n;
	    }
}

//void QuadraticMeshTopology::loadFromMeshLoader(sofa::component::container::MeshLoader* loader)
//{
//    nbPoints = loader->getNbPoints();
//    //loader->getPoints(*seqPoints.beginEdit());
//    vector<helper::fixed_array<SReal,3> > points;
//    loader->getPoints(points);
//    vector< defaulttype::Vec<3,SReal> >& opoints = *seqPoints.beginEdit();
//    opoints.resize(points.size());
//    for (unsigned int i=0;i<points.size();++i) opoints[i] = defaulttype::Vec<3,SReal>(points[i].data());
//    seqPoints.endEdit();
//    loader->getEdges(*seqEdges.beginEdit()); seqEdges.endEdit();
//    loader->getTriangles(*seqTriangles.beginEdit()); seqTriangles.endEdit();
//    loader->getQuads(*seqQuads.beginEdit()); seqQuads.endEdit();
//    loader->getTetrahedra(*seqTetrahedra.beginEdit()); seqTetrahedra.endEdit();
//    //TODO new added, MeshLoader new define?
////    loader->getQuadTetrahedra
//    loader->getHexahedra(*seqHexahedra.beginEdit()); seqHexahedra.endEdit();
//}

const QuadraticMeshTopology::SeqQuadraticTriangles& QuadraticMeshTopology::getQuadraticTriangles()
{
    if (!validQuadraticTriangles)
    {
        updateQuadraticTriangles();
        validQuadraticTriangles = true;
    }
    return seqQuadraticTriangles.getValue();
}

const QuadraticMeshTopology::SeqQuadraticEdges& QuadraticMeshTopology::getQuadraticEdges()
{
    if (!validQuadraticEdges)
    {
        updateQuadraticEdges();
        validQuadraticEdges = true;
    }
    return seqQuadraticEdges.getValue();
}

const QuadraticMeshTopology::SeqQuadraticTriangles& QuadraticMeshTopology::getQuadraticTrianglesVTK()
{
    if (!validQuadraticTriangles)
    {
        updateQuadraticTriangles();
        validQuadraticTriangles = true;
    }
    return seqQuadraticTrianglesVTK.getValue();
}

void QuadraticMeshTopology::clear()
{
	nbPoints = 0;
	seqPoints.beginEdit()->clear(); seqPoints.endEdit();
	seqEdges.beginEdit()->clear(); seqEdges.endEdit();
	seqTriangles.beginEdit()->clear(); seqTriangles.endEdit();
	seqQuads.beginEdit()->clear();seqQuads.endEdit();
	seqTetrahedra.beginEdit()->clear();seqTetrahedra.endEdit();
	seqHexahedra.beginEdit()->clear();seqHexahedra.endEdit();

	seqQuadTetrahedra.beginEdit()->clear();seqQuadTetrahedra.endEdit();


	invalidate();
}

void QuadraticMeshTopology::invalidate()
{
    validEdges = false;
    validTriangles = false;
    validQuads = false;
    validTetrahedra = false;
    validHexahedra = false;

	//validUVs = false;

    m_edgesAroundVertex.clear();
    m_edgesInTriangle.clear();
    m_edgesInQuad.clear();
    m_edgesInTetrahedron.clear();
    m_edgesInHexahedron.clear();
    m_trianglesAroundVertex.clear();
    m_trianglesAroundEdge.clear();
    m_trianglesInTetrahedron.clear();
    m_quadsAroundVertex.clear();
    m_quadsAroundEdge.clear();
    m_quadsInHexahedron.clear();
    m_tetrahedraAroundVertex.clear();
    m_tetrahedraAroundEdge.clear();
    m_tetrahedraAroundTriangle.clear();
    m_hexahedraAroundVertex.clear();
    m_hexahedraAroundEdge.clear();
    m_hexahedraAroundQuad.clear();
    ++revision;
    //sout << "MeshTopology::invalidate()"<<sendl;
}

void QuadraticMeshTopology::addPoint(double px, double py, double pz)
{
    seqPoints.beginEdit()->push_back(defaulttype::Vec<3,SReal>((SReal)px, (SReal)py, (SReal)pz));
    seqPoints.endEdit();
    if (seqPoints.getValue().size() > (unsigned)nbPoints)
        nbPoints = seqPoints.getValue().size();
}

void QuadraticMeshTopology::addEdge( int a, int b )
{
    seqEdges.beginEdit()->push_back(Edge(a,b));
    seqEdges.endEdit();
    if (a >= (int)nbPoints) nbPoints = a+1;
    if (b >= (int)nbPoints) nbPoints = b+1;
}

void QuadraticMeshTopology::addTriangle( int a, int b, int c )
{
    seqTriangles.beginEdit()->push_back( Triangle(a,b,c) );
    seqTriangles.endEdit();
    if (a >= (int)nbPoints) nbPoints = a+1;
    if (b >= (int)nbPoints) nbPoints = b+1;
    if (c >= (int)nbPoints) nbPoints = c+1;
}

void QuadraticMeshTopology::addQuad(int a, int b, int c, int d)
{
    seqQuads.beginEdit()->push_back(Quad(a,b,c,d));
    seqQuads.endEdit();
    if (a >= (int)nbPoints) nbPoints = a+1;
    if (b >= (int)nbPoints) nbPoints = b+1;
    if (c >= (int)nbPoints) nbPoints = c+1;
    if (d >= (int)nbPoints) nbPoints = d+1;
}

void QuadraticMeshTopology::addTetra( int a, int b, int c, int d )
{
    seqTetrahedra.beginEdit()->push_back( Tetra(a,b,c,d) );
    seqTetrahedra.endEdit();
    if (a >= (int)nbPoints) nbPoints = a+1;
    if (b >= (int)nbPoints) nbPoints = b+1;
    if (c >= (int)nbPoints) nbPoints = c+1;
    if (d >= (int)nbPoints) nbPoints = d+1;
}

void QuadraticMeshTopology::addHexa(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8)
{
#ifdef SOFA_NEW_HEXA
    seqHexahedra.beginEdit()->push_back(Hexa(p1,p2,p3,p4,p5,p6,p7,p8));
#else
    seqHexahedra.beginEdit()->push_back(Hexa(p1,p2,p4,p3,p5,p6,p8,p7));
#endif
    seqHexahedra.endEdit();
    if (p1 >= (int)nbPoints) nbPoints = p1+1;
    if (p2 >= (int)nbPoints) nbPoints = p2+1;
    if (p3 >= (int)nbPoints) nbPoints = p3+1;
    if (p4 >= (int)nbPoints) nbPoints = p4+1;
    if (p5 >= (int)nbPoints) nbPoints = p5+1;
    if (p6 >= (int)nbPoints) nbPoints = p6+1;
    if (p7 >= (int)nbPoints) nbPoints = p7+1;
    if (p8 >= (int)nbPoints) nbPoints = p8+1;
}

/*
 * TODO new added function
 */
void QuadraticMeshTopology::addQuadTetra(int p1, int p2, int p3, int p4, int p5,
								  int p6, int p7, int p8, int p9, int p10)
{
	seqQuadTetrahedra.beginEdit()->push_back(QuadTetra(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10));
	seqQuadTetrahedra.endEdit();
    if (p1 >= (int)nbPoints) nbPoints = p1+1;
    if (p2 >= (int)nbPoints) nbPoints = p2+1;
    if (p3 >= (int)nbPoints) nbPoints = p3+1;
    if (p4 >= (int)nbPoints) nbPoints = p4+1;
    if (p5 >= (int)nbPoints) nbPoints = p5+1;
    if (p6 >= (int)nbPoints) nbPoints = p6+1;
    if (p7 >= (int)nbPoints) nbPoints = p7+1;
    if (p8 >= (int)nbPoints) nbPoints = p8+1;
    if (p9 >= (int)nbPoints) nbPoints = p9+1;
    if (p10 >= (int)nbPoints) nbPoints = p10+1;
}


const QuadraticMeshTopology::SeqEdges& QuadraticMeshTopology::getEdges()
{
    if (!validEdges)
    {
        updateEdges();
        validEdges = true;
    }
    return seqEdges.getValue();
}

const QuadraticMeshTopology::SeqTriangles& QuadraticMeshTopology::getTriangles()
{
    if (!validTriangles)
    {
        updateTriangles();
        validTriangles = true;
    }
    return seqTriangles.getValue();
}

const QuadraticMeshTopology::SeqQuads& QuadraticMeshTopology::getQuads()
{
    if (!validQuads)
    {
	updateQuads();
	validQuads = true;
    }
    return seqQuads.getValue();
}

const QuadraticMeshTopology::SeqTetrahedra& QuadraticMeshTopology::getTetrahedra()
{
    if (!validTetrahedra)
    {
        updateTetrahedra();
	validTetrahedra = true;
    }
    return seqTetrahedra.getValue();
}

const QuadraticMeshTopology::SeqHexahedra& QuadraticMeshTopology::getHexahedra()
{
    if (!validHexahedra)
    {
        updateHexahedra();
	validHexahedra = true;
    }
    return seqHexahedra.getValue();
}
/*
 * TODO getQuadTetrahedra()
 */
const QuadraticMeshTopology::SeqQuadTetrahedra& QuadraticMeshTopology::getQuadTetrahedra()
{
	if (!validQuadTetrahedra)
	{
		updateQuadTetrahedra();
		validQuadTetrahedra = true;
	}
	return seqQuadTetrahedra.getValue();
}



int QuadraticMeshTopology::getNbPoints() const
{
    return nbPoints;
}

void QuadraticMeshTopology::setNbPoints(int n)
{
    nbPoints = n;
}

int QuadraticMeshTopology::getNbEdges()
{
    return getEdges().size();
}

int QuadraticMeshTopology::getNbTriangles()
{
    return getTriangles().size();
}

int QuadraticMeshTopology::getNbQuadraticTriangles()
{
    return getQuadraticTriangles().size();
}

int QuadraticMeshTopology::getNbQuads()
{
    return getQuads().size();
}

int QuadraticMeshTopology::getNbTetrahedra()
{
    return getTetrahedra().size();
}

int QuadraticMeshTopology::getNbHexahedra()
{
    return getHexahedra().size();
}

/*
 * TODO getNbQuadTetrahedra
 */
int QuadraticMeshTopology::getNbQuadTetrahedra()
{
	return getQuadTetrahedra().size();
}

const QuadraticMeshTopology::Edge QuadraticMeshTopology::getEdge(index_type i)
{
    return getEdges()[i];
}

const QuadraticMeshTopology::Triangle QuadraticMeshTopology::getTriangle(index_type i)
{
    return getTriangles()[i];
}

const QuadraticMeshTopology::Quad QuadraticMeshTopology::getQuad(index_type i)
{
    return getQuads()[i];
}

const QuadraticMeshTopology::Tetra QuadraticMeshTopology::getTetrahedron(index_type i)
{
    return getTetrahedra()[i];
}

const QuadraticMeshTopology::Hexa QuadraticMeshTopology::getHexahedron(index_type i)
{
    return getHexahedra()[i];
}

/*
 * TODO getQuadTetrahedra(ID)
 */
const QuadraticMeshTopology::QuadTetra QuadraticMeshTopology::getQuadTetrahedra(index_type i)
{
	return getQuadTetrahedra()[i];
}

const QuadraticMeshTopology::QuadraticTriangle QuadraticMeshTopology::getQuadraticTriangle(TriangleID i)
{
	return getQuadraticTriangles()[i];
}

const QuadraticMeshTopology::QuadraticEdge QuadraticMeshTopology::getQuadraticEdge(EdgeID i)
{
	return getQuadraticEdges()[i];
}

const QuadraticMeshTopology::QuadraticTriangle QuadraticMeshTopology::getQuadraticTriangleVTK(TriangleID i)
{
	return getQuadraticTrianglesVTK()[i];
}

void QuadraticMeshTopology::updateEdges()
{
}

void QuadraticMeshTopology::updateQuads()
{
}

void QuadraticMeshTopology::updateTriangles()
{
}

void QuadraticMeshTopology::updateQuadraticTriangles()
{
    if (!seqQuadraticTriangles.getValue().empty()) return; // triangles already defined
    if (getNbQuadTetrahedra()==0) return; // no tetrahedra to extract edges from
    const SeqQuadraticTetrahedra& tetrahedra = getQuadTetrahedra(); // do not use seqTriangles directly as it might not be up-to-date

    // create a temporary map to find redundant triangles
    std::map<QuadraticTriangle,unsigned int> triangleMap;
    std::map<QuadraticTriangle,unsigned int>::iterator itt;
    QuadraticTriangle tr;
    int triangleIndex;
    unsigned int v[6],val,val1, v2[6];
    /// create the m_edge array at the same time than it fills the m_trianglesInTetrahedron array
    for (unsigned int i = 0; i < tetrahedra.size(); ++i)
    {
		const QuadTetra &t=seqQuadTetrahedra.getValue()[i];
		for (unsigned int j=0;j<4;++j)
		{
			switch (j)
			{
			case 0:
				v[0] = t[1]; v[1] = t[5]; v[2] = t[2]; v[3] = t[9]; v[4] = t[3]; v[5] = t[8];
				v2[0] = t[3]; v2[1] = t[1]; v2[2] = t[2]; v2[3] = t[8]; v2[4] = t[5]; v2[5] = t[9];
				break;
			case 1:
				v[0] = t[0]; v[1] = t[7]; v[2] = t[3]; v[3] = t[9]; v[4] = t[2]; v[5] = t[6];
				v2[0] = t[0]; v2[1] = t[3]; v2[2] = t[2]; v2[3] = t[7]; v2[4] = t[9]; v2[5] = t[6];
				break;
			case 2:
				v[0] = t[0]; v[1] = t[4]; v[2] = t[1]; v[3] = t[8]; v[4] = t[3]; v[5] = t[7];
				v2[0] = t[0]; v2[1] = t[1]; v2[2] = t[3]; v2[3] = t[4]; v2[4] = t[8]; v2[5] = t[7];
				break;
			case 3:
				v[0] = t[0]; v[1] = t[6]; v[2] = t[2]; v[3] = t[5]; v[4] = t[1]; v[5] = t[4];
				v2[0] = t[1]; v2[1] = t[0]; v2[2] = t[2]; v2[3] = t[4]; v2[4] = t[6]; v2[5] = t[5];
				break;
			default:
				std::cout<<"Error, cant compute QuadTriangles\n";
				break;
			}

			//		std::sort(v,v+2);
			// sort v such that v[0] is the smallest one
			while ((v[0]>v[2]) || (v[0]>v[4]))// || (v[0]>v[3]) || (v[0]>v[4]) || (v[0]>v[5]))
			{
			val=v[0]; val1=v[1]; v[0]=v[2];v[1]=v[3]; v[2]=v[4]; v[3]=v[5]; v[4]=val; v[5]=val1;
			}
			// check if a triangle with an opposite orientation already exists
			tr=QuadraticTriangle(v[0],v[5],v[4],v[3],v[2],v[1]);
			itt=triangleMap.find(tr);
			if (itt==triangleMap.end())
			{
			// edge not in edgeMap so create a new one
			triangleIndex=seqQuadraticTriangles.getValue().size();
			tr=QuadraticTriangle(v[0],v[1],v[2],v[3],v[4],v[5]);
			triangleMap[tr]=triangleIndex;
			seqQuadraticTriangles.beginEdit()->push_back(tr);
			seqQuadraticTriangles.endEdit();

			tr=QuadraticTriangle(v2[0],v2[1],v2[2],v2[3],v2[4],v2[5]);
			seqQuadraticTrianglesVTK.beginEdit()->push_back(tr);
			seqQuadraticTrianglesVTK.endEdit();
			}
			else
			{
			triangleIndex=(*itt).second;
			}
			//m_trianglesInTetrahedron[i][j]=triangleIndex;
		}
    }

}

void QuadraticMeshTopology::updateQuadraticEdges()
{
    if (!seqQuadraticEdges.getValue().empty()) return; // triangles already defined
    if (getNbQuadraticTriangles()==0) return; // no tetrahedra to extract edges from
    const SeqQuadraticTriangles& triangles = getQuadraticTrianglesVTK(); // do not use seqTriangles directly as it might not be up-to-date

    m_QuadraticEdgesInQuadraticTriangle.resize(triangles.size());
    m_QuadraticEdgesInQuadraticTriangleOrientation.resize(triangles.size());

    // create a temporary map to find redundant edges
    std::map<QuadraticEdge,unsigned int> edgeMap;
    std::map<QuadraticEdge,unsigned int>::iterator itt;
    QuadraticEdge tr;
    int edgeIndex;
    unsigned int v[3],val;
    bool isInMap;
    int edgeOrientation;

    /// create the m_edge array at the same time than it fills the m_EdgesInTriangle array
    for (unsigned int i = 0; i < triangles.size(); ++i)
    {
		const QuadraticTriangle &t=triangles[i];
		for (unsigned int j=0;j<3;++j)
		{
			switch (j)
			{
			//edge nr 0 is where r=0 etc !!!!
			case 0:
				v[0] = t[1]; v[1] = t[4]; v[2] = t[2];
				break;
			case 1:
				v[0] = t[2]; v[1] = t[5]; v[2] = t[0];
				break;
			case 2:
				v[0] = t[0]; v[1] = t[3]; v[2] = t[1];
				break;
			default:
				std::cout<<"Error, cant compute QuadEdges\n";
				break;
			}

			//		std::sort(v,v+2);
//			// sort v such that v[0] is the smallest one
//			while ((v[0]>v[1]) || (v[0]>v[2]))// || (v[0]>v[3]) || (v[0]>v[4]) || (v[0]>v[5]))
//			{
//			val=v[0]; v[0]=v[1];v[1]=v[2]; v[2]=val;
//			}

			isInMap = false;
			//check if edge is  in map
			tr=QuadraticEdge(v[0],v[1],v[2]);
			itt=edgeMap.find(tr);
			if (!(itt==edgeMap.end()))
			{
				isInMap = true;
				edgeOrientation = 1;
				edgeIndex=(*itt).second;
				//std::cout<<"Edge found in map\n";
			}

			tr=QuadraticEdge(v[2],v[1],v[0]);
			itt=edgeMap.find(tr);
			if (!(itt==edgeMap.end()))
			{
				isInMap = true;
				edgeOrientation = -1;
				edgeIndex=(*itt).second;
				//std::cout<<"Edge found in map\n";
			}

			if(!isInMap)
			{
				edgeIndex=seqQuadraticEdges.getValue().size();
				tr=QuadraticEdge(v[0],v[1],v[2]);
				edgeMap[tr]=edgeIndex;
				seqQuadraticEdges.beginEdit()->push_back(tr);
				seqQuadraticEdges.endEdit();
				edgeOrientation = 1;
			}

//			tr=QuadraticEdge(v[0],v[1],v[2]);
//			itt=edgeMap.find(tr);
//			if (!(itt==edgeMap.end()))
//			{
//				std::cout<<"Edge found in map\n";
//			}

			m_QuadraticEdgesInQuadraticTriangle[i][j]=edgeIndex;
			m_QuadraticEdgesInQuadraticTriangleOrientation[i][j]=edgeOrientation;
		}
    }

}

int QuadraticMeshTopology::getQuadraticTriangleIndexInTetrahedron(const TrianglesInTetrahedron &t, TriangleID triangleIndex) const
{
    if (t[0]==triangleIndex)
        return 0;
    else if (t[1]==triangleIndex)
        return 1;
    else if (t[2]==triangleIndex)
        return 2;
    else if (t[3]==triangleIndex)
        return 3;
    else
        return -1;
}

int QuadraticMeshTopology::getQuadraticEdgeIndexInTriangle(const EdgesInTriangle &t, EdgeID edgeIndex) const
{
    if (t[0]==edgeIndex)
        return 0;
    else if (t[1]==edgeIndex)
        return 1;
    else if (t[2]==edgeIndex)
        return 2;
    else
        return -1;
}

const vector< QuadraticMeshTopology::TrianglesInTetrahedron >& QuadraticMeshTopology::getQuadraticTrianglesInQuadraticTetrahedronArray()
{
  if (m_trianglesInTetrahedron.empty()) // this method should only be called when the array exists.
  {
#ifndef NDEBUG
    sout << "Warning. [MeshTopology::getTrianglesInTetrahedronArray] TrianglesInTetrahedron array is empty." << std::endl;
#endif

    createQuadraticTrianglesInQuadraticTetrahedronArray();
  }

  return m_trianglesInTetrahedron;
}

//const vector< QuadraticMeshTopology::TrianglesInTetrahedron >& QuadraticMeshTopology::getQuadraticTrianglesInQuadraticTetrahedronArray()
//{
//  if (m_trianglesInTetrahedron.empty()) // this method should only be called when the array exists.
//  {
//#ifndef NDEBUG
//    sout << "Warning. [MeshTopology::getTrianglesInTetrahedronArray] TrianglesInTetrahedron array is empty." << endl;
//#endif
//
//    createQuadraticTrianglesInQuadraticTetrahedronArray();
//  }
//
//  return m_trianglesInTetrahedron;
//}

void QuadraticMeshTopology::createSurfaceNodes()
{
	m_SurfaceNodes.clear();

	//this->getN

    const SeqQuadraticTriangles& triangles = getQuadraticTriangles(); // do not use seqTriangles directly as it might not be up-to-date

	std::map<PointID,unsigned int> surfacePointMap;
	std::map<PointID,unsigned int>::iterator itt;
	QuadraticTriangle tr;
	sofa::core::topology::BaseMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron;
	int currentSurfaceNode;
	QuadraticMeshTopology::TetrahedraAroundTriangle currentTetrahedraAroundTriangle;

	std::cout<<"createSurfaceNodes, No of triangles:"<<triangles.size()<<"\n";

	for (unsigned int i = 0; i < triangles.size(); ++i)
	{
		tr = triangles [i];
		currentTetrahedraAroundTriangle = getQuadraticTetrahedraAroundQuadraticTriangle(i);

		if (currentTetrahedraAroundTriangle.size()==1)
		{
			for(unsigned int iterTriNode=0; iterTriNode<6;iterTriNode++)
			{
				currentSurfaceNode = tr[iterTriNode];
				itt=surfacePointMap.find(currentSurfaceNode);
				if (itt==surfacePointMap.end()) //if not already inside map, add to map and vector
				{
					m_SurfaceNodes.push_back(currentSurfaceNode);
					surfacePointMap[currentSurfaceNode]=m_SurfaceNodes.size();
				}
			}
		}
	}

}

//const sofa::helper::vector< PointID > &QuadraticMeshTopology::getSurfaceNodeArray()
//{
//
//}

const sofa::helper::vector< QuadraticMeshTopology::TrianglesAroundVertex > & QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundSurfaceNodeArray()
{
	if (!this->m_SurfaceQuadraticTrianglesAroundSurfaceNodes.size())
		createQuadraticTrianglesAroundSurfaceNodeArray();
		    return m_SurfaceQuadraticTrianglesAroundSurfaceNodes;
}

const sofa::helper::vector< QuadraticMeshTopology::TrianglesAroundVertex > & QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundNodeArray()
{
	if (!this->m_SurfaceQuadraticTrianglesAroundNodes.size())
		createQuadraticTrianglesAroundNodeArray();
		    return m_SurfaceQuadraticTrianglesAroundNodes;
}

const vector<QuadraticMeshTopology::PointID> & QuadraticMeshTopology::getSurfaceNodes()
{
	if (!this->m_SurfaceNodes.size())
	        createSurfaceNodes();
	//std::cout<<"getSurfaceNodesCalled, No of nodes:"<<this->m_SurfaceNodes.size()<<"\n";
	    return m_SurfaceNodes;
}

const  QuadraticMeshTopology::TrianglesAroundVertex QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundSurfaceNode(unsigned int i)
{
	if (!this->m_SurfaceQuadraticTrianglesAroundSurfaceNodes.size() || i > this->m_SurfaceQuadraticTrianglesAroundSurfaceNodes.size()-1)
				createQuadraticTrianglesAroundSurfaceNodeArray();
	    return this->m_SurfaceQuadraticTrianglesAroundSurfaceNodes[i];
}

const  QuadraticMeshTopology::TrianglesAroundVertex QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundNode(unsigned int i)
{
	if (!this->m_SurfaceQuadraticTrianglesAroundNodes.size() || i > this->m_SurfaceQuadraticTrianglesAroundNodes.size()-1)
				createQuadraticTrianglesAroundNodeArray();
	    return this->m_SurfaceQuadraticTrianglesAroundNodes[i];
}

const  QuadraticMeshTopology::TrianglesAroundVertex QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundQuadraticTriangle(unsigned int i)
{
	if (!m_SurfaceQuadraticTrianglesAroundQuadraticTriangle.size() || i > m_SurfaceQuadraticTrianglesAroundQuadraticTriangle.size()-1)
		createSurfaceQuadraticTrianglesAroundQuadraticTriangleArray();
	    return m_SurfaceQuadraticTrianglesAroundQuadraticTriangle[i];
}

const  QuadraticMeshTopology::TrianglesAroundVertex QuadraticMeshTopology::getSurfaceQuadraticTrianglesAroundQuadraticEdge(unsigned int edgeID)
{
	if (!m_SurfaceQuadraticTrianglesAroundQuadraticEdge.size() || edgeID > m_SurfaceQuadraticTrianglesAroundQuadraticEdge.size()-1)
		createSurfaceQuadraticTrianglesAroundQuadraticEdgeArray();
	    return m_SurfaceQuadraticTrianglesAroundQuadraticEdge[edgeID];
}


void QuadraticMeshTopology::createQuadraticTrianglesAroundSurfaceNodeArray()
{
	TrianglesAroundVertex currentTrianglesAroundVertex;

	sofa::core::topology::BaseMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron;
	int currentTriangle;
	QuadraticMeshTopology::TetrahedraAroundTriangle currentTetrahedraAroundTriangle;
	unsigned int currentSurfaceNode;

	vector<PointID> surfaceNodes = getSurfaceNodes();

	//unsigned int numberOfNodes = this->getNbPoints();

	m_SurfaceQuadraticTrianglesAroundSurfaceNodes.clear();
	m_SurfaceQuadraticTrianglesAroundSurfaceNodes.resize(surfaceNodes.size());


	//loop over all surface nodes
	for (unsigned int i = 0; i < surfaceNodes.size(); ++i)
	{
		//for each triangle around node: check if surface triangle
		currentSurfaceNode = surfaceNodes[i];
		currentTrianglesAroundVertex = getQuadraticTrianglesAroundVertex(currentSurfaceNode);
		//if so, add to array
		for(unsigned int iterTri=0; iterTri<currentTrianglesAroundVertex.size();iterTri++)
		{
			currentTriangle = currentTrianglesAroundVertex[iterTri];
			currentTetrahedraAroundTriangle = getQuadraticTetrahedraAroundQuadraticTriangle(currentTriangle);
			if(currentTetrahedraAroundTriangle.size()==1)
			{
				m_SurfaceQuadraticTrianglesAroundSurfaceNodes[i].push_back( currentTriangle );
			}
		}
	}

}

void QuadraticMeshTopology::createQuadraticTrianglesAroundNodeArray()
{
	TrianglesAroundVertex currentTrianglesAroundVertex;

	sofa::core::topology::BaseMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron;
	int currentTriangle;
	QuadraticMeshTopology::TetrahedraAroundTriangle currentTetrahedraAroundTriangle;
	unsigned int currentSurfaceNode;

	//vector<PointID> surfaceNodes = getSurfaceNodes();

	unsigned int numberOfNodes = this->getNbPoints();

	m_SurfaceQuadraticTrianglesAroundNodes.clear();
	m_SurfaceQuadraticTrianglesAroundNodes.resize(numberOfNodes);


	//loop over all surface nodes
	for (unsigned int i = 0; i < numberOfNodes; ++i)
	{
		//for each triangle around node: check if surface triangle
		currentSurfaceNode = i;
		currentTrianglesAroundVertex = getQuadraticTrianglesAroundVertex(currentSurfaceNode);
		//if so, add to array
		for(unsigned int iterTri=0; iterTri<currentTrianglesAroundVertex.size();iterTri++)
		{
			currentTriangle = currentTrianglesAroundVertex[iterTri];
			currentTetrahedraAroundTriangle = getQuadraticTetrahedraAroundQuadraticTriangle(currentTriangle);
			if(currentTetrahedraAroundTriangle.size()==1)
			{
				m_SurfaceQuadraticTrianglesAroundNodes[i].push_back( currentTriangle );
			}
		}
	}

}

void QuadraticMeshTopology::createSurfaceQuadraticTrianglesAroundQuadraticTriangleArray()
{
	//this->getVertexIndexInTetrahedron()
	vector<PointID> surfaceNodes = getSurfaceNodes();
	const SeqQuadraticTriangles& triangles = getQuadraticTriangles();

	m_SurfaceQuadraticTrianglesAroundQuadraticTriangle.clear();
	m_SurfaceQuadraticTrianglesAroundQuadraticTriangle.resize(triangles.size());
	TrianglesAroundVertex currentSurfaceTriangles;
	unsigned int currentNumberOfTriangles;
	unsigned int currentTriangleId;
	//int currentNode;



	for (unsigned int iter = 0; iter < triangles.size(); ++iter)
	{

		//create temporary map
		std::map<unsigned int,unsigned int> triangleMap;
		std::map<unsigned int,unsigned int>::iterator itt;
		unsigned int tr;
		QuadraticTriangle rootTriangle = triangles[iter];
		TrianglesAroundVertex currentTrianglesAroundVertex;

		//int triangleIndex;

//		if(this->getQuadraticTetrahedraAroundQuadraticTriangle(iter).size() == 1)
//		{
			//loop over all surface nodes
			for (unsigned int i = 0; i < 6; ++i)
			{
				//get all surface triangles around the node
				//currentNode = surfaceNodes[i];
				currentSurfaceTriangles = getSurfaceQuadraticTrianglesAroundNode(rootTriangle[i]);
				currentNumberOfTriangles = currentSurfaceTriangles.size();
//				if(iter == 3312)
//				{
//					std::cout<<"Current triangle node:"<<rootTriangle[i]<<", currentSurroundingTriangles: "<<currentSurfaceTriangles<<"\n";
//				}

				//for each triangle: look if already in set
				for(unsigned int iterTri=0; iterTri<currentNumberOfTriangles; ++iterTri)
				{
					tr = currentSurfaceTriangles[iterTri];
					itt=triangleMap.find(tr);
					if (itt==triangleMap.end())
					{
						//std::cout<<"hello\n";
						triangleMap[tr]=tr;
						currentTrianglesAroundVertex.push_back(tr);
					}

				}

			}
//		}

		m_SurfaceQuadraticTrianglesAroundQuadraticTriangle[iter] = currentTrianglesAroundVertex;
//		if(iter == 3312)
//		{
//			std::cout<<"FinalSurroundingTriangles: "<<currentTrianglesAroundVertex<<"\n";
//		}
		//std::cout<<"triangle no:"<<iter<<" , surrounding triangles:"<< currentTrianglesAroundVertex;
	}



}

void QuadraticMeshTopology::createSurfaceQuadraticTrianglesAroundQuadraticEdgeArray()
{
	//this->getVertexIndexInTetrahedron()
	//vector<PointID> surfaceNodes = getSurfaceNodes();
	//const SeqQuadraticEdges& edges = getQuadraticEdges();
	const SeqQuadraticEdges& edges = getQuadraticEdges();

	const SeqQuadraticTriangles&  triangles = getQuadraticTriangles();

	m_SurfaceQuadraticTrianglesAroundQuadraticEdge.clear();
	m_SurfaceQuadraticTrianglesAroundQuadraticEdge.resize(edges.size());

	std::cout<<"Number of edges "<<edges.size()<<", number of triangles:"<<triangles.size()<<"\n";

//	TrianglesAroundVertex currentSurfaceTriangles;
//	unsigned int currentNumberOfTriangles;
//	unsigned int currentTriangleId;
//	//int currentNode;
	//const EdgesInTriangle edges;

	//loop over all triangles
	for (unsigned int iter = 0; iter < triangles.size(); ++iter)
	{

		//if triangle is surface triangle:
		if(this->getQuadraticTetrahedraAroundQuadraticTriangle(iter).size() == 1)
		{
			const EdgesInTriangle& edgesInTriangle = this->getQuadraticEdgesInQuadraticTriangle(iter);
			//loop over all edges
			for (unsigned int i = 0; i < 3; ++i)
			{
				//add triangle to data structure
				//std::cout<<"edges in triangle[i]"<<edgesInTriangle[i]<<"n";
//				if(this->getQuadraticTetrahedraAroundQuadraticTriangle(iter).size() == 1)
				m_SurfaceQuadraticTrianglesAroundQuadraticEdge[ edgesInTriangle[i] ].push_back(iter);
//				if(edgesInTriangle[i] == 10264)
//					std::cout<<"Neighbouring triangle to edge 10264:"<<iter<<"\n";
			}
		}
	}




}

const QuadraticMeshTopology::TrianglesInTetrahedron& QuadraticMeshTopology::getQuadraticTrianglesInQuadraticTetrahedron(TetraID i)
{
    if (!m_QuadraticTrianglesInQuadraticTetrahedron.size() || i > m_QuadraticTrianglesInQuadraticTetrahedron.size()-1)
        createQuadraticTrianglesInQuadraticTetrahedronArray();
    return m_QuadraticTrianglesInQuadraticTetrahedron[i];
}

const QuadraticMeshTopology::EdgesInTriangle& QuadraticMeshTopology::getQuadraticEdgesInQuadraticTriangle(TriangleID i)
{
    if (!m_QuadraticEdgesInQuadraticTriangle.size() || i > m_QuadraticEdgesInQuadraticTriangle.size()-1)
        this->updateQuadraticEdges();
    return m_QuadraticEdgesInQuadraticTriangle[i];
}

const QuadraticMeshTopology::EdgesInTriangle& QuadraticMeshTopology::getQuadraticEdgesInQuadraticTriangleOrientation(TriangleID i)
{
    if (!m_QuadraticEdgesInQuadraticTriangleOrientation.size() || i > m_QuadraticEdgesInQuadraticTriangleOrientation.size()-1)
        this->updateQuadraticEdges();
    return m_QuadraticEdgesInQuadraticTriangleOrientation[i];
}


int QuadraticMeshTopology::getSurfaceQuadraticEdgeInInQuadraticTetrahedron(TetraID i, double r, double s, double t)
{
	//edge enumeration is first face: 0: Node 0-1, 1: Node 1-2, 2: 2-3, second face: 3: 0-3 4: 2-0 third face 5: 0-1
	int localEdgeNumber;
	int globalEdgeNumber;

	QuadraticMeshTopology::TrianglesInTetrahedron triangles = this->getQuadraticTrianglesInQuadraticTetrahedron(i);
	//QuadraticMeshTopology::EdgesInTriangle edgesInTriangle = getQuadraticEdgesInQuadraticTriangle(TriangleID i);

	const QuadraticMeshTopology::QuadTetra theTetra = this->getQuadTetrahedron(i);

	unsigned int node1, node2;
	std::vector<unsigned int> node1Vec;
	std::vector<unsigned int> node2Vec;
	bool edgeFound = false;

	if((r<=TOLERANCE) && (s+t)>=(1-TOLERANCE) )
	{
		localEdgeNumber = 0;
		node1Vec.push_back(3);
		node2Vec.push_back(1);
		edgeFound = true;
		//globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[0]) [0];
	}
	if((r<=TOLERANCE) && (t<=TOLERANCE) )
	{
		localEdgeNumber = 1;
		node1Vec.push_back(1);
		node2Vec.push_back(2);
		edgeFound = true;
		//		globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[0]) [1];
	}
	if((r<=TOLERANCE) && (s<=TOLERANCE) )
	{
		localEdgeNumber = 2;
		node1Vec.push_back(2);
		node2Vec.push_back(3);
		edgeFound = true;
		//		globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[0]) [2];
	}
	if((s<=TOLERANCE) && (r+t)>=(1-TOLERANCE) )
	{
		localEdgeNumber = 3;
		node1Vec.push_back(0);
		node2Vec.push_back(3);
		edgeFound = true;
		//		globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[1]) [0];
	}
	if((s<=TOLERANCE) && (t<=TOLERANCE) )
	{
		localEdgeNumber = 4;
		node1Vec.push_back(2);
		node2Vec.push_back(0);
		edgeFound = true;
		//		globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[1]) [2];
	}
	if((t<=TOLERANCE) && (r+s)>=(1-TOLERANCE) )
	{
		localEdgeNumber = 5;
		node1Vec.push_back(0);
		node2Vec.push_back(1);
		edgeFound = true;
		//		globalEdgeNumber = getQuadraticEdgesInQuadraticTriangle(triangles[2]) [1];
	}

	if(!edgeFound)
	{
		localEdgeNumber = -1;
		globalEdgeNumber = -1;

	}

	if(localEdgeNumber == -1)
	{
		std::cout<<"Error: Coordinates are not on edge, coords are"<< r << " "<< s <<" "<< t <<" " <<"\n";
		return -1;
	}

	globalEdgeNumber = -1;

	//std::cout<<"Edge pairs:" ;

	for(unsigned int edgeIter = 0; edgeIter < node1Vec.size(); edgeIter++)
	{
		QuadraticMeshTopology::EdgesAroundVertex edgeVec1 = this->getQuadraticEdgesAroundVertex(theTetra[node1Vec[edgeIter] ]);
		QuadraticMeshTopology::EdgesAroundVertex edgeVec2 = this->getQuadraticEdgesAroundVertex(theTetra[node2Vec[edgeIter] ]);

		//std::cout<<node1Vec[edgeIter] <<" " << node2Vec[edgeIter] <<"\n" ;

		// The destination vector must be large enough to contain the result.
		vector<TriangleID> out1(edgeVec1.size()+edgeVec2.size());
		vector<TriangleID>::iterator result1;
		result1 = std::set_intersection(edgeVec1.begin(),edgeVec1.end(),edgeVec2.begin(),edgeVec2.end(),out1.begin());
		out1.erase(result1,out1.end());



		if(out1.size() == 1)
		{
			const sofa::component::topology::QuadraticMeshTopology::TrianglesAroundVertex surfaceTriangles = this->getSurfaceQuadraticTrianglesAroundQuadraticEdge(out1[0]);
			if(surfaceTriangles.size() != 0)
				globalEdgeNumber = out1[0];
		}

		//std::cout<<"Current local edge number: "<<out1[0]<<"\n";

	}

	if(globalEdgeNumber == -1)
		std::cout<<"No surface edge found\n";

	return globalEdgeNumber;


}


int QuadraticMeshTopology::getQuadraticEdgeInQuadraticTriangle(TriangleID i, double r, double s, int & localEdgeNumber)
{
	QuadraticMeshTopology::EdgesInTriangle edges = this->getQuadraticEdgesInQuadraticTriangle(i);

	localEdgeNumber = -1;

	if(r<=TOLERANCE)
	{
		localEdgeNumber = 0;
	}
	if (s<=TOLERANCE)
	{
		localEdgeNumber = 1;
	}

	if ((r+s)>=(1-TOLERANCE))
	{
		localEdgeNumber = 2;
	}

	if(localEdgeNumber == -1)
	{
		return -1;
		std::cout<<"Error, point is not on edge";
	}

   return edges[localEdgeNumber];
}

int QuadraticMeshTopology::getSurfaceQuadraticTriangleInQuadraticTetrahedron(TetraID i, double r, double s, double t, int & localFaceNr)
{
	QuadraticMeshTopology::TrianglesInTetrahedron triangles = this->getQuadraticTrianglesInQuadraticTetrahedron(i);

	bool isSurface0 = 0;
	bool isSurface1 = 0;
	bool isSurface2 = 0;
	bool isSurface3 = 0;
	if((this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[0])).size() == 1)
	{

		isSurface0 = 1;
	}
	if((this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[1])).size() == 1)
	{

		isSurface1 = 1;
	}
	if((this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[2])).size() == 1)
	{

		isSurface2 = 1;
	}
	if((this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[3])).size() == 1)
	{

		isSurface3 = 1;
	}

//	std::cout<<"Number of TetrahedraAroundTriangle0:"<<(this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[0])).size()<<"\n";
//	std::cout<<"Number of TetrahedraAroundTriangle1:"<<(this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[1])).size()<<"\n";
//	std::cout<<"Number of TetrahedraAroundTriangle2:"<<(this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[2])).size()<<"\n";
//	std::cout<<"Number of TetrahedraAroundTriangle3:"<<(this->getQuadraticTetrahedraAroundQuadraticTriangle(triangles[3])).size()<<"\n";
	//bool faceFlag1 = 0;

	localFaceNr = -1;

	if(r<=TOLERANCE)
	{
		if(isSurface0)
			localFaceNr = 0;
	}
	if (s<=TOLERANCE)
	{
		if(isSurface1)
			localFaceNr = 1;
	}
	if ( t<=TOLERANCE)
	{
		if(isSurface3)
			localFaceNr = 3;
	}
	if ((r+s+t)>=(1-TOLERANCE))
	{
		if(isSurface2)
			localFaceNr = 2;
	}
//	else
//		std::cout<<"Error: Local coords are not on face";

	if(localFaceNr == -1)
		return -1;
		//std::cout<<"Error, local coords are not on surface, aborting with segfault";

   return triangles[localFaceNr];
}

void QuadraticMeshTopology::createQuadraticTrianglesInQuadraticTetrahedronArray ()
{
    //const SeqEdges& edges = getEdges(); // do not use seqEdges directly as it might not be up-to-date
    const SeqQuadraticTetrahedra& tetrahedra = getQuadTetrahedra(); // do not use seqTetrahedra directly as it might not be up-to-date
    m_QuadraticTrianglesInQuadraticTetrahedron.clear();
    m_QuadraticTrianglesInQuadraticTetrahedron.resize(tetrahedra.size());
    int v[6];

    for (unsigned int i = 0; i < tetrahedra.size(); ++i)
    {
        const QuadTetra &t=tetrahedra[i];
	// adding triangles in the triangle list of the ith tetrahedron  i
	for (unsigned int j=0;j<4;++j)
	{
		switch (j)
		{
		case 0:
			v[0] = t[1]; v[1] = t[5]; v[2] = t[2]; v[3] = t[9]; v[4] = t[3]; v[5] = t[8];
			break;
		case 1:
			v[0] = t[0]; v[1] = t[7]; v[2] = t[3]; v[3] = t[9]; v[4] = t[2]; v[5] = t[6];
			break;
		case 2:
			v[0] = t[0]; v[1] = t[4]; v[2] = t[1]; v[3] = t[8]; v[4] = t[3]; v[5] = t[7];
			break;
		case 3:
			v[0] = t[0]; v[1] = t[6]; v[2] = t[2]; v[3] = t[5]; v[4] = t[1]; v[5] = t[4];
			break;
		default:
			std::cout<<"Error, cant compute QuadTriangles\n";
			break;
		}

	    int triangleIndex=getQuadraticTriangleIndex(v);
	    //std::cout<<"i="<<i<<" ,j="<<j<<"\n";
	    assert(triangleIndex!= -1);
	    m_QuadraticTrianglesInQuadraticTetrahedron[i][j]=triangleIndex;
	}
    }
}


void QuadraticMeshTopology::createQuadraticTrianglesAroundVertexArray ()
{
    const SeqQuadraticTriangles& triangles = getQuadraticTriangles(); // do not use seqTriangles directly as it might not be up-to-date
    m_QuadraticTrianglesAroundVertex.clear();
    m_QuadraticTrianglesAroundVertex.resize( nbPoints );

    for (unsigned int i = 0; i < triangles.size(); ++i)
    {
	// adding triangle i in the triangle shell of all points
        for (unsigned int j=0;j<6;++j)
	    m_QuadraticTrianglesAroundVertex[ triangles[i][j]  ].push_back( i );
    }
}

void QuadraticMeshTopology::createQuadraticEdgesAroundVertexArray ()
{
    const SeqQuadraticEdges& edges = getQuadraticEdges(); // do not use seqEdges directly as it might not be up-to-date
    m_QuadraticEdgesAroundVertex.clear();
    m_QuadraticEdgesAroundVertex.resize( nbPoints );

    for (unsigned int i = 0; i < edges.size(); ++i)
    {
	// adding triangle i in the triangle shell of all points
        for (unsigned int j=0;j<3;++j)
	    m_QuadraticEdgesAroundVertex[ edges[i][j]  ].push_back( i );
    }
}

const QuadraticMeshTopology::TrianglesAroundVertex & QuadraticMeshTopology::getQuadraticTrianglesAroundVertex(PointID i)
{

	if (!m_QuadraticTrianglesAroundVertex.size() || i > m_QuadraticTrianglesAroundVertex.size()-1)
		createQuadraticTrianglesAroundVertexArray();
	return m_QuadraticTrianglesAroundVertex[i];

}

const QuadraticMeshTopology::EdgesAroundVertex & QuadraticMeshTopology::getQuadraticEdgesAroundVertex(PointID i)
{

	if (!m_QuadraticEdgesAroundVertex.size() || i > m_QuadraticEdgesAroundVertex.size()-1)
		createQuadraticEdgesAroundVertexArray();
	return m_QuadraticEdgesAroundVertex[i];

}


const vector< QuadraticMeshTopology::TrianglesAroundVertex >& QuadraticMeshTopology::getQuadraticTrianglesAroundVertexArray()
{
  if(m_QuadraticTrianglesAroundVertex.empty())	// this method should only be called when the array exists.
  {
#ifndef NDEBUG
    sout << "Warning. [MeshTopology::getTrianglesAroundVertexArray] TrianglesAroundVertex array is empty." << std::endl;
#endif

    createQuadraticTrianglesAroundVertexArray();
  }

  return m_QuadraticTrianglesAroundVertex;
}

const QuadraticMeshTopology::TetrahedraAroundTriangle& QuadraticMeshTopology::getQuadraticTetrahedraAroundQuadraticTriangle(TriangleID i)
{
    if (!m_QuadraticTetrahedraAroundTriangle.size() || i > m_QuadraticTetrahedraAroundTriangle.size()-1)
        createQuadraticTetrahedraAroundQuadraticTriangleArray();
    return m_QuadraticTetrahedraAroundTriangle[i];
}

void QuadraticMeshTopology::createQuadraticTetrahedraAroundQuadraticTriangleArray ()
{
    if (!m_QuadraticTrianglesInQuadraticTetrahedron.size())
        createQuadraticTrianglesInQuadraticTetrahedronArray();
    m_QuadraticTetrahedraAroundTriangle.resize( getNbQuadraticTriangles());
    unsigned int j;
    const vector< TrianglesInTetrahedron > &tta=m_QuadraticTrianglesInQuadraticTetrahedron;

    for (unsigned int i = 0; i < seqQuadTetrahedra.getValue().size(); ++i)
    {
        for (j=0;j<4;++j)
	    m_QuadraticTetrahedraAroundTriangle[ tta[i][j] ].push_back( i );
    }
}

int QuadraticMeshTopology::getQuadraticTriangleIndex(int* v)
{
    //const vector< TrianglesAroundVertex > &tvs=getTrianglesAroundVertexArray();

    const vector<TriangleID> &set1=getQuadraticTrianglesAroundVertex(v[0]);
    const vector<TriangleID> &set2=getQuadraticTrianglesAroundVertex(v[1]);
    const vector<TriangleID> &set3=getQuadraticTrianglesAroundVertex(v[2]);
    const vector<TriangleID> &set4=getQuadraticTrianglesAroundVertex(v[3]);
    const vector<TriangleID> &set5=getQuadraticTrianglesAroundVertex(v[4]);
    const vector<TriangleID> &set6=getQuadraticTrianglesAroundVertex(v[5]);


    // The destination vector must be large enough to contain the result.
    vector<TriangleID> out1(set1.size()+set2.size());
    vector<TriangleID>::iterator result1;
    result1 = std::set_intersection(set1.begin(),set1.end(),set2.begin(),set2.end(),out1.begin());
    out1.erase(result1,out1.end());

    vector<TriangleID> out2(set3.size()+out1.size());
    vector<TriangleID>::iterator result2;
    result2 = std::set_intersection(set3.begin(),set3.end(),out1.begin(),out1.end(),out2.begin());
    out2.erase(result2,out2.end());

//    std::cout<<"out3map size:"<<set4.size()+out2.size()<<"\n";

    vector<TriangleID> out3(set4.size()+out2.size());
    vector<TriangleID>::iterator result3;
    result3 = std::set_intersection(set4.begin(),set4.end(),out2.begin(),out2.end(),out3.begin());
    out3.erase(result3,out3.end());

//    std::cout<<"out4map size:"<<set5.size()+out3.size()<<"\n";

    vector<TriangleID> out4(set5.size()+out3.size());
    vector<TriangleID>::iterator result4;
    result4 = std::set_intersection(set5.begin(),set5.end(),out3.begin(),out3.end(),out4.begin());
    out4.erase(result4,out4.end());

//    std::cout<<"out5map size:"<<set6.size()+out4.size()<<"\n";
//    std::cout<<"set6 size:"<<set6.size()<<"\n";
//    std::cout<<"out4 size:"<<out4.size()<<"\n";

    vector<TriangleID> out5(set6.size()+out4.size());
    vector<TriangleID>::iterator result5;
    result5 = std::set_intersection(set6.begin(),set6.end(),out4.begin(),out4.end(),out5.begin());
    out5.erase(result5,out5.end());
    //std::cout<<"out5map size:"<<out5.size()<<"\n";
    assert(out5.size()==0 || out5.size()==1);

    if (out5.size()==1)
        return (int) (out5[0]);
    else
        return -1;
}

//int QuadraticMeshTopology::getQuadraticEdgeIndex(int* v)
//{
//    //const vector< TrianglesAroundVertex > &tvs=getTrianglesAroundVertexArray();
//
//    const vector<EdgeID> &set1=getQuadraticTrianglesAroundVertex(v[0]);
//    const vector<EdgeID> &set2=getQuadraticTrianglesAroundVertex(v[1]);
//    const vector<EdgeID> &set3=getQuadraticTrianglesAroundVertex(v[2]);
//    const vector<EdgeID> &set4=getQuadraticTrianglesAroundVertex(v[3]);
//    const vector<EdgeID> &set5=getQuadraticTrianglesAroundVertex(v[4]);
//    const vector<EdgeID> &set6=getQuadraticTrianglesAroundVertex(v[5]);
//
//
//    // The destination vector must be large enough to contain the result.
//    vector<TriangleID> out1(set1.size()+set2.size());
//    vector<TriangleID>::iterator result1;
//    result1 = std::set_intersection(set1.begin(),set1.end(),set2.begin(),set2.end(),out1.begin());
//    out1.erase(result1,out1.end());
//
//    vector<TriangleID> out2(set3.size()+out1.size());
//    vector<TriangleID>::iterator result2;
//    result2 = std::set_intersection(set3.begin(),set3.end(),out1.begin(),out1.end(),out2.begin());
//    out2.erase(result2,out2.end());
//
////    std::cout<<"out3map size:"<<set4.size()+out2.size()<<"\n";
//
//    vector<TriangleID> out3(set4.size()+out2.size());
//    vector<TriangleID>::iterator result3;
//    result3 = std::set_intersection(set4.begin(),set4.end(),out2.begin(),out2.end(),out3.begin());
//    out3.erase(result3,out3.end());
//
////    std::cout<<"out4map size:"<<set5.size()+out3.size()<<"\n";
//
//    vector<TriangleID> out4(set5.size()+out3.size());
//    vector<TriangleID>::iterator result4;
//    result4 = std::set_intersection(set5.begin(),set5.end(),out3.begin(),out3.end(),out4.begin());
//    out4.erase(result4,out4.end());
//
////    std::cout<<"out5map size:"<<set6.size()+out4.size()<<"\n";
////    std::cout<<"set6 size:"<<set6.size()<<"\n";
////    std::cout<<"out4 size:"<<out4.size()<<"\n";
//
//    vector<TriangleID> out5(set6.size()+out4.size());
//    vector<TriangleID>::iterator result5;
//    result5 = std::set_intersection(set6.begin(),set6.end(),out4.begin(),out4.end(),out5.begin());
//    out5.erase(result5,out5.end());
//    //std::cout<<"out5map size:"<<out5.size()<<"\n";
//    assert(out5.size()==0 || out5.size()==1);
//
//    if (out5.size()==1)
//        return (int) (out5[0]);
//    else
//        return -1;
//}

void QuadraticMeshTopology::updateHexahedra()
{
    if (!seqHexahedra.getValue().empty()) return; // hexahedra already defined
    // No 4D elements yet! ;)
}

void QuadraticMeshTopology::updateTetrahedra()
{
    if (!seqTetrahedra.getValue().empty()) return; // tetrahedra already defined
    // No 4D elements yet! ;)
}

/*
 * TODO updateQuadTetrahedra
 */
void QuadraticMeshTopology::updateQuadTetrahedra()
{
	if (!seqQuadTetrahedra.getValue().empty()) return; // Quadratic tetrahedra already defined
	// No 4D elements yet! ;)
}

const QuadraticMeshTopology::QuadsAroundVertex& QuadraticMeshTopology::getQuadsAroundVertex(PointID i)
{
    if (!m_tetrahedraAroundVertex.size() || i > m_tetrahedraAroundVertex.size()-1)
        createTetrahedraAroundVertexArray();
    return m_tetrahedraAroundVertex[i];
}

void QuadraticMeshTopology::createTetrahedraAroundVertexArray ()
{
	 m_tetrahedraAroundVertex.resize( nbPoints );
	 unsigned int j;

	 for (unsigned int i = 0; i < seqQuadTetrahedra.getValue().size(); ++i)
	 {
		 for (j=0;j<10;++j)
			 m_tetrahedraAroundVertex[ seqQuadTetrahedra.getValue()[i][j]  ].push_back( i );
	 }
}

const int QuadraticMeshTopology::getVertexIndexInTetrahedron(const QuadraticTetra &t, PointID vertexIndex)
{
	if (t[0]==vertexIndex)
		return 0;
	else if (t[1]==vertexIndex)
		return 1;
	else if (t[2]==vertexIndex)
		return 2;
	else if (t[3]==vertexIndex)
		return 3;
	else if (t[4]==vertexIndex)
		return 4;
	else if (t[5]==vertexIndex)
		return 5;
	else if (t[6]==vertexIndex)
		return 6;
	else if (t[7]==vertexIndex)
		return 7;
	else if (t[8]==vertexIndex)
		return 8;
	else if (t[9]==vertexIndex)
		return 9;
	else
		return -1;

}

const int QuadraticMeshTopology::getVertexIndexInQuadraticTriangle(const QuadraticTriangle &t, PointID vertexIndex)
{
	if (t[0]==vertexIndex)
		return 0;
	else if (t[1]==vertexIndex)
		return 1;
	else if (t[2]==vertexIndex)
		return 2;
	else if (t[3]==vertexIndex)
		return 3;
	else if (t[4]==vertexIndex)
		return 4;
	else if (t[5]==vertexIndex)
		return 5;
	else
		return -1;

}

 const int QuadraticMeshTopology::getVertexIndexInTetrahedron(double r, double s, double t)
{
	if(r > (1-TOLERANCE) )
		return 0;
	else if(s > (1-TOLERANCE) )
		return 1;
	else if((r < TOLERANCE)&&(s < TOLERANCE)&&(t < TOLERANCE) )
		return 2;
	else if(t > (1-TOLERANCE) )
		return 3;
	else return -1;

}

//void QuadraticMeshTopology::draw()
//{
//	if (_draw.getValue())
//	{
//		glDisable(GL_LIGHTING);
//
//
//		glColor3f(1,0,0);
//		for (int i=0; i<getNbHexahedra(); i++)
//		{
//			const Hexa& c = getHexahedron(i);
//			glBegin(GL_LINE_STRIP);
//			glVertex3d(getPX(c[0]), getPY(c[0]), getPZ(c[0]));
//			glVertex3d(getPX(c[1]), getPY(c[1]), getPZ(c[1]));
//			glVertex3d(getPX(c[2]), getPY(c[2]), getPZ(c[2]));
//			glVertex3d(getPX(c[3]), getPY(c[3]), getPZ(c[3]));
//			glVertex3d(getPX(c[0]), getPY(c[0]), getPZ(c[0]));
//			glEnd();
//			glBegin(GL_LINE_STRIP);
//			glVertex3d(getPX(c[4]), getPY(c[4]), getPZ(c[4]));
//			glVertex3d(getPX(c[5]), getPY(c[5]), getPZ(c[5]));
//			glVertex3d(getPX(c[6]), getPY(c[6]), getPZ(c[6]));
//			glVertex3d(getPX(c[7]), getPY(c[7]), getPZ(c[7]));
//			glVertex3d(getPX(c[4]), getPY(c[4]), getPZ(c[4]));
//			glEnd();
//			glBegin(GL_LINES);
//			glVertex3d(getPX(c[3]), getPY(c[3]), getPZ(c[3]));
//			glVertex3d(getPX(c[7]), getPY(c[7]), getPZ(c[7]));
//			//glEnd();
//			//glBegin(GL_LINES);
//			glVertex3d(getPX(c[2]), getPY(c[2]), getPZ(c[2]));
//			glVertex3d(getPX(c[6]), getPY(c[6]), getPZ(c[6]));
//			//glEnd();
//			//glBegin(GL_LINES);
//			glVertex3d(getPX(c[0]), getPY(c[0]), getPZ(c[0]));
//			glVertex3d(getPX(c[4]), getPY(c[4]), getPZ(c[4]));
//			//glEnd();
//			//glBegin(GL_LINES);
//			glVertex3d(getPX(c[1]), getPY(c[1]), getPZ(c[1]));
//			glVertex3d(getPX(c[5]), getPY(c[5]), getPZ(c[5]));
//			glEnd();
//		}
//	}
//}

} // namespace topology

} // namespace component

} // namespace sofa
