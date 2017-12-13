/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:	MediAssist Examples, LinearTetrahedralCorotationalFEMForceField
  Author:	Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "LinearTetrahedralCorotationalFEMForceField.h"
#include <sofa/helper/AdvancedTimer.h>
//#include <sofa/helper/PolarDecompose.h>
#include <sofa/helper/decompose.h>
#include <sofa/component/topology/TetrahedronSetGeometryAlgorithms.h>
#include <sofa/defaulttype/Quat.h>

// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
#include "MediAssistConfig.h"

#define PerElementRotation

namespace sofa
{

namespace component
{

namespace forcefield
{

// ****************************************************************************
// Constructor / Destructor
// ****************************************************************************
template <class DataTypes>
	   LinearTetrahedralCorotationalFEMForceField<DataTypes>::LinearTetrahedralCorotationalFEMForceField()
	: InternalData(new LinearTetrahedralCorotationalFEMForceFieldInternalData<DataTypes>)
	, m_updateStiffnessMatrix(core::objectmodel::BaseObject::initData(&m_updateStiffnessMatrix,true,"updateStiffnessMatrix",""))
	, m_assembling(core::objectmodel::BaseObject::initData(&m_assembling,true,"computeGlobalMatrix",""))
	, m_PoissonRatio(core::objectmodel::BaseObject::initData(&m_PoissonRatio,(Real)0.49f,"poissonRatio",""))
	, m_YoungModulus(initData(&m_YoungModulus,"youngModulus","FEM Young Modulus"))
	, m_LinearMode(core::objectmodel::BaseObject::initData(&m_LinearMode,false,"disableCorotation",""))
	, f_drawing(initData(&f_drawing,true,"drawing"," draw the forcefield if true"))
	{
	this->addAlias(&m_YoungModulus, "setYoungModulus");
	this->addAlias(&m_PoissonRatio, "setPoissonRatio");
//	Real poissonRatio = this->m_PoissonRatio.getValue();
//	Real youngModulus = this->m_YoungModulus.getValue();
//	std::cerr<< "Young Modulus:"<<this->m_YoungModulus.getValue()<<"\n";
//
//	this->m_ShearModulus = youngModulus/(2*(1+poissonRatio));
//	this->m_LamesFirstParameter = (youngModulus*poissonRatio)/((1+poissonRatio)*(1-2*poissonRatio));
	counter=0;
	}

template <class DataTypes>
	   LinearTetrahedralCorotationalFEMForceField<DataTypes>::~LinearTetrahedralCorotationalFEMForceField()
{
	//cleanup here
	delete InternalData;
}

// ****************************************************************************
// Public Methods
// ****************************************************************************
template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::init()
	   {
		//init superclass
		this->core::behavior::ForceField<DataTypes>::init();
		m_Topology = (BaseMeshTopology*)this->getContext()->getMeshTopology();

//		Real poissonRatio = this->m_PoissonRatio.getValue();
//		Real youngModulus = this->m_YoungModulus.getValue();
//
//		this->m_ShearModulus = youngModulus/(2*(1+poissonRatio));
//		this->m_LamesFirstParameter = (youngModulus*poissonRatio)/((1+poissonRatio)*(1-2*poissonRatio));
		this->reinit();
	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::reinit()
	   {
		unsigned int nElements=m_Topology->getNbTetrahedra();
		unsigned int nNodes = m_Topology->getNbTetrahedra();
		this->m_MeanRotationMatricesPerNode.resize(nNodes);
		this->m_LinearTetrahedronInfos.resize(nElements);
		InitializeIntegrationPointsAndWeights();
		InitializeShapeFunctionDerivatives();
		InitializeShapeFunctions();
		InitializeElementData();
		InitializePrecomputedStiffnessBlocks();

		ElementStiffnessMatrixType elementStiffnessMatrix ;
		PrecomputedStiffnessBlockType currentStiffnessBlock;
		PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
		helper::fixed_array<PrecomputedStiffnessBlockMatrixType,numberOfIntegrationPoints> stiffnessBlockMatrixArray;

		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
		   stiffnessBlockMatrixArray = m_LinearTetrahedronInfos[iterElement].precomputedElementStiffnessBlock;
		   stiffnessBlockMatrix = stiffnessBlockMatrixArray[0];
		   elementStiffnessMatrix.clear() ;

		   int mn, ij;

			  //loop over all i,m
			  for(unsigned int i=0;i<4;i++)
			  {
				  for(unsigned int m=0;m<4;m++)
				  {

					//extract 3x3 precomputed stiffness block Fim
					currentStiffnessBlock = stiffnessBlockMatrix[i][m];


					//add the blocks into elementStiffnessMatrix and elementStiffnessMatrixX0
					for(unsigned int n=0;n<3;n++)
					{
					  for(unsigned int j=0;j<3;j++)
					  {
						  mn = 3*m+n;
						  ij = 3*i+j;

						  elementStiffnessMatrix[ij][mn] += currentStiffnessBlock[j][n];

					  }
					}
				  }
			  }

			  m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear = elementStiffnessMatrix ;
		  }

		std::cout<<"LInear matrices initialized \n";

	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v)
	   {

//	std::cout<<"Calling AddForce\n";

		//**************IMPORTANT: Assume that addForce is always called before addDForce and addKToMatrix*******//

		unsigned int nElements=m_Topology->getNbTetrahedra();
		LinearTetraType tetraIndices;
		ElementStiffnessMatrixType elementStiffnessMatrix ;
		ElementStiffnessMatrixType elementStiffnessMatrixX0 ;
		ElementStiffnessMatrixType elementStiffnessMatrixLinear ;
		PrecomputedStiffnessBlockType currentStiffnessBlock;
		PrecomputedStiffnessBlockType currentStiffnessBlockX0;
		PrecomputedStiffnessBlockType currentStiffnessBlockLinear;
		PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
		helper::fixed_array<PrecomputedStiffnessBlockMatrixType,numberOfIntegrationPoints> stiffnessBlockMatrixArray;

		int mn, ij;
		RotationMatrixType S, R, Rtransp;
		Mat<3,3,Real> phi;
		Mat<4,3,Real> shapeFunctionDerivatives;
		Mat<3,4,Real> currentCoords;

		const VecCoord *X0=this->mstate->getX0();

		 //calculate mean rotation per node

				 //sofa::helper::AdvancedTimer::stepBegin("Calculate Mean Rotation");
				 unsigned int nNodes = m_Topology->getNbPoints();


				 RotationMatrixType RMean;
				 	RMean.clear();
				 	component::topology::TetrahedraAroundVertex tetrahedraAroundVertex;
				 	unsigned int numberOfElementsAroundNode;
				 	int currentElementId;
				 	double weight;
				 	int localNodeId;

				 	Mat<3,3,Real> PhiMean;
				 	PhiMean.clear();

		//		 	double currentVolume;
		//		 	double wholeVolume = 0;


				for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
				{
					PhiMean.clear();
					//wholeVolume = 0;

				   //get all elements around the node
					(tetrahedraAroundVertex) = m_Topology->getTetrahedraAroundVertex(iterNode);
					numberOfElementsAroundNode = tetrahedraAroundVertex.size();
					//weight = (double)1.0/(double)numberOfElementsAroundNode;

					// get all the rotation matrices for each element around the node
					for(unsigned int i=0;i<numberOfElementsAroundNode;i++)
					{
						currentElementId = tetrahedraAroundVertex[i];
						tetraIndices = m_Topology->getTetra(currentElementId);
						//find local node number
						//localNodeId = m_Topology->getVertexIndexInTetrahedron(tetraIndices,iterNode);

						//determine the rotation matrix

						shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[currentElementId].shapeFunctionDerivativesGlobal[0];

						for(int iter1=0;iter1<4;iter1++){
						  for(int iter2=0;iter2<3;iter2++){
							  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
							}
						}

						phi	= currentCoords * shapeFunctionDerivatives;

		//				currentVolume = this->m_LinearTetrahedronInfos[currentElementId].volume;
		//				wholeVolume += currentVolume;

						PhiMean = PhiMean + phi;//+ currentVolume*phi;


					}


					PhiMean = PhiMean/numberOfElementsAroundNode;
					//polar_decomp(PhiMean, RMean, S); polar_decomp decrapeted...
					helper::Decompose<Real>::polarDecomposition(PhiMean,RMean);


					this->m_MeanRotationMatricesPerNode[iterNode] = RMean;



				 }

				//sofa::helper::AdvancedTimer::stepEnd("Calculate Mean Rotation");


		sofa::helper::AdvancedTimer::stepBegin("ExtractRotation");

#ifdef PerElementRotation

		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
		   tetraIndices = m_Topology->getTetrahedron(iterElement);
		   stiffnessBlockMatrixArray = m_LinearTetrahedronInfos[iterElement].precomputedElementStiffnessBlock;

		  //first rebuild the elementStiffnessMatrix and elementStiffnessMatrixX0;
		  //delete the temp matrices
		  elementStiffnessMatrix.clear() ;
		  elementStiffnessMatrixX0.clear() ;
		  elementStiffnessMatrixLinear.clear() ;

		  //loop over all integration points
		   for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
		  {
			   //determine the rotation matrix and transpose

			shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];

			for(int iter1=0;iter1<4;iter1++){
			  for(int iter2=0;iter2<3;iter2++){
				  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
				}
			}

			phi	= currentCoords * shapeFunctionDerivatives;

			//polar_decomp(phi, R, S); decrapted
			//sofa::helper::AdvancedTimer::stepBegin("PolarDecomposition");
			helper::Decompose<Real>::polarDecomposition(phi,R);
			//sofa::helper::AdvancedTimer::stepEnd("PolarDecomposition");

			Rtransp = R.transposed();

			this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
			this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;

			  stiffnessBlockMatrix = stiffnessBlockMatrixArray[iterIntPoint];



			  //loop over all i,m
			  for(unsigned int i=0;i<4;i++)
			  {
				  for(unsigned int m=0;m<4;m++)
			  	  {

					//extract 3x3 precomputed stiffness block Fim
					currentStiffnessBlock = stiffnessBlockMatrix[i][m];

					//determine the rotation matrix and transpose

//					shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];
//
//					for(int iter1=0;iter1<4;iter1++){
//					  for(int iter2=0;iter2<3;iter2++){
//						  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
//						}
//					}
//
//					phi	= currentCoords * shapeFunctionDerivatives;
//
//					polar_decomp(phi, R, S);
//					Rtransp = R.transposed();
//
//					this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
//					this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;


//					R = this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint];
//					Rtransp =

					//calculate R*Fim and save as currentStiffnessBlockX0
					currentStiffnessBlockX0 = R*currentStiffnessBlock;

					//calculate currentStiffnessBlockX0*RT (=R*Fim*RT) and save as currentStiffnessBlock
					currentStiffnessBlock = currentStiffnessBlockX0*Rtransp;

					//*****************uncomment this in order to revert to linear FEM (no co-rotation)*******//
				  if(this->m_LinearMode.getValue())
					   {
					//  std::cerr<<"LInear mode enabled \n";
								currentStiffnessBlockX0 = stiffnessBlockMatrix[i][m];
								currentStiffnessBlock = stiffnessBlockMatrix[i][m];
					   }
//					currentStiffnessBlockX0 = stiffnessBlockMatrix[i][m];
//					currentStiffnessBlock = stiffnessBlockMatrix[i][m];
					currentStiffnessBlockLinear = stiffnessBlockMatrix[i][m];
					//****************************************************************************************//

					//add the blocks into elementStiffnessMatrix and elementStiffnessMatrixX0
					for(unsigned int n=0;n<3;n++)
					{
					  for(unsigned int j=0;j<3;j++)
					  {
						  mn = 3*m+n;
						  ij = 3*i+j;

						  elementStiffnessMatrix[ij][mn] += currentStiffnessBlock[j][n];
						  elementStiffnessMatrixX0[ij][mn] += currentStiffnessBlockX0[j][n];
						  elementStiffnessMatrixLinear[ij][mn] += currentStiffnessBlockLinear[j][n];

					  }
					}
				  }
			  }
		  }



