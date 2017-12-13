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
#ifndef SOFA_Flexible_MassFromDensity_H
#define SOFA_Flexible_MassFromDensity_H

#include <SofaEigen2Solver/EigenSparseMatrix.h>
#include <image/ImageTypes.h>
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/DataEngine.h>
#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/core/BaseMapping.h>


#include <sofa/core/objectmodel/Event.h>
#include <sofa/simulation/common/AnimateEndEvent.h>

#include <Compliant/assembly/AssemblyHelper.h>

namespace sofa
{
namespace component
{
namespace engine
{


/**
 * compute a mass matrix (for a mass component) from a density image, and generic mapping
 * must me in a separate node with a dummy MechnanicalObject<Vec3> and mapping<DofType,Vec3>
 * @author: Benjamin Gilles
 */


template <class _DataTypes, class _ImageTypes>
class MassFromDensity : public core::DataEngine
{
public:
    typedef core::DataEngine Inherited;
    SOFA_CLASS(SOFA_TEMPLATE2(MassFromDensity,_DataTypes,_ImageTypes),Inherited);

    typedef _ImageTypes ImageTypes;
    typedef typename ImageTypes::T T;
    typedef typename ImageTypes::imCoord imCoord;
    typedef helper::ReadAccessor<Data< ImageTypes > > raImage;

    typedef SReal Real;
    typedef defaulttype::ImageLPTransform<Real> TransformType;
    typedef helper::ReadAccessor<Data< TransformType > > raTransform;

    typedef defaulttype::Vec3Types VecT; // expected mechanical state type
    typedef typename VecT::VecCoord VecCoord;
    typedef typename VecT::Coord Coord;

    typedef _DataTypes DataTypes;
    typedef component::linearsolver::EigenSparseMatrix<DataTypes,DataTypes> MassMatrix;
    typedef Eigen::SparseMatrix<Real, Eigen::RowMajor> rmat;

    Data< ImageTypes > image;
    Data< TransformType > transform;
    Data< MassMatrix > massMatrix;

    enum { NO_LUMPING=0, BLOCK_LUMPING=1, DIAGONAL_LUMPING=2 };
    Data< int > f_lumping; ///< is the mass matrix lumped? (copy each non-diagonal term on the diagonal term of the same line)  0->no, 1->by bloc, 2->diagonal matrix

    virtual std::string getTemplateName() const    { return templateName(this);    }
    static std::string templateName(const MassFromDensity<DataTypes,ImageTypes>* = NULL) { return DataTypes::Name()+std::string(",")+ImageTypes::Name();  }

    MassFromDensity()    :   Inherited()
      , image(initData(&image,ImageTypes(),"image",""))
      , transform(initData(&transform,TransformType(),"transform",""))
      , massMatrix( initData(&massMatrix, "massMatrix", "Mass Matrix") )
      , f_lumping( initData( &f_lumping, (int)NO_LUMPING, "lumping", "Should the mass matrix be lumped? 0->no, 1->by bloc, 2->diagonal matrix" ) )
      , deformationMapping(NULL)
      , dofs(NULL)
      , time((unsigned int)0)
    {
        //f_listening.setValue(true); // listening -> update at each time step. Disabled by default
        image.setReadOnly(true);
        transform.setReadOnly(true);
    }

    virtual ~MassFromDensity() {}

    virtual void init()
    {
        addInput(&image);
        addInput(&transform);
        addInput(&f_lumping);
        addOutput(&massMatrix);
        setDirtyValue();

        this->getContext()->get( deformationMapping, core::objectmodel::BaseContext::Local);
        this->getContext()->get( dofs, core::objectmodel::BaseContext::Local);
    }

    virtual void reinit() { update(); }

protected:

