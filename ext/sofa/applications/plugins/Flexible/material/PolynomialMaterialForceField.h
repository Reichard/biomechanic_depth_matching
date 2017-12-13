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
*                               SOFA :: Plugins                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_PolynomialMaterialFORCEFIELD_H
#define SOFA_PolynomialMaterialFORCEFIELD_H

#include <Flexible/config.h>
#include "../material/BaseMaterialForceField.h"
#include "../material/PolynomialMaterialBlock.h"

#include <sofa/simulation/common/AnimateEndEvent.h>

namespace sofa
{
namespace component
{
namespace forcefield
{

using helper::vector;

/** Apply Polynomial Material's Law for isotropic homogeneous incompressible materials.
  * The energy is : sum Cij ( I1/ I3^1/3  - 3)^i.( I2/ I3^2/3  - 3)^j + bulk/2 (I3-1)^2
*/

template <class _DataTypes>
class PolynomialMaterialForceField : public BaseMaterialForceFieldT<defaulttype::PolynomialMaterialBlock<_DataTypes> >
{
public:
    typedef defaulttype::PolynomialMaterialBlock<_DataTypes> BlockType;
    typedef BaseMaterialForceFieldT<BlockType> Inherit;

    SOFA_CLASS(SOFA_TEMPLATE(PolynomialMaterialForceField,_DataTypes),SOFA_TEMPLATE(BaseMaterialForceFieldT, BlockType));

    typedef typename Inherit::Real Real;

    /** @name  Material parameters */
    //@{
    Data<vector<Real> > f_C10;
    Data<vector<Real> > f_C01;
    Data<vector<Real> > f_C20;
    Data<vector<Real> > f_C02;
    Data<vector<Real> > f_C30;
    Data<vector<Real> > f_C03;
    Data<vector<Real> > f_C11;
    Data<vector<Real> > f_bulk;
//    Data<bool > f_PSDStabilization;
    //@}

    virtual void reinit()
    {
        Real C10=0.0;
        Real C01=0,C20=0,C02=0,C30=0,C03=0,C11=0,bulk=0;
        for(unsigned int i=0; i<this->material.size(); i++)
        {
            if(i<f_C10.getValue().size()) C10=f_C10.getValue()[i]; else if(f_C10.getValue().size()) C10=f_C10.getValue()[0];
            if(i<f_C01.getValue().size()) C01=f_C01.getValue()[i]; else if(f_C01.getValue().size()) C01=f_C01.getValue()[0];
            if(i<f_C20.getValue().size()) C20=f_C20.getValue()[i]; else if(f_C20.getValue().size()) C20=f_C20.getValue()[0];
            if(i<f_C02.getValue().size()) C02=f_C02.getValue()[i]; else if(f_C02.getValue().size()) C02=f_C02.getValue()[0];
            if(i<f_C30.getValue().size()) C30=f_C30.getValue()[i]; else if(f_C30.getValue().size()) C30=f_C30.getValue()[0];
            if(i<f_C03.getValue().size()) C03=f_C03.getValue()[i]; else if(f_C03.getValue().size()) C03=f_C03.getValue()[0];
            if(i<f_C11.getValue().size()) C11=f_C11.getValue()[i]; else if(f_C11.getValue().size()) C11=f_C11.getValue()[0];
            if(i<f_bulk.getValue().size()) bulk=f_bulk.getValue()[i]; else if(f_bulk.getValue().size()) bulk=f_bulk.getValue()[0];
            this->material[i].init( C10,C01,C20,C02,C30,C03,C11, bulk );
        }
        Inherit::reinit();
    }

    void handleEvent(sofa::core::objectmodel::Event *event)
    {
        if (simulation::AnimateEndEvent::checkEventType(event))
        {
            if(f_C10.isDirty() || f_C01.isDirty() || f_C20.isDirty() || f_C02.isDirty() || f_C30.isDirty() || f_C03.isDirty() || f_C11.isDirty() || f_bulk.isDirty() ) reinit();
        }
    }


protected:
    PolynomialMaterialForceField(core::behavior::MechanicalState<_DataTypes> *mm = NULL)
        : Inherit(mm)
        , f_C10(initData(&f_C10,vector<Real>((int)1,(Real)0),"C10","weight of (~I1-3) term in energy"))
        , f_C01(initData(&f_C01,vector<Real>((int)1,(Real)0),"C01","weight of (~I2-3) term in energy"))
        , f_C20(initData(&f_C20,vector<Real>((int)1,(Real)0),"C20","weight of (~I1-3)^2 term in energy"))
        , f_C02(initData(&f_C02,vector<Real>((int)1,(Real)0),"C02","weight of (~I2-3)^2 term in energy"))
        , f_C30(initData(&f_C30,vector<Real>((int)1,(Real)0),"C30","weight of (~I1-3)^3 term in energy"))
        , f_C03(initData(&f_C03,vector<Real>((int)1,(Real)0),"C03","weight of (~I2-3)^3 term in energy"))
        , f_C11(initData(&f_C11,vector<Real>((int)1,(Real)0),"C11","weight of (~I1-3)(~I2-3) term in energy"))
        , f_bulk(initData(&f_bulk,vector<Real>((int)1,(Real)0),"bulk","bulk modulus (working on I3=J=detF=volume variation)"))
//        , f_PSDStabilization(initData(&f_PSDStabilization,false,"PSDStabilization","project stiffness matrix to its nearest symmetric, positive semi-definite matrix"))
//        , _viscosity(initData(&_viscosity,(Real)0,"viscosity","Viscosity (stress/strainRate)"))
    {
        this->f_listening.setValue(true);
    }

    virtual ~PolynomialMaterialForceField()     {    }

};


}
}
}

#endif