		  //save element stiffness matrix in tetrahedron datastructure
		  m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix = elementStiffnessMatrix ;
		  m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear = elementStiffnessMatrixLinear ;

//		   m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear;
//		   elementStiffnessMatrixX0 = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear;
//		   elementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear;
		 //now use the element stiffness matrices to calculate force
		  for(unsigned int m=0; m<4;m++)
		  {
			  for(unsigned int n=0; n<3;n++)
				{
				  int posColumn = tetraIndices[m];
				  int posColumnElement = 3*m+n;

				  for(unsigned int i=0; i<4;i++)
				  {
					  	  int posRow = tetraIndices[i];

//						  double test1 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+0][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
//						  double test2 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+1][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
//						  double test3 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+2][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];

						  Real test1 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+0][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
						  Real test2 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+1][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
						  Real test3 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+2][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];
							
						  f[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
				  }
				}
			}

		  }

#else

		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
		   tetraIndices = m_Topology->getTetrahedron(iterElement);
		   stiffnessBlockMatrixArray = m_LinearTetrahedronInfos[iterElement].precomputedElementStiffnessBlock;

		  //first rebuild the elementStiffnessMatrix and elementStiffnessMatrixX0;
		  //delete the temp matrices
		  elementStiffnessMatrix.clear() ;
		  elementStiffnessMatrixX0.clear() ;
		  elementStiffnessMatrixLinear.clear() ;

