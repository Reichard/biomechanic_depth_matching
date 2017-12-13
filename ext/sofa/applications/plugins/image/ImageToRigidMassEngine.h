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
#ifndef SOFA_IMAGE_ImageToRigidMassEngine_H
#define SOFA_IMAGE_ImageToRigidMassEngine_H

#include <image/config.h>
#include "ImageTypes.h"
#include <sofa/core/objectmodel/Event.h>
#include <sofa/simulation/common/AnimateEndEvent.h>
#include <sofa/helper/OptionsGroup.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/helper/decompose.h>

namespace sofa
{
namespace component
{
namespace engine
{

using helper::vector;
using defaulttype::Vec;
using defaulttype::Mat;
using cimg_library::CImg;

/**
 * Compute rigid mass from a density image
 */


template <class _ImageTypes>
class ImageToRigidMassEngine : public core::DataEngine
{
public:
    typedef core::DataEngine Inherited;
    SOFA_CLASS(SOFA_TEMPLATE(ImageToRigidMassEngine,_ImageTypes),Inherited);

    typedef SReal Real;

    typedef _ImageTypes ImageTypes;
    typedef typename ImageTypes::T T;
    typedef typename ImageTypes::imCoord imCoord;
    typedef helper::ReadAccessor<Data< ImageTypes > > raImage;
    Data< ImageTypes > image;

    typedef defaulttype::ImageLPTransform<Real> TransformType;
    typedef typename TransformType::Coord Coord;
    typedef helper::ReadAccessor<Data< TransformType > > raTransform;
    Data< TransformType > transform;

    typedef defaulttype::RigidCoord<3,Real> RigidCoord;
    Data< RigidCoord > d_position;

    /** @name  Outputs */
    //@{
    Data< Real > d_mass;
    Data< Coord > d_inertia;

    typedef defaulttype::RigidMass<3,Real> RigidMass;
    typedef typename RigidMass::Mat3x3 Mat3x3;
    Data< RigidMass > d_rigidMass;
    //@}

    /** @name  Inputs */
    //@{
    Data< Real > d_density;
    Data< bool > d_mult;
    //@}


    virtual std::string getTemplateName() const    { return templateName(this);    }
    static std::string templateName(const ImageToRigidMassEngine<ImageTypes>* = NULL) { return ImageTypes::Name();    }

    ImageToRigidMassEngine()    :   Inherited()
      , image(initData(&image,ImageTypes(),"image",""))
      , transform(initData(&transform,TransformType(),"transform",""))
      , d_position(initData(&d_position,RigidCoord(),"position","position"))
      , d_mass(initData(&d_mass,(Real)0,"mass","mass"))
      , d_inertia(initData(&d_inertia,Coord(),"inertia","axis-aligned inertia tensor"))
      , d_rigidMass(initData(&d_rigidMass,RigidMass(),"rigidMass","rigidMass"))
      , d_density(initData(&d_density,(Real)1000.,"density","density (in kg/m^3)"))
      , d_mult(initData(&d_mult,false,"multiply","multiply density by image intensity?"))
      , time((unsigned int)0)
    {
        image.setReadOnly(true);
        transform.setReadOnly(true);
        f_listening.setValue(true);
    }

    virtual void init()
    {
        addInput(&image);
        addInput(&transform);
        addInput(&d_density);
        addInput(&d_mult);
        addOutput(&d_position);
        addOutput(&d_rigidMass);
        addOutput(&d_inertia);
        setDirtyValue();
    }

    virtual void reinit() { update(); }

protected:

    unsigned int time;

    virtual void update()
    {
        raTransform inT(this->transform);
        raImage in(this->image);
        if(in->isEmpty()) return;
        const CImg<T>& img = in->getCImg(this->time);

        d_density.updateIfDirty();
        d_mult.updateIfDirty();

        cleanDirty();

        helper::WriteOnlyAccessor<Data< RigidCoord > > pos(this->d_position);
        helper::WriteOnlyAccessor<Data< RigidMass > > rigidMass(this->d_rigidMass);
        helper::WriteOnlyAccessor<Data< Coord > > inertia(this->d_inertia);

        pos->clear();
        rigidMass->mass=0;
        rigidMass->volume=0;
        rigidMass->inertiaMatrix.clear();

        Real voxelVol = inT->getScale()[0]*inT->getScale()[1]*inT->getScale()[2];
        Mat3x3 C;

        cimg_forXYZ(img,x,y,z)
                if(img(x,y,z)!=(T)0)
        {
            Real density = (d_mult.getValue()==true) ? (Real)img(x,y,z)*d_density.getValue() : d_density.getValue();
            Real m = density*voxelVol;
            rigidMass->volume+=voxelVol;
            rigidMass->mass+=m;
            Coord p = inT->fromImage(Coord(x,y,z));
            pos->getCenter()+=p*m;
            C+=dyad(p,p)*m; // covariance matrix
        }

        if(rigidMass->mass)
        {
            d_mass.setValue(rigidMass->mass);

            pos->getCenter()/=rigidMass->mass;
            C-=dyad(pos->getCenter(),pos->getCenter())*rigidMass->mass; // recenter covariance matrix around mean
            rigidMass->inertiaMatrix = Mat3x3::s_identity*trace(C) - C;   // covariance matrix to inertia matrix

            typename RigidMass::Mat3x3 R;
            helper::Decompose<Real>::eigenDecomposition(rigidMass->inertiaMatrix, R, inertia.wref());

            pos->getOrientation().fromMatrix(R);
            rigidMass->inertiaMatrix.clear();
            for(size_t i=0;i<3;i++) rigidMass->inertiaMatrix[i][i]=inertia.ref()[i]/rigidMass->mass;

            rigidMass->recalc();
        }

    }

    void handleEvent(sofa::core::objectmodel::Event *event)
    {
        if (simulation::AnimateEndEvent::checkEventType(event))
        {
            raImage in(this->image);
            raTransform inT(this->transform);

            // get current time modulo dimt
            const unsigned int dimt=in->getDimensions()[ImageTypes::DIMENSION_T];
            if(!dimt) return;
            Real t=inT->toImage(this->getContext()->getTime()) ;
            t-=(Real)((int)((int)t/dimt)*dimt);
            t=(t-floor(t)>0.5)?ceil(t):floor(t); // nearest
            if(t<0) t=0.0; else if(t>=(Real)dimt) t=(Real)dimt-1.0; // clamp

            if(this->time!=(unsigned int)t) { this->time=(unsigned int)t; update(); }
        }
    }

};


} // namespace engine
} // namespace component
} // namespace sofa

#endif // SOFA_IMAGE_ImageToRigidMassEngine_H
