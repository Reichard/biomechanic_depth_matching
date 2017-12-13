/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, LinearTetrahedralCorotationalFEMForceField
  Author:    Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "LinearMeshMatrixMass.h"
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/defaulttype/DataTypeInfo.h>
#include <sofa/component/mass/AddMToMatrixFunctor.h>
#include <iostream>


// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
#include "MediAssistConfig.h"

namespace sofa
{

namespace component
{

namespace mass
{

// ****************************************************************************
// Constructor / Destructor
// ****************************************************************************
template <class DataTypes, class MassType>
  LinearMeshMatrixMass<DataTypes, MassType>::LinearMeshMatrixMass()
  : InternalData(new LinearMeshMatrixMassInternalData<DataTypes, MassType>)
    , m_massDensity(core::objectmodel::BaseObject::initData(&m_massDensity,(Real)1000,"massDensity",""))
    , m_isDiagonal(core::objectmodel::BaseObject::initData(&m_isDiagonal,(bool)false,"isDiag",""))
  {

  }

template <class DataTypes, class MassType>
  LinearMeshMatrixMass<DataTypes, MassType>::~LinearMeshMatrixMass()
  {

  }

// ****************************************************************************
// Public Methods
// ****************************************************************************
template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::init()
	   {
		//init superclass
		this->Inherited::init();
		m_Topology = (BaseMeshTopology*)this->getContext()->getMeshTopology();
		this->reinit();

		m_PotentialEnergy = 0;

		std::cout<<"CPU Init finished\n";

	   }

template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::reinit()
	   {
		//unsigned int nElements=m_Topology->getNbTetrahedra();
		std::cout<<"init int points\n";
		InitializeIntegrationPointsAndWeights();
		std::cout<<"init derivs\n";
		InitializeShapeFunctionDerivatives();
		InitializeShapeFunctions();

		std::cout<<"init matrices\n";
		AssembleElementMatrices();
		AssembleGravityForceVector();

		std::cout<<"init diag\n";

		AssembleDiagonalForceVector();

		std::cout<<"reinit finished\n";
	   }


template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::handleTopologyChange()
  {
//    std::list<const TopologyChange *>::const_iterator itBegin=_topology->beginChange();
//    std::list<const TopologyChange *>::const_iterator itEnd=_topology->endChange();


  }



  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::clear()
  {
//    MassVector& vertexMass = *vertexMassInfo.beginEdit();
//    MassVector& edgeMass = *edgeMassInfo.beginEdit();
//    vertexMass.clear();
//    edgeMass.clear();
//    vertexMassInfo.endEdit();
//    edgeMassInfo.endEdit();
  }


