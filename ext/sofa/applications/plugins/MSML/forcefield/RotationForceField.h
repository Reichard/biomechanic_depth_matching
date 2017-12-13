/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, RotationForceField
  Author:    Stefan Suwelack

=========================================================================*/

#ifndef __RotationForceField_h
#define __RotationForceField_h

// ****************************************************************************
// Includes
// ****************************************************************************
#include <sofa/core/MultiVecId.h>
#include <sofa/core/MechanicalParams.h>





// ****************************************************************************
// Defines
// ****************************************************************************




// ****************************************************************************
// RotationForceField
// ****************************************************************************

/** \class AbsImageFilter
 * \brief This class does some really fancy stuff
 *
 * \ingroup Examples
 */
namespace sofa
{

namespace core
{

namespace behavior
{


class RotationForceField
{
public:

	// public methods
	 virtual void applyRotation(const MechanicalParams* mparams /* PARAMS FIRST */,sofa::core::MultiVecDerivId dxId,  MultiVecDerivId dfId , bool invert)=0;

};

} // namespace behavior

} // namespace core

} // namespace sofa


#endif /* __RotationForceField_h */

//#ifndef SOFA_CORE_BEHAVIOR_BASEFORCEFIELD_H
//#define SOFA_CORE_BEHAVIOR_BASEFORCEFIELD_H
//
//#include <sofa/core/core.h>
//#include <sofa/core/MultiVecId.h>
//#include <sofa/core/MechanicalParams.h>
//#include <sofa/core/objectmodel/BaseObject.h>
//#include <sofa/core/behavior/MultiMatrixAccessor.h>
//#include <sofa/defaulttype/BaseMatrix.h>
//#include <sofa/defaulttype/BaseVector.h>
//#include <sofa/defaulttype/Vec.h>
//
//namespace sofa
//{
//
//namespace core
//{
//
//namespace behavior
//{
//
///**
// *  \brief Component computing forces within simulated bodies.
// *
// *  This class define the abstract API common to all force fields.
// *  A force field computes forces applied to one or more simulated body
// *  given its current position and velocity.
// *
// *  Forces can be internal to a given body (attached to one MechanicalState,
// *  see the ForceField class), or link several bodies together (such as contact
// *  forces, see the InteractionForceField class).
// *
// *  For implicit integration schemes, it must also compute the derivative
// *  ( df, given a displacement dx ).
// *
// */
//class SOFA_CORE_API BaseForceField : public virtual objectmodel::BaseObject
//{
//public:
//    SOFA_CLASS(BaseForceField, objectmodel::BaseObject);
//
//    virtual ~BaseForceField() {}
//
//    /// @name Vector operations
//    /// @{
//
//    /// Given the current position and velocity states, update the current force
//    /// vector by computing and adding the forces associated with this
//    /// ForceField.
//    ///
//    /// If the ForceField can be represented as a matrix, this method computes
//    ///                          $ f += B v + K x $
//
//    /// \param mparams defines the state vectors to use for positions, velocities, forces: mparams->getX(), mparams->getV(), and mparams->getF(), respectively.
//	/// If mparams->energy() is true, compute and internally store the potential energy, which will be subsequently returned by method getPotentialEnergy()
//
//    /// K is the stiffness matrix (associated with forces which derive from a potential),
//    /// B is the damping matrix (associated with viscous forces),
//    /// Very often, at least one of these matrices is null.
//    /// \param mparams->getX() input vector of position
//    /// \param mparams->getV() input vector of velocity
//    /// \param fId output vector of forces
//	virtual void addForce(const MechanicalParams* mparams /* PARAMS FIRST */, MultiVecDerivId fId )=0;
//
//    /// Compute the force derivative given a small displacement from the
//    /// position and velocity used in the previous call to addForce().
//    ///
//    /// The derivative should be directly derived from the computations
//    /// done by addForce. Any forces neglected in addDForce will be integrated
//    /// explicitly (i.e. using its value at the beginning of the timestep).
//    ///
//    /// If the ForceField can be represented as a matrix, this method computes
//    ///                    $ df += kFactor K dx + bFactor B dx $
//    /// K is the stiffness matrix (associated with forces which derive from a potential),
//    /// B is the damping matrix (associated with viscous forces)
//	/// \param mparams->getDx() input vector
//	/// \param dfId output vector
//	/// \param mparams->mFactor() coefficient for mass contributions (i.e. second-order derivatives term in the ODE)
//	/// \param mparams->kFactor() coefficient for stiffness contributions (i.e. DOFs term in the ODE)
//    virtual void addDForce(const MechanicalParams* mparams /* PARAMS FIRST */, MultiVecDerivId dfId )=0;
//
//    /// Accumulate the contribution of M, B, and/or K matrices multiplied
//    /// by the dx vector with the given coefficients.
//    ///
//    /// This method computes
//    /// $ df += mFactor M dx + bFactor B dx + kFactor K dx $
//    /// In most cases only one of these matrices will be non-null for a given
//    /// component. For forcefields without mass it simply calls addDForce.
//    ///
//    /// M is the mass matrix (associated with inertial forces),
//    /// K is the stiffness matrix (associated with forces which derive from a potential),
//    /// B is the damping matrix (associated with viscous forces),
//    /// Very often, at least one of these matrices is null.
//    /// \param mparams->getDx() input vector
//    /// \param dfId output vector
//    /// \param mparams->mFactor() coefficient for mass contributions (i.e. second-order derivatives term in the ODE)
//    /// \param mparams->bFactor() coefficient for damping contributions (i.e. first derivatives term in the ODE)
//    /// \param mparams->kFactor() coefficient for stiffness contributions (i.e. DOFs term in the ODE)
//    virtual void addMBKdx(const MechanicalParams* mparams /* PARAMS FIRST */, MultiVecDerivId dfId);
//
//    /// Get the potential energy associated to this ForceField during the last call of addForce( const MechanicalParams* mparams );
//    ///
//    /// Used to extimate the total energy of the system by some
//    /// post-stabilization techniques.
//    virtual double getPotentialEnergy( const MechanicalParams* mparams = MechanicalParams::defaultInstance() ) const=0;
//    /// @}
//
//
//    /// @name Matrix operations
//    /// @{
//
//    /// Compute the system matrix corresponding to k K
//    ///
//    /// \param matrix matrix to add the result to
//    /// \param mparams->kFactor() coefficient for stiffness contributions (i.e. DOFs term in the ODE)
//    virtual void addKToMatrix(const MechanicalParams* mparams /* PARAMS FIRST */, const sofa::core::behavior::MultiMatrixAccessor* matrix ) = 0;
//    //virtual void addKToMatrix(sofa::defaulttype::BaseMatrix * matrix, double kFact, unsigned int &offset);
//
//    /// Compute the system matrix corresponding to b B
//    ///
//    /// \param matrix matrix to add the result to
//    /// \param mparams->bFactor() coefficient for damping contributions (i.e. first derivatives term in the ODE)
//    virtual void addBToMatrix(const MechanicalParams* mparams /* PARAMS FIRST */, const sofa::core::behavior::MultiMatrixAccessor* matrix );
//    //virtual void addBToMatrix(sofa::defaulttype::BaseMatrix * matrix, double bFact, unsigned int &offset);
//
//
//    /// Compute the system matrix corresponding to m M + b B + k K
//    ///
//    /// \param matrix matrix to add the result to
//    /// \param mparams->mFactor() coefficient for mass contributions (i.e. second-order derivatives term in the ODE)
//    /// \param mparams->bFactor() coefficient for damping contributions (i.e. first derivatives term in the ODE)
//    /// \param mparams->kFactor() coefficient for stiffness contributions (i.e. DOFs term in the ODE)
//    virtual void addMBKToMatrix(const MechanicalParams* mparams /* PARAMS FIRST */, const sofa::core::behavior::MultiMatrixAccessor* matrix );
//    ////virtual void addMBKToMatrix(sofa::defaulttype::BaseMatrix * matrix, double mFact, double bFact, double kFact, unsigned int &offset);
//
//    /// @}
//
//    /// If the forcefield is applied only on a subset of particles.
//    /// That way, we can optimize the time spent to transfer forces through the mechanical mappings
//    /// Deactivated by default. The forcefields using only a subset of particles should activate the mask,
//    /// and during addForce(), insert the indices of the particles modified
//    virtual bool useMask() const { return false; }
//};
//
//} // namespace behavior
//
//} // namespace core
//
//} // namespace sofa
//
//#endif  /* SOFA_CORE_BEHAVIOR_BASEFORCEFIELD_H */

