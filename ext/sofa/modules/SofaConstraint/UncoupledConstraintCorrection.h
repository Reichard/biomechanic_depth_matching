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
#ifndef SOFA_COMPONENT_CONSTRAINTSET_UNCOUPLEDCONSTRAINTCORRECTION_H
#define SOFA_COMPONENT_CONSTRAINTSET_UNCOUPLEDCONSTRAINTCORRECTION_H
#include "config.h"

#include <sofa/core/behavior/ConstraintCorrection.h>


namespace sofa
{

namespace component
{

namespace constraintset
{

/**
 *  \brief Component computing contact forces within a simulated body using the compliance method.
 */
template<class TDataTypes>
class UncoupledConstraintCorrection : public sofa::core::behavior::ConstraintCorrection< TDataTypes >
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(UncoupledConstraintCorrection,TDataTypes), SOFA_TEMPLATE(sofa::core::behavior::ConstraintCorrection, TDataTypes));

    typedef TDataTypes DataTypes;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename DataTypes::Coord Coord;
    typedef typename DataTypes::Deriv Deriv;
    typedef typename DataTypes::MatrixDeriv::RowConstIterator MatrixDerivRowConstIterator;
    typedef typename DataTypes::MatrixDeriv::ColConstIterator MatrixDerivColConstIterator;
    typedef typename DataTypes::MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef typename DataTypes::MatrixDeriv::ColIterator MatrixDerivColIterator;
    typedef typename Coord::value_type Real;

    typedef helper::vector<Real> VecReal;

    typedef sofa::core::behavior::ConstraintCorrection< TDataTypes > Inherit;
protected:
    UncoupledConstraintCorrection(sofa::core::behavior::MechanicalState<DataTypes> *mm = NULL);

    virtual ~UncoupledConstraintCorrection();
public:
    virtual void init();

    void reinit();

    /// Handle Topological Changes.
    void handleTopologyChange();

    virtual void addComplianceInConstraintSpace(const sofa::core::ConstraintParams *cparams, sofa::defaulttype::BaseMatrix *W);

    virtual void getComplianceMatrix(defaulttype::BaseMatrix* ) const;

    // for multigrid approach => constraints are merged
    virtual void getComplianceWithConstraintMerge(defaulttype::BaseMatrix* Wmerged, std::vector<int> &constraint_merge);


    /// @name Correction API
    /// @{

    virtual void computeAndApplyMotionCorrection(const sofa::core::ConstraintParams *cparams, Data< VecCoord > &x, Data< VecDeriv > &v, Data< VecDeriv > &f, const sofa::defaulttype::BaseVector *lambda);

    virtual void computeAndApplyPositionCorrection(const sofa::core::ConstraintParams *cparams, Data< VecCoord > &x, Data< VecDeriv > &f, const sofa::defaulttype::BaseVector *lambda);

    virtual void computeAndApplyVelocityCorrection(const sofa::core::ConstraintParams *cparams, Data< VecDeriv > &v, Data< VecDeriv > &f, const sofa::defaulttype::BaseVector *lambda);

    virtual void applyPredictiveConstraintForce(const sofa::core::ConstraintParams *cparams, Data< VecDeriv > &f, const sofa::defaulttype::BaseVector *lambda);

    /// @}


    /// @name Deprecated API
    /// @{

    virtual void applyContactForce(const defaulttype::BaseVector *f);

    virtual void resetContactForce();

    /// @}


    /// @name Unbuilt constraint system during resolution
    /// @{

    virtual bool hasConstraintNumber(int index) ;  // virtual ???

    virtual void resetForUnbuiltResolution(double * f, std::list<unsigned int>& /*renumbering*/)  ;

    virtual void addConstraintDisplacement(double *d, int begin,int end) ;

    virtual void setConstraintDForce(double *df, int begin, int end, bool update) ;

    virtual void getBlockDiagonalCompliance(defaulttype::BaseMatrix* W, int begin, int end) ;

    /// @}

    Data< VecReal > compliance;

    Data< Real > defaultCompliance;

    Data<bool> f_verbose;

private:
    // new :  for non building the constraint system during solving process //
    VecDeriv constraint_disp, constraint_force;
    std::list<int> constraint_dofs;		// list of indices of each point which is involve with constraint

    //std::vector< std::vector<int> >  dof_constraint_table;   // table of indices of each point involved with each constraint

protected:
    /**
     * @brief Compute dx correction from motion space force vector.
     */
    void computeDx(const Data< VecDeriv > &f);
};

template<>
UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::UncoupledConstraintCorrection(sofa::core::behavior::MechanicalState<sofa::defaulttype::Rigid3Types> *mm);

template<>
void UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::init();

template<>
void UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::addComplianceInConstraintSpace(const sofa::core::ConstraintParams *cparams, sofa::defaulttype::BaseMatrix * /*W*/);

template<>
void UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::getComplianceMatrix(sofa::defaulttype::BaseMatrix * /*m*/) const;

template<>
void UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::computeDx(const sofa::core::objectmodel::Data< VecDeriv > &/*f*/);

template<>
void UncoupledConstraintCorrection< sofa::defaulttype::Rigid3Types >::applyContactForce(const sofa::defaulttype::BaseVector * /*f*/);

template<>
void UncoupledConstraintCorrection< defaulttype::Rigid3Types >::setConstraintDForce(double * /*df*/, int /*begin*/, int /*end*/, bool /*update*/);

template<>
void UncoupledConstraintCorrection<defaulttype::Rigid3Types>::getBlockDiagonalCompliance(defaulttype::BaseMatrix * /*W*/, int /*begin*/, int /*end*/);

#if defined(SOFA_EXTERN_TEMPLATE) && !defined(SOFA_COMPONENT_CONSTRAINTSET_UNCOUPLEDCONSTRAINTCORRECTION_CPP)
#ifndef SOFA_FLOAT
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Vec3dTypes>;
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Vec1dTypes>;
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Rigid3dTypes>;
#endif
#ifndef SOFA_DOUBLE
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Vec3fTypes>;
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Vec1fTypes>;
extern template class SOFA_CONSTRAINT_API UncoupledConstraintCorrection<defaulttype::Rigid3fTypes>;
#endif
#endif

} // namespace constraintset

} // namespace component

} // namespace sofa

#endif
