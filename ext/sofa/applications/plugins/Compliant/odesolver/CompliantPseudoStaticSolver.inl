#include "CompliantPseudoStaticSolver.h"

#include "../assembly/AssemblyVisitor.h"

#include <sofa/helper/rmath.h>


namespace sofa {
namespace component {
namespace odesolver {


template< typename CompliantOdeSolver >
CompliantPseudoStaticSolver<CompliantOdeSolver>::CompliantPseudoStaticSolver()
    : d_threshold(initData(&d_threshold,
                   (SReal) 1.0e-6,
                   "threshold",
                   "stop criterion: min difference between 2 iterations"))
    , d_iterations(initData(&d_iterations,
                     (unsigned) 1000,
                     "iterations",
                     "maximum number of iterations"))
    , d_velocityFactor(initData(&d_velocityFactor,
                   (SReal) 0.5,
                   "velocityFactor",
                   "amount of kept velocity at each iteration (0=fully damped, 1=fully dynamics)"))
    , d_lastVelocity(initData(&d_lastVelocity,
                   "lastVelocity",
                   "(output) last velocity square norm"))
{
    d_lastVelocity.setReadOnly(true);
}

template< typename CompliantOdeSolver >
void CompliantPseudoStaticSolver<CompliantOdeSolver>::init()
{
    // clamp velocityFactor [0,1]
    d_velocityFactor.setValue( helper::rclamp<SReal>( d_velocityFactor.getValue(), 0, 1 ) );

    CompliantOdeSolver::init();
}

template< typename CompliantOdeSolver >
void CompliantPseudoStaticSolver<CompliantOdeSolver>::solve(const core::ExecParams* params,
                         SReal dt,
                         core::MultiVecCoordId posId,
                         core::MultiVecDerivId velId)
{
    typename CompliantOdeSolver::SolverOperations sop( params, this->getContext(), this->alpha.getValue(), this->beta.getValue(), dt, posId, velId, true );

    const SReal& threshold = d_threshold.getValue();
    const SReal& velocityFactor = d_velocityFactor.getValue();

    SReal lastVelocity = 0;

    unsigned i=0;
    for( unsigned imax=d_iterations.getValue() ; i<imax ; ++i )
    {
        // dynamics integation
        CompliantOdeSolver::solve( params, dt, posId, velId );

        // stop if the velocity norm is too small i.e. it does not move enough from previous iteration
        sop.vop.v_dot( velId, velId );
        lastVelocity = sop.vop.finish();

        // damp velocity
        sop.vop.v_teq( velId, velocityFactor );

        // propagate damped velocity
        {
        simulation::MechanicalPropagatePositionAndVelocityVisitor bob( sofa::core::MechanicalParams::defaultInstance() );
        this->getContext()->executeVisitor( &bob );
        }

        if( this->f_printLog.getValue() )
            serr<<"velocity norm: "<<sqrt(lastVelocity)<<sendl;

        if( lastVelocity < threshold*threshold ) break;
    }

    d_lastVelocity.setValue(lastVelocity);

    if( this->f_printLog.getValue() ) serr<<i+1<<" iterations"<<sendl;

}



}
}
}
