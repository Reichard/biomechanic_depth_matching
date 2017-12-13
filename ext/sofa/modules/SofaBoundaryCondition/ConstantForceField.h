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
#ifndef SOFA_COMPONENT_FORCEFIELD_CONSTANTFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_CONSTANTFORCEFIELD_H
#include "config.h"

#include <sofa/core/behavior/ForceField.h>

#include <SofaBaseTopology/TopologySubsetData.h>


namespace sofa
{

namespace component
{

namespace forcefield
{

/// Apply constant forces to given degrees of freedom.
template<class DataTypes>
class ConstantForceField : public core::behavior::ForceField<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(ConstantForceField, DataTypes), SOFA_TEMPLATE(core::behavior::ForceField, DataTypes));

    typedef core::behavior::ForceField<DataTypes> Inherit;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef helper::vector<unsigned int> VecIndex;
    typedef core::objectmodel::Data<VecCoord> DataVecCoord;
    typedef core::objectmodel::Data<VecDeriv> DataVecDeriv;

    typedef sofa::component::topology::PointSubsetData< VecIndex > SetIndex;

public:
    /// indices of the points the force applies to
    SetIndex points;
    /// Per-point forces.
    Data< VecDeriv > forces;
    /// Force applied at each point, if per-point forces are not specified
    Data< Deriv > force;
    /// Sum of the forces applied at each point, if per-point forces are not specified
    Data< Deriv > totalForce;
    ///S for drawing. The sign changes the direction, 0 doesn't draw arrow
    Data< SReal > arrowSizeCoef; // for drawing. The sign changes the direction, 0 doesn't draw arrow
    /// display color
    Data< defaulttype::Vec4f > d_color;
    /// Concerned DOFs indices are numbered from the end of the MState DOFs vector
    Data< bool > indexFromEnd;
protected:
    ConstantForceField();
public:
    /// Set a force to a given particle
    void setForce( unsigned i, const Deriv& f );

    /// Init function
    void init();

    /// Add the forces
    virtual void addForce (const core::MechanicalParams* params, DataVecDeriv& f, const DataVecCoord& x, const DataVecDeriv& v);

    /// Constant force has null variation
    virtual void addDForce(const core::MechanicalParams* mparams, DataVecDeriv& d_df , const DataVecDeriv& d_dx)
    {
        //TODO: remove this line (avoid warning message) ...
        mparams->setKFactorUsed(true);
        sofa::helper::WriteAccessor< core::objectmodel::Data< VecDeriv > > _f1 = d_df;
        _f1.resize(d_dx.getValue().size());
    }

    using Inherit::addKToMatrix;

    /// Constant force has null variation
    virtual void addKToMatrix(sofa::defaulttype::BaseMatrix *m, SReal kFactor, unsigned int &offset);

    /// Constant force has null variation
    virtual void addKToMatrix(const sofa::core::behavior::MultiMatrixAccessor* /*matrix*/, SReal /*kFact*/) {}

    virtual SReal getPotentialEnergy(const core::MechanicalParams* params, const DataVecCoord& x) const;

    void draw(const core::visual::VisualParams* vparams);

protected:
    /// Pointer to the current topology
    sofa::core::topology::BaseMeshTopology* topology;

};

#ifndef SOFA_FLOAT
template <>
SReal ConstantForceField<defaulttype::Rigid3dTypes>::getPotentialEnergy(const core::MechanicalParams*, const DataVecCoord& ) const;
template <>
SReal ConstantForceField<defaulttype::Rigid2dTypes>::getPotentialEnergy(const core::MechanicalParams*, const DataVecCoord& ) const;
#endif

#ifndef SOFA_DOUBLE
template <>
SReal ConstantForceField<defaulttype::Rigid3fTypes>::getPotentialEnergy(const core::MechanicalParams*, const DataVecCoord& ) const;
template <>
SReal ConstantForceField<defaulttype::Rigid2fTypes>::getPotentialEnergy(const core::MechanicalParams*, const DataVecCoord& ) const;
#endif

#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_FORCEFIELD_CONSTANTFORCEFIELD_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec3dTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec2dTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec1dTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec6dTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Rigid3dTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Rigid2dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec3fTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec2fTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec1fTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Vec6fTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Rigid3fTypes>;
extern template class SOFA_BOUNDARY_CONDITION_API ConstantForceField<sofa::defaulttype::Rigid2fTypes>;
#endif
#endif

} // namespace forcefield

} // namespace component

} // namespace sofa

#endif // SOFA_COMPONENT_FORCEFIELD_CONSTANTFORCEFIELD_H