  // -- Mass interface
  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::addMDx(VecDeriv& res, const VecDeriv& dx, double factor)
  {
	  //implement addMDx on a per element basis - it will be faster to just use the complete mass matrix and
	  //perform a matrix vector multiplication
	  unsigned int ndof = this->mstate->getSize();

	  if(m_isDiagonal.getValue())
	  {
		  unsigned int ndof = this->mstate->getSize();

		  for(unsigned int i=0;i<ndof;i++)
		  {
			  for(unsigned int j=0;j<3;j++)
			  {
				  Real test = (Real)factor*((dx[i])[j])*m_DiagonalForceVector[3*i+j];
				  (res[i])[j] += test;
//				  std::cout<<m_DiagonalForceVector[3*i+j]<<"\n";
//				  std::cout<<factor<<"\n";
//				  std::cout<<(dx[i])[j]<<"\n";
			  }

		  }
	  }
	  else
	  {
	  //get vector of element matrices
	  ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();

	  unsigned int nElements=this->m_Topology->getNbTetrahedra();
	  unsigned int rowPos, columnPos;
	  LinearTetraType tetraIndices;
	  ElementMassMatrixType elementMassMatrix;

//		  for(unsigned int i=0;i<ndof;i++)
//		  {
//			  for(unsigned int j=0;j<3;j++)
//			  {
//				  Real test = (Real)factor*((dx[i])[j])*m_DiagonalForceVector[3*i+j];
//				  (res[i])[j] = test;
//  //				  std::cout<<m_DiagonalForceVector[3*i+j]<<"\n";
//  //				  std::cout<<factor<<"\n";
//  //				  std::cout<<(dx[i])[j]<<"\n";
//			  }
//
//		  }

		if (factor == 1.0)
		{
			//loop over all elements
		  for (unsigned int i=0;i<nElements;i++)
		  {
			  tetraIndices = this->m_Topology->getTetra(i);
			  elementMassMatrix = elementMassMatrixVector[i];

			  //go through the element matrix line by line
			  for(unsigned int m=0; m<4;m++)
				{
				  rowPos = tetraIndices[m];

				  for(unsigned int n=0; n<4;n++)
					{
					  columnPos =  tetraIndices[n];
					  //for each spatial direction j
					  for(unsigned int j=0;j<3;j++)
					  {
						  (res[rowPos])[j] += (dx[columnPos])[j]* elementMassMatrix[m][n];
//						  (res[rowPos])[j] += (dx[rowPos])[j]* elementMassMatrix[m][n];

					  }
					}
				}
		  }

		}
		else
		{
			//loop over all elements
				  for (unsigned int i=0;i<nElements;i++)
				  {
					  tetraIndices = this->m_Topology->getTetra(i);
					  elementMassMatrix = elementMassMatrixVector[i];

					  //go through the element matrix line by line
					  for(unsigned int m=0; m<4;m++)
						{
						  rowPos = tetraIndices[m];

						  for(unsigned int n=0; n<4;n++)
							{
							  columnPos = tetraIndices[n];
							  //for each spatial direction j
							  for(unsigned int j=0;j<3;j++)
							  {
								  (res[rowPos])[j] += (dx[columnPos])[j] * elementMassMatrix[m][n]* (Real)factor;
//								  (res[rowPos])[j] += (dx[rowPos])[j]* elementMassMatrix[m][n]* (Real)factor;
							  }
							}
						}
				  }
		}

		 //sum force to get energy

		 m_PotentialEnergy = 0;
		 unsigned int nNodes = m_Topology->getNbPoints();

		for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
		{
			m_PotentialEnergy += res[iterNode].norm();
		}

//		//loop over all elements
//						for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//						{
//						  elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
//						  tetraIndices = m_Topology->getQuadTetrahedron(iterElement);
//
//						  for(unsigned int m=0; m<10;m++)
//						  {
//							  for(unsigned int n=0; n<3;n++)
//								{
//								  int posColumn = tetraIndices[m];
//								  int posColumnElement = 3*m+n;
//
//								  for(unsigned int i=0; i<10;i++)
//								  {
//									  for(unsigned int j=0; j<3;j++)
//									  {
//										  int posRow = tetraIndices[i];
//										  int posRowElement = 3*i+j;
//										  (df[posRow])[j] += (x[posColumn])[n]* elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor;
//		//								  (df[posRow])[j] += ((x[posColumn])[n]+(*X0)[posColumn][n])* elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor;
//									  }
//								  }
//								}
//							}
//						  }
	  }

  }



  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::accFromF(VecDeriv& a, const VecDeriv& f)
  {
    (void)a;
    (void)f;
    serr << "WARNING: the methode 'accFromF' can't be used with LinearMeshMatrixMass as this mass matrix can't be inversed easily." << sendl;
    return;
  }


  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v)
  {
	GravityForceVectorType  gravityForceVector = this->m_GravityForceVector.getValue();

	Vec3d g ( this->getContext()->getGravity() );
	  Deriv theGravity;
	  DataTypes::set ( theGravity, g[0], g[1], g[2]);
	//	 	  double testa = g[0];
	//	 	 double testb = g[1];
	//	 	double testc = g[2];

	// add weight
	for (unsigned int i=0; i<f.size(); ++i)
	{
		 f[i]+= gravityForceVector[i];
	}




  }


  template <class DataTypes, class MassType>
  	double LinearMeshMatrixMass<DataTypes, MassType>::getPotentialEnergy(const sofa::core::MechanicalParams* mparams) const
  	   {

  			return m_PotentialEnergy;
  	   }