		  //loop over all integration points
		   for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
		  {
			   //determine the rotation matrix and transpose

//			shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];
//
//			for(int iter1=0;iter1<4;iter1++){
//			  for(int iter2=0;iter2<3;iter2++){
//				  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
//				}
//			}
//
//			phi	= currentCoords * shapeFunctionDerivatives;
//
//			polar_decomp(phi, R, S);
//			Rtransp = R.transposed();
//
//			this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
//			this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;

			 //	Mat<3,3,Real> PhiMean;
				//			 	PhiMean.clear();

					//		 	double currentVolume;
					//		 	double wholeVolume = 0;

			helper::fixed_array<Mat<3,3,Real> ,4> RotationMatricesPerNode;

			for(unsigned int iterNode=0;iterNode<4;iterNode++)
			{
//				//PhiMean.clear();
//				//wholeVolume = 0;
//
//			   //get all elements around the node
//
//
//				// get all the rotation matrices for each element around the node
//				shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[0];
//
//					for(int iter1=0;iter1<4;iter1++){
//					  for(int iter2=0;iter2<3;iter2++){
//						  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
//						}
//					}
//
//					phi	= currentCoords * shapeFunctionDerivatives;
//
//	//				currentVolume = this->m_LinearTetrahedronInfos[currentElementId].volume;
//	//				wholeVolume += currentVolume;
//
//					//PhiMean = PhiMean + phi;//+ currentVolume*phi;
//
//					polar_decomp(phi, R, S);
//
//					 //determine the rotation matrix and transpose
//
////					shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[0];
////
////					for(int iter1=0;iter1<4;iter1++){
////					  for(int iter2=0;iter2<3;iter2++){
////						  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
////						}
////					}
////
////					phi	= currentCoords * shapeFunctionDerivatives;
////
////					polar_decomp(phi, R, S);
////					Rtransp = R.transposed();
////
//////					this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
//////					this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;
//
//
//					RotationMatricesPerNode[iterNode] = R;

				PhiMean.clear();
				//wholeVolume = 0;

			   //get all elements around the node
				tetrahedraAroundVertex = m_Topology->getTetrahedraAroundVertex(tetraIndices[iterNode]);
				numberOfElementsAroundNode = tetrahedraAroundVertex.size();
				//weight = (double)1.0/(double)numberOfElementsAroundNode;

				// get all the rotation matrices for each element around the node
				for(unsigned int i=0;i<numberOfElementsAroundNode;i++)
				{
					currentElementId = tetrahedraAroundVertex[i];
					tetraIndices = m_Topology->getTetra(currentElementId);//iterElement);//currentElementId);
					//find local node number
					//localNodeId = m_Topology->getVertexIndexInTetrahedron(tetraIndices,iterNode);

					//determine the rotation matrix

					shapeFunctionDerivatives = this->m_LinearTetrahedronInfos[currentElementId].shapeFunctionDerivativesGlobal[0];

					for(int iter1=0;iter1<4;iter1++){
					  for(int iter2=0;iter2<3;iter2++){
						  currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
						}
					}

					phi	= currentCoords * shapeFunctionDerivatives;

	//				currentVolume = this->m_LinearTetrahedronInfos[currentElementId].volume;
	//				wholeVolume += currentVolume;

					PhiMean = PhiMean + phi;//+ currentVolume*phi;


				}


				PhiMean = phi;//PhiMean/numberOfElementsAroundNode;
				polar_decomp(PhiMean, R, S);


				RotationMatricesPerNode[iterNode] = R;


			}












			  stiffnessBlockMatrix = stiffnessBlockMatrixArray[iterIntPoint];
			  //loop over all i,m
			  for(unsigned int i=0;i<4;i++)
			  {
				  R = RotationMatricesPerNode[0];//this->m_MeanRotationMatricesPerNode[ tetraIndices[i] ];


				  for(unsigned int m=0;m<4;m++)
				  {
					Rtransp = RotationMatricesPerNode[0].transposed();//this->m_MeanRotationMatricesPerNode[ tetraIndices[m] ].transposed();
					//extract 3x3 precomputed stiffness block Fim
					currentStiffnessBlock = stiffnessBlockMatrix[i][m];

					//calculate R*Fim and save as currentStiffnessBlockX0
					currentStiffnessBlockX0 = R*currentStiffnessBlock;

					//calculate currentStiffnessBlockX0*RT (=R*Fim*RT) and save as currentStiffnessBlock
					currentStiffnessBlock = currentStiffnessBlockX0*Rtransp;

					//*****************uncomment this in order to revert to linear FEM (no co-rotation)*******//
//					currentStiffnessBlockX0 = stiffnessBlockMatrix[i][m];
//					currentStiffnessBlock = stiffnessBlockMatrix[i][m];
					currentStiffnessBlockLinear = stiffnessBlockMatrix[i][m];
					//****************************************************************************************//

					//add the blocks into elementStiffnessMatrix and elementStiffnessMatrixX0
					for(unsigned int n=0;n<3;n++)
					{
					  for(unsigned int j=0;j<3;j++)
					  {
						  mn = 3*m+n;
						  ij = 3*i+j;

						  elementStiffnessMatrix[ij][mn] += currentStiffnessBlock[j][n];
						  elementStiffnessMatrixX0[ij][mn] += currentStiffnessBlockX0[j][n];
						  elementStiffnessMatrixLinear[ij][mn] += currentStiffnessBlockLinear[j][n];

					  }
					}
				  }
			  }
		  }



