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
#ifndef SOFA_COMPONENT_FORCEFIELD_INTERPOLATIONCONTROLLER_H
#define SOFA_COMPONENT_FORCEFIELD_INTERPOLATIONCONTROLLER_H
#include "config.h"

#include <SofaUserInteraction/Controller.h>
#include <sofa/core/State.h>
#include <sofa/core/behavior/MechanicalState.h>

#include <sofa/defaulttype/RigidTypes.h>
#include <sofa/defaulttype/VecTypes.h>

namespace sofa
{

namespace component
{

namespace controller
{

/// Apply constant forces to given degrees of freedom.
template<class DataTypes>
class InterpolationController : public Controller
{
public:
  SOFA_CLASS(SOFA_TEMPLATE(InterpolationController,DataTypes),Controller);

	typedef typename DataTypes::VecCoord VecCoord;
	typedef typename DataTypes::Coord Coord;
	typedef core::objectmodel::Data<VecCoord> DataVecCoord;

    enum Evolution_Type
    {
        STABLE = 0,
        INFLATING,
        DEFLATING,
        EVOLUTION_COUNT
    };

    Data< int > f_evolution;
    Data< double > f_period;
    Data< float > f_alphaMax;
    Data< float > f_alpha0;
    Data< VecCoord > f_interpValues;

    void bwdInit();

    void interpolation();//VecCoord &interpXs);

    void handleEvent(core::objectmodel::Event *);

    void draw(const core::visual::VisualParams* vparams);

	virtual std::string getTemplateName() const
    {
      return templateName(this);
    }

    static std::string templateName(const InterpolationController<DataTypes>* = NULL)
    {
      return DataTypes::Name();
    }
	
protected:
    SingleLink<InterpolationController, core::State<DataTypes>, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> fromModel;
    SingleLink<InterpolationController, core::State<DataTypes>, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> toModel;
    //SingleLink<InterpolationController, core::State<DataTypes>, BaseLink::FLAG_STOREPATH|BaseLink::FLAG_STRONGLINK> interpModel;

    core::behavior::MechanicalState<DataTypes> *stateInterp; //mechanicalState containing the interpolated positions

    const VecCoord *fromXs;
    const VecCoord *toXs;
    float alpha; //coeff for positioning the interpoled points on the segments defined by points of original and objective meshes (0 < alpha < 1")
    float dAlpha; //alpha amount to add or substract to alpha when inflation or deflation

    InterpolationController();
};


#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_FORCEFIELD_INTERPOLATIONCONTROLLER_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_USER_INTERACTION_API InterpolationController<defaulttype::Vec3dTypes>;
extern template class SOFA_USER_INTERACTION_API InterpolationController<defaulttype::Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_USER_INTERACTION_API InterpolationController<defaulttype::Vec3fTypes>;
extern template class SOFA_USER_INTERACTION_API InterpolationController<defaulttype::Rigid3fTypes>;
#endif
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_FORCEFIELD_CONSTANTFORCEFIELD_H
