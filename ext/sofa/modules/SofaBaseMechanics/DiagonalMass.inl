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
#ifndef SOFA_COMPONENT_MASS_DIAGONALMASS_INL
#define SOFA_COMPONENT_MASS_DIAGONALMASS_INL

#include <SofaBaseMechanics/DiagonalMass.h>
#include <sofa/core/visual/VisualParams.h>
#include <sofa/helper/io/MassSpringLoader.h>
#include <sofa/helper/gl/template.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/DataTypeInfo.h>
#include <SofaBaseTopology/TopologyData.inl>
#include <SofaBaseTopology/RegularGridTopology.h>
#include <SofaBaseMechanics/AddMToMatrixFunctor.h>

#ifdef SOFA_SUPPORT_MOVING_FRAMES
#include <sofa/core/behavior/InertiaForce.h>
#endif

namespace sofa
{

namespace component
{

namespace mass
{

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyCreateFunction(unsigned int, MassType &m, const Point &, const sofa::helper::vector<unsigned int> &, const sofa::helper::vector<double> &)
{
    m=0;
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyEdgeCreation(const sofa::helper::vector< unsigned int >& edgeAdded,
        const sofa::helper::vector< Edge >& /*elems*/,
        const sofa::helper::vector< sofa::helper::vector< unsigned int > >& /*ancestors*/,
        const sofa::helper::vector< sofa::helper::vector< double > >& /*coefs*/)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_EDGESET)
    {

        helper::WriteAccessor<Data<MassVector> > masses(this->m_topologyData);
        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<edgeAdded.size(); ++i)
        {
            /// get the edge to be added
            const Edge &e=dm->_topology->getEdge(edgeAdded[i]);
            // compute its mass based on the mass density and the edge length
            if(dm->edgeGeo)
            {
                mass=(md*dm->edgeGeo->computeRestEdgeLength(edgeAdded[i]))/(typename DataTypes::Real)2.0;
            }
            // added mass on its two vertices
            masses[e[0]]+=mass;
            masses[e[1]]+=mass;
        }
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyEdgeDestruction(const sofa::helper::vector<unsigned int> & edgeRemoved)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_EDGESET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<edgeRemoved.size(); ++i)
        {
            /// get the edge to be added
            const Edge &e=dm->_topology->getEdge(edgeRemoved[i]);
            // compute its mass based on the mass density and the edge length
            if(dm->edgeGeo)
            {
                mass=(md*dm->edgeGeo->computeRestEdgeLength(edgeRemoved[i]))/(typename DataTypes::Real)2.0;
            }
            // removed mass on its two vertices
            masses[e[0]]-=mass;
            masses[e[1]]-=mass;
        }
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::EdgesAdded* e)
{
    const sofa::helper::vector< unsigned int >& edgeIndex = e->getIndexArray();
    const sofa::helper::vector< Edge >& edges = e->getArray();
    const sofa::helper::vector< sofa::helper::vector< unsigned int > >& ancestors = e->ancestorsList;
    const sofa::helper::vector< sofa::helper::vector< double > >& coeffs = e->coefs;

    applyEdgeCreation(edgeIndex, edges, ancestors, coeffs);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::EdgesRemoved* e)
{
    const sofa::helper::vector<unsigned int> & edgeRemoved = e->getArray();

    applyEdgeDestruction(edgeRemoved);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyTriangleCreation(const sofa::helper::vector< unsigned int >& triangleAdded,
        const sofa::helper::vector< Triangle >& /*elems*/,
        const sofa::helper::vector< sofa::helper::vector< unsigned int > >& /*ancestors*/,
        const sofa::helper::vector< sofa::helper::vector< double > >& /*coefs*/)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_TRIANGLESET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<triangleAdded.size(); ++i)
        {
            /// get the triangle to be added
            const Triangle &t=dm->_topology->getTriangle(triangleAdded[i]);
            // compute its mass based on the mass density and the triangle area
            if(dm->triangleGeo)
            {
                mass=(md*dm->triangleGeo->computeRestTriangleArea(triangleAdded[i]))/(typename DataTypes::Real)3.0;
            }
            // added mass on its three vertices
            masses[t[0]]+=mass;
            masses[t[1]]+=mass;
            masses[t[2]]+=mass;
        }
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyTriangleDestruction(const sofa::helper::vector<unsigned int> & triangleRemoved)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_TRIANGLESET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<triangleRemoved.size(); ++i)
        {
            /// get the triangle to be added
            const Triangle &t=dm->_topology->getTriangle(triangleRemoved[i]);
            // compute its mass based on the mass density and the triangle area
            if(dm->triangleGeo)
            {
                mass=(md*dm->triangleGeo->computeRestTriangleArea(triangleRemoved[i]))/(typename DataTypes::Real)3.0;
            }
            // removed  mass on its three vertices
            masses[t[0]]-=mass;
            masses[t[1]]-=mass;
            masses[t[2]]-=mass;
            // Commented to prevent from printing in case of triangle removal
            //serr<< "mass vertex " << t[0]<< " = " << masses[t[0]]<<sendl;
            //serr<< "mass vertex " << t[1]<< " = " << masses[t[1]]<<sendl;
            //serr<< "mass vertex " << t[2]<< " = " << masses[t[2]]<<sendl;
        }
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::TrianglesAdded* e)
{
    const sofa::helper::vector< unsigned int >& triangleAdded = e->getIndexArray();
    const sofa::helper::vector< Triangle >& elems = e->getElementArray();
    const sofa::helper::vector< sofa::helper::vector< unsigned int > >& ancestors = e->ancestorsList;
    const sofa::helper::vector< sofa::helper::vector< double > >& coefs = e->coefs;

    applyTriangleCreation(triangleAdded,elems,ancestors,coefs);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::TrianglesRemoved* e)
{
    const sofa::helper::vector<unsigned int> & triangleRemoved = e->getArray();

    applyTriangleDestruction(triangleRemoved);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyTetrahedronCreation(const sofa::helper::vector< unsigned int >& tetrahedronAdded,
        const sofa::helper::vector< Tetrahedron >& /*elems*/,
        const sofa::helper::vector< sofa::helper::vector< unsigned int > >& /*ancestors*/,
        const sofa::helper::vector< sofa::helper::vector< double > >& /*coefs*/)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_TETRAHEDRONSET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<tetrahedronAdded.size(); ++i)
        {
            /// get the tetrahedron to be added
            const Tetrahedron &t=dm->_topology->getTetrahedron(tetrahedronAdded[i]);
            // compute its mass based on the mass density and the tetrahedron volume
            if(dm->tetraGeo)
            {
                mass=(md*dm->tetraGeo->computeRestTetrahedronVolume(tetrahedronAdded[i]))/(typename DataTypes::Real)4.0;
            }
            // added  mass on its four vertices
            masses[t[0]]+=mass;
            masses[t[1]]+=mass;
            masses[t[2]]+=mass;
            masses[t[3]]+=mass;

        }

    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyTetrahedronDestruction(const sofa::helper::vector<unsigned int> & tetrahedronRemoved)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_TETRAHEDRONSET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<tetrahedronRemoved.size(); ++i)
        {
            /// get the tetrahedron to be added
            const Tetrahedron &t=dm->_topology->getTetrahedron(tetrahedronRemoved[i]);
            if(dm->tetraGeo)
            {
                // compute its mass based on the mass density and the tetrahedron volume
                mass=(md*dm->tetraGeo->computeRestTetrahedronVolume(tetrahedronRemoved[i]))/(typename DataTypes::Real)4.0;
            }
            // removed  mass on its four vertices
            masses[t[0]]-=mass;
            masses[t[1]]-=mass;
            masses[t[2]]-=mass;
            masses[t[3]]-=mass;
        }

    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::TetrahedraAdded* e)
{
    const sofa::helper::vector< unsigned int >& tetrahedronAdded = e->getIndexArray();
    const sofa::helper::vector< Tetrahedron >& elems = e->getElementArray();
    const sofa::helper::vector< sofa::helper::vector< unsigned int > >& ancestors = e->ancestorsList;
    const sofa::helper::vector< sofa::helper::vector< double > >& coefs = e->coefs;

    applyTetrahedronCreation(tetrahedronAdded, elems, ancestors, coefs);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::TetrahedraRemoved* e)
{
    const sofa::helper::vector<unsigned int> & tetrahedronRemoved = e->getArray();

    applyTetrahedronDestruction(tetrahedronRemoved);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyHexahedronCreation(const sofa::helper::vector< unsigned int >& hexahedronAdded,
        const sofa::helper::vector< Hexahedron >& /*elems*/,
        const sofa::helper::vector< sofa::helper::vector< unsigned int > >& /*ancestors*/,
        const sofa::helper::vector< sofa::helper::vector< double > >& /*coefs*/)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_HEXAHEDRONSET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<hexahedronAdded.size(); ++i)
        {
            /// get the tetrahedron to be added
            const Hexahedron &t=dm->_topology->getHexahedron(hexahedronAdded[i]);
            // compute its mass based on the mass density and the tetrahedron volume
            if(dm->hexaGeo)
            {
                mass=(md*dm->hexaGeo->computeRestHexahedronVolume(hexahedronAdded[i]))/(typename DataTypes::Real)8.0;
            }
            // added  mass on its four vertices
            for (unsigned int j=0; j<8; ++j)
                masses[t[j]]+=mass;
        }

    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::applyHexahedronDestruction(const sofa::helper::vector<unsigned int> & hexahedronRemoved)
{
    if (dm->getMassTopologyType()==DiagonalMass<DataTypes, MassType>::TOPOLOGY_HEXAHEDRONSET)
    {
        helper::WriteAccessor<Data<MassVector> > masses(*this->m_topologyData);

        typename DataTypes::Real md=dm->getMassDensity();
        typename DataTypes::Real mass=(typename DataTypes::Real) 0;
        unsigned int i;

        for (i=0; i<hexahedronRemoved.size(); ++i)
        {
            /// get the tetrahedron to be added
            const Hexahedron &t=dm->_topology->getHexahedron(hexahedronRemoved[i]);
            if(dm->hexaGeo)
            {
                // compute its mass based on the mass density and the tetrahedron volume
                mass=(md*dm->hexaGeo->computeRestHexahedronVolume(hexahedronRemoved[i]))/(typename DataTypes::Real)8.0;
            }
            // removed  mass on its four vertices
            for (unsigned int j=0; j<8; ++j)
                masses[t[j]]-=mass;
        }

    }
}
template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::HexahedraAdded* e)
{
    const sofa::helper::vector< unsigned int >& hexahedronAdded = e->getIndexArray();
    const sofa::helper::vector< Hexahedron >& elems = e->getElementArray();
    const sofa::helper::vector< sofa::helper::vector< unsigned int > >& ancestors = e->ancestorsList;
    const sofa::helper::vector< sofa::helper::vector< double > >& coefs = e->coefs;

    applyHexahedronCreation(hexahedronAdded,elems,ancestors,coefs);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes,MassType>::DMassPointHandler::ApplyTopologyChange(const core::topology::HexahedraRemoved* e)
{
    const sofa::helper::vector<unsigned int> & hexahedronRemoved = e->getArray();

    applyHexahedronDestruction(hexahedronRemoved);
}

template <class DataTypes, class MassType>
DiagonalMass<DataTypes, MassType>::DiagonalMass()
    : f_mass( initData(&f_mass, "mass", "values of the particles masses") )
    , pointHandler(NULL)
    , m_massDensity( initData(&m_massDensity, (Real)1.0,"massDensity", "mass density that allows to compute the  particles masses from a mesh topology and geometry.\nOnly used if > 0") )
    , m_computeMassOnRest(initData(&m_computeMassOnRest, false, "computeMassOnRest", "if true, the mass of every element is computed based on the rest position rather than the position"))
    , m_totalMass(initData(&m_totalMass, "totalMass", "Total mass of the object (read only)"))
    , showCenterOfGravity( initData(&showCenterOfGravity, false, "showGravityCenter", "display the center of gravity of the system" ) )
    , showAxisSize( initData(&showAxisSize, 1.0f, "showAxisSizeFactor", "factor length of the axis displayed (only used for rigids)" ) )
    , fileMass( initData(&fileMass,  "fileMass", "File to specify the mass" ) )
    , topologyType(TOPOLOGY_UNKNOWN)
{
    this->addAlias(&fileMass,"filename");

    m_totalMass.setReadOnly(true);
}




template <class DataTypes, class MassType>
DiagonalMass<DataTypes, MassType>::~DiagonalMass()
{
    if (pointHandler)
        delete pointHandler;
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::clear()
{
    MassVector& masses = *f_mass.beginEdit();
    masses.clear();
    f_mass.endEdit();
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addMass(const MassType& m)
{
    MassVector& masses = *f_mass.beginEdit();
    masses.push_back(m);
    f_mass.endEdit();
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::resize(int vsize)
{
    MassVector& masses = *f_mass.beginEdit();
    masses.resize(vsize);
    f_mass.endEdit();
}

// -- Mass interface
template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addMDx(const core::MechanicalParams* /*mparams*/, DataVecDeriv& res, const DataVecDeriv& dx, SReal factor)
{
    const MassVector &masses= f_mass.getValue();
    //std::cout << "DIAGONALMASS: dx size = " << dx.size() << " res size = " << res.size() << " masses size = " << masses.size() << std::endl;
    helper::WriteAccessor< DataVecDeriv > _res = res;
    helper::ReadAccessor< DataVecDeriv > _dx = dx;

    size_t n = masses.size();
    if (_dx.size() < n) n = _dx.size();
    if (_res.size() < n) n = _res.size();
    if (factor == 1.0)
    {
        for (size_t i=0; i<n; i++)
        {
            _res[i] += _dx[i] * masses[i];
        }
    }
    else
    {
        for (size_t i=0; i<n; i++)
        {
            _res[i] += (_dx[i] * masses[i]) * (Real)factor;
        }
    }
}



template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::accFromF(const core::MechanicalParams* /*mparams*/, DataVecDeriv& a, const DataVecDeriv& f)
{

    const MassVector &masses= f_mass.getValue();
    helper::WriteOnlyAccessor< DataVecDeriv > _a = a;
    const VecDeriv& _f = f.getValue();

    for (unsigned int i=0; i<masses.size(); i++)
    {
        _a[i] = _f[i] / masses[i];
    }
}

template <class DataTypes, class MassType>
SReal DiagonalMass<DataTypes, MassType>::getKineticEnergy( const core::MechanicalParams* /*mparams*/, const DataVecDeriv& v ) const
{

    const MassVector &masses= f_mass.getValue();
    helper::ReadAccessor< DataVecDeriv > _v = v;
    SReal e = 0.0;
    for (unsigned int i=0; i<masses.size(); i++)
    {
        e += _v[i]*masses[i]*_v[i]; // v[i]*v[i]*masses[i] would be more efficient but less generic
    }
    return e/2;
}

template <class DataTypes, class MassType>
SReal DiagonalMass<DataTypes, MassType>::getPotentialEnergy( const core::MechanicalParams* /*mparams*/, const DataVecCoord& x ) const
{

    const MassVector &masses= f_mass.getValue();
    helper::ReadAccessor< DataVecCoord > _x = x;
    SReal e = 0;
    // gravity
    defaulttype::Vec3d g ( this->getContext()->getGravity() );
    Deriv theGravity;
    DataTypes::set ( theGravity, g[0], g[1], g[2]);
    for (unsigned int i=0; i<masses.size(); i++)
    {
        e -= theGravity*masses[i]*_x[i];
    }
    return e;
}

// does nothing by default, need to be specialized in .cpp
template <class DataTypes, class MassType>
defaulttype::Vector6
DiagonalMass<DataTypes, MassType>::getMomentum ( const core::MechanicalParams*, const DataVecCoord& /*vx*/, const DataVecDeriv& /*vv*/  ) const
{
    return defaulttype::Vector6();
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addMToMatrix(const core::MechanicalParams *mparams, const sofa::core::behavior::MultiMatrixAccessor* matrix)
{
    const MassVector &masses= f_mass.getValue();
    const int N = defaulttype::DataTypeInfo<Deriv>::size();
    AddMToMatrixFunctor<Deriv,MassType> calc;
    sofa::core::behavior::MultiMatrixAccessor::MatrixRef r = matrix->getMatrix(this->mstate);
    Real mFactor = (Real)mparams->mFactorIncludingRayleighDamping(this->rayleighMass.getValue());
    for (unsigned int i=0; i<masses.size(); i++)
        calc(r.matrix, masses[i], r.offset + N*i, mFactor);
}


template <class DataTypes, class MassType>
SReal DiagonalMass<DataTypes, MassType>::getElementMass(unsigned int index) const
{
    return (SReal)(f_mass.getValue()[index]);
}


//TODO: special case for Rigid Mass
template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::getElementMass(unsigned int index, defaulttype::BaseMatrix *m) const
{
    static const defaulttype::BaseMatrix::Index dimension = (defaulttype::BaseMatrix::Index) defaulttype::DataTypeInfo<Deriv>::size();
    if (m->rowSize() != dimension || m->colSize() != dimension) m->resize(dimension,dimension);

    m->clear();
    AddMToMatrixFunctor<Deriv,MassType>()(m, f_mass.getValue()[index], 0, 1);
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::reinit()
{
    if (_topology && (m_massDensity.getValue() > 0 || f_mass.getValue().size() == 0))
    {
        if (_topology->getNbTetrahedra()>0 && tetraGeo)
        {

            MassVector& masses = *f_mass.beginEdit();
            topologyType=TOPOLOGY_TETRAHEDRONSET;

            // resize array
            clear();
            masses.resize(this->mstate->getSize());

            for(unsigned int i=0; i<masses.size(); ++i)
                masses[i]=(Real)0;

            Real md=m_massDensity.getValue();
            Real mass=(Real)0;
            Real total_mass=(Real)0;

            for (int i=0; i<_topology->getNbTetrahedra(); ++i)
            {

                const Tetrahedron &t=_topology->getTetrahedron(i);
                if(tetraGeo)
                {
                    if (m_computeMassOnRest.getValue())
                        mass=(md*tetraGeo->computeRestTetrahedronVolume(i))/(Real)4.0;
                    else
                        mass=(md*tetraGeo->computeTetrahedronVolume(i))/(Real)4.0;
                }
                for (unsigned int j = 0 ; j < t.size(); j++)
                {
                    masses[t[j]] += mass;
                    total_mass += mass;
                }
            }
            m_totalMass.setValue(total_mass);
            f_mass.endEdit();
        }
        else if (_topology->getNbTriangles()>0 && triangleGeo)
        {
            MassVector& masses = *f_mass.beginEdit();
            topologyType=TOPOLOGY_TRIANGLESET;

            // resize array
            clear();
            masses.resize(this->mstate->getSize());

            for(unsigned int i=0; i<masses.size(); ++i)
                masses[i]=(Real)0;

            Real md=m_massDensity.getValue();
            Real mass=(Real)0;
            Real total_mass=(Real)0;

            for (int i=0; i<_topology->getNbTriangles(); ++i)
            {
                const Triangle &t=_topology->getTriangle(i);
                if(triangleGeo)
                {
                    if (m_computeMassOnRest.getValue())
                        mass=(md*triangleGeo->computeRestTriangleArea(i))/(Real)3.0;
                    else
                        mass=(md*triangleGeo->computeTriangleArea(i))/(Real)3.0;
                }
                for (unsigned int j = 0 ; j < t.size(); j++)
                {
                    masses[t[j]] += mass;
                    total_mass += mass;
                }
            }
            m_totalMass.setValue(total_mass);
            f_mass.endEdit();
        }

        else if (_topology->getNbHexahedra()>0)
        {

            MassVector& masses = *f_mass.beginEdit();
            topologyType=TOPOLOGY_HEXAHEDRONSET;

            masses.resize(this->mstate->getSize());
            for(unsigned int i=0; i<masses.size(); ++i)
              masses[i]=(Real)0;

            Real md=m_massDensity.getValue();
            Real mass=(Real)0;
            Real total_mass=(Real)0;

            for (int i=0; i<_topology->getNbHexahedra(); ++i)
            {
                const Hexahedron &h=_topology->getHexahedron(i);
                if (hexaGeo)
                {
                    if (m_computeMassOnRest.getValue())
                        mass=(md*hexaGeo->computeRestHexahedronVolume(i))/(Real)8.0;
                    else
                        mass=(md*hexaGeo->computeHexahedronVolume(i))/(Real)8.0;

                    for (unsigned int j = 0 ; j < h.size(); j++)
                    {
                        masses[h[j]] += mass;
                        total_mass += mass;
                    }
                }
            }

            m_totalMass.setValue(total_mass);
            f_mass.endEdit();

        }
        else if (_topology->getNbQuads()>0 && quadGeo) {
            MassVector& masses = *f_mass.beginEdit();
            topologyType=TOPOLOGY_QUADSET;

            // resize array
            clear();
            masses.resize(this->mstate->getSize());

            for(unsigned int i=0; i<masses.size(); ++i)
                masses[i]=(Real)0;

            Real md=m_massDensity.getValue();
            Real mass=(Real)0;
            Real total_mass=(Real)0;

            for (int i=0; i<_topology->getNbQuads(); ++i)
            {
                const Quad &t=_topology->getQuad(i);
                if(quadGeo)
                {
                    if (m_computeMassOnRest.getValue())
                        mass=(md*quadGeo->computeRestQuadArea(i))/(Real)4.0;
                    else
                        mass=(md*quadGeo->computeQuadArea(i))/(Real)4.0;
                }
                for (unsigned int j = 0 ; j < t.size(); j++)
                {
                    masses[t[j]] += mass;
                    total_mass += mass;
                }
            }
            m_totalMass.setValue(total_mass);
            f_mass.endEdit();
        }
        else if (_topology->getNbEdges()>0 && edgeGeo)
        {

            MassVector& masses = *f_mass.beginEdit();
            topologyType=TOPOLOGY_EDGESET;

            // resize array
            clear();
            masses.resize(this->mstate->getSize());

            for(unsigned int i=0; i<masses.size(); ++i)
                masses[i]=(Real)0;

            Real md=m_massDensity.getValue();
            Real mass=(Real)0;
            Real total_mass=(Real)0;

            for (int i=0; i<_topology->getNbEdges(); ++i)
            {
                const Edge &e=_topology->getEdge(i);
                if(edgeGeo)
                {
                    if (m_computeMassOnRest.getValue())
                        mass=(md*edgeGeo->computeRestEdgeLength(i))/(Real)2.0;
                    else
                        mass=(md*edgeGeo->computeEdgeLength(i))/(Real)2.0;
                }
                for (unsigned int j = 0 ; j < e.size(); j++)
                {
                    masses[e[j]] += mass;
                    total_mass += mass;
                }
            }
            m_totalMass.setValue(total_mass);
            f_mass.endEdit();
        }
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::initTopologyHandlers()
{
    // add the functions to handle topology changes.
    pointHandler = new DMassPointHandler(this, &f_mass);
    f_mass.createTopologicalEngine(_topology, pointHandler);
    if (edgeGeo)
        f_mass.linkToEdgeDataArray();
    if (triangleGeo)
        f_mass.linkToTriangleDataArray();
    if (quadGeo)
        f_mass.linkToQuadDataArray();
    if (tetraGeo)
        f_mass.linkToTetrahedronDataArray();
    if (hexaGeo)
        f_mass.linkToHexahedronDataArray();
    f_mass.registerTopologicalData();
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::init()
{
    if (!fileMass.getValue().empty())
        load(fileMass.getFullPath().c_str());

    _topology = this->getContext()->getMeshTopology();

    this->getContext()->get(edgeGeo);
    this->getContext()->get(triangleGeo);
    this->getContext()->get(quadGeo);
    this->getContext()->get(tetraGeo);
    this->getContext()->get(hexaGeo);

    if (_topology)
    {
        if (_topology->getNbTetrahedra() > 0 && !tetraGeo)
            serr << "Tetrahedron topology but no geometry algorithms found. Add the component TetrahedronSetGeometryAlgorithms." << sendl;
        else if (_topology->getNbTriangles() > 0 && !triangleGeo)
            serr << "Triangle topology but no geometry algorithms found. Add the component TriangleSetGeometryAlgorithms." << sendl;
        else if (_topology->getNbHexahedra() > 0 && !hexaGeo)
            serr << "Hexahedron topology but no geometry algorithms found. Add the component HexahedronSetGeometryAlgorithms." << sendl;
       else if (_topology->getNbQuads() > 0 && !quadGeo)
           serr << "Quad topology but no geometry algorithms found. Add the component QuadSetGeometryAlgorithms." << sendl;
        else if (_topology->getNbEdges() > 0 && !edgeGeo)
            serr << "Edge topology but no geometry algorithms found. Add the component EdgeSetGeometryAlgorithms." << sendl;
    }

    Inherited::init();
    initTopologyHandlers();

    if (this->mstate && f_mass.getValue().size() > 0 && f_mass.getValue().size() < (unsigned)this->mstate->getSize())
    {
        MassVector &masses= *f_mass.beginEdit();
        size_t i = masses.size()-1;
        size_t n = (size_t)this->mstate->getSize();
        masses.reserve(n);
        while (masses.size() < n)
            masses.push_back(masses[i]);
        f_mass.endEdit();
    }

    if ((f_mass.getValue().size()==0) && (_topology!=0))
    {
        reinit();
    }
}

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addGravityToV(const core::MechanicalParams* mparams, DataVecDeriv& d_v)
{
    if(mparams)
    {
        VecDeriv& v = *d_v.beginEdit();
        // gravity
        sofa::defaulttype::Vec3d g ( this->getContext()->getGravity() );
        Deriv theGravity;
        DataTypes::set ( theGravity, g[0], g[1], g[2]);
        Deriv hg = theGravity * (typename DataTypes::Real)mparams->dt();

        for (unsigned int i=0; i<v.size(); i++)
        {
            v[i] += hg;
        }
        d_v.endEdit();
    }
}


#ifdef SOFA_SUPPORT_MOVING_FRAMES
template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addForce(const core::MechanicalParams* /*mparams*/, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v)
{

    const MassVector &masses= f_mass.getValue();
    helper::WriteAccessor< DataVecDeriv > _f = f;
    helper::ReadAccessor< DataVecCoord > _x = x;
    helper::ReadAccessor< DataVecDeriv > _v = v;

    // gravity
    Vec3d g ( this->getContext()->getGravity() );
    Deriv theGravity;
    DataTypes::set ( theGravity, g[0], g[1], g[2]);

    // velocity-based stuff
    core::objectmodel::BaseContext::SpatialVector vframe = this->getContext()->getVelocityInWorld();
    core::objectmodel::BaseContext::Vec3 aframe = this->getContext()->getVelocityBasedLinearAccelerationInWorld() ;

    // project back to local frame
    vframe = this->getContext()->getPositionInWorld() / vframe;
    aframe = this->getContext()->getPositionInWorld().backProjectVector( aframe );

    // add weight and inertia force
    if(this->m_separateGravity.getValue()) for (unsigned int i=0; i<masses.size(); i++)
        {
            _f[i] += core::behavior::inertiaForce(vframe,aframe,masses[i],_x[i],_v[i]);
        }
    else for (unsigned int i=0; i<masses.size(); i++)
        {
            _f[i] += theGravity*masses[i] + core::behavior::inertiaForce(vframe,aframe,masses[i],_x[i],_v[i]);
        }
}
#else
template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::addForce(const core::MechanicalParams* /*mparams*/, DataVecDeriv& f, const DataVecCoord& , const DataVecDeriv& )
{
    //if gravity was added separately (in solver's "solve" method), then nothing to do here
    if(this->m_separateGravity.getValue())
        return;

    const MassVector &masses= f_mass.getValue();
    helper::WriteAccessor< DataVecDeriv > _f = f;

    // gravity
    sofa::defaulttype::Vec3d g ( this->getContext()->getGravity() );
    Deriv theGravity;
    DataTypes::set ( theGravity, g[0], g[1], g[2]);


    // add weight and inertia force
    for (unsigned int i=0; i<masses.size(); i++)
    {
        _f[i] += theGravity*masses[i];
    }
}
#endif

template <class DataTypes, class MassType>
void DiagonalMass<DataTypes, MassType>::draw(const core::visual::VisualParams* vparams)
{
#ifndef SOFA_NO_OPENGL
    if (!vparams->displayFlags().getShowBehaviorModels()) return;
    const MassVector &masses= f_mass.getValue();
    if (masses.empty()) return;

    const VecCoord& x = this->mstate->read(core::ConstVecCoordId::position())->getValue();
    Coord gravityCenter;
    Real totalMass=0.0;

    std::vector<  sofa::defaulttype::Vector3 > points;
//    std::vector<  sofa::defaulttype::Vec<2,int> > indices;

    for (unsigned int i=0; i<x.size(); i++)
    {
        sofa::defaulttype::Vector3 p;
        p = DataTypes::getCPos(x[i]);

        points.push_back(p);
        gravityCenter += x[i]*masses[i];
        totalMass += masses[i];
    }

//    vparams->drawTool()->drawPoints(points, 2, sofa::defaulttype::Vec<4,float>(1,1,1,1));

    if(showCenterOfGravity.getValue())
    {
        glBegin (GL_LINES);
        glColor4f (1,1,0,1);
        glPointSize(5);
        gravityCenter /= totalMass;
        for(unsigned int i=0 ; i<Coord::spatial_dimensions ; i++)
        {
            Coord v;
            v[i] = showAxisSize.getValue();
            helper::gl::glVertexT(gravityCenter-v);
            helper::gl::glVertexT(gravityCenter+v);
        }
        glEnd();
    }
#endif /* SOFA_NO_OPENGL */
}

template <class DataTypes, class MassType>
class DiagonalMass<DataTypes, MassType>::Loader : public helper::io::MassSpringLoader
{
public:
    DiagonalMass<DataTypes, MassType>* dest;
    Loader(DiagonalMass<DataTypes, MassType>* dest) : dest(dest) {}
    virtual void addMass(SReal /*px*/, SReal /*py*/, SReal /*pz*/, SReal /*vx*/, SReal /*vy*/, SReal /*vz*/, SReal mass, SReal /*elastic*/, bool /*fixed*/, bool /*surface*/)
    {
        dest->addMass(MassType((Real)mass));
    }
};

template <class DataTypes, class MassType>
bool DiagonalMass<DataTypes, MassType>::load(const char *filename)
{
    clear();
    if (filename!=NULL && filename[0]!='\0')
    {
        Loader loader(this);
        return loader.load(filename);
    }
    else return false;
}

} // namespace mass

} // namespace component

} // namespace sofa

#endif