		  //save element stiffness matrix in tetrahedron datastructure
		  m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix = elementStiffnessMatrix ;
		  m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrixLinear = elementStiffnessMatrixLinear ;
		 //now use the element stiffness matrices to calculate force
		  for(unsigned int m=0; m<4;m++)
		  {
			  for(unsigned int n=0; n<3;n++)
				{
				  int posColumn = tetraIndices[m];
				  int posColumnElement = 3*m+n;

				  for(unsigned int i=0; i<4;i++)
				  {
						  int posRow = tetraIndices[i];

//						  double test1 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+0][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
//						  double test2 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+1][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
//						  double test3 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+2][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];

						  Real test1 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+0][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
						  Real test2 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+1][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
						  Real test3 = (*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+2][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];

						  f[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
				  }
				}
			}

		  }

#endif

		 sofa::helper::AdvancedTimer::stepEnd("ExtractRotation");

		 //sum force to get energy

		 m_PotentialEnergy = 0;

		for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
		{
			m_PotentialEnergy += f[iterNode].norm();
		}



	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::addDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor)
	   {
		//implement addMDx on a per element basis - it will be faster to just use the complete mass matrix and
		//perform a matrix vector multiplication

	//df.clear();

	kFactor = kFactor*(-1);

		unsigned int nElements=m_Topology->getNbTetrahedra();

//		WriteVectorsToCVSFile(df, "ForceFieldDf", counter);
//		counter++;



		 //add in the element matrix
		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
			ElementStiffnessMatrixType elementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix;

			LinearTetraType tetraIndices = m_Topology->getTetrahedron(iterElement);

			for(unsigned int m=0; m<4;m++)
			{

			  for(unsigned int n=0; n<3;n++)
				{

				  int posColumn = tetraIndices[m];
				  int posColumnElement = 3*m+n;

				  for(unsigned int i=0; i<4;i++)
				  {
					int posRow = tetraIndices[i];

					double test1 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement]*kFactor;
					double test2 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement]*kFactor;
					double test3 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement]*kFactor;

					df[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];

				  }
				}
			}
		}




		////////////////////////////////////////////////////////////////////////////////////////////////////