    template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::addGravityToV(double dt)
  {

  }




  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::addMToMatrix(defaulttype::BaseMatrix * mat, double mFact, unsigned int &offset)
  {

	unsigned int nElements=m_Topology->getNbTetrahedra();

	ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();
	ElementMassMatrixType elementMassMatrix;

	LinearTetraType tetraIndices;

	unsigned int posRow, posColumn;

	  unsigned int ndof = this->mstate->getSize();

	  if(m_isDiagonal.getValue())
	  {
		  unsigned int ndof = this->mstate->getSize();

		  for(unsigned int i=0;i<ndof;i++)
		  {
			  for(unsigned int j=0;j<3;j++)
			  {

				  mat->add(3*i+j, 3*i+j, m_DiagonalForceVector[3*i+j]*mFact);

			  }

		  }
	  }
	  else
	  {

			for(unsigned int i=0; i<nElements;i++)
			{
			  elementMassMatrix = elementMassMatrixVector[i];
			  tetraIndices = this->m_Topology->getTetra(i);

			  for(unsigned int m=0; m<4;m++)
				{
				  for(unsigned int n=0; n<4;n++)
					{
					  posRow = 3*tetraIndices[m];
					  posColumn = 3*tetraIndices[n];

					  for(unsigned int j=0;j<3;j++)
					  {
						  mat->add(posRow+j, posColumn+j, elementMassMatrix[m][n]*mFact);

					  }
					}
				}


			}
	  }

	//	this->WriteMatrixToCVSFile(mat, "MassMatrixQuadTetra.csv");

  }




  template <class DataTypes, class MassType>
  double LinearMeshMatrixMass<DataTypes, MassType>::getElementMass(unsigned int index) const
  {
	 return 1.0;
  }


  //TODO: special case for Rigid Mass
  template <class DataTypes, class MassType>
  void LinearMeshMatrixMass<DataTypes, MassType>::getElementMass(unsigned int index, defaulttype::BaseMatrix *m) const
  {


  }

// ****************************************************************************
// Protected/Private Methods
// ****************************************************************************
  template <class DataTypes, class MassType>
    void LinearMeshMatrixMass<DataTypes, MassType>::AssembleElementMatrices()
    {
     // unsigned int ndof = this->mstate->getSize();
      unsigned int nElements=this->m_Topology->getNbTetrahedra();

      ElementMassMatrixVectorType & elementMassMatrixVector = *this->m_ElementMassMatrixVector.beginEdit();

      elementMassMatrixVector.resize(nElements);

      ElementMassMatrixType elementMassMatrix;

      for(unsigned int i=0; i<nElements;i++)
      {
    	  this->BuildElementMatrix(elementMassMatrix, i);

    	  elementMassMatrixVector[i] = elementMassMatrix;
      }

      this->m_ElementMassMatrixVector.endEdit();

    }

  template <class DataTypes, class MassType>
    void LinearMeshMatrixMass<DataTypes, MassType>::BuildElementMatrix(ElementMassMatrixType & elementMatrix, unsigned int elementID)
    {
		  const VecCoord *X0=this->mstate->getX0();

		  const LinearTetraType tetraIndices = m_Topology->getTetrahedron(elementID);

		  helper::fixed_array<Coord,4> initialPositions;

		  for(unsigned int i=0; i<4;i++)
		  {
			  initialPositions[i] = (*X0)[tetraIndices[i]];
		  }

		  double result=0;

		  for(unsigned int p=0;p<4;p++)
		  {
			  for(unsigned int q=0;q<4;q++)
			  {
				  result = 0;

				  //sum over all integration points
				  for(unsigned int i=0;i<numberOfIntegrationPoints;i++)
				  {
					  //calculate determinant
						//debugging
					  double test1 = m_IntWeights[i];
					  double test2 = (this->m_massDensity.getValue());
					  double test3 = (this->m_ShapeFunctions[i])[p];
					  double test4 = (this->m_ShapeFunctions[i])[q];
					  double test5 = GetJacobianDeterminant(initialPositions,i);
					  result += test1*test2*test3*test4*test5;

					  /////////////////////////////////////////////////////////
					  //result += m_IntWeights[i]*(this->m_massDensity.getValue())*(this->m_ShapeFunctions[i])[p]*(this->m_ShapeFunctions[i])[q]*GetJacobianDeterminant(initialPositions,i);
				  }

				  elementMatrix[q][p] = result;
			  }
		  }

//		  this->WriteElementMatrixToCVSFile(elementMatrix, "ElementMatrixNo_",elementID);

    }