    virtual void update()
    {
        if(!deformationMapping) { serr<<SOFA_CLASS_METHOD<<"can't compute the mass : no mapping found"<<sendl; return; }
        if(!dofs) { serr<<SOFA_CLASS_METHOD<<"can't compute the mass : no MechanicalObject<Vec3> found"<<sendl; return; }

        // data access
        raImage in(this->image);
        raTransform inT(this->transform);
        if(in->isEmpty()) return;
        const cimg_library::CImg<T>& img = in->getCImg(this->time);

        f_lumping.updateIfDirty();

        cleanDirty();

        // count non zero voxels
        unsigned int nb=0;
        cimg_forXYZ(img,x,y,z) if(img(x,y,z)) nb++;

        // build mass and mapped dofs
        rmat Me(3*nb,3*nb);
        Me.reserve(3*nb);
        dofs->resize(nb);
        helper::WriteOnlyAccessor<Data<VecCoord> > rpos ( dofs->writeOnlyRestPositions() );
        helper::WriteOnlyAccessor<Data<VecCoord> > pos ( dofs->writeOnlyPositions() );

        Real voxelVol = inT->getScale()[0]*inT->getScale()[1]*inT->getScale()[2];
        nb=0;
        Real totalMass=0;
        cimg_forXYZ(img,x,y,z)
                if(img(x,y,z))
        {
            rpos[nb]=pos[nb]=inT->fromImage(Coord(x,y,z));
            for (int k=0; k<3; ++k)  Me.insert(3*nb+k,3*nb+k) = voxelVol*(Real)img(x,y,z);
            totalMass+=voxelVol*(Real)img(x,y,z);
            nb++;
        }
        Me.makeCompressed();

        // output some information
        sout<<"Total Volume = "<<voxelVol*nb<<" ("<<pow((double)voxelVol*nb,1.0/3.0)<<")"<<sendl;
        sout<<"Total Mass = "<<totalMass<<sendl;

        // compute J
        deformationMapping->init();

        const vector<defaulttype::BaseMatrix*>* js = deformationMapping->getJs();
        if(!js) { serr<<SOFA_CLASS_METHOD<<"can't compute J : problem with mapping ?"<<sendl; return; }

        // compute mass
        for(size_t i=0; i<js->size(); i++)
        {
            // check if input type is compatible with template type
            const core::State< DataTypes >* compatible = dynamic_cast<const core::State< DataTypes >*> (deformationMapping->getFrom()[i]);
            if(compatible)
            {
                MySPtr<rmat> J( convertSPtr<rmat>( (*js)[i] ) );
                rmat JTMe=J->transpose()*Me;
                MassMatrix& M = *massMatrix.beginWriteOnly();
                M.compressedMatrix=JTMe*(*J);
                if( f_lumping.getValue()==BLOCK_LUMPING )
                {
                    MassMatrix nM(M.rows(),M.cols());
                    nM.compressedMatrix.reserve(M.rows()*M.Nin);
                    unsigned int nbBlocks = M.cols()/M.Nin;
                    // sum blocks line by line -> non symmetric mass
                    // for (int r=0; r<nbBlocks; ++r)
                    //     for (int n=0; n<M.Nout; ++n)
                    //         for (int m=0; m<M.Nin; ++m)
                    //             for (int c=0; c<nbBlocks; ++c)
                    //                 nM.compressedMatrix.coeffRef(r*M.Nout+n,r*M.Nout+m) += M(r*M.Nout+n,c*M.Nin+m);
                    // keep only diagonal block -> symmetric mass
                    for (unsigned int r=0; r<nbBlocks; ++r)
                        for (int n=0; n<M.Nout; ++n)
                            for (int m=0; m<M.Nin; ++m)
                                    nM.compressedMatrix.coeffRef(r*M.Nout+n,r*M.Nout+m) += M(r*M.Nout+n,r*M.Nin+m);
                    nM.compress();
                    M.compressedMatrix=nM.compressedMatrix;
                }
                else if( f_lumping.getValue()==DIAGONAL_LUMPING ) // sum scalars line by line
                {
                    MassMatrix nM(M.rows(),M.cols());
                    nM.compressedMatrix.reserve(M.rows());
                    for (int r=0; r<M.rows(); ++r)
                        for (int c=0; c<M.cols(); ++c)
                            nM.compressedMatrix.coeffRef(r,r) += M(r,c);
                    nM.compress();
                    M.compressedMatrix=nM.compressedMatrix;
                }

                massMatrix.endEdit();
            }
        }

        // clean
        dofs->resize(0);
        deformationMapping->init();
    }

    void handleEvent(sofa::core::objectmodel::Event *event)
    {
        if (simulation::AnimateEndEvent::checkEventType(event))
        {
            raImage in(this->image);
            raTransform inT(this->transform);

            // get current time modulo dimt
            const unsigned int dimt=in->getDimensions()[4];
            if(!dimt) return;
            Real t=inT->toImage(this->getContext()->getTime()) ;
            t-=(Real)((int)((int)t/dimt)*dimt);
            t=(t-floor(t)>0.5)?ceil(t):floor(t); // nearest
            if(t<0) t=0.0; else if(t>=(Real)dimt) t=(Real)dimt-1.0; // clamp

            if(this->time!=(unsigned int)t) { this->time=(unsigned int)t;  }

            update(); // update at each time step (deformation has changed)
        }
    }

    core::BaseMapping* deformationMapping;
    core::State<VecT>* dofs;
    unsigned int time;
};


} // namespace engine
} // namespace component
} // namespace sofa

#endif // SOFA_Flexible_MassFromDensity_H