//		VecDeriv temp1;
//		temp1.resize(nElements);
//		//temp1.clear();
//
//		VecDeriv temp2;
//		temp2.resize(nElements);
//		//temp2.clear();
//
//				LinearTetraType tetraIndices;
//				ElementStiffnessMatrixType elementStiffnessMatrix ;
//				ElementStiffnessMatrixType elementStiffnessMatrixX0 ;
//				ElementStiffnessMatrixType elementStiffnessMatrixLinear ;
//				PrecomputedStiffnessBlockType currentStiffnessBlock;
//				PrecomputedStiffnessBlockType currentStiffnessBlockX0;
//				PrecomputedStiffnessBlockType currentStiffnessBlockLinear;
//				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
//				helper::fixed_array<PrecomputedStiffnessBlockMatrixType,numberOfIntegrationPoints> stiffnessBlockMatrixArray;
//
//				int mn, ij;
//				RotationMatrixType S, R, Rtransp;
//				Mat<3,3,Real> phi;
//				Mat<4,3,Real> shapeFunctionDerivatives;
//				Mat<3,4,Real> currentCoords;
//
//
//
//		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//						{
//						   tetraIndices = m_Topology->getTetrahedron(iterElement);
//						   stiffnessBlockMatrixArray = m_LinearTetrahedronInfos[iterElement].precomputedElementStiffnessBlock;
//
//						  //first rebuild the elementStiffnessMatrix and elementStiffnessMatrixX0;
//						  //delete the temp matrices
//						  elementStiffnessMatrix.clear() ;
//						  elementStiffnessMatrixX0.clear() ;
//						  elementStiffnessMatrixLinear.clear() ;
//
//						  //loop over all integration points
//						   for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
//						  {
//							   //determine the rotation matrix and transpose
//
//								R = this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint];
//								Rtransp = this->m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint];
//
//							  stiffnessBlockMatrix = stiffnessBlockMatrixArray[iterIntPoint];
//							  //loop over all i,m
//							  for(unsigned int m=0;m<4;m++)
//							  {
//								  for(unsigned int i=0;i<4;i++)
//								  {
//									//extract 3x3 precomputed stiffness block Fim
//									currentStiffnessBlock = stiffnessBlockMatrix[i][m];
//
//									//calculate R*Fim and save as currentStiffnessBlockX0
//									currentStiffnessBlockX0 = R*currentStiffnessBlock;
//
//									//calculate currentStiffnessBlockX0*RT (=R*Fim*RT) and save as currentStiffnessBlock
//									currentStiffnessBlock = currentStiffnessBlockX0*Rtransp;
//
//									int posRow = tetraIndices[i];
//									int posColumn = tetraIndices[m];
//
//									Deriv currentDx = (temp1[posColumn]);
//									Deriv result;
//
//									result = currentStiffnessBlock * currentDx;
//									result = result*kFactor;
//
//									temp2[posRow] += result;
//
//								  }
//							  }
//						  }
//						  }



		///////////////////////////////////////////////////////////////////////////////

	   }

//---------------------------------------------------------------------------
// ApplyRotation Function
//---------------------------------------------------------------------------
template <class DataTypes>
void LinearTetrahedralCorotationalFEMForceField< DataTypes >::applyRotation(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, sofa::core::MultiVecDerivId dxId, sofa::core::MultiVecDerivId dfId, bool invert )
{

//	std::cout<<"Applying rotation...\n";

	unsigned int nElements=m_Topology->getNbTetrahedra();
		unsigned int nNodes = m_Topology->getNbPoints();
		//this->

		//const DataVecDeriv & dxData = *mparams->readDx(this->mstate);

		const DataVecDeriv & dxData = *dxId[(this->mstate).get(mparams)].read();

		//*fId[mstate.get(mparams)].write()
		//DataVecDeriv & dxData = *dxId[this->mstate.get(mparams)].read();

		const VecDeriv& dx = dxData.getValue(mparams);

		DataVecDeriv & dfData = *dfId[(this->mstate).get()].write();

		VecDeriv& df =  *dfData.beginEdit(mparams);



//		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//		{
//			RotationMatrixType* rotationMatrix;
//			RotationMatrixType* rotationMatrixTransposed;
//
//			if(invert)
//			{
//				rotationMatrix = &m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[0];
//				rotationMatrixTransposed = &m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[0];
//			}
//			else
//			{
//				rotationMatrix = &m_LinearTetrahedronInfos[iterElement].elementRotationMatrixArray[0];
//				rotationMatrixTransposed = &m_LinearTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[0];
//			}
//
//
//			LinearTetraType tetraIndices = this->m_Topology->getTetrahedron(iterElement);
//
//			Deriv tempCoords;
//			Deriv result;
//
//
//
//			for(unsigned int m=0; m<4;m++)
//			{
//				int pos = tetraIndices[m];
//				tempCoords = dx[pos];
//				//result = (*rotationMatrix)*tempCoords;
//				result = (*rotationMatrix)*tempCoords;
//				//result = tempCoords;
//				df[pos]+=result;
//
//
//			}
//		}

	RotationMatrixType RMean;
	LinearTetraType tetraIndices;
	int currentNodeId;



	for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
	{
	   // get mean rotation matrix
		RMean = this->m_MeanRotationMatricesPerNode[iterNode];

		if(invert)
			 RMean = RMean.transposed();


		Deriv currentDx = (dx[iterNode]);
		Deriv result;

		result = RMean*currentDx;//RMean * currentDx;

		//result = RMean.transposed()*result;

		df[iterNode] = result;


	  }



	dfData.endEdit(mparams);
//	this->WriteVectorsToCVSFile(df, "ApplyRotationForecFieldAfter",counter);
//	counter++;

}

