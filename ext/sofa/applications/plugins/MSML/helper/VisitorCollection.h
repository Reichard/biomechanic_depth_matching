/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, RotationForceField
  Author:    Stefan Suwelack

=========================================================================*/

#ifndef __VisitorCollection_h
#define __VisitorCollection_h

// ****************************************************************************
// Includes
// ****************************************************************************
#include <sofa/core/MultiVecId.h>
#include <sofa/core/MechanicalParams.h>
#include "RotationForceField.h"

#include <sofa/simulation/common/Visitor.h>
#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/simulation/common/VisualVisitor.h>
#include <sofa/core/objectmodel/Event.h>
#include <sofa/component/typedef/Particles_double.h>
#include "LandmarkSpringsForceField.h"





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

namespace simulation
{

class MechanicalApplyRotationVisitor : public MechanicalVisitor
{
public:
    MultiVecDerivId res;
    MultiVecDerivId dxId;
    bool invert; ///< Accumulate everything back to the DOFs through the mappings

    MechanicalApplyRotationVisitor(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, MultiVecDerivId dxId, MultiVecDerivId res)
        : MechanicalVisitor(mparams) , res(res), invert(true)
    {
//#ifdef SOFA_DUMP_VISITOR_INFO
//        setReadWriteVectors();
//#endif
    }

    MechanicalApplyRotationVisitor(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */,MultiVecDerivId dxId,  MultiVecDerivId res, bool invert)
        : MechanicalVisitor(mparams) , res(res), invert(invert), dxId(dxId)
    {
//#ifdef SOFA_DUMP_VISITOR_INFO
//        setReadWriteVectors();
//#endif
    }

    virtual Result fwdForceField(simulation::Node* /*node*/, core::behavior::BaseForceField* ff)
    {
    	sofa::core::behavior::RotationForceField* theFF = NULL;
    	theFF = dynamic_cast<sofa::core::behavior::RotationForceField*>(ff);
    	if(theFF)
    		theFF->applyRotation(this->mparams /* PARAMS FIRST */, dxId, res, invert);

    	return RESULT_CONTINUE;
    }


    /// Return a class name for this visitor
    /// Only used for debugging / profiling purposes
    virtual const char* getClassName() const { return "MechanicalApplyRotationVisitor"; }
    virtual std::string getInfos() const { std::string name= "["+res.getName()+"]"; return name; }

    /// Specify whether this action can be parallelized.
    virtual bool isThreadSafe() const
    {
        return true;
    }

//#ifdef SOFA_DUMP_VISITOR_INFO
//    void setReadWriteVectors()
//    {
//        addReadWriteVector(res);
//    }
//#endif
};

class SetChargeFactorVisitor : public Visitor
{

public:
	SetChargeFactorVisitor(const core::ExecParams* params, double chargeFactor):Visitor(params)
	{
		m_ChargeFactor = chargeFactor;
	}

	  virtual Result processNodeTopDown(simulation::Node* node)
	  {
		for_each(this, node, node->unsorted, &SetChargeFactorVisitor::setChargeFactor);
		return RESULT_CONTINUE;
	  }

	virtual void setChargeFactor(simulation::Node*, core::objectmodel::BaseObject* ff)
	{
		//down cast to boundaryconditionhandler
		sofa::component::forcefield::LandmarkSpringsForceField<Vec3dTypes>* realff= dynamic_cast<sofa::component::forcefield::LandmarkSpringsForceField<Vec3dTypes>*>(ff);
		//if type is visual model
		if(realff)
		{
			realff->setChargeFactor(m_ChargeFactor);

		}
	}

	virtual const char* getClassName() const { return "SetChargeFactorVisitor"; }



protected:
	double m_ChargeFactor;



};



class InitializeBoundaryConditionsEvent : public sofa::core::objectmodel::Event
{
public:
	InitializeBoundaryConditionsEvent(helper::vector< Coord3d >  globalPointCoords)
    : sofa::core::objectmodel::Event()
    {
		m_GlobalPointCoords = globalPointCoords;
    }

    ~InitializeBoundaryConditionsEvent(){}

    helper::vector< Coord3d>  m_GlobalPointCoords;
    virtual const char* getClassName() const { return "InitializeBoundaryConditions"; }
protected:

};


class UpdateBoundaryConditionsEvent : public sofa::core::objectmodel::Event
{
public:
	UpdateBoundaryConditionsEvent(helper::vector< Coord3d > pointDisplacements )
    : sofa::core::objectmodel::Event()
    {
		m_PointDisplacements = pointDisplacements;
    }

    ~UpdateBoundaryConditionsEvent(){}

    helper::vector< Coord3d > m_PointDisplacements;
    virtual const char* getClassName() const { return "UpdateBoundaryConditions"; }
protected:

};


//
//class ReadInputDataEvent : public sofa::core::objectmodel::Event
//{
//public:
//    ReadInputDataEvent( )
//    : sofa::core::objectmodel::Event()
//    {
//
//    }
//
//    ~ReadInputDataEvent(){ }
//
//
//    virtual const char* getClassName() const { return "ReadInputData"; }
//protected:
//
//};
//
//class PointDisplacementDataEvent : public sofa::core::objectmodel::Event
//{
//public:
//    PointDisplacementDataEvent(const void* pointDisplacements )
//    : sofa::core::objectmodel::Event()
//    {
//    	m_PointDisplacements = pointDisplacements;
//    }
//
//    ~PointDisplacementDataEvent(){ }
//
//
//    virtual const char* getClassName() const { return "ReadInputData"; }
//protected:
//    const void* m_PointDisplacements;
//};
//
//class WriteOutputDataEvent : public sofa::core::objectmodel::Event
//{
//public:
//    WriteOutputDataEvent( )
//    : sofa::core::objectmodel::Event()
//    {
//
//    }
//
//    ~WriteOutputDataEvent(){}
//
//
//    virtual const char* getClassName() const { return "WriteOutputData"; }
//protected:
//
//};

} // namespace simulation


} // namespace sofa




#endif /* __VisitorCollection_h */


