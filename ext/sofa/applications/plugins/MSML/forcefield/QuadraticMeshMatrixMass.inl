/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
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
#ifndef SOFA_COMPONENT_MASS_QuadraticMeshMatrixMass_INL
#define SOFA_COMPONENT_MASS_QuadraticMeshMatrixMass_INL

#include "QuadraticMeshMatrixMass.h"
#include <sofa/helper/gl/template.h>
#include <sofa/defaulttype/DataTypeInfo.h>
//#include <sofa/component/topology/TopologyChangedEvent.h>
//#include <sofa/component/topology/PointData.inl>
//#include <sofa/component/topology/EdgeData.inl>
#include <sofa/component/topology/RegularGridTopology.h>
#include <sofa/component/mass/AddMToMatrixFunctor.h>
#include <sofa/simulation/common/Simulation.h>


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


using namespace	sofa::component::topology;
using namespace core::topology;
using namespace sofa::defaulttype;
using namespace sofa::core::behavior;


  template <class DataTypes, class MassType>
  QuadraticMeshMatrixMass<DataTypes, MassType>::QuadraticMeshMatrixMass()
  : InternalData(new QuadraticMeshMatrixMassInternalData<DataTypes, MassType>)
    , m_massDensity(core::objectmodel::BaseObject::initData(&m_massDensity,(Real)1000,"massDensity",""))
    , m_isDiagonal(core::objectmodel::BaseObject::initData(&m_isDiagonal,(bool)false,"isDiag",""))
  {
	  //m_massDensity.setValue(1070);
  }


  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::init()
  {
	numberOfIntegrationPoints = 5;//11;

    this->Inherited::init();
    
    m_Topology = (QuadraticBaseMeshTopology*)this->getContext()->getMeshTopology();
    
    this->reinit();

  }

  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::reinit()
  {

	  InitializeIntegrationPointsAndWeights(numberOfIntegrationPoints);
	  InitializeShapeFunctionDerivatives(numberOfIntegrationPoints);
	  InitializeShapeFunctions(numberOfIntegrationPoints);


	  AssembleElementMatrices();
	  AssembleGravityForceVector();

	  AssembleDiagonalForceVector();


 }


  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::handleTopologyChange()
  {
    std::list<const TopologyChange *>::const_iterator itBegin=m_Topology->beginChange();
    std::list<const TopologyChange *>::const_iterator itEnd=m_Topology->endChange();


  }


  
  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::clear()
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
  void QuadraticMeshMatrixMass<DataTypes, MassType>::addMDx(VecDeriv& res, const VecDeriv& dx, double factor)
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
			  }

		  }
	  }
	  else
	  {
	  //get vector of element matrices
	  ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();

	  unsigned int nElements=m_Topology->getNbQuadTetrahedra();
	  unsigned int rowPos, columnPos;
	  QuadraticTetraType tetraIndices;
	  ElementMassMatrixType elementMassMatrix;

		if (factor == 1.0)
		{
			//loop over all elements
		  for (unsigned int i=0;i<nElements;i++)
		  {
			  tetraIndices = m_Topology->getQuadTetrahedron(i);
			  elementMassMatrix = elementMassMatrixVector[i];

			  //go through the element matrix line by line
			  for(unsigned int m=0; m<10;m++)//TODO:10 befor was 4
				{
				  rowPos = tetraIndices[m];

				  for(unsigned int n=0; n<10;n++)//TODO:10
					{
					  columnPos =  tetraIndices[n];
					  //for each spatial direction j
					  for(unsigned int j=0;j<3;j++)
					  {
						  (res[rowPos])[j] += (dx[columnPos])[j]* elementMassMatrix[m][n];

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
					  tetraIndices = m_Topology->getQuadTetrahedron(i);
					  elementMassMatrix = elementMassMatrixVector[i];

					  //go through the element matrix line by line
					  for(unsigned int m=0; m<10;m++)
						{
						  rowPos = tetraIndices[m];

						  for(unsigned int n=0; n<10;n++)
							{
							  columnPos = tetraIndices[n];
							  //for each spatial direction j
							  for(unsigned int j=0;j<3;j++)
							  {
								  (res[rowPos])[j] += (dx[columnPos])[j] * elementMassMatrix[m][n]* (Real)factor;

							  }
							}
						}
				  }
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


//		printf("\nPrint result of AddMdx\n");
//		{
//			for(unsigned int i=0;i<res.size();i++)
//			{
//				for(int j=0; j<3; j++)
//				{
//					printf("%f  ", res[i][j]);
//				}
//			printf("@ tetraIndex:%d\n", i);//end tetrahedron
//			}
//		}
//		printf("\n");// endelement
	  }

  }



  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::accFromF(VecDeriv& a, const VecDeriv& f)
  {
    (void)a;
    (void)f;
    serr << "WARNING: the methode 'accFromF' can't be used with QuadraticMeshMatrixMass as this mass matrix can't be inversed easily." << sendl;
    return;
  }


  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v)
  {
	 // in this method we don't handle inertia forces (handled differently in MeshMatrixMass)

	  //if gravity was added separately (in solver's "solve" method), then nothing to do here
//	  if(this->m_separateGravity.getValue())
//		return;

	  GravityForceVectorType  gravityForceVector = this->m_GravityForceVector.getValue();

	  //unused variables, formerly used for debugging
//	  int test = gravityForceVector.size();
//	  int test2 = f.size();

	 // Deriv theMass;
//	  double mass = 0.00005;
	 // DataTypes::set ( theMass, mass, mass, mass);

	  Vec3d g ( this->getContext()->getGravity() );
	 	  Deriv theGravity;
	 	  DataTypes::set ( theGravity, g[0], g[1], g[2]);
	 	  double testa = g[0];
	 	 double testb = g[1];
	 	double testc = g[2];

	  // add weight
	   for (unsigned int i=0; i<f.size(); ++i)
	   {
	         //double test = gravityForceVector[i];
	         f[i]+= gravityForceVector[i];
		   //f[i]+= theGravity*mass;
	   }


  }

  
  template <class DataTypes, class MassType>
  double QuadraticMeshMatrixMass<DataTypes, MassType>::getPotentialEnergy( const VecCoord& x ) const
  {


    return 0.0;
  }



    template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::addGravityToV(double dt)
  {

  }


  

  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::addMToMatrix(defaulttype::BaseMatrix * mat, double mFact, unsigned int &offset)
  {



			unsigned int nElements=m_Topology->getNbQuadTetrahedra();

			ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();

			ElementMassMatrixType elementMassMatrix;

			QuadraticTetraType tetraIndices;

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
					  tetraIndices = m_Topology->getQuadTetrahedron(i);

					  for(unsigned int m=0; m<10;m++)
						{
						  for(unsigned int n=0; n<10;n++)
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
  double QuadraticMeshMatrixMass<DataTypes, MassType>::getElementMass(unsigned int index) const
  {
//     const MassVector &vertexMass= vertexMassInfo.getValue();
//     double mass = vertexMass[index] * massLumpingCoeff;
//
//     return mass;
	 return 1.0;
  }


  //TODO: special case for Rigid Mass
  template <class DataTypes, class MassType>
  void QuadraticMeshMatrixMass<DataTypes, MassType>::getElementMass(unsigned int index, defaulttype::BaseMatrix *m) const
  {

//    const unsigned int dimension = defaulttype::DataTypeInfo<Deriv>::size();
//    if (m->rowSize() != dimension || m->colSize() != dimension) m->resize(dimension,dimension);
//
//    m->clear();
//    AddMToMatrixFunctor<Deriv,MassType>()(m, vertexMassInfo.getValue()[index] * massLumpingCoeff, 0, 1);
  }


  template <class DataTypes, class MassType>
    void QuadraticMeshMatrixMass<DataTypes, MassType>::AssembleElementMatrices()
    {
     // unsigned int ndof = this->mstate->getSize();
      unsigned int nElements=m_Topology->getNbQuadTetrahedra();

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
    void QuadraticMeshMatrixMass<DataTypes, MassType>::BuildElementMatrix(ElementMassMatrixType & elementMatrix, unsigned int elementID)
    {
		  const VecCoord *X0=this->mstate->getX0();

//		  int test1 = (*X0).size();//unused
		  const QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(elementID);

		  helper::fixed_array<Coord,10> initialPositions;

		  for(unsigned int i=0; i<10;i++)
		  {
			 // int test = tetraIndices[i];
			 // Coord test2 = (*X0)[test];
			  initialPositions[i] = (*X0)[tetraIndices[i]];
		  }

		  double result=0;

		  for(unsigned int p=0;p<10;p++)
		  {
			  for(unsigned int q=0;q<10;q++)
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
      void QuadraticMeshMatrixMass<DataTypes, MassType>::AssembleGravityForceVector()
	{

	  //m_massDensity.setValue(5.0);

	   unsigned int ndof = this->mstate->getSize();
	   unsigned int nElements=m_Topology->getNbQuadTetrahedra();

	   GravityForceVectorType & gravityForceVector = *this->m_GravityForceVector.beginEdit();

	   gravityForceVector.resize(ndof);



	   QuadraticTetraType tetraIndices;

	   for(unsigned int i=0; i<nElements;i++)
	   {
		  tetraIndices = m_Topology->getQuadTetrahedron(i);

		  helper::fixed_array<Deriv,10>  elementGravityVector;

		  this->BuildElementGravityForceVector(elementGravityVector, i);

		  for(unsigned int p=0;p<10;p++)
		  {
			  gravityForceVector[tetraIndices[p]] += elementGravityVector[p];
		  }

	   }



	  // this->WriteVectorToMatFile(gravityForceVector, "CompleteGravityForceVector");

	   this->m_GravityForceVector.endEdit();
	}

  template <class DataTypes, class MassType>
        void QuadraticMeshMatrixMass<DataTypes, MassType>::AssembleDiagonalForceVector()
  	{

  	  //m_massDensity.setValue(5.0);

  	   unsigned int ndof = this->mstate->getSize();
  	   unsigned int nElements=m_Topology->getNbQuadTetrahedra();

 	  ElementMassMatrixVectorType  elementMassMatrixVector = this->m_ElementMassMatrixVector.getValue();

  	  unsigned int rowPos, columnPos;
 	  QuadraticTetraType tetraIndices;
 	  ElementMassMatrixType elementMassMatrix;


  	   m_DiagonalForceVector.resize(3*ndof);


  		//loop over all elements
		  for (unsigned int i=0;i<nElements;i++)
		  {
			  tetraIndices = m_Topology->getQuadTetrahedron(i);
			  elementMassMatrix = elementMassMatrixVector[i];

			  //go through the element matrix line by line
			  for(unsigned int m=0; m<10;m++)//TODO:10 befor was 4
				{
				  rowPos = tetraIndices[m];

				  for(unsigned int n=0; n<10;n++)//TODO:10
					{

					  for(unsigned int j=0;j<3;j++)
					  {
						   m_DiagonalForceVector[3*rowPos+j] += elementMassMatrix[m][n];

					  }
					}
				}
		  }


  	}


  template <class DataTypes, class MassType>
      void QuadraticMeshMatrixMass<DataTypes, MassType>::BuildElementGravityForceVector(helper::fixed_array<Deriv,10>  & elementGravityVector, unsigned int elementID)
	{


	  const VecCoord *X0=this->mstate->getX0();

	  const QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(elementID);

	  helper::fixed_array<Coord,10> initialPositions;

	  for(unsigned int i=0; i<10;i++)
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

	  for(unsigned int p=0;p<10;p++)
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
    double QuadraticMeshMatrixMass<DataTypes, MassType>::GetJacobianDeterminant(helper::fixed_array<Coord,10> coords, int indexOfIntegrationPoint)
    {

	  Mat<10,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivatives[indexOfIntegrationPoint];
	  MatNoInit<3,3,Real> jacobianMatrix;
	  jacobianMatrix.clear();

	  double test1,test2,test3,test4;
	  //build jacobian matrix
	  for(int m=0;m<3;m++)
	  {
		  for(int n=0;n<3;n++)
		  {
			  //sum over all shape functions
			  for(int i=0;i<10;i++)
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
  void QuadraticMeshMatrixMass<DataTypes, MassType>::InitializeShapeFunctions(unsigned int numberOfIntegrationPoints)
  {


	  m_ShapeFunctions.resize(numberOfIntegrationPoints);

	  helper::fixed_array<Real,10> currentShapeFunctions;
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
		  /*
			 * The shape functions of quadratic Tetrahedra:
			 *   N1 = (2r-1)r
			 *   N2 = (2s-1)s
			 *   N3 = (2t-1)t
			 *   N4 = [2(1-r-s-t)-1]*(1-r-s-t)
			 *   N5 = 4rs
			 *   N6 = 4st
			 *   N7 = 4rt
			 *   N8 = 4(1-r-s-t)r
			 *   N9 = 4(1-r-s-t)s
			 *   N10 = 4(1-r-s-t)t
			 *
			 *   so the corresponding derivative matrix is:
			 *            r             s             t
			 *   N1     4r-1            0             0
			 *   N2       0           4s-1            0
			 *   N3       0             0           4t-1
			 *   N4 -4(1-r-s-t)+1 -4(1-r-s-t)+1 -4(1-r-s-t)+1
			 *   N5      4s            4r             0
			 *   N6       0            4t            4s
			 *   N7      4t             0            4r
			 *   N8 4[(1-r-s-t)-r]    -4r           -4r
			 *   N9     -4s       4[(1-r-s-t)-s]    -4s
			 *   N10    -4t           -4t       4[(1-r-s-t)-t]
			 */

//		  currentShapeFunctions[0] = (2*r-1)*r;
//		  currentShapeFunctions[1] = (2*s-1)*s;
//		  currentShapeFunctions[2] = (2*t-1)*t;
//		  currentShapeFunctions[3] = (2*tempExp-1)*tempExp;
//		  currentShapeFunctions[4] = 4*r*s;
//		  currentShapeFunctions[5] = 4*s*t;
//		  currentShapeFunctions[6] = 4*r*t;
//		  currentShapeFunctions[7] = 4*tempExp*r;
//		  currentShapeFunctions[8] = 4*tempExp*s;
//		  currentShapeFunctions[9] = 4*tempExp*t;

		  currentShapeFunctions[0] = (2*r-1)*r;
		  currentShapeFunctions[1] = (2*s-1)*s;
		  currentShapeFunctions[2] = (2*tempExp-1)*tempExp;
		  currentShapeFunctions[3] = (2*t-1)*t;
		  currentShapeFunctions[4] = 4*r*s;
		  currentShapeFunctions[5] = 4*tempExp*s;
		  currentShapeFunctions[6] = 4*tempExp*r;
		  currentShapeFunctions[7] = 4*r*t;
		  currentShapeFunctions[8] = 4*s*t;
		  currentShapeFunctions[9] = 4*tempExp*t;


		  m_ShapeFunctions[i] = currentShapeFunctions;
	  }

  }

  	template <class DataTypes, class MassType>
    void QuadraticMeshMatrixMass<DataTypes, MassType>::InitializeShapeFunctionDerivatives(unsigned int numberOfIntegrationPoints)
    {

	  m_ShapeFunctionDerivatives.resize(numberOfIntegrationPoints);

	  MatNoInit<10,3,Real> currentShapeFunctionDerivatives;
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
		  /*
			 * The shape functions of quadratic Tetrahedra:
			 *   N1 = (2r-1)r
			 *   N2 = (2s-1)s
			 *   N3 = (2t-1)t
			 *   N4 = [2(1-r-s-t)-1]*(1-r-s-t)
			 *   N5 = 4rs
			 *   N6 = 4st
			 *   N7 = 4rt
			 *   N8 = 4(1-r-s-t)r
			 *   N9 = 4(1-r-s-t)s
			 *   N10 = 4(1-r-s-t)t
			 *
			 *   so the corresponding derivative matrix is:
			 *            r             s             t
			 *   N1     4r-1            0             0
			 *   N2       0           4s-1            0
			 *   N3       0             0           4t-1
			 *   N4 -4(1-r-s-t)+1 -4(1-r-s-t)+1 -4(1-r-s-t)+1
			 *   N5      4s            4r             0
			 *   N6       0            4t            4s
			 *   N7      4t             0            4r
			 *   N8 4[(1-r-s-t)-r]    -4r           -4r
			 *   N9     -4s       4[(1-r-s-t)-s]    -4s
			 *   N10    -4t           -4t       4[(1-r-s-t)-t]
			 */

//		  currentShapeFunctionDerivatives[0][0] = 4*r-1;
//		  currentShapeFunctionDerivatives[0][1] = 0;
//		  currentShapeFunctionDerivatives[0][2] = 0;
//
//		  currentShapeFunctionDerivatives[1][0] = 0;
//		  currentShapeFunctionDerivatives[1][1] = 4*s-1;
//		  currentShapeFunctionDerivatives[1][2] = 0;
//
//		  currentShapeFunctionDerivatives[2][0] = 0;
//		  currentShapeFunctionDerivatives[2][1] = 0;
//		  currentShapeFunctionDerivatives[2][2] = 4*t-1;
//
//		  currentShapeFunctionDerivatives[3][0] = -4*tempExp+1;
//		  currentShapeFunctionDerivatives[3][1] = -4*tempExp+1;
//		  currentShapeFunctionDerivatives[3][2] = -4*tempExp+1;
//
//		  currentShapeFunctionDerivatives[4][0] = 4*s;
//		  currentShapeFunctionDerivatives[4][1] = 4*r;
//		  currentShapeFunctionDerivatives[4][2] = 0;
//
//		  currentShapeFunctionDerivatives[5][0] = 0;
//		  currentShapeFunctionDerivatives[5][1] = 4*t;
//		  currentShapeFunctionDerivatives[5][2] = 4*s;
//
//		  currentShapeFunctionDerivatives[6][0] = 4*t;
//		  currentShapeFunctionDerivatives[6][1] = 0;
//		  currentShapeFunctionDerivatives[6][2] = 4*r;
//
//		  currentShapeFunctionDerivatives[7][0] = 4*(tempExp-r);
//		  currentShapeFunctionDerivatives[7][1] = -4*r;
//		  currentShapeFunctionDerivatives[7][2] = -4*r;
//
//		  currentShapeFunctionDerivatives[8][0] = -4*s;
//		  currentShapeFunctionDerivatives[8][1] = 4*(tempExp-s);
//		  currentShapeFunctionDerivatives[8][2] = -4*s;
//
//		  currentShapeFunctionDerivatives[9][0] = -4*t;
//		  currentShapeFunctionDerivatives[9][1] = -4*t;
//		  currentShapeFunctionDerivatives[9][2] = 4*(tempExp-t);

		  currentShapeFunctionDerivatives[0][0] = 4*r-1;
		  currentShapeFunctionDerivatives[0][1] = 0;
		  currentShapeFunctionDerivatives[0][2] = 0;

		  currentShapeFunctionDerivatives[1][0] = 0;
		  currentShapeFunctionDerivatives[1][1] = 4*s-1;
		  currentShapeFunctionDerivatives[1][2] = 0;

		  currentShapeFunctionDerivatives[2][0] = -4*tempExp+1;
		  currentShapeFunctionDerivatives[2][1] = -4*tempExp+1;
		  currentShapeFunctionDerivatives[2][2] = -4*tempExp+1;

		  currentShapeFunctionDerivatives[3][0] = 0;
		  currentShapeFunctionDerivatives[3][1] = 0;
		  currentShapeFunctionDerivatives[3][2] = 4*t-1;

		  currentShapeFunctionDerivatives[4][0] = 4*s;
		  currentShapeFunctionDerivatives[4][1] = 4*r;
		  currentShapeFunctionDerivatives[4][2] = 0;

		  currentShapeFunctionDerivatives[5][0] = -4*s;
		  currentShapeFunctionDerivatives[5][1] = 4*(tempExp-s);
		  currentShapeFunctionDerivatives[5][2] = -4*s;

		  currentShapeFunctionDerivatives[6][0] = 4*(tempExp-r);
		  currentShapeFunctionDerivatives[6][1] = -4*r;
		  currentShapeFunctionDerivatives[6][2] = -4*r;

		  currentShapeFunctionDerivatives[7][0] = 4*t;
		  currentShapeFunctionDerivatives[7][1] = 0;
		  currentShapeFunctionDerivatives[7][2] = 4*r;

		  currentShapeFunctionDerivatives[8][0] = 0;
		  currentShapeFunctionDerivatives[8][1] = 4*t;
		  currentShapeFunctionDerivatives[8][2] = 4*s;

		  currentShapeFunctionDerivatives[9][0] = -4*t;
		  currentShapeFunctionDerivatives[9][1] = -4*t;
		  currentShapeFunctionDerivatives[9][2] = 4*(tempExp-t);

		  m_ShapeFunctionDerivatives[i] = currentShapeFunctionDerivatives;
	  }

    }

  	template <class DataTypes, class MassType>
  	    void QuadraticMeshMatrixMass<DataTypes, MassType>::InitializeIntegrationPointsAndWeights(int numberOfIntegrationPoints)
  	    {

  		double alpha1 = 0.58541020;
		double beta1 = 0.13819660;
//		double intPoints4[12] = {alpha1,beta1,beta1,beta1,alpha1,beta1,beta1,beta1,alpha1,beta1,beta1,beta1};
//		double intPoints5[15] = {0.25,0.25,0.25,0.5,1.0/6.0,1.0/6.0,1.0/6.0,0.5,1.0/6.0,1.0/6.0,1.0/6.0,0.5,1.0/6.0,1.0/6.0,1.0/6.0};
//		double intWeights4[4] = {0.25,0.25,0.25,0.25};
//		double intWeights5[5] = {-0.8,9.0/20.0,9.0/20.0,9.0/20.0,9.0/20.0};
		double alpha2 = 0.785714285714286;
		double beta2 = 0.071428571428571;
		double alpha3 = 0.399403576166799;
		double  beta3 = 0.100596423833201;
//		double intPoints11[33] = {0.25,0.25,0.25,
//												alpha2,beta2,beta2,
//												beta2,alpha2,beta2,
//												beta2,beta2,alpha2,
//												beta2,beta2,beta2,
//												alpha3,alpha3,beta3,
//												alpha3,beta3,alpha3,
//												alpha3,beta3,beta3,
//												beta3,alpha3,alpha3,
//												beta3,alpha3,beta3,
//												beta3,beta3,alpha3};
//		double intWeights11[11]={-0.013133333333336,0.007622222222222,0.007622222222222,0.007622222222222,0.007622222222222,
//						0.24888888888889,0.24888888888889,0.24888888888889,0.24888888888889,0.24888888888889,0.24888888888889};

  		switch(numberOfIntegrationPoints){
  			 case 4:
				  m_IntPoints[0] = alpha1;
				  m_IntPoints[1] = beta1;
				  m_IntPoints[2] = beta1;
				  m_IntPoints[3] = beta1;
				  m_IntPoints[4] = alpha1;
				  m_IntPoints[5] = beta1;
				  m_IntPoints[6] = beta1;
				  m_IntPoints[7] = beta1;
				  m_IntPoints[8] = alpha1;
				  m_IntPoints[9] = beta1;
				  m_IntPoints[10] = beta1;
				  m_IntPoints[11] = beta1;
				  m_IntWeights[0] = 0.25;
				  m_IntWeights[1] = 0.25;
				  m_IntWeights[2] = 0.25;
				  m_IntWeights[3] = 0.25;
				  break;
			  case 5:
				  m_IntPoints[0] = 0.25;
				  m_IntPoints[1] = 0.25;
				  m_IntPoints[2] = 0.25;
				  m_IntPoints[3] = 0.5;
				  m_IntPoints[4] = 1.0/6.0;
				  m_IntPoints[5] = 1.0/6.0;
				  m_IntPoints[6] = 1.0/6.0;
				  m_IntPoints[7] = 0.5;
				  m_IntPoints[8] = 1.0/6.0;
				  m_IntPoints[9] = 1.0/6.0;
				  m_IntPoints[10] = 1.0/6.0;
				  m_IntPoints[11] = 0.5;
				  m_IntPoints[12] = 1.0/6.0;
				  m_IntPoints[13] = 1.0/6.0;
				  m_IntPoints[14] = 1.0/6.0;
				  m_IntWeights[0] = -0.8;
				  m_IntWeights[1] = 9.0/20.0;
				  m_IntWeights[2] = 9.0/20.0;
				  m_IntWeights[3] = 9.0/20.0;
				  m_IntWeights[4] = 9.0/20.0;
			  		  break;
			  case 11:
				  m_IntPoints[0] = 0.25;
                  m_IntPoints[1] = 0.25;
                  m_IntPoints[2] = 0.25;
				  m_IntPoints[3] = alpha2;
				  m_IntPoints[4] = beta2;
				  m_IntPoints[5] = beta2;
				  m_IntPoints[6] = beta2;
				  m_IntPoints[7] = alpha2;
				  m_IntPoints[8] = beta2;
				  m_IntPoints[9] = beta2;
				  m_IntPoints[10] = beta2;
				  m_IntPoints[11] = alpha2;
				  m_IntPoints[12] = beta2;
				  m_IntPoints[13] = beta2;
				  m_IntPoints[14] = beta2;
				  m_IntPoints[15] = alpha3;
				  m_IntPoints[16] = alpha3;
				  m_IntPoints[17] = beta3;
				  m_IntPoints[18] = alpha3;
				  m_IntPoints[19] = beta3;
				  m_IntPoints[20] = alpha3;
				  m_IntPoints[21] = alpha3;
				  m_IntPoints[22] = beta3;
				  m_IntPoints[23] = beta3;
				  m_IntPoints[24] = beta3;
				  m_IntPoints[25] = alpha3;
				  m_IntPoints[26] = alpha3;
				  m_IntPoints[27] = beta3;
				  m_IntPoints[28] = alpha3;
				  m_IntPoints[29] = beta3;
				  m_IntPoints[30] = beta3;
				  m_IntPoints[31] = beta3;
				  m_IntPoints[32] = alpha3;
				  m_IntWeights[0] = -0.013133333333336;
				  m_IntWeights[1] = 0.007622222222222;
				  m_IntWeights[2] = 0.007622222222222;
				  m_IntWeights[3] = 0.007622222222222;
				  m_IntWeights[4] = 0.007622222222222;
				  m_IntWeights[5] = 0.24888888888889;
				  m_IntWeights[6] = 0.24888888888889;
				  m_IntWeights[7] = 0.24888888888889;
				  m_IntWeights[8] = 0.24888888888889;
				  m_IntWeights[9] = 0.24888888888889;
				  m_IntWeights[10] = 0.24888888888889;
			  		  break;
  			  default:
  				  serr << "Wrong number of integration points" << sendl;

  			  }

  	    }

  	template <class DataTypes, class MassType>
  	  	    void QuadraticMeshMatrixMass<DataTypes, MassType>::WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename)
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
  	  	  	    void QuadraticMeshMatrixMass<DataTypes, MassType>::WriteElementMatrixToCVSFile(Mat<10,10,Real>  mat,char* filename, int elementNumber)
	{
  		//open file and generate textstream
  		  		QString completeFilename(MEDIASSIST_DATA_PATH);
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
  				 unsigned int rowSize = 10;
  				 unsigned int columnSize = 10;

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
  		void QuadraticMeshMatrixMass<DataTypes, MassType>::WriteElementVectorToMatFile(helper::fixed_array<Deriv,10> & vec,char* filename,int elementNumber)
  	{
  		//open file and generate textstream
  		  		  		QString completeFilename(MEDIASSIST_DATA_PATH);
  		  		  		//QString elementNumberString(elementNumber);
  		  		  		completeFilename += "/Temp/";
  		  		  		completeFilename += filename;
  		  		  		completeFilename += QString::number(elementNumber);
  		  		  		completeFilename +=".mat";

  		  		  		QFile file(completeFilename);

  		  		  		//unused variable, formerly used for debugging
//  		  		  		const char* test = completeFilename.toStdString().c_str();

  		  				 if (!file.open(QFile::WriteOnly | QFile::Truncate))
  		  					 return ;

  		  				 QTextStream out(&file);

  		  				 //vector line by line
  		  				 unsigned int rowSize = 10;

  		  				Deriv result;
  		  				//DataTypes::set ( test, g[0], g[1], g[2]);

  		  				 for(unsigned int m=0;m<rowSize;m++)
  		  				 {
  		  					 result = vec[m];
  		  					 out << result[0]<<"\n";
  		  					 out << result[1]<<"\n";
  		  					 out << result[2]<<"\n";
  		  					 out<<"\n";
  		  				 }

  		  				 file.close();
  	}

  	template <class DataTypes, class MassType>
  		void QuadraticMeshMatrixMass<DataTypes, MassType>::WriteVectorToMatFile(GravityForceVectorType& vec,char* filename)
  		{
  					//open file and generate textstream
  		  		  		  		QString completeFilename(MEDIASSIST_DATA_PATH);
  		  		  		  		//QString elementNumberString(elementNumber);
  		  		  		  		completeFilename += "/Temp/";
  		  		  		  		completeFilename += filename;
  		  		  		  		completeFilename +=".mat";

  		  		  		  		QFile file(completeFilename);

  		  		  				 if (!file.open(QFile::WriteOnly | QFile::Truncate))
  		  		  					 return ;

  		  		  				 QTextStream out(&file);

  		  		  				 //vector line by line
  		  		  				 unsigned int rowSize = vec.size();

  		  		  			Deriv result;

  		  		  				 for(unsigned int m=0;m<rowSize;m++)
  		  		  				 {
  		  		  					 	 result = vec[m];
  		  		  					out << result[0]<<"\n";
									 out << result[1]<<"\n";
									 out << result[2]<<"\n";
										 out<<"\n";
  		  		  				 }

  		  		  				 file.close();

  		}




} // namespace mass

} // namespace component

} // namespace sofa

#endif