template <class DataTypes>
	   double LinearTetrahedralCorotationalFEMForceField<DataTypes>::getPotentialEnergy(const sofa::core::MechanicalParams* mparams) const
	   {

			return m_PotentialEnergy;
	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::addKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset)
	   {

//	std::cout<<"Calling AddKToMatrix\n";

		kFactor = kFactor*(-1);

		unsigned int nElements=this->m_Topology->getNbTetrahedra();

		ElementStiffnessMatrixType elementStiffnessMatrix;

		LinearTetraType tetraIndices;

		unsigned int posRow, posColumn, posRowElement, posColumnElement;

		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
		  elementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix;
		  tetraIndices = m_Topology->getTetrahedron(iterElement);

		  for(unsigned int m=0; m<4;m++)
		  {
			  for(unsigned int n=0; n<3;n++)
				{
				  posColumn = 3*tetraIndices[m]+n + offset;
				  posColumnElement = 3*m+n;

				  for(unsigned int i=0; i<4;i++)
				  {
					  for(unsigned int j=0; j<3;j++)
					  {
						  posRow = 3*tetraIndices[i]+j + offset;
						  posRowElement = 3*i+j;

						  mat->add(posRow, posColumn, elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor);
					  }
				  }

				}
			}
		}

		//			this->WriteMatrixToCVSFile(mat, "StiffnessMatrixLinearTetra.csv");

	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::handleTopologyChange()
	   {

	   }

// ****************************************************************************
// Protected/Private Methods
// ****************************************************************************
template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializePrecomputedStiffnessBlocks()
	   {

//		double mu = this->m_ShearModulus ;
//		double lambda = this->m_LamesFirstParameter;
//		std::cout<<"mu: "<<mu<<"\n";
//		std::cout<<"lambda: "<<lambda<<"\n";

		unsigned int nElements=m_Topology->getNbTetrahedra();
		for(unsigned int i=0; i<nElements;i++)
		{
			this->InitializePrecomputedStiffnessBlocks(i); //inline definition follows right after!
		}
	   }

//ElementStiffnessBlock is computed here for element with elementID!TODO: Thinking about rename to InitializePrecomputedElementStiffnessBlock(unsigned int elementID)
template <class DataTypes>
	   inline void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializePrecomputedStiffnessBlocks(unsigned int elementID)
	   {
		PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
		stiffnessBlockMatrix.clear();

		const VecCoord *X0=this->mstate->getX0();

		const LinearTetraType tetraIndices = m_Topology->getTetrahedron(elementID);

		ElementCoordsType coords;

		for(unsigned int i=0; i<4;i++)
		{
		  coords[i] = (*X0)[tetraIndices[i]];
		}

		for(unsigned int iter=0;iter<numberOfIntegrationPoints;iter++)
		{
		  this->BuildStiffnessBlock(elementID, stiffnessBlockMatrix, coords, iter);
		  m_LinearTetrahedronInfos[elementID].precomputedElementStiffnessBlock[iter] = stiffnessBlockMatrix;

		}

		m_LinearTetrahedronInfos[elementID].volume = this->GetElementVolume(elementID, coords);

	   }

template <class DataTypes>
	   inline double LinearTetrahedralCorotationalFEMForceField<DataTypes>::GetElementVolume(unsigned int elementID, ElementCoordsType coords)
	   {
			double result=0;
			for(unsigned int iter=0;iter<numberOfIntegrationPoints;iter++)
			{
				Mat<3,3,Real> jacobianMatrix;
				this->GetJacobian(jacobianMatrix,coords,iter);
				double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);
				result += this->m_IntWeights[iter]*jacobianDeterminant;
			}
			return result;

	   }

template <class DataTypes>
	   inline void LinearTetrahedralCorotationalFEMForceField<DataTypes>::BuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlockMatrix,ElementCoordsType coords, unsigned int integrationPointIndex)
	   {
		//get lambda, mu
		Real youngModulusElement;
		if (m_YoungModulus.getValue().size() == this->m_Topology->getNbTetrahedra()) youngModulusElement = m_YoungModulus.getValue()[elementID];
		else if (m_YoungModulus.getValue().size() > 0) youngModulusElement = m_YoungModulus.getValue()[0];
		else
		{
			setYoungModulus(300000.0f);
			youngModulusElement = m_YoungModulus.getValue()[0];
		}

		double mu = youngModulusElement/(2*(1+this->m_PoissonRatio.getValue()));
		double lambda = (youngModulusElement*this->m_PoissonRatio.getValue())/((1+this->m_PoissonRatio.getValue())*(1-2*this->m_PoissonRatio.getValue()));



		ElementStiffnessMatrixType stiffnessBlock;

		stiffnessBlock.clear();



//		std::cout<<"mu: "<<mu<<"\n";
//		std::cout<<"lambda: "<<lambda<<"\n";

		Mat<3,3,Real> jacobianMatrix;

		this->GetJacobian(jacobianMatrix,coords,integrationPointIndex);

		double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);

		Mat<3,3,Real> inverseJacobian;
		sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

		Mat<4,3,Real> shapeFunctionDerivativesGlobal = this->m_LinearTetrahedronInfos[elementID].shapeFunctionDerivativesGlobal[integrationPointIndex];

		//Only used for debug
