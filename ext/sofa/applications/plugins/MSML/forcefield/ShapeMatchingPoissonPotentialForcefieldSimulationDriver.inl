/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingPoissonPotentialForcefieldSimulationDriver
  Author:    Tino Milschewski

=========================================================================*/

#include "ShapeMatchingPoissonPotentialForcefieldSimulationDriver.h"
#include "DirichletBoundaryConstraint.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"

#include <sofa/component/forcefield/ConstantForceField.h>

namespace sofa
{

namespace component
{

namespace collision
{

template <class DataTypes>
	ShapeMatchingPoissonPotentialForcefieldSimulationDriver<DataTypes>::ShapeMatchingPoissonPotentialForcefieldSimulationDriver(void) :
	m_UndeformedMeshFilename(initData(&m_UndeformedMeshFilename,std::string("unnamed"),"undeformedMeshFilename","name of the undeformed vtk mesh"))
	{

	}

template <class DataTypes>
	void ShapeMatchingPoissonPotentialForcefieldSimulationDriver<DataTypes>::init()
	{
		sofa::component::projectiveconstraintset::DirichletBoundaryConstraint<DataTypes> * constraint;
		this->getContext()->get(constraint);

		sofa::component::forcefield::ConstantForceField<sofa::defaulttype::Vec3dTypes> *  constantForcefield;
		this->getContext()->get(constantForcefield);

		if(constraint == 0)
			std::cerr<<"SimulationDriver: No DirichletBoundaryConstraint module found!\n"<<std::endl;

		if(constantForcefield == 0)
			std::cerr<<"SimulationDriver: No constant forcefield module found!\n"<<std::endl;

//		this->getContext()->getMechanicalState()->

		vtkUnstructuredGridReader * vtkReader = vtkUnstructuredGridReader::New();
		vtkReader->SetFileName(m_UndeformedMeshFilename.getValue().c_str());
		vtkReader->Update();
		vtkUnstructuredGrid * undeformedMesh = vtkReader->GetOutput();

		double bounds[6];
		undeformedMesh->GetBounds(bounds);
		double yMin = bounds[2];
		double yMax = bounds[3];

		double point[3];
		Vec<3, Real> force;
		for(int i = 0; i < undeformedMesh->GetNumberOfPoints(); i++)
		{
			undeformedMesh->GetPoint(i, point);

			if(isInEpsilon(point[1], yMin, 1.0))
			{
				constraint->addMarkerDisplacementBC(i, Coord(0, 0, 0));
			}

//			if(isInEpsilon(point[1], yMax, 10.0))
//			{
//				force.set(0.0, -100000.0, 0);
//				constantForcefield->setForce(i, force);
//			}
		}
	}

template <class DataTypes>
	bool ShapeMatchingPoissonPotentialForcefieldSimulationDriver<DataTypes>::isInEpsilon(Real value, Real valueToBe, Real epsilon)
	{
		if(fabs(valueToBe - value) < epsilon)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

}

}