  template <class DataTypes, class MassType>
      void LinearMeshMatrixMass<DataTypes, MassType>::AssembleGravityForceVector()
	{

	  //m_massDensity.setValue(5.0);

	   unsigned int ndof = this->mstate->getSize();
	   unsigned int nElements=this->m_Topology->getNbTetrahedra();

	   GravityForceVectorType & gravityForceVector = *this->m_GravityForceVector.beginEdit();

	   gravityForceVector.resize(ndof);



	   LinearTetraType tetraIndices;

	   for(unsigned int i=0; i<nElements;i++)
	   {
		  tetraIndices = m_Topology->getTetra(i);

		  helper::fixed_array<Deriv,4>  elementGravityVector;

		  this->BuildElementGravityForceVector(elementGravityVector, i);

		  for(unsigned int p=0;p<4;p++)
		  {
			  gravityForceVector[tetraIndices[p]] += elementGravityVector[p];
		  }

	   }



	  // this->WriteVectorToMatFile(gravityForceVector, "CompleteGravityForceVector");

	   this->m_GravityForceVector.endEdit();
	}

  template <class DataTypes, class MassType>
        void LinearMeshMatrixMass<DataTypes, MassType>::AssembleDiagonalForceVector()
  	{

  	  //m_massDensity.setValue(5.0);

  	   unsigned int ndof = this->mstate->getSize();
  	   unsigned int nElements=this->m_Topology->getNbTetrahedra();

  	  this->m_DiagonalForceVector.resize(3*ndof);

//  	  std::cout<<"dofs are "<<ndof<<"\n";

	  ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();

		  unsigned int rowPos, columnPos;
	  LinearTetraType tetraIndices;
	  ElementMassMatrixType elementMassMatrix;

//	  std::cout<<ndof<<"\n";
		//loop over all elements
	  for (unsigned int i=0;i<nElements;i++)
	  {
		  tetraIndices = this->m_Topology->getTetra(i);
		  elementMassMatrix = elementMassMatrixVector[i];

		  //go through the element matrix line by line
		  for(unsigned int m=0; m<4;m++)
			{
			  rowPos = tetraIndices[m];

			  for(unsigned int j=0;j<3;j++)
			  {
				  for(unsigned int n=0; n<4;n++)
					{


					  m_DiagonalForceVector[3*rowPos+j] += elementMassMatrix[m][n];


				  }
//				  std::cout<<m_DiagonalForceVector[3*rowPos+j]<<"\n";
				}
			}
	  }


  	}