//		double debug11 = shapeFunctionDerivativesGlobal[0][0];
//		double debug12 = shapeFunctionDerivativesGlobal[0][1];
//		double debug13 = shapeFunctionDerivativesGlobal[0][2];
//
//		double debug21 = shapeFunctionDerivativesGlobal[1][0];
//		double debug22 = shapeFunctionDerivativesGlobal[1][1];
//		double debug23 = shapeFunctionDerivativesGlobal[1][2];
//
//		double debug31 = shapeFunctionDerivativesGlobal[2][0];
//		double debug32 = shapeFunctionDerivativesGlobal[2][1];
//		double debug33 = shapeFunctionDerivativesGlobal[2][2];
//
//		double debug41 = shapeFunctionDerivativesGlobal[3][0];
//		double debug42 = shapeFunctionDerivativesGlobal[3][1];
//		double debug43 = shapeFunctionDerivativesGlobal[3][2];

		unsigned int mn,ij;

		//build stiffness block (force derivative \partial F _ij over \partial U _mn)
		for(unsigned int m=0;m<4;m++)
		{
			for(unsigned int n=0;n<3;n++)
			{
			  for(unsigned int i=0;i<4;i++)
			  {
				  for(unsigned int j=0;j<3;j++)
				  {
					  mn = 3*m+n;
					  ij = 3*i+j;

					  stiffnessBlock[ij][mn]= 0;

					  //factor (-1) is due to sofa convention
					  stiffnessBlock[ij][mn] += (lambda *shapeFunctionDerivativesGlobal[m][n]*shapeFunctionDerivativesGlobal[i][j]
													 + mu*shapeFunctionDerivativesGlobal[m][j]*shapeFunctionDerivativesGlobal[i][n])*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
					  if(j==n)
					  {
						  for(unsigned int k=0;k<3;k++)
						  {
							  stiffnessBlock[ij][mn] += mu*shapeFunctionDerivativesGlobal[m][k]*shapeFunctionDerivativesGlobal[i][k]*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
						  }
					  }

				  }
			  }
			}
		}

		//this->WriteElementMatrixToCVSFile(stiffnessBlock, "StiffnessElementMatrixNo_",elementID);

		//copy stiffness block to new datastructure
		PrecomputedStiffnessBlockType currentStiffnessBlock;

		for(unsigned int m=0;m<4;m++)
		{
			for(unsigned int i=0;i<4;i++)
			{
			  currentStiffnessBlock.clear();

			  for(unsigned int n=0;n<3;n++)
			  {
				  for(unsigned int j=0;j<3;j++)
				  {
					  mn = 3*m+n;
					  ij = 3*i+j;
					  currentStiffnessBlock[j][n] = stiffnessBlock[ij][mn];

				  }
			  }

			  stiffnessBlockMatrix[i][m] = currentStiffnessBlock;

			}
		}


	   }


template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializeShapeFunctionDerivatives()
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

		//iterate nodes
			for(unsigned int i=0; i< NUMBER_OF_ELEMENT_NODES;i++)
			{
			  DataTypes::set ( currentIntegrationPoint, this->m_NodalLocalCoordinates[3*i], this->m_NodalLocalCoordinates[3*i+1], this->m_NodalLocalCoordinates[3*i+2]);
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

			  m_ShapeFunctionDerivativesPerNode[i] = currentShapeFunctionDerivatives;

			}


	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializeShapeFunctions()
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

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializeIntegrationPointsAndWeights()
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

		this->m_NodalLocalCoordinates[0] = 1;
		this->m_NodalLocalCoordinates[1] = 0;
		this->m_NodalLocalCoordinates[2] = 0;
		this->m_NodalLocalCoordinates[3] = 0;
		this->m_NodalLocalCoordinates[4] = 1;
		this->m_NodalLocalCoordinates[5] = 0;
		this->m_NodalLocalCoordinates[6] = 0;
		this->m_NodalLocalCoordinates[7] = 0;
		this->m_NodalLocalCoordinates[8] = 0;
		this->m_NodalLocalCoordinates[9] = 0;
		this->m_NodalLocalCoordinates[10] = 0;
		this->m_NodalLocalCoordinates[11] = 1;


	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::InitializeElementData()
	   {
		//initialize local shape functions
		const VecCoord *X0=this->mstate->getX0();
		//loop over all elements
		unsigned int nElements=m_Topology->getNbTetrahedra();
		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
		{
			//extract coords
			const LinearTetraType tetraIndices = m_Topology->getTetrahedron(iterElement);

			ElementCoordsType coords;

			for(unsigned int i=0; i<4;i++)
			{
				coords[i] = (*X0)[tetraIndices[i]];
			}

			//loop over all integration points
			for(unsigned int integrationPointIndex=0; integrationPointIndex< numberOfIntegrationPoints;integrationPointIndex++)
			{
				  Mat<3,3,Real> jacobianMatrix;

				  this->GetJacobian(jacobianMatrix,coords,integrationPointIndex);

				  Mat<3,3,Real> inverseJacobian;
				  sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

				  //get the shape function derivatives in local coordinates
				  Mat<4,3,Real> shapeFunctionDerivativesLocal = this->m_ShapeFunctionDerivatives[integrationPointIndex];

				  //transform shape function derivatives in global coordinates
				  Mat<4,3,Real> shapeFunctionDerivativesGlobal;
				  shapeFunctionDerivativesGlobal.clear();
				  for(unsigned int i=0;i<4;i++)
				  {
					  for(unsigned int j=0;j<3;j++)
					  {
						  for(unsigned int k=0;k<3;k++)
						  {
							  shapeFunctionDerivativesGlobal[i][j]+=shapeFunctionDerivativesLocal[i][k]*inverseJacobian[k][j];
						  }
					  }
				  }
				this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[integrationPointIndex]=shapeFunctionDerivativesGlobal;
			}

			//loop over all nodal points
			for(unsigned int nodalPointIndex=0; nodalPointIndex< NUMBER_OF_ELEMENT_NODES;nodalPointIndex++)
			{
				  Mat<3,3,Real> jacobianMatrix;
				  this->GetNodalJacobian(jacobianMatrix,coords,nodalPointIndex);

				  Mat<3,3,Real> inverseJacobian;
				  sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

				  //get the shape function derivatives in local coordinates
				  Mat<4,3,Real> shapeFunctionDerivativesLocal = this->m_ShapeFunctionDerivativesPerNode[nodalPointIndex];
				  Mat<4,3,Real> shapeFunctionDerivativesGlobal;
				  shapeFunctionDerivativesGlobal.clear();
				  for(unsigned int i=0;i<4;i++)
				  {
					  for(unsigned int j=0;j<3;j++)
					  {
						  for(unsigned int k=0;k<3;k++)
						  {
							  shapeFunctionDerivativesGlobal[i][j]+=shapeFunctionDerivativesLocal[i][k]*inverseJacobian[k][j];
						  }
					  }
				  }
				  this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobalPerNode[nodalPointIndex]=shapeFunctionDerivativesGlobal;
			}
		}

	   }

