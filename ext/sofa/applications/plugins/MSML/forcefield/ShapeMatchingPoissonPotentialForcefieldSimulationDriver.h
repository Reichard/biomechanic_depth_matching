/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefieldSimulationDriver
  Author:    Tino Milschewski

=========================================================================*/

#ifndef __ShapeMatchingPoissonPotentialForcefieldSimulationDriver_h
#define __ShapeMatchingPoissonPotentialForcefieldSimulationDriver_h

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/defaulttype/Vec.h>

namespace sofa
{

namespace component
{

namespace collision
{

using namespace sofa::defaulttype;
using namespace sofa::core::objectmodel;

template <class DataTypes>
class ShapeMatchingPoissonPotentialForcefieldSimulationDriver : public virtual BaseObject
{


public:
	SOFA_CLASS(SOFA_TEMPLATE(ShapeMatchingPoissonPotentialForcefieldSimulationDriver,DataTypes),sofa::core::objectmodel::BaseObject);

	ShapeMatchingPoissonPotentialForcefieldSimulationDriver();

	typedef typename DataTypes::Coord Coord;
	typedef typename DataTypes::Coord::value_type Real;

	virtual void init(void);

protected:

	bool isInEpsilon(Real value, Real valueToBe, Real epsilon);

	Data<std::string> m_UndeformedMeshFilename;
};

}

}

}

#endif
