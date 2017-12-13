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
#ifndef SOFA_COMPONENT_MAPPING_ProjectionToLineMapping_INL
#define SOFA_COMPONENT_MAPPING_ProjectionToLineMapping_INL

#include "ProjectionToLineMapping.h"
#include <sofa/core/visual/VisualParams.h>
#include <iostream>

namespace sofa
{

namespace component
{

namespace mapping
{

template <class TIn, class TOut>
ProjectionToTargetLineMapping<TIn, TOut>::ProjectionToTargetLineMapping()
    : Inherit()
    , f_indices(initData(&f_indices, "indices", "Indices of the parent points"))
    , f_origins(initData(&f_origins, "origins", "Origins of the lines on which the points are projected"))
    , f_directions(initData(&f_directions, "directions", "Directions of the lines on which the points are projected"))
    , d_drawScale(initData(&d_drawScale, SReal(10), "drawScale", "Draw scale"))
    , d_drawColor(initData(&d_drawColor, defaulttype::Vec4f(0,1,0,1), "drawColor", "Draw color"))
{
}

template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::init()
{
    assert( f_indices.getValue().size()==f_origins.getValue().size()) ;
    assert( f_indices.getValue().size()==f_directions.getValue().size()) ;

    this->getToModel()->resize( f_indices.getValue().size() );

    baseMatrices.resize( 1 );
    baseMatrices[0] = &jacobian;


    // ensuring direction are normalized
    helper::WriteAccessor< Data<OutVecCoord> > directions(f_directions);
    for( size_t i=0 ; i<directions.size() ; ++i )
        directions[i].normalize( OutCoord(1,0,0) ); // failsafe for null norms

    this->Inherit::init();
}


template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::apply(const core::MechanicalParams * /*mparams*/ , Data<OutVecCoord>& dOut, const Data<InVecCoord>& dIn)
{
    helper::WriteAccessor< Data<OutVecCoord> >  out = dOut;
    helper::ReadAccessor< Data<InVecCoord> >  in = dIn;
    helper::ReadAccessor< Data<vector<unsigned> > > indices(f_indices);
    helper::ReadAccessor< Data<OutVecCoord> > origins(f_origins);
    helper::ReadAccessor< Data<OutVecCoord> > directions(f_directions);

    jacobian.resizeBlocks(out.size(),in.size());

    for(unsigned i=0; i<indices.size(); i++ )
    {
        const InCoord& p = in[indices[i]];
        const OutCoord& o = origins[i];
        const OutCoord& n = directions[i];

        out[i] = o + n * ( n * ( TIn::getCPos(p) - o ) ); // projection on the line

        for(unsigned j=0; j<Nout; j++)
        {
            for(unsigned k=0; k<Nout; k++ )
            {
                jacobian.insertBack( i*Nout+j, indices[i]*Nin+k, n[j]*n[k] );
            }
        }
    }
    jacobian.compress();
}



template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::applyJ(const core::MechanicalParams * /*mparams*/ , Data<OutVecDeriv>& dOut, const Data<InVecDeriv>& dIn)
{
    jacobian.mult(dOut,dIn);
}

template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::applyJT(const core::MechanicalParams * /*mparams*/ , Data<InVecDeriv>& dIn, const Data<OutVecDeriv>& dOut)
{
    jacobian.addMultTranspose(dIn,dOut);
}

template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::applyJT(const core::ConstraintParams*, Data<InMatrixDeriv>& , const Data<OutMatrixDeriv>& )
{
    //    cerr<<"ProjectionToTargetLineMapping<TIn, TOut>::applyJT is not implemented " << endl;
}



template <class TIn, class TOut>
const sofa::defaulttype::BaseMatrix* ProjectionToTargetLineMapping<TIn, TOut>::getJ()
{
    return &jacobian;
}

template <class TIn, class TOut>
const vector<sofa::defaulttype::BaseMatrix*>* ProjectionToTargetLineMapping<TIn, TOut>::getJs()
{
    return &baseMatrices;
}



template <class TIn, class TOut>
void ProjectionToTargetLineMapping<TIn, TOut>::draw(const core::visual::VisualParams* vparams)
{
    if( !vparams->displayFlags().getShowMechanicalMappings() ) return;

    const SReal& scale = d_drawScale.getValue();
    if(!scale) return;

    const defaulttype::Vec4f color = d_drawColor.getValue();

    helper::ReadAccessor< Data<OutVecCoord> > origins(f_origins);
    helper::ReadAccessor< Data<OutVecCoord> > directions(f_directions);

    vector< defaulttype::Vector3 > points;
    for(unsigned i=0; i<origins.size(); i++ )
    {
        points.push_back( origins[i] - directions[i]*scale );
        points.push_back( origins[i] + directions[i]*scale );
    }
#ifndef SOFA_NO_OPENGL
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    vparams->drawTool()->drawLines( points, 1, color );
    glPopAttrib();
#endif // SOFA_NO_OPENGL
}



} // namespace mapping

} // namespace component

} // namespace sofa

#endif