template<class DataTypes>
		void LinearTetrahedralCorotationalFEMForceField<DataTypes>::draw(const core::visual::VisualParams* vparams)
		{
			// This mothod has been copied verbatim from
			// TetrahedralCorotationalFEMForceField
			if (!vparams->displayFlags().getShowForceFields()) return;
			if (!this->mstate) return;
			if (!f_drawing.getValue()) return;

			const VecCoord& x = *this->mstate->getX();

			if (vparams->displayFlags().getShowWireFrame())
				vparams->drawTool()->setPolygonMode(0,true);


			std::vector< Vector3 > points[4];
			for(int i = 0 ; i < m_Topology->getNbTetrahedra(); ++i)
			{
				const sofa::component::topology::Tetrahedron t =
					m_Topology->getTetrahedron(i);

				Index a = t[0];
				Index b = t[1];
				Index c = t[2];
				Index d = t[3];
				Coord center = (x[a]+x[b]+x[c]+x[d])*0.125;
				Coord pa = (x[a]+center)*(Real)0.666667;
				Coord pb = (x[b]+center)*(Real)0.666667;
				Coord pc = (x[c]+center)*(Real)0.666667;
				Coord pd = (x[d]+center)*(Real)0.666667;

				points[0].push_back(pa);
				points[0].push_back(pb);
				points[0].push_back(pc);

				points[1].push_back(pb);
				points[1].push_back(pc);
				points[1].push_back(pd);

				points[2].push_back(pc);
				points[2].push_back(pd);
				points[2].push_back(pa);

				points[3].push_back(pd);
				points[3].push_back(pa);
				points[3].push_back(pb);
			}

			vparams->drawTool()->drawTriangles(points[0], Vec<4,float>(0.0,0.0,1.0,1.0));
			vparams->drawTool()->drawTriangles(points[1], Vec<4,float>(0.0,0.5,1.0,1.0));
			vparams->drawTool()->drawTriangles(points[2], Vec<4,float>(0.0,1.0,1.0,1.0));
			vparams->drawTool()->drawTriangles(points[3], Vec<4,float>(0.5,1.0,1.0,1.0));

			if (vparams->displayFlags().getShowWireFrame())
				vparams->drawTool()->setPolygonMode(0,false);
		}



template <class DataTypes>
	   inline void LinearTetrahedralCorotationalFEMForceField<DataTypes>::GetJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfIntegrationPoint)
	   {
		Mat<4,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivatives[indexOfIntegrationPoint];
		jacobianMatrix.clear();

		//build jacobian matrix
		for(int m=0;m<3;m++)
		{
		  for(int n=0;n<3;n++)
		  {
			  //sum over all shape functions
			  for(int i=0;i<4;i++)
			  {
				  jacobianMatrix[m][n] += (coords[i])[m]*currentShapeFunctionDerivatives[i][n];
//				  std::cout<<"jacobian matrix value "<<jacobianMatrix[m][n]<<" at mXn "<<m<<" "<<n<<std::endl;
			  }
		  }
		}

	   }

template <class DataTypes>
	   inline void LinearTetrahedralCorotationalFEMForceField<DataTypes>::GetNodalJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfNodalPoint)
	   {
		Mat<4,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivativesPerNode[indexOfNodalPoint];
		jacobianMatrix.clear();

		//build jacobian matrix
		for(int m=0;m<3;m++)
		{
		  for(int n=0;n<3;n++)
		  {
			  //sum over all shape functions
			  for(int i=0;i<4;i++)
			  {
				  jacobianMatrix[m][n] += (coords[i])[m]*currentShapeFunctionDerivatives[i][n];
//				  std::cout<<"jacobian matrix value "<<jacobianMatrix[m][n]<<" at mXn "<<m<<" "<<n<<std::endl;
			  }
		  }
		}

	   }

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename)
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

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::WriteElementMatrixToCVSFile(Mat<12,12,Real>  mat,char* filename, int elementNumber)
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

template <class DataTypes>
	   void LinearTetrahedralCorotationalFEMForceField<DataTypes>::WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber)
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
		 unsigned int rowSize = vec.size();
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