  template <class DataTypes, class MassType>
      void LinearMeshMatrixMass<DataTypes, MassType>::BuildElementGravityForceVector(helper::fixed_array<Deriv,4>  & elementGravityVector, unsigned int elementID)
	{


	  const VecCoord *X0=this->mstate->getX0();

	  const LinearTetraType tetraIndices = m_Topology->getTetrahedron(elementID);

	  helper::fixed_array<Coord,4> initialPositions;

	  for(unsigned int i=0; i<4;i++)
	  {
		  initialPositions[i] = (*X0)[tetraIndices[i]];
	  }

	  Deriv result;

	  result.clear();

//	  double test1 = this->m_massDensity.getValue();
//	  double test2 = this->m_massDensity.getValue();


	  // gravity
	  Vec3d g ( this->getContext()->getGravity() );
	  Deriv theGravity;
	  DataTypes::set ( theGravity, g[0], g[1], g[2]);

	  //sum over all integration points
	  for(unsigned int i=0;i<numberOfIntegrationPoints;i++)
	  {

	  for(unsigned int p=0;p<4;p++)
	  {
		  result.clear();

		  for(unsigned int j=0;j<3;j++)
		  {
//				  double test3 = GetJacobianDeterminant(initialPositions,i);
//				  double shapeFunctionValue = (this->m_ShapeFunctions[i])[p];
//
//				  double shapeFunctionValue1 = (this->m_ShapeFunctions[i])[0];
//				  double shapeFunctionValue2 = (this->m_ShapeFunctions[i])[1];
//				  double shapeFunctionValue3 = (this->m_ShapeFunctions[i])[2];
//				  double shapeFunctionValue4 = (this->m_ShapeFunctions[i])[3];
//				  double shapeFunctionValue5 = (this->m_ShapeFunctions[i])[4];
//				  double shapeFunctionValue6 = (this->m_ShapeFunctions[i])[5];
//				  double shapeFunctionValue7 = (this->m_ShapeFunctions[i])[6];
//				  double shapeFunctionValue8 = (this->m_ShapeFunctions[i])[7];
//				  double shapeFunctionValue9 = (this->m_ShapeFunctions[i])[8];
//				  double shapeFunctionValue10 = (this->m_ShapeFunctions[i])[9];
//				  double resultTemp = (1.0/6.0)*m_IntWeights[i]*theGravity[j]*(this->m_massDensity.getValue())*shapeFunctionValue*GetJacobianDeterminant(initialPositions,i);
				  result[j] = m_IntWeights[i]*theGravity[j]*(this->m_massDensity.getValue())*(this->m_ShapeFunctions[i])[p]*GetJacobianDeterminant(initialPositions,i);
			}

		  (elementGravityVector[p]) += result;
//		  Deriv testDeriv;
//		  testDeriv = elementGravityVector[p];
//		  double testD1 = testDeriv[0];
//		  double testD2 = testDeriv[1];
//		  double testD3 = testDeriv[2];
	  }

	 // this->WriteElementVectorToMatFile(elementGravityVector, "ElementGravityForceVectorIntPoint", 10*elementID+i);

	}

	//  this->WriteElementVectorToMatFile(elementGravityVector, "ElementGravityForceVector", elementID);

}



template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::InitializeShapeFunctionDerivatives()
	   {

		MatNoInit<4,3,Real> currentShapeFunctionDerivatives;
		Coord currentIntegrationPoint;
		double r,s,t, tempExp;
		//iterate over all integration points
		for(unsigned int i=0; i< numberOfIntegrationPoints;i++)
		{
		  DataTypes::set ( currentIntegrationPoint, m_IntPoints[3*i], m_IntPoints[3*i+1], m_IntPoints[3*i+2]);
		  r=currentIntegrationPoint[0];
		  s=currentIntegrationPoint[1];
		  t=currentIntegrationPoint[2];

		  tempExp = 1-r-s-t;


		  currentShapeFunctionDerivatives[0][0] = 1;
		  currentShapeFunctionDerivatives[0][1] = 0;
		  currentShapeFunctionDerivatives[0][2] = 0;

		  currentShapeFunctionDerivatives[1][0] = 0;
		  currentShapeFunctionDerivatives[1][1] = 1;
		  currentShapeFunctionDerivatives[1][2] = 0;

		  currentShapeFunctionDerivatives[2][0] = -1;
		  currentShapeFunctionDerivatives[2][1] = -1;
		  currentShapeFunctionDerivatives[2][2] = -1;

		  currentShapeFunctionDerivatives[3][0] = 0;
		  currentShapeFunctionDerivatives[3][1] = 0;
		  currentShapeFunctionDerivatives[3][2] = 1;

		  m_ShapeFunctionDerivatives[i] = currentShapeFunctionDerivatives;

		}


	   }

template <class DataTypes, class MassType>
	  void LinearMeshMatrixMass<DataTypes, MassType>::InitializeShapeFunctions()
	   {
			m_ShapeFunctions.resize(numberOfIntegrationPoints);

		  helper::fixed_array<Real,4> currentShapeFunctions;
		  Coord currentIntegrationPoint;
		  double r,s,t, tempExp;
		  //iterate over all integration points
		  for(unsigned int i=0; i< numberOfIntegrationPoints;i++)
		  {
			  DataTypes::set ( currentIntegrationPoint, m_IntPoints[3*i], m_IntPoints[3*i+1], m_IntPoints[3*i+2]);
			  r=currentIntegrationPoint[0];
			  s=currentIntegrationPoint[1];
			  t=currentIntegrationPoint[2];

			  tempExp = 1-r-s-t;

			  currentShapeFunctions[0] = r;
			  currentShapeFunctions[1] = s;
			  currentShapeFunctions[2] = tempExp;
			  currentShapeFunctions[3] = t;

			  m_ShapeFunctions[i] = currentShapeFunctions;
		  }

	   }

template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::InitializeIntegrationPointsAndWeights()
	   {
		double alpha1 = 0.58541020;
		double beta1 = 0.13819660;

		double alpha2 = 0.785714285714286;
		double beta2 = 0.071428571428571;

		double alpha3 = 0.399403576166799;
		double  beta3 = 0.100596423833201;

		double intPoints[33];
		double intWeights[11];

		switch(numberOfIntegrationPoints){
			  case 1:
				  intPoints[0] = 0.25;
				  intPoints[1] = 0.25;
				  intPoints[2] = 0.25;
				  intWeights[0] = (double)1.0/(double)6.0;
				  break;
			  case 4:
				  intPoints[0] = alpha1;
				  intPoints[1] = beta1;
				  intPoints[2] = beta1;
				  intPoints[3] = beta1;
				  intPoints[4] = alpha1;
				  intPoints[5] = beta1;
				  intPoints[6] = beta1;
				  intPoints[7] = beta1;
				  intPoints[8] = alpha1;
				  intPoints[9] = beta1;
				  intPoints[10] = beta1;
				  intPoints[11] = beta1;
				  intWeights[0] = 0.25;
				  intWeights[1] = 0.25;
				  intWeights[2] = 0.25;
				  intWeights[3] = 0.25;
				  break;
			  case 5:
				  intPoints[0] = 0.25;
				  intPoints[1] = 0.25;
				  intPoints[2] = 0.25;
				  intPoints[3] = 0.5;
				  intPoints[4] = 1.0/6.0;
				  intPoints[5] = 1.0/6.0;
				  intPoints[6] = 1.0/6.0;
				  intPoints[7] = 0.5;
				  intPoints[8] = 1.0/6.0;
				  intPoints[9] = 1.0/6.0;
				  intPoints[10] = 1.0/6.0;
				  intPoints[11] = 0.5;
				  intPoints[12] = 1.0/6.0;
				  intPoints[13] = 1.0/6.0;
				  intPoints[14] = 1.0/6.0;
				  intWeights[0] = -0.8;
				  intWeights[1] = 9.0/20.0;
				  intWeights[2] = 9.0/20.0;
				  intWeights[3] = 9.0/20.0;
				  intWeights[4] = 9.0/20.0;
			  		  break;
			  case 11:
				  intPoints[0] = 0.25;
                  intPoints[1] = 0.25;
                  intPoints[2] = 0.25;
				  intPoints[3] = alpha2;
				  intPoints[4] = beta2;
				  intPoints[5] = beta2;
				  intPoints[6] = beta2;
				  intPoints[7] = alpha2;
				  intPoints[8] = beta2;
				  intPoints[9] = beta2;
				  intPoints[10] = beta2;
				  intPoints[11] = alpha2;
				  intPoints[12] = beta2;
				  intPoints[13] = beta2;
				  intPoints[14] = beta2;
				  intPoints[15] = alpha3;
				  intPoints[16] = alpha3;
				  intPoints[17] = beta3;
				  intPoints[18] = alpha3;
				  intPoints[19] = beta3;
				  intPoints[20] = alpha3;
				  intPoints[21] = alpha3;
				  intPoints[22] = beta3;
				  intPoints[23] = beta3;
				  intPoints[24] = beta3;
				  intPoints[25] = alpha3;
				  intPoints[26] = alpha3;
				  intPoints[27] = beta3;
				  intPoints[28] = alpha3;
				  intPoints[29] = beta3;
				  intPoints[30] = beta3;
				  intPoints[31] = beta3;
				  intPoints[32] = alpha3;
				  intWeights[0] = -0.013133333333336;
				  intWeights[1] = 0.007622222222222;
				  intWeights[2] = 0.007622222222222;
				  intWeights[3] = 0.007622222222222;
				  intWeights[4] = 0.007622222222222;
				  intWeights[5] = 0.24888888888889;
				  intWeights[6] = 0.24888888888889;
				  intWeights[7] = 0.24888888888889;
				  intWeights[8] = 0.24888888888889;
				  intWeights[9] = 0.24888888888889;
				  intWeights[10] = 0.24888888888889;
			  		  break;
			  default:
				  serr << "Wrong number of integration points" << sendl;

			  }

		for(unsigned int i=0; i<numberOfIntegrationPoints;i++)
		{
			this->m_IntWeights[i] = intWeights[i];
		}

		for(unsigned int i=0; i<(3*numberOfIntegrationPoints);i++)
		{
			this->m_IntPoints[i] = intPoints[i];
		}

	   }


template <class DataTypes, class MassType>
  double LinearMeshMatrixMass<DataTypes, MassType>::GetJacobianDeterminant(helper::fixed_array<Coord,4> coords, int indexOfIntegrationPoint)
  {

	  Mat<4,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivatives[indexOfIntegrationPoint];
	  MatNoInit<3,3,Real> jacobianMatrix;
	  jacobianMatrix.clear();

	  double test1,test2,test3,test4;
	  //build jacobian matrix
	  for(int m=0;m<3;m++)
	  {
		  for(int n=0;n<3;n++)
		  {
			  //sum over all shape functions
			  for(int i=0;i<4;i++)
			  {
				  test1 = (coords[i])[m];
				  test2 =  currentShapeFunctionDerivatives[i][n];
				  jacobianMatrix[m][n] += test1*test2; //(coords[i])[n]*currentShapeFunctionDerivatives[i][m];
			  }

			  //just4debug
			  test3 = jacobianMatrix[m][n];
			  test4=test3;

		  }
	  }

	  return sofa::defaulttype::determinant(jacobianMatrix);

  }

template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename)
	   {
		//open file and generate textstream
		QString completeFilename(MEDIASSIST_DATA_PATH);
		completeFilename += "Temp/";
		completeFilename += filename;

		QFile file(completeFilename);

		 if (!file.open(QFile::WriteOnly | QFile::Truncate))
			 return ;

		 QTextStream out(&file);

		 //matrix line by line
		 unsigned int rowSize = mat->rowSize();
		 unsigned int columnSize = mat->colSize();

		 for(unsigned int m=0;m<rowSize;m++)
		 {
			 for(unsigned int n=0;n<columnSize;n++)
			 {
				 out << mat->element(m,n);
				 if(n!=(columnSize-1))
					 out<<",";
			 }
			 out<<"\n";
		 }

	   }

template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::WriteElementMatrixToCVSFile(Mat<12,12,Real>  mat,char* filename, int elementNumber)
	   {
		//open file and generate textstream
		QString completeFilename("/home/mediassi/MediAssistData/");
		//QString elementNumberString(elementNumber);
		completeFilename += "Temp/";
		completeFilename += filename;
		completeFilename += QString::number(elementNumber);
		completeFilename +=".csv";

		QFile file(completeFilename);

		 if (!file.open(QFile::WriteOnly | QFile::Truncate))
			 return ;

		 QTextStream out(&file);

		 //matrix line by line
		 unsigned int rowSize = 12;
		 unsigned int columnSize = 12;

		 for(unsigned int m=0;m<rowSize;m++)
		 {
			 for(unsigned int n=0;n<columnSize;n++)
			 {
				 out << mat[m][n];
				 if(n!=(columnSize-1))
					 out<<",";
			 }
			 out<<"\n";
		 }

	   }

template <class DataTypes, class MassType>
	   void LinearMeshMatrixMass<DataTypes, MassType>::WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber)
	   {
		//open file and generate textstream
		QString completeFilename("/home/mediassi/MediAssistData/");
		completeFilename += "Temp/";
		completeFilename += filename;
		completeFilename += QString::number(elementNumber);
		completeFilename +=".csv";

		QFile file(completeFilename);

		 if (!file.open(QFile::WriteOnly | QFile::Truncate))
			 return ;

		 QTextStream out(&file);

		 //matrix line by line
		 unsigned int rowSize = 26;
		 unsigned int columnSize = 3;

		 for(unsigned int m=0;m<rowSize;m++)
		 {
			 for(unsigned int n=0;n<columnSize;n++)
			 {
				 out << vec[m][n];
				 if(n!=(columnSize-1))
					 out<<",";
			 }
			 out<<"\n";
		 }

	   }

} // end namespace forcefield
} //end namespace component
} //end namespace sofa


