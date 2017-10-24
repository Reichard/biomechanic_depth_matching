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
*                              SOFA :: Framework                              *
*                                                                             *
* Authors: The SOFA Team (see Authors.txt)                                    *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_CORE_BEHAVIOR_PROJECTIVECONSTRAINTSET_INL
#define SOFA_CORE_BEHAVIOR_PROJECTIVECONSTRAINTSET_INL

#include <sofa/core/behavior/ProjectiveConstraintSet.h>
#include <iostream>


namespace sofa
{

namespace core
{

namespace behavior
{

template<class DataTypes>
ProjectiveConstraintSet<DataTypes>::ProjectiveConstraintSet(MechanicalState<DataTypes> *mm)
    : endTime( initData(&endTime,(Real)-1,"endTime","The constraint stops acting after the given value.\nUse a negative value for infinite constraints") )
    , mstate(initLink("mstate", "MechanicalState used by this projective constraint"), mm)
{
}

template<class DataTypes>
ProjectiveConstraintSet<DataTypes>::~ProjectiveConstraintSet()
{
}

template <class DataTypes>
bool ProjectiveConstraintSet<DataTypes>::isActive() const
{
    if( endTime.getValue()<0 ) return true;
    return endTime.getValue()>getContext()->getTime();
}

template<class DataTypes>
void ProjectiveConstraintSet<DataTypes>::init()
{
    BaseProjectiveConstraintSet::init();
    mstate = dynamic_cast< MechanicalState<DataTypes>* >(getContext()->getMechanicalState());
    if(!mstate) this->serr<<"ProjectiveConstraintSet<DataTypes>::init(), no mstate . This may be because there is no MechanicalState in the local context, or because the type is not compatible." << this->sendl;
}

template<class DataTypes>
void ProjectiveConstraintSet<DataTypes>::projectJacobianMatrix(const MechanicalParams* mparams, MultiMatrixDerivId cId)
{
    if (!isActive())
        return;

    if (mstate)
    {
        projectJacobianMatrix(mparams, *cId[mstate.get(mparams)].write());
    }
}

#ifdef SOFA_SMP
template<class T>
struct projectResponseTask
{
    void operator()(const MechanicalParams* mparams, void *c, Shared_rw< objectmodel::Data< typename T::VecDeriv> > dx)
    {
        ((T *)c)->T::projectResponse(mparams, dx.access());
    }
};

template<class T>
struct projectVelocityTask
{
    void operator()(const MechanicalParams* mparams, void *c, Shared_rw< objectmodel::Data< typename T::VecDeriv> > v)
    {
        ((T *)c)->T::projectVelocity(mparams, v.access());
    }
};

template<class T>
struct projectPositionTask
{
    void operator()(const MechanicalParams* mparams, void *c, Shared_rw< objectmodel::Data< typename T::VecCoord> > x)
    {
        ((T *)c)->T::projectPosition(mparams, x.access());
    }
};

template<class DataTypes>
struct projectResponseTask<ProjectiveConstraintSet< DataTypes > >
{
    void operator()(const MechanicalParams* mparams, ProjectiveConstraintSet<DataTypes>  *c, Shared_rw< objectmodel::Data< typename DataTypes::VecDeriv> > dx)
    {
        c->projectResponse(mparams, dx.access());
    }
};

template<class DataTypes>
struct projectVelocityTask<ProjectiveConstraintSet< DataTypes > >
{
    void operator()(const MechanicalParams* mparams, ProjectiveConstraintSet<DataTypes>  *c, Shared_rw< objectmodel::Data< typename DataTypes::VecDeriv> > v)
    {
        c->projectVelocity(mparams, v.access());
    }
};

template<class DataTypes>
struct projectPositionTask<ProjectiveConstraintSet< DataTypes > >
{
    void operator()(const MechanicalParams* mparams, ProjectiveConstraintSet<DataTypes>  *c, Shared_rw< objectmodel::Data< typename DataTypes::VecCoord> > x)
    {
        c->projectPosition(mparams, x.access());
    }
};
#endif /* SOFA_SMP */

template<class DataTypes>
void ProjectiveConstraintSet<DataTypes>::projectResponse(const MechanicalParams* mparams, MultiVecDerivId dxId)
{
//    std::cerr << "ProjectiveConstraintSet<DataTypes>::projectResponse(const MechanicalParams* mparams, MultiVecDerivId dxId) " << this->getName() << std::endl;

    if (!isActive())
        return;
    if (mstate)
    {
//        std::cerr << "ProjectiveConstraintSet<DataTypes>::projectResponse(const MechanicalParams* mparams, MultiVecDerivId dxId) " << this->getName() << " has mstate " << std::endl;
#ifdef SOFA_SMP
        if (mparams->execMode() == ExecParams::EXEC_KAAPI)
            Task<projectResponseTask<ProjectiveConstraintSet< DataTypes > > >(mparams, this,
                    **defaulttype::getShared(*dxId[mstate.get(mparams)].write()));
        else
#endif /* SOFA_SMP */
            projectResponse(mparams, *dxId[mstate.get(mparams)].write());
    }
    else serr << "ProjectiveConstraintSet<DataTypes>::projectResponse(const MechanicalParams* mparams, MultiVecDerivId dxId), no mstate for " << this->getName() << sendl;
}

template<class DataTypes>
void ProjectiveConstraintSet<DataTypes>::projectVelocity(const MechanicalParams* mparams, MultiVecDerivId vId)
{
    if (!isActive())
        return;

    if (mstate)
    {
#ifdef SOFA_SMP
        if (mparams->execMode() == ExecParams::EXEC_KAAPI)
            Task<projectVelocityTask<ProjectiveConstraintSet< DataTypes > > >(mparams, this,
                    **defaulttype::getShared(*vId[mstate.get(mparams)].write()));
        else
#endif /* SOFA_SMP */
            projectVelocity(mparams, *vId[mstate.get(mparams)].write());
    }
    else serr << "ProjectiveConstraintSet<DataTypes>::projectVelocity(const MechanicalParams* mparams, MultiVecDerivId dxId), no mstate for " << this->getName() << sendl;
}

template<class DataTypes>
void ProjectiveConstraintSet<DataTypes>::projectPosition(const MechanicalParams* mparams, MultiVecCoordId xId)
{
    if (!isActive())
        return;

    if (mstate)
    {
#ifdef SOFA_SMP
        if (mparams->execMode() == ExecParams::EXEC_KAAPI)
            Task<projectPositionTask<ProjectiveConstraintSet< DataTypes > > >(mparams, this,
                    **defaulttype::getShared(*xId[mstate.get(mparams)].write()));
        else
#endif /* SOFA_SMP */
            projectPosition(mparams, *xId[mstate.get(mparams)].write());
    }
}

#ifdef SOFA_SMP

// TODO
// template<class DataTypes>
// void ProjectiveConstraintSet<DataTypes>::projectFreeVelocity()
// {
// 	if( !isActive() ) return;
// 	if (mstate)
// 		Task<projectVelocityTask<ProjectiveConstraintSet< DataTypes > > >(this,**mstate->getVfree());
// }
//
// template<class DataTypes>
// void ProjectiveConstraintSet<DataTypes>::projectFreePosition()
// {
// 	if( !isActive() ) return;
// 	if (mstate)
// 		Task<projectPositionTask<ProjectiveConstraintSet< DataTypes > > >(this,**mstate->read(sofa::core::ConstVecCoordId::freePosition())->getValue());
// }

#endif /* SOFA_SMP */

} // namespace behavior

} // namespace core

} // namespace sofa

#endif
