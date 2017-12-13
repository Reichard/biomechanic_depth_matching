/*=========================================================================

 Program:   MediAssist Computer Assisted Surgery System
 Module:    MediAssist Examples, LinearTetrahedralCorotationalFEMForceField
 Author:    Stefan Suwelack ( nip/tuck by Mario Laugisch)

 =========================================================================*/

/*
 * The topology type are defined in the TetrahedronData.h, but the functions are defined in BaseMeshTopology.h, which
 * return a component as a topology. So both of these two head file should be changed at the same time.
 *
 */
#ifndef SOFA_COMPONENT_FORCEFIELD_QUADRATICTETRAHEDRONFEMFORCEFIELD_INL
#define SOFA_COMPONENT_FORCEFIELD_QUADRATICTETRAHEDRONFEMFORCEFIELD_INL

//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
/*
 * Change the define wrote to select different method.
 * The second polar-method, which is the same as the implementation of SOFA, has still mistakes.
 */
#define NON_LINEAR_FEM_TET10
//#define POLAR
//#define POLAR2
//#define SMALL

//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include <sofa/core/behavior/ForceField.inl>
#include <QuadraticTetrahedralCorotationalFEMForceField.h>
#include <sofa/component/topology/GridTopology.h>
#include <sofa/simulation/common/Simulation.h>
#include <sofa/helper/decompose.h>
#include <sofa/helper/gl/template.h>
#include <sofa/helper/AdvancedTimer.h>
//TODO include new head file of Quadratic Tetrahedon
#include <QuadraticTetrahedronData.h>
#include <QuadraticMeshTopology.h>

#include <assert.h>
#include <iostream>
#include <set>
#include <math.h>

// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
//FIXME this configure file can not used!!!
#include "MediAssistConfig.h"

#include<sofa/core/objectmodel/BaseContext.h>

//---------------------------------------------------------------------------
// Implementations of the class
//---------------------------------------------------------------------------
namespace sofa
{

	namespace component
	{

		namespace forcefield
		{

			using std::set;

			using namespace sofa::defaulttype;
			using namespace sofa::component::topology;
			using namespace core::topology;

// ****************************************************************************
// Constructor / Destructor
// ****************************************************************************
			template <class DataTypes>
			QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::QuadraticTetrahedralCorotationalFEMForceField()
			: InternalData(new QuadraticTetrahedralCorotationalFEMForceFieldInternalData<DataTypes>)
			, m_updateStiffnessMatrix(core::objectmodel::BaseObject::initData(&m_updateStiffnessMatrix,true,"updateStiffnessMatrix",""))
			, m_assembling(core::objectmodel::BaseObject::initData(&m_assembling,true,"computeGlobalMatrix",""))
			, m_PoissonRatio(core::objectmodel::BaseObject::initData(&m_PoissonRatio,(Real)0.49f,"poissonRatio",""))
			, m_YoungModulus(initData(&m_YoungModulus,"youngModulus","FEM Young Modulus"))
			, m_LinearMode(core::objectmodel::BaseObject::initData(&m_LinearMode,false,"disableCorotation",""))
			, m_UseSimpleRotations(core::objectmodel::BaseObject::initData(&m_UseSimpleRotations,true,"simpleRotationMode",""))
			{
//    	this->m_PoissonRatio = 0.49;//365;//49;
//    	this->m_YoungModulus = 2061;//3000;
//    	this->m_ShearModulus = this->m_YoungModulus/(2*(1+this->m_PoissonRatio));
//    	this->m_LamesFirstParameter = (this->m_YoungModulus*this->m_PoissonRatio)/((1+this->m_PoissonRatio)*(1-2*this->m_PoissonRatio));

				counter = 0;
			}

			template <class DataTypes>
			QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::~QuadraticTetrahedralCorotationalFEMForceField()
			{
				//cleanup here
				delete InternalData;
			}

// ****************************************************************************
// Public Methods
// ****************************************************************************
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::init()
			{
				//init superclass
				this->core::behavior::ForceField<DataTypes>::init();
				m_Topology = (QuadraticBaseMeshTopology*)this->getContext()->getMeshTopology();

//	Real poissonRatio = this->m_PoissonRatio.getValue();
//	Real youngModulus = this->m_YoungModulus.getValue();
//
//	this->m_ShearModulus = youngModulus/(2*(1+poissonRatio));
//	this->m_LamesFirstParameter = (youngModulus*poissonRatio)/((1+poissonRatio)*(1-2*poissonRatio));
				this->reinit();
			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::reinit()
			{
				//---------------------------------------------------------------------------
				// Get the starting node postions!
				//---------------------------------------------------------------------------
				m_X0 = this->mstate->getX0();

				//---------------------------------------------------------------------------
				// Initialization of member variables with topology features
				//---------------------------------------------------------------------------
				m_nElements = this->m_Topology->getNbQuadTetrahedra();
				m_nNodes = this->m_Topology->getNbPoints();//TODO: make it also a quadfunction (virutal)????

				this->m_MeanRotationMatricesPerNode.resize(m_nNodes);

				this->m_QuadTetrahedronInfos.resize(m_nElements);
				m_ShapeFunctionDerivatives.resize(numberOfIntegrationPoints);
				m_ShapeFunctions.resize(numberOfIntegrationPoints);
//	InternalData.m_QuadTetrahedronInfos.resize(InternalData.m_nElements);
//	InternalData.m_CudaTetrahedraIndices.resize(10*InternalData.m_nElements);

				//---------------------------------------------------------------------------
				// The protected variables we want on the GPU!
				//---------------------------------------------------------------------------
				InitializeIntegrationPointsAndWeights();
				InitializeShapeFunctionDerivatives();
				InitializeShapeFunctions();
				InitializeElementData();
				InitializePrecomputedStiffnessBlocks();

//	OldInitializeIntegrationPointsAndWeights(numberOfIntegrationPoints);
//	OldInitializeShapeFunctionDerivatives(numberOfIntegrationPoints);
//	OldInitializeShapeFunctions(numberOfIntegrationPoints);

//	AssembleElementMatrices();
			}

//---------------------------------------------------------------------------
// AddForce function
//---------------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v)
			{
				//**************IMPORTANT: Assume that addForce is always called before addDForce and addKToMatrix*******//

				//---------------------------------------------------------------------------
				// Debugging Vectors to calculate error
				//---------------------------------------------------------------------------
//	VecDeriv debug_force(f);
//	VecDeriv error(f.size());

				//---------------------------------------------------------------------------
				// Standard CPU Code
				//---------------------------------------------------------------------------

				//----------------------------------------------------------------------------------------------------------------
				// Define local variables for CPU code
				//----------------------------------------------------------------------------------------------------------------
				QuadraticTetraType tetraIndices;

				ElementStiffnessMatrixType elementStiffnessMatrix;
				ElementStiffnessMatrixType elementStiffnessMatrixX0;
				PrecomputedStiffnessBlockType currentStiffnessBlock;
				PrecomputedStiffnessBlockType currentStiffnessBlockX0;
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrixArray[numberOfIntegrationPoints];

				int mn, ij;
				RotationMatrixType S, R, Rtransp;
				// For easy calculations
				Mat<3,3,Real> Sofa_S, Sofa_R, Sofa_Rtransp;
				Mat<3,3,Real> Sofa_currentStiffnessBlock, Sofa_currentStiffnessBlockX0;
				Mat<3,3,Real> phi;
				Mat<nodesPerElement,3,Real> shapeFunctionDerivatives;
				Mat<4,3,Real> shapeFunctionDerivativesSimple;
				Mat<3,nodesPerElement,Real> currentCoords;
				Mat<3,4,Real> currentCoordsSimple;

//	const VecCoord *X0=this->mstate->getX0();//done before in init

				//-----------------------------------------------------------------------
				// Loop over all elements to calculate forces there!
				//-----------------------------------------------------------------------

				sofa::helper::AdvancedTimer::stepBegin("ExtractRotation");

				unsigned int currentNumberOfIntPoints =4;

				if(this->m_UseSimpleRotations.getValue() || this->m_LinearMode.getValue() )
				currentNumberOfIntPoints =1;

				for(unsigned int iterElement=0;iterElement<m_nElements;iterElement++)
				{
					tetraIndices = this->m_Topology->getQuadTetrahedron(iterElement);

					std::memcpy(&stiffnessBlockMatrixArray, &(m_QuadTetrahedronInfos[iterElement].precomputedElementStiffnessBlockMatrixArray), sizeof(PrecomputedStiffnessBlockMatrixType)*numberOfIntegrationPoints );

					//first rebuild the elementStiffnessMatrix and elementStiffnessMatrixX0; TODO: necessary?
					for (int hoch = 0; hoch <3*nodesPerElement;hoch++ )
					{
						for (int zaehlen = 0; zaehlen<3*nodesPerElement;zaehlen++)
						{
							elementStiffnessMatrix[hoch][zaehlen] = 0;
							elementStiffnessMatrixX0[hoch][zaehlen] = 0;
						}
					}

					//-----------------------------------------------------------------------
					//loop over all integration points
					//-----------------------------------------------------------------------

					for(unsigned int iterIntPoint=0;iterIntPoint<currentNumberOfIntPoints;iterIntPoint++)
					{

						std::memcpy( &(stiffnessBlockMatrix), &(stiffnessBlockMatrixArray[iterIntPoint]), sizeof(PrecomputedStiffnessBlockMatrixType) );

						//loop over all i,m
						for(unsigned int m=0;m<nodesPerElement;m++)
						{
							for(unsigned int i=0;i<nodesPerElement;i++)
							{

								//extract 3x3 precomputed stiffness block Fim
//				currentStiffnessBlock = stiffnessBlockMatrix[i][m];
								std::memcpy( (Sofa_currentStiffnessBlock.begin()), &stiffnessBlockMatrix[i][m], sizeof(PrecomputedStiffnessBlockType) );

								if(this->m_UseSimpleRotations.getValue())
								{
									//determine the rotation matrix and transpose
									//				shapeFunctionDerivatives = m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];
									std::memcpy( shapeFunctionDerivativesSimple.begin(), &(m_QuadTetrahedronInfos[iterElement].simpleShapeFunctionDerivativesGlobal[iterIntPoint]), sizeof(SimpleShapeFunctionDerivativesGlobalType) );

									for(int iter1=0;iter1<4;iter1++) {
										for(int iter2=0;iter2<3;iter2++) {
											currentCoordsSimple[iter2][iter1] = x[tetraIndices[iter1]][iter2];
										}
									}

									// Make it non-linear
									phi = currentCoordsSimple * shapeFunctionDerivativesSimple;
								}
								else
								{
									//determine the rotation matrix and transpose
									//				shapeFunctionDerivatives = m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];
									std::memcpy( shapeFunctionDerivatives.begin(), &(m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint]), sizeof(ShapeFunctionDerivativesGlobalType) );

									for(int iter1=0;iter1<nodesPerElement;iter1++) {
										for(int iter2=0;iter2<3;iter2++) {
											currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
										}
									}

									// Make it non-linear
									phi = currentCoords * shapeFunctionDerivatives;
								}

								helper::Decompose<Real>::polarDecomposition(phi, Sofa_R);
								Sofa_Rtransp = Sofa_R.transposed();

//				m_QuadTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
//				m_QuadTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;

								std::memcpy( &m_QuadTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] ,&Sofa_R, sizeof(RotationMatrixType) );
								std::memcpy( &m_QuadTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint], &Sofa_Rtransp , sizeof(RotationMatrixType) );

								if(!this->m_LinearMode.getValue())
								{
									//calculate R*Fim and save as currentStiffnessBlockX0
									Sofa_currentStiffnessBlockX0 = Sofa_R*Sofa_currentStiffnessBlock;

									//calculate currentStiffnessBlockX0*RT (=R*Fim*RT) and save as currentStiffnessBlock
									Sofa_currentStiffnessBlock = Sofa_currentStiffnessBlockX0*Sofa_Rtransp;
								}
								else
								{
									Sofa_currentStiffnessBlockX0 = Sofa_currentStiffnessBlock;
								}

								// Copy the results to the C array
								std::memcpy( &currentStiffnessBlock, Sofa_currentStiffnessBlock.begin() , sizeof(PrecomputedStiffnessBlockType) );
								std::memcpy( &currentStiffnessBlockX0 , Sofa_currentStiffnessBlockX0.begin() ,sizeof(PrecomputedStiffnessBlockType) );

								//add the blocks into elementStiffnessMatrix and elementStiffnessMatrixX0
								for(unsigned int n=0;n<3;n++)
								{
									for(unsigned int j=0;j<3;j++)
									{
										mn = 3*m+n;
										ij = 3*i+j;

										elementStiffnessMatrix[ij][mn] += currentStiffnessBlock[j][n];
										elementStiffnessMatrixX0[ij][mn] += currentStiffnessBlockX0[j][n];
//					  elementStiffnessMatrix[ij][mn] += stiffnessBlockMatrix[i][m][j][n];			// make it linear and do not use currentStiffnessBlock
//					  elementStiffnessMatrixX0[ij][mn] += stiffnessBlockMatrix[i][m][j][n];			// make it linear and do not use currentStiffnessBlock

									} //end for j
								} //end for n
							} //end for i
						} //end for m
					} //end for iterintpiont

					//-----------------------------------------------------------------------
					//save element stiffness matrix in tetrahedron datastructure //TODO: NOT COPY TO HAVE A LOOK AS GPU does it already: Seems to work well
					//-----------------------------------------------------------------------
					std::memcpy(&(m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix), &(elementStiffnessMatrix), sizeof(ElementStiffnessMatrixType) );

					// Write Elementmatrix of element 1 to CVS file!
//	if(iterElement == 13)
//	{
//		WriteElementMatrixToCVSFile(elementStiffnessMatrix, "QuadraticElementStiffnessMatrix97_", 13);
//		std::cin.get();
//	}
					//-----------------------------------------------------------------------
					//	Now use the element stiffness matrices to calculate force
					//-----------------------------------------------------------------------
					for(unsigned int m=0; m<nodesPerElement;m++)
					{
						for(unsigned int n=0; n<3;n++) // VecCoord: which coordinate (x,y,z)
						{
							int posColumn = tetraIndices[m]; // VecCoord: which node
							int posColumnElement = 3*m+n;

							for(unsigned int i=0; i<nodesPerElement;i++)
							{
								int posRow = tetraIndices[i];

								Real forceX = ((*m_X0)[posColumn][n])*elementStiffnessMatrixX0[3*i+0][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
								Real forceY = ((*m_X0)[posColumn][n])*elementStiffnessMatrixX0[3*i+1][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
								Real forceZ = ((*m_X0)[posColumn][n])*elementStiffnessMatrixX0[3*i+2][posColumnElement]-((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];

								// For putting force together node-wise (used while parallelization)
//				if(m==0 && n==0)
//				{
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][0] = forceX;
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][1] = forceY;
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][2] = forceZ;
//				}
//				else
//				{
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][0] += forceX;
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][1] += forceY;
//					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][2] += forceZ;
//				}

//				f[posRow] += Deriv((float)iterElement, (float)iterElement, (float)iterElement);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
								f[posRow] += Deriv(forceX, forceY, forceZ);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];

							}
						}
					}

					//-----------------------------------------------------------------------
					// print force vector
					//-----------------------------------------------------------------------

//#ifdef VERBOSITY
//					//-----------------------------------------------------------------------
//					// print element CoordForces
//					//-----------------------------------------------------------------------
//					printf("elementCoordForces of Element #%d for debugging\n", iterElement);
//
//					for(int node=0;node<nodesPerElement;node++)
//					{
//						for(int pos=0;pos<3;pos++)
//						{
//							printf("%f ",m_QuadTetrahedronInfos[iterElement].elementCoordForces[node][pos] );
//						}
//						printf("\n");
//					}
//
//					//-----------------------------------------------------------------------
//					// Print CPU force vector
//					//-----------------------------------------------------------------------
//					printf("ForceVetor saved in CPU for debugging\n");
//
//					for (int m=0; m<nodesPerElement;m++)
//					{
//						for(int j=0; j<3; j++)
//						{
//							printf("%f  ", debug_force[tetraIndices[m]][j]);
//						}
//						printf("@ tetraIndex:%d\n", tetraIndices[m]); //end tetrahedron
//					}
//					printf("\n"); // endelement
//#endif//VERBOSITY
				} //over  elements

				  //-----------------------------------------------------------------------
				  // print force vector
				  //-----------------------------------------------------------------------
//	for(unsigned int i=0;i<m_nNodes;i++)
//	{
//		for(unsigned int j=0;j<3;j++)
//		{
//			printf("f[%d][%d] = %f ",i,j, (f)[i][j] );
//		}
//		printf("\n");
//	}
//	std::cin.get();

//#ifdef VERBOSITIY
//#ifndef USE_CPU_CODE
//				//-----------------------------------------------------------------------
//				// Sum up for the forcevector from internaldata on CPU
//				//-----------------------------------------------------------------------
//				for(unsigned int iterNodes=0; iterNodes<InternalData.m_nNodes; iterNodes++)
//				{
//					for(unsigned int iterConnectedElementsToNode=0; iterConnectedElementsToNode < cudaNElementsPerNode[iterNodes]; iterConnectedElementsToNode++)
//					{
//						for(unsigned int i=0; i<nodesPerElement;i++)
//						{
////
//							int iterElement = cudaElementsByNode[cudaWhereDoesNextNodeStartInElementsByNode[iterNodes]+iterConnectedElementsToNode];
//							int node = InternalData.m_CudaTetrahedraIndices[4*iterElement+i];
//
//							if(node == iterNodes)// The other possibility is to go through all nodes of the element and disable element for other nodes
//							{
//								Real t0 = m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][0];
//								Real t1 = m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][1];
//								Real t2 = m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][2];
//
//								debug_force[node][0] += t0;
//								debug_force[node][1] += t1;
//								debug_force[node][2] += t2;
//
////					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][0] = 0.0;
////					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][1] = 0.0;
////					m_QuadTetrahedronInfos[iterElement].elementCoordForces[i][2] = 0.0;
//							}
//
//						}
//					}
//
//				}
//#endif //USE_CPU_CODE
//				//-----------------------------------------------------------------------
//				// Calculate absolute error between GPU and CPU
//				//-----------------------------------------------------------------------
//				printf("\nErrorVetor GPU minus CPU\n");
//
//				{
//					for(unsigned int i=0;i<error.size();i++)
//					{
//						error[i] = f[i] - debug_force[i];
//						for(int j=0; j<3; j++)
//						{
//							printf("%f  ", error[i][j]);
//						}
//						printf("@ tetraIndex:%d\n", i); //end tetrahedron
//					}
//				}
//				printf("\n"); // endelement
//				printf("\n Number of Nodes: %d\n",InternalData.m_nNodes);
//#endif//VERBOSITY
//#endif//USE_CPU_CODE
				//calculate mean rotation per node
				sofa::helper::AdvancedTimer::stepEnd("ExtractRotation");

				//sofa::helper::AdvancedTimer::stepBegin("Calculate Mean Rotation");
				unsigned int nNodes = m_Topology->getNbPoints();

				RotationMatrixType RMean;
				RMean.clear();
				component::topology::QuadraticMeshTopology::QuadTetrahedraAroundVertex tetrahedraAroundVertex;
				unsigned int numberOfElementsAroundNode;
				int currentElementId;
				int localNodeId;

				Mat<3,3,Real> PhiMean;
				PhiMean.clear();

				if(this->m_UseSimpleRotations.getValue() )
				{
					for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
					{
						PhiMean.clear();
						//wholeVolume = 0;

						//get all elements around the node
						(tetrahedraAroundVertex) = m_Topology->getQuadsAroundVertex(iterNode);
						numberOfElementsAroundNode = tetrahedraAroundVertex.size();

						// get all the rotation matrices for each element around the node
						for(unsigned int i=0;i<numberOfElementsAroundNode;i++)
						{
							currentElementId = tetrahedraAroundVertex[i];
							tetraIndices = this->m_Topology->getQuadTetrahedron(currentElementId);
							//find local node number
							//localNodeId = m_Topology->getVertexIndexInTetrahedron(tetraIndices,iterNode);

							//determine the rotation matrix

							for(unsigned int iterIntPoints=0;iterIntPoints<1;iterIntPoints++)
							{

								shapeFunctionDerivatives = this->m_QuadTetrahedronInfos[currentElementId].simpleShapeFunctionDerivativesGlobal[iterIntPoints];

								for(int iter1=0;iter1<4;iter1++) {
									for(int iter2=0;iter2<3;iter2++) {
										currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
									}
								}

								phi = currentCoords * shapeFunctionDerivatives;

								//					currentVolume = this->m_QuadTetrahedronInfos[currentElementId].volume;
								//					wholeVolume += currentVolume;

								PhiMean = phi + PhiMean;//PhiMean + currentVolume*phi;

							}

						}

						PhiMean = PhiMean/(numberOfElementsAroundNode); //wholeVolume;
						helper::Decompose<Real>::polarDecomposition(PhiMean, RMean);

						this->m_MeanRotationMatricesPerNode[iterNode] = RMean;

					}
				}
				else
				{

				for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
				{
					PhiMean.clear();
					//wholeVolume = 0;

					//get all elements around the node
					(tetrahedraAroundVertex) = m_Topology->getQuadsAroundVertex(iterNode);
					numberOfElementsAroundNode = tetrahedraAroundVertex.size();

					// get all the rotation matrices for each element around the node
					for(unsigned int i=0;i<numberOfElementsAroundNode;i++)
					{
						currentElementId = tetrahedraAroundVertex[i];
						tetraIndices = this->m_Topology->getQuadTetrahedron(currentElementId);
						//find local node number
						//localNodeId = m_Topology->getVertexIndexInTetrahedron(tetraIndices,iterNode);

						//determine the rotation matrix

						for(unsigned int iterIntPoints=0;iterIntPoints<4;iterIntPoints++)
						{

							shapeFunctionDerivatives = this->m_QuadTetrahedronInfos[currentElementId].shapeFunctionDerivativesGlobal[iterIntPoints];

							for(int iter1=0;iter1<NUMBER_OF_ELEMENT_NODES_TET10;iter1++) {
								for(int iter2=0;iter2<3;iter2++) {
									currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
								}
							}

							phi = currentCoords * shapeFunctionDerivatives;

							//					currentVolume = this->m_QuadTetrahedronInfos[currentElementId].volume;
							//					wholeVolume += currentVolume;

							PhiMean = phi + PhiMean;//PhiMean + currentVolume*phi;

						}

					}

					PhiMean = PhiMean/(4*numberOfElementsAroundNode); //wholeVolume;
					helper::Decompose<Real>::polarDecomposition(PhiMean, RMean);

					this->m_MeanRotationMatricesPerNode[iterNode] = RMean;

				}
				}

				//sofa::helper::AdvancedTimer::stepEnd("Calculate Mean Rotation");

			} //addForce

//---------------------------------------------------------------------------
// AddDForce Function
//---------------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::addDForce ( VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor)
			{
				kFactor = kFactor*(-1);

				//---------------------------------------------------------------------------
				//	implement addMDx on a per element basis - it will be faster to just use the complete mass matrix and perform a matrix vector multiplication
				//---------------------------------------------------------------------------
				for(unsigned int iterElement=0;iterElement<m_nElements;iterElement++)
				{
					//ElementStiffnessMatrixType elementStiffnessMatrix;// = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;

					//std::memcpy(&(elementStiffnessMatrix), &(m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix), sizeof(ElementStiffnessMatrixType) );

					ElementStiffnessMatrixType* elementStiffnessMatrix = &(m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix);

					// Write Elementmatrix of element 1 to CVS file!
//		if(iterElement == 13)
					{
//		WriteElementMatrixToCVSFile(elementStiffnessMatrix, "FloatQuadraticElementStiffnessMatrix97_df", iterElement);
//		std::cin.get();
					}

					QuadraticTetraType tetraIndices = this->m_Topology->getQuadTetrahedron(iterElement);

					for(unsigned int m=0; m<nodesPerElement;m++)
					{

						for(unsigned int n=0; n<3;n++)
						{

							int posColumn = tetraIndices[m];
							int posColumnElement = 3*m+n;

							for(unsigned int i=0; i<nodesPerElement;i++)
							{
								int posRow = tetraIndices[i];

//				double test1 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement]*kFactor;
//				double test2 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement]*kFactor;
//				double test3 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement]*kFactor;

								Real dForceX = ((dx[posColumn])[n])*(*elementStiffnessMatrix)[3*i+0][posColumnElement]*kFactor;
								Real dForceY = ((dx[posColumn])[n])*(*elementStiffnessMatrix)[3*i+1][posColumnElement]*kFactor;
								Real dForceZ = ((dx[posColumn])[n])*(*elementStiffnessMatrix)[3*i+2][posColumnElement]*kFactor;

								df[posRow] += Deriv(dForceX, dForceY, dForceZ);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];

							}
						}
					}
				}
//	for(unsigned int i=0;i<m_nNodes;i++)
//	{
//		for(unsigned int j=0;j<3;j++)
//		{
//			printf("dx[%d] = %f ",i, dx[i][j] );
//		}
//		printf("\n");
//	}
//	std::cin.get();

			} //addDForce

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::applyRotation(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, sofa::core::MultiVecDerivId dxId, sofa::core::MultiVecDerivId dfId, bool invert )
			{
				unsigned int nElements=m_Topology->getNbTetrahedra();
				unsigned int nNodes = m_Topology->getNbPoints();
				//this->

				//const DataVecDeriv & dxData = *mparams->readDx(this->mstate);

				const DataVecDeriv & dxData = *dxId[(this->mstate).get(mparams)].read();

				//*fId[mstate.get(mparams)].write()
				//DataVecDeriv & dxData = *dxId[this->mstate.get(mparams)].read();

				const VecDeriv& dx = dxData.getValue(mparams);

				DataVecDeriv & dfData = *dfId[(this->mstate).get()].write();

				VecDeriv& df = *dfData.beginEdit(mparams);

				RotationMatrixType RMean;
				QuadraticTetraType tetraIndices;
				int currentNodeId;

				for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
				{
					// get mean rotation matrix
					RMean = this->m_MeanRotationMatricesPerNode[iterNode];

					if(invert)
					RMean = RMean.transposed();

					Deriv currentDx = (dx[iterNode]);
					Deriv result;

					result = RMean * currentDx;
//					result = currentDx;

					df[iterNode] = result;

				}

				dfData.endEdit(mparams);

			}

//---------------------------------------------------------------------------
// AddKToMatrix Function
//---------------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::addKToMatrix( sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset)
			{
				kFactor = kFactor*(-1);

				unsigned int nElements=this->m_Topology->getNbQuadTetrahedra();

				ElementStiffnessMatrixType elementStiffnessMatrix;

				QuadraticTetraType tetraIndices;

				unsigned int posRow, posColumn, posRowElement, posColumnElement;

				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
				{
//	  elementStiffnessMatrix = InternalData.m_CudaLinearTetrahedraInformation[iterElement].ElementStiffnessMatrix;
					std::memcpy(&(elementStiffnessMatrix), &(m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix), sizeof(ElementStiffnessMatrixType));
					tetraIndices = this->m_Topology->getQuadTetrahedron(iterElement);

					for(unsigned int m=0; m<nodesPerElement;m++)
					{
						for(unsigned int n=0; n<3;n++)
						{
							posColumn = 3*tetraIndices[m]+n + offset;
							posColumnElement = 3*m+n;

							for(unsigned int i=0; i<nodesPerElement;i++)
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

			} // end AddKToMatrix

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::handleTopologyChange()
			{
				// std::list<const TopologyChange *>::const_iterator itBegin=_topology->beginChange();
				//std::list<const TopologyChange *>::const_iterator itEnd=_topology->endChange();

			}

			template <class DataTypes>
			double QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::getPotentialEnergy(const VecCoord& x) const
			{
				serr << "Method getPotentialEnergy() not implemented"<<sendl;
			}

// ****************************************************************************
// Protected/Private Methods
// ****************************************************************************

//-----------------------------------------------------------------------
// The integration points and weights needed by the other init functions calculated here
//-----------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializeIntegrationPointsAndWeights()
			{

				//Integration points for some cases!
				double alpha1 = 0.58541020;
				double beta1 = 0.13819660;

				double alpha2 = 0.785714285714286;
				double beta2 = 0.071428571428571;

				double alpha3 = 0.399403576166799;
				double beta3 = 0.100596423833201;

				//Fill the arrays!
				switch(numberOfIntegrationPoints)
				{
					case 1:
					m_IntPoints[0] = 0.25;
					m_IntPoints[1] = 0.25;
					m_IntPoints[2] = 0.25;
					m_IntWeights[0] = (double)1.0/(double)6.0;
					break;
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
					std::cerr << "Wrong number of integration points" << std::endl;
					break;
				} //end switch

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
				this->m_NodalLocalCoordinates[12] = 0.5;
				this->m_NodalLocalCoordinates[13] = 0.5;
				this->m_NodalLocalCoordinates[14] = 0.0;
				this->m_NodalLocalCoordinates[15] = 0.0;
				this->m_NodalLocalCoordinates[16] = 0.5;
				this->m_NodalLocalCoordinates[17] = 0.0;
				this->m_NodalLocalCoordinates[18] = 0.5;
				this->m_NodalLocalCoordinates[19] = 0.0;
				this->m_NodalLocalCoordinates[20] = 0.0;
				this->m_NodalLocalCoordinates[21] = 0.0;
				this->m_NodalLocalCoordinates[22] = 0.5;
				this->m_NodalLocalCoordinates[23] = 0.5;
				this->m_NodalLocalCoordinates[24] = 0.5;
				this->m_NodalLocalCoordinates[25] = 0.0;
				this->m_NodalLocalCoordinates[26] = 0.5;
				this->m_NodalLocalCoordinates[27] = 0.0;
				this->m_NodalLocalCoordinates[28] = 0.0;
				this->m_NodalLocalCoordinates[29] = 0.5;

			} //InitializeIntegrationPointsAndWeights

//-----------------------------------------------------------------------
// Shape functions and their derivatives
//-----------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializeShapeFunctionDerivatives()
			{
				// local variables
				Real r, s, t, tempExp;

				//iterate over all integration points
				for(unsigned int i=0; i< numberOfIntegrationPoints;i++)
				{
					r = m_IntPoints[3*i];
					s = m_IntPoints[3*i+1];
					t = m_IntPoints[3*i+2];

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

					// Fill the vector and member variable directly in order to avoid pointer problems and unnecessary copy actions!
					m_ShapeFunctionDerivatives[i][0][0] = 4*r-1;
					m_ShapeFunctionDerivatives[i][0][1] = 0;
					m_ShapeFunctionDerivatives[i][0][2] = 0;

					m_ShapeFunctionDerivatives[i][1][0] = 0;
					m_ShapeFunctionDerivatives[i][1][1] = 4*s-1;
					m_ShapeFunctionDerivatives[i][1][2] = 0;

					m_ShapeFunctionDerivatives[i][2][0] = -4*tempExp+1;
					m_ShapeFunctionDerivatives[i][2][1] = -4*tempExp+1;
					m_ShapeFunctionDerivatives[i][2][2] = -4*tempExp+1;

					m_ShapeFunctionDerivatives[i][3][0] = 0;
					m_ShapeFunctionDerivatives[i][3][1] = 0;
					m_ShapeFunctionDerivatives[i][3][2] = 4*t-1;

					m_ShapeFunctionDerivatives[i][4][0] = 4*s;
					m_ShapeFunctionDerivatives[i][4][1] = 4*r;
					m_ShapeFunctionDerivatives[i][4][2] = 0;

					m_ShapeFunctionDerivatives[i][5][0] = -4*s;
					m_ShapeFunctionDerivatives[i][5][1] = 4*(tempExp-s);
					m_ShapeFunctionDerivatives[i][5][2] = -4*s;

					m_ShapeFunctionDerivatives[i][6][0] = 4*(tempExp-r);
					m_ShapeFunctionDerivatives[i][6][1] = -4*r;
					m_ShapeFunctionDerivatives[i][6][2] = -4*r;

					m_ShapeFunctionDerivatives[i][7][0] = 4*t;
					m_ShapeFunctionDerivatives[i][7][1] = 0;
					m_ShapeFunctionDerivatives[i][7][2] = 4*r;

					m_ShapeFunctionDerivatives[i][8][0] = 0;
					m_ShapeFunctionDerivatives[i][8][1] = 4*t;
					m_ShapeFunctionDerivatives[i][8][2] = 4*s;

					m_ShapeFunctionDerivatives[i][9][0] = -4*t;
					m_ShapeFunctionDerivatives[i][9][1] = -4*t;
					m_ShapeFunctionDerivatives[i][9][2] = 4*(tempExp-t);
				}

				//iterate nodes
				for(unsigned int i=0; i< NUMBER_OF_ELEMENT_NODES_TET10;i++)
				{

					r = this->m_NodalLocalCoordinates[3*i];
					s = this->m_NodalLocalCoordinates[3*i+1];
					t = this->m_NodalLocalCoordinates[3*i+2];

					tempExp = 1-r-s-t;

					// Fill the vector and member variable directly in order to avoid pointer problems and unnecessary copy actions!
					m_ShapeFunctionDerivativesPerNode[i][0][0] = 4*r-1;
					m_ShapeFunctionDerivativesPerNode[i][0][1] = 0;
					m_ShapeFunctionDerivativesPerNode[i][0][2] = 0;

					m_ShapeFunctionDerivativesPerNode[i][1][0] = 0;
					m_ShapeFunctionDerivativesPerNode[i][1][1] = 4*s-1;
					m_ShapeFunctionDerivativesPerNode[i][1][2] = 0;

					m_ShapeFunctionDerivativesPerNode[i][2][0] = -4*tempExp+1;
					m_ShapeFunctionDerivativesPerNode[i][2][1] = -4*tempExp+1;
					m_ShapeFunctionDerivativesPerNode[i][2][2] = -4*tempExp+1;

					m_ShapeFunctionDerivativesPerNode[i][3][0] = 0;
					m_ShapeFunctionDerivativesPerNode[i][3][1] = 0;
					m_ShapeFunctionDerivativesPerNode[i][3][2] = 4*t-1;

					m_ShapeFunctionDerivativesPerNode[i][4][0] = 4*s;
					m_ShapeFunctionDerivativesPerNode[i][4][1] = 4*r;
					m_ShapeFunctionDerivativesPerNode[i][4][2] = 0;

					m_ShapeFunctionDerivativesPerNode[i][5][0] = -4*s;
					m_ShapeFunctionDerivativesPerNode[i][5][1] = 4*(tempExp-s);
					m_ShapeFunctionDerivativesPerNode[i][5][2] = -4*s;

					m_ShapeFunctionDerivativesPerNode[i][6][0] = 4*(tempExp-r);
					m_ShapeFunctionDerivativesPerNode[i][6][1] = -4*r;
					m_ShapeFunctionDerivativesPerNode[i][6][2] = -4*r;

					m_ShapeFunctionDerivativesPerNode[i][7][0] = 4*t;
					m_ShapeFunctionDerivativesPerNode[i][7][1] = 0;
					m_ShapeFunctionDerivativesPerNode[i][7][2] = 4*r;

					m_ShapeFunctionDerivativesPerNode[i][8][0] = 0;
					m_ShapeFunctionDerivativesPerNode[i][8][1] = 4*t;
					m_ShapeFunctionDerivativesPerNode[i][8][2] = 4*s;

					m_ShapeFunctionDerivativesPerNode[i][9][0] = -4*t;
					m_ShapeFunctionDerivativesPerNode[i][9][1] = -4*t;
					m_ShapeFunctionDerivativesPerNode[i][9][2] = 4*(tempExp-t);

				}

				if(this->m_UseSimpleRotations.getValue())
				{
					MatNoInit<4,3,Real> currentShapeFunctionDerivatives;

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

					m_SimpleShapeFunctionDerivative = currentShapeFunctionDerivatives;

				}

			} //InitializeShapeFunctionDerivatives

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializeShapeFunctions()
			{
				// local variables
				Real r, s, t, tempExp;

				//iterate over all integration points
				for(unsigned int i=0; i< numberOfIntegrationPoints;i++)
				{
					r = m_IntPoints[3*i];
					s = m_IntPoints[3*i+1];
					t = m_IntPoints[3*i+2];

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

					m_ShapeFunctions[i][0] = (2*r-1)*r;
					m_ShapeFunctions[i][1] = (2*s-1)*s;
					m_ShapeFunctions[i][2] = (2*tempExp-1)*tempExp;
					m_ShapeFunctions[i][3] = (2*t-1)*t;
					m_ShapeFunctions[i][4] = 4*r*s;
					m_ShapeFunctions[i][5] = 4*tempExp*s;
					m_ShapeFunctions[i][6] = 4*tempExp*r;
					m_ShapeFunctions[i][7] = 4*r*t;
					m_ShapeFunctions[i][8] = 4*s*t;
					m_ShapeFunctions[i][9] = 4*tempExp*t;
				} // for i

			} //InitializeShapeFunction

//-----------------------------------------------------------------------
// Finish by putting calculated data until now in element struct
//-----------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializeElementData()
			{
				//loop over all elements
				for(unsigned int iterElement=0;iterElement<m_nElements;iterElement++)
				{
					//extract coords (this is a fixed array of 10)TODO: can be replaced as indices should be saved in a vector as well!
					const QuadraticTetraType tetraIndices = this->m_Topology->getQuadTetrahedron(iterElement);

					// The coords of the tetrahedron are stored here locally!
					ElementCoordsType coords;

					for(unsigned int i=0; i<nodesPerElement;i++)
					{
						coords[i] = (*m_X0)[tetraIndices[i]];
					}

					//loop over all integration points
					for(unsigned int integrationPointIndex=0; integrationPointIndex< numberOfIntegrationPoints;integrationPointIndex++)
					{
						//Get the Jacobian Matrix
						Mat<3,3,Real> jacobianMatrix;
						GetJacobian(jacobianMatrix,coords,integrationPointIndex);

						Mat<3,3,Real> inverseJacobian;
						sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

						//transform shape function derivatives in global coordinates

//			  Mat<10,3,Real> shapeFunctionDerivativesGlobal;
//			  shapeFunctionDerivativesGlobal.clear();

						ShapeFunctionDerivativesGlobalType shapeFunctionDerivativesGlobal = {0};

						for(unsigned int i=0;i<10;i++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								for(unsigned int k=0;k<3;k++)
								{
									shapeFunctionDerivativesGlobal[i][j]+=m_ShapeFunctionDerivatives[integrationPointIndex][i][k]*inverseJacobian[k][j];
//										  InternalData.m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[integrationPointIndex][i][j]+=InternalData.m_CudaShapeFunctionDerivatives[integrationPointIndex][i][k]*inverseJacobian[k][j];
//										  printf(" shapeFunctionDerivativeGlobal[%d][%d] = %f at adress: %p\n", i , j, shapeFunctionDerivativesGlobal[i][j], &(shapeFunctionDerivativesGlobal[i][j]) );
								}
							}
						}

						// Fill part of the TetrahedronInfos!
						std::memcpy( &(m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[integrationPointIndex]), &(shapeFunctionDerivativesGlobal), sizeof(ShapeFunctionDerivativesGlobalType) );

					} //over integration points

					  //loop over all nodal points
					for(unsigned int nodalPointIndex=0; nodalPointIndex< NUMBER_OF_ELEMENT_NODES_TET10; nodalPointIndex++)
					{

						Mat<3,3,Real> jacobianMatrix;

						this->GetNodalJacobian(jacobianMatrix,coords,nodalPointIndex);

						Mat<3,3,Real> inverseJacobian;
						sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

						//get the shape function derivatives in local coordinates
						Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real> shapeFunctionDerivativesLocal = this->m_ShapeFunctionDerivativesPerNode[nodalPointIndex];

						Mat<NUMBER_OF_ELEMENT_NODES_TET10,3,Real> shapeFunctionDerivativesGlobal;
						shapeFunctionDerivativesGlobal.clear();
						for(unsigned int i=0;i<NUMBER_OF_ELEMENT_NODES_TET10;i++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								for(unsigned int k=0;k<3;k++)
								{
									shapeFunctionDerivativesGlobal[i][j]+=shapeFunctionDerivativesLocal[i][k]*inverseJacobian[k][j];
								}
							}
						}

						this->m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobalPerNode[nodalPointIndex]=shapeFunctionDerivativesGlobal;

					}

					if(this->m_UseSimpleRotations.getValue())
					{
						//extract coords
						ElementSimpleCoordsType simpleCoords;

						for(unsigned int i=0; i<4;i++)
						{
							simpleCoords[i] = coords[i];
						}

						Mat<3,3,Real> jacobianMatrix;

						this->GetSimpleJacobian(jacobianMatrix,simpleCoords);

						Mat<3,3,Real> inverseJacobian;
						sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

						//get the shape function derivatives in local coordinates
						Mat<4,3,Real> shapeFunctionDerivativesLocal = this->m_SimpleShapeFunctionDerivative;

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
						this->m_QuadTetrahedronInfos[iterElement].simpleShapeFunctionDerivativesGlobal[0]=shapeFunctionDerivativesGlobal;

					}

				} //over elements

			} //Initialize ElementData

//-----------------------------------------------------------------------
// Reference StiffnessMatrix
//-----------------------------------------------------------------------
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializePrecomputedStiffnessBlocks()
			{

				for(unsigned int iterElement=0; iterElement<m_nElements;iterElement++)
				{
					InitializePrecomputedStiffnessBlockForElement(iterElement);
				}

			} //InitializePrecomputedStiffnessBlocks

			template <class DataTypes>
			inline double QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::GetElementVolume(unsigned int elementID, ElementCoordsType coords)
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
			inline void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::InitializePrecomputedStiffnessBlockForElement(unsigned int elementID)
			{
				// Helper matrix to store the stiffness block for the current element
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;// = {0};//TODO: check for error!
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrixSimpleRotation;

				//Nodes of the current element
				const QuadraticTetraType tetraIndices = this->m_Topology->getQuadTetrahedron(elementID);

				//The coords of each element node (Quadratic Tetrahedra means 10 nodes)
				ElementCoordsType elementCoords;

				// Read the coords for each node of the element!
				for(unsigned int i=0; i<nodesPerElement;i++)
				{
					elementCoords[i] = (*m_X0)[tetraIndices[i]];
				}

				for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
				{

					BuildStiffnessBlock(elementID, stiffnessBlockMatrix, elementCoords, iterIntPoint);

					//Now copy in strucure containing the important data on gpu!
//		InternalData.m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[iter] = stiffnessBlockMatrix;//TODO: Does the following line work?
					std::memcpy( &(m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[iterIntPoint][0][0]), &(stiffnessBlockMatrix), sizeof(PrecomputedStiffnessBlockMatrixType) );

				}

				//in simple mode: use just one integration point
				if(this->m_UseSimpleRotations.getValue())
				{
					for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
					{
						std::memcpy(&(stiffnessBlockMatrix), &(m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[iterIntPoint][0][0]), sizeof(PrecomputedStiffnessBlockMatrixType) );

						//loop over all i,m
						for(unsigned int m=0;m<nodesPerElement;m++)
						{
							for(unsigned int i=0;i<nodesPerElement;i++)
							{
								stiffnessBlockMatrixSimpleRotation[m][i] += stiffnessBlockMatrix[m][i];
							}
						}
					}

					std::memcpy( &(m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[0][0][0]), &(stiffnessBlockMatrixSimpleRotation), sizeof(PrecomputedStiffnessBlockMatrixType) );
				}

				this->m_QuadTetrahedronInfos[elementID].volume = this->GetElementVolume(elementID, elementCoords);

			} //InitializePrecomputedStiffnessBlockForElement

			template <class DataTypes>
			inline void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::BuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlockMatrix,ElementCoordsType coords, unsigned int integrationPointIndex)
			{
				//-----------------------------------------------------------------------
				//Matrix containing stiffnessBlock
				//-----------------------------------------------------------------------
				ElementStiffnessMatrixType stiffnessBlock;

				//-----------------------------------------------------------------------
				//Get the parameters used for calculation
				//-----------------------------------------------------------------------
				//get lambda, mu
				Real youngModulusElement;
				if (m_YoungModulus.getValue().size() == this->m_Topology->getNbQuadTetrahedra()) youngModulusElement = m_YoungModulus.getValue()[elementID];
				else if (m_YoungModulus.getValue().size() > 0) youngModulusElement = m_YoungModulus.getValue()[0];
				else
				{
					setYoungModulus(300000.0f);
					youngModulusElement = m_YoungModulus.getValue()[0];
				}

				double mu = youngModulusElement/(2*(1+this->m_PoissonRatio.getValue()));
				double lambda = (youngModulusElement*this->m_PoissonRatio.getValue())/((1+this->m_PoissonRatio.getValue())*(1-2*this->m_PoissonRatio.getValue()));

				//std::cout<<"Young modulus at element "<<elementID<<" is "<<m_YoungModulus.getValue()[elementID]<<"\n";
				//printf("BuildStiffnessBlock\nMu: %f \n Lambda: %f \n",mu, lambda);

//	//In case you want to do it all in float or double!
//	Real mu = this->m_ShearModulus ;
//	Real lambda = this->m_LamesFirstParameter;

				//-----------------------------------------------------------------------
				//Calculate the jacobianmatrix and its inversion at this integrationpoint
				//-----------------------------------------------------------------------
				Mat<3,3,Real> jacobianMatrix;
				GetJacobian(jacobianMatrix,coords,integrationPointIndex);

				double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);

//				std::cout<<"hi there\n";
				if(jacobianDeterminant <=1e-10)
					std::cout<<"WARNING: Jacobian for elementId "<<elementID<<" is "<<jacobianDeterminant<<"\n";// ERROR, jacobian <=0, mesh is wrong!!\n";
//	//In case you want to do it all in float or double!
//	Real jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);

				Mat<3,3,Real> inverseJacobian;
				sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

				//-----------------------------------------------------------------------
				// Fill the stiffness block by calculations with the shapeFunctions
				//-----------------------------------------------------------------------
				// Helper matrix to store shapefunctions for this integrationpoint of this element locally!
				ShapeFunctionDerivativesGlobalType shapeFunctionDerivativesGlobal = {0};
				std::memcpy(&(shapeFunctionDerivativesGlobal), &(m_QuadTetrahedronInfos[elementID].shapeFunctionDerivativesGlobal[integrationPointIndex]), sizeof(ShapeFunctionDerivativesGlobalType) );

				unsigned int mn,ij;

				//build stiffness block (force derivative \partial F _ij over \partial U _mn)
				for(unsigned int m=0;m<nodesPerElement;m++)
				{
					for(unsigned int n=0;n<3;n++)
					{
						for(unsigned int i=0;i<nodesPerElement;i++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								mn = 3*m+n;
								ij = 3*i+j;

								stiffnessBlock[ij][mn]= 0;

								stiffnessBlock[ij][mn] += (lambda *shapeFunctionDerivativesGlobal[m][n]*shapeFunctionDerivativesGlobal[i][j]
										+ mu*shapeFunctionDerivativesGlobal[m][j]*shapeFunctionDerivativesGlobal[i][n])*jacobianDeterminant*m_IntWeights[integrationPointIndex];
								if(j==n)
								{
									for(unsigned int k=0;k<3;k++)
									{
										stiffnessBlock[ij][mn] += mu*shapeFunctionDerivativesGlobal[m][k]*shapeFunctionDerivativesGlobal[i][k]*jacobianDeterminant*m_IntWeights[integrationPointIndex];
									}
								}
							} //j
						} //i
					} //n
				} //m

//	this->WriteElementMatrixToCVSFile(stiffnessBlock, "StiffnessElementMatrixNo_",elementID);
//	std::cin.get();

				//copy stiffness block to new datastructure (can this be done as a memcpy operation?)
				PrecomputedStiffnessBlockType currentStiffnessBlock;

				for(unsigned int m=0;m<nodesPerElement;m++)
				{
					for(unsigned int i=0;i<nodesPerElement;i++)
					{
//							currentStiffnessBlock.clear();

						for(unsigned int n=0;n<3;n++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								mn = 3*m+n;
								ij = 3*i+j;
								currentStiffnessBlock[j][n] = stiffnessBlock[ij][mn];
							}
						}

						// This is the result quasi returned by the function!
//							stiffnessBlockMatrix[i][m] = currentStiffnessBlock;
						std::memcpy( &(stiffnessBlockMatrix[i][m]), &(currentStiffnessBlock), sizeof(PrecomputedStiffnessBlockType) );
					}
				}
			} //BuildStiffnessBlock

//-----------------------------------------------------------------------
// Calculate Jacobian Matrix for several occasions
//-----------------------------------------------------------------------
			template <class DataTypes>
			inline void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::GetJacobian( Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfIntegrationPoint)
			{
				//build jacobian matrix
				for(int m=0;m<3;m++)
				{
					for(int n=0;n<3;n++)
					{
						//sum over all shape functions
						for(int i=0;i<nodesPerElement;i++)
						{
							jacobianMatrix[m][n] += ((coords[i])[m])*m_ShapeFunctionDerivatives[indexOfIntegrationPoint][i][n];
//				std::cout<<"jacobian matrix value "<<jacobianMatrix[m][n]<<" at mXn "<<m<<" "<<n<<std::endl;
						}
					}
				}
			} //GetJacobian

			template <class DataTypes>
			inline void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::GetSimpleJacobian( Mat<3,3,Real> &jacobianMatrix, ElementSimpleCoordsType coords)
			{
				//build jacobian matrix
				for(int m=0;m<3;m++)
				{
					for(int n=0;n<3;n++)
					{
						//sum over all shape functions
						for(int i=0;i<4;i++)
						{
							jacobianMatrix[m][n] += ((coords[i])[m])*m_SimpleShapeFunctionDerivative[i][n];
//				std::cout<<"jacobian matrix value "<<jacobianMatrix[m][n]<<" at mXn "<<m<<" "<<n<<std::endl;
						}
					}
				}
			} //GetJacobian

			template <class DataTypes>
			inline void QuadraticTetrahedralCorotationalFEMForceField< DataTypes >::GetNodalJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfNodalPoint)
			{
				Mat<10,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivativesPerNode[indexOfNodalPoint];
				jacobianMatrix.clear();

				//build jacobian matrix
				for(int m=0;m<3;m++)
				{
					for(int n=0;n<3;n++)
					{
						//sum over all shape functions
						for(int i=0;i<10;i++)
						{
							jacobianMatrix[m][n] += (coords[i])[m]*currentShapeFunctionDerivatives[i][n];
//				  std::cout<<"jacobian matrix value "<<jacobianMatrix[m][n]<<" at mXn "<<m<<" "<<n<<std::endl;
						}
					}
				}

			}

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//OLDER FUNCTIONS CAN STILL BE USED AS THEY ARE starting with a capitalLetternow!
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
			/**
			 * there are two way to implement the Polar-method
			 * 1.use the same process of the program in Matlab:
			 *     calculate the stiffness matrix at first without rotation
			 *     rotate the total element with rotation-matrix in addForce and addDForce
			 *     calculate the rotation-matrix with a iterative algorithm
			 *
			 * 2.use the process of the program of SOFA
			 *     calculate the rotation-matrix at first with the rotation of three basis vector of element, and save it
			 *     the derivative of force is calculated directly by the strain-displacement-matrix and material-matrix but not stiffness-matrix
			 */

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::AssembleElementMatrices()
			{
				// unsigned int ndof = this->mstate->getSize();
				unsigned int nElements=m_Topology->getNbQuadTetrahedra();

				ElementStiffnessMatrixType elementStiffnessMatrix;

				for(unsigned int i=0; i<nElements;i++)
				{
					this->BuildElementMatrix(elementStiffnessMatrix, i);
					m_QuadTetrahedronInfos[i].ElementStiffnessMatrix = elementStiffnessMatrix;
				}

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::BuildElementMatrix(ElementStiffnessMatrixType & elementMatrix, unsigned int elementID)
			{
				//  m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray.resize(m_NumberOfIntegrationPoints);

				elementMatrix.clear();
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
				stiffnessBlockMatrix.clear();

				const VecCoord *X0=this->mstate->getX0();

				const QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(elementID);

				RotatedInitialElementType coords;

				for(unsigned int i=0; i<10;i++)
				{
					// int test = tetraIndices[i];
					// Coord test2 = (*X0)[test];
					coords[i] = (*X0)[tetraIndices[i]];
				}

				PrecomputedStiffnessBlockType currentStiffnessBlock;
				int mn,ij;
				for(unsigned int iter=0;iter<numberOfIntegrationPoints;iter++)
				{
					this->OldBuildStiffnessBlock(elementID, stiffnessBlockMatrix, coords, iter);
					m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[iter] = stiffnessBlockMatrix;
					//elementMatrix += stiffnessBlock

					// PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrixTest  = m_QuadTetrahedronInfos[elementID].precomputedElementStiffnessBlockMatrixArray[iter];//;
					for(unsigned int m=0;m<10;m++)
					{
						for(unsigned int i=0;i<10;i++)
						{
							currentStiffnessBlock = stiffnessBlockMatrix[i][m];

							for(unsigned int n=0;n<3;n++)
							{
								for(unsigned int j=0;j<3;j++)
								{
									mn = 3*m+n;
									ij = 3*i+j;

									elementMatrix[ij][mn]+=currentStiffnessBlock[j][n];

								}
							}

						}
					}
				}

			}

//for addForce
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::BuildCoRotationalElementMatrix(const VecCoord& x, unsigned int elementID)
			{

			}

//for addDForce
			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::BuildCoRotationalElementMatrix(unsigned int elementID, double kFactor)
			{

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::OldBuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlockMatrix,helper::fixed_array<Coord,10> coords, unsigned int integrationPointIndex)
			{
				ElementStiffnessMatrixType stiffnessBlock;

//  double mu = this->m_ShearModulus ;
//
//  double lambda = this->m_LamesFirstParameter;

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

				Mat<3,3,Real> jacobianMatrix;

				this->OldGetJacobian(jacobianMatrix,coords,integrationPointIndex);

				double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);

				Mat<3,3,Real> inverseJacobian;
				sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

				Mat<10,3,Real> shapeFunctionDerivativesLocal = this->m_QuadTetrahedronInfos[elementID].shapeFunctionDerivativesGlobal[integrationPointIndex];

				unsigned int mn,ij; //,kj;

				//build stiffness block (force derivative \partial F _ij over \partial U _mn)
				for(unsigned int m=0;m<10;m++)
				{
					for(unsigned int n=0;n<3;n++)
					{
						for(unsigned int i=0;i<10;i++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								mn = 3*m+n;
								ij = 3*i+j;

								stiffnessBlock[ij][mn]= 0;

								stiffnessBlock[ij][mn] += (lambda *shapeFunctionDerivativesLocal[m][n]*shapeFunctionDerivativesLocal[i][j]
										+ mu*shapeFunctionDerivativesLocal[m][j]*shapeFunctionDerivativesLocal[i][n])*(1.0/6.0)*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
								if(j==n)
								{
									for(unsigned int k=0;k<3;k++)
									{
										stiffnessBlock[ij][mn] += mu*shapeFunctionDerivativesLocal[m][k]*shapeFunctionDerivativesLocal[i][k]*(1.0/6.0)*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
										//stiffnessBlock[ij][mn] += shapeFunctionDerivativesLocal[i][k]*stressDerivativeMatrix[mn][3*k+j]*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
									}
								}

							}
						}
					}
				}

				//copy stiffness block to new datastructure
				PrecomputedStiffnessBlockType currentStiffnessBlock;

				for(unsigned int m=0;m<10;m++)
				{
					for(unsigned int i=0;i<10;i++)
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

			/*****************************************************************************
			 *
			 *  Jacobi Matrix
			 *
			 *****************************************************************************/
			template <class DataTypes>
			double QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::OldGetJacobian( Mat<3,3,Real> &jacobianMatrix,helper::fixed_array<Coord,10> coords, int indexOfIntegrationPoint)
			{

//		  Mat<10,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivatives;
//		//  MatNoInit<3,3,Real> jacobianMatrix;
//		  jacobianMatrix.clear();
//
//		  /*
//		   * FIXME: 2 Questions:
//		   *   1: the derivatives of shape function are all the same, or it's relative to the different quadratic tetrahedra
//		   *   2: the index of the loop of calculating of Jacobi matrix
//		   */
//		  // unused variables
//		  double test1,test2;//,test3,test4;
//		  //build jacobian matrix
//		  for(int m=0;m<3;m++)
//		  {
//			  for(int n=0;n<3;n++)
//			  {
//				  //sum over all shape functions
//				  for(int i=0;i<30;i++)
//				  {
//					  test1 = (coords[i])[m];
//					  test2 =  currentShapeFunctionDerivatives[i][n];
//					  jacobianMatrix[m][n] += test1*test2; //(coords[i])[n]*currentShapeFunctionDerivatives[i][m];
//				  }
//
//			  }
//		  }

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::OldInitializeShapeFunctions(unsigned int numberOfIntegrationPoints)
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

//			  currentShapeFunctions[0] = (2*r-1)*r;
//			  currentShapeFunctions[1] = (2*s-1)*s;
//			  currentShapeFunctions[2] = (2*t-1)*t;
//			  currentShapeFunctions[3] = (2*tempExp-1)*tempExp;
//			  currentShapeFunctions[4] = 4*r*s;
//			  currentShapeFunctions[5] = 4*s*t;
//			  currentShapeFunctions[6] = 4*r*t;
//			  currentShapeFunctions[7] = 4*tempExp*r;
//			  currentShapeFunctions[8] = 4*tempExp*s;
//			  currentShapeFunctions[9] = 4*tempExp*t;
					currentShapeFunctions[0] = (2*r-1)*r;
					currentShapeFunctions[1] = (2*s-1)*s;
					currentShapeFunctions[2] = (2*tempExp-1)*tempExp;
					currentShapeFunctions[3] = (2*t-1)*t;
					currentShapeFunctions[4] = 4*r*s;
					currentShapeFunctions[5] = 4*tempExp*s;
					currentShapeFunctions[6] = 4*tempExp*r;
					currentShapeFunctions[7] = 4*s*t;
					currentShapeFunctions[8] = 4*r*t;
					currentShapeFunctions[9] = 4*tempExp*t;

					m_ShapeFunctions[i] = currentShapeFunctions;
				}

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::OldInitializeShapeFunctionDerivatives(unsigned int numberOfIntegrationPoints)
			{

				m_ShapeFunctionDerivatives.resize(numberOfIntegrationPoints);
				ShapeFunctionDerivative.clear();

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

//			  currentShapeFunctionDerivatives[0][0] = 4*r-1;
//			  currentShapeFunctionDerivatives[0][1] = 0;
//			  currentShapeFunctionDerivatives[0][2] = 0;
//
//			  currentShapeFunctionDerivatives[1][0] = 0;
//			  currentShapeFunctionDerivatives[1][1] = 4*s-1;
//			  currentShapeFunctionDerivatives[1][2] = 0;
//
//			  currentShapeFunctionDerivatives[2][0] = 0;
//			  currentShapeFunctionDerivatives[2][1] = 0;
//			  currentShapeFunctionDerivatives[2][2] = 4*t-1;
//
//			  currentShapeFunctionDerivatives[3][0] = -4*tempExp+1;
//			  currentShapeFunctionDerivatives[3][1] = -4*tempExp+1;
//			  currentShapeFunctionDerivatives[3][2] = -4*tempExp+1;
//
//			  currentShapeFunctionDerivatives[4][0] = 4*s;
//			  currentShapeFunctionDerivatives[4][1] = 4*r;
//			  currentShapeFunctionDerivatives[4][2] = 0;
//
//			  currentShapeFunctionDerivatives[5][0] = 0;
//			  currentShapeFunctionDerivatives[5][1] = 4*t;
//			  currentShapeFunctionDerivatives[5][2] = 4*s;
//
//			  currentShapeFunctionDerivatives[6][0] = 4*t;
//			  currentShapeFunctionDerivatives[6][1] = 0;
//			  currentShapeFunctionDerivatives[6][2] = 4*r;
//
//			  currentShapeFunctionDerivatives[7][0] = 4*(tempExp-r);
//			  currentShapeFunctionDerivatives[7][1] = -4*r;
//			  currentShapeFunctionDerivatives[7][2] = -4*r;
//
//			  currentShapeFunctionDerivatives[8][0] = -4*s;
//			  currentShapeFunctionDerivatives[8][1] = 4*(tempExp-s);
//			  currentShapeFunctionDerivatives[8][2] = -4*s;
//
//			  currentShapeFunctionDerivatives[9][0] = -4*t;
//			  currentShapeFunctionDerivatives[9][1] = -4*t;
//			  currentShapeFunctionDerivatives[9][2] = 4*(tempExp-t);
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

					//Interpolation of Derivative of Shape function
					ShapeFunctionDerivative = ShapeFunctionDerivative + currentShapeFunctionDerivatives * m_IntWeights[i];

				}

				//initialize local shape functions
				const VecCoord *X0=this->mstate->getX0();
				//loop over all elements
				unsigned int nElements=m_Topology->getNbQuadTetrahedra();
				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
				{
					//extract coords
					const QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(iterElement);

					RotatedInitialElementType coords;

					for(unsigned int i=0; i<10;i++)
					{
						coords[i] = (*X0)[tetraIndices[i]];
					}

					//loop over all integration points
					for(unsigned int integrationPointIndex=0; integrationPointIndex< numberOfIntegrationPoints;integrationPointIndex++)
					{
						Mat<3,3,Real> jacobianMatrix;

						this->OldGetJacobian(jacobianMatrix,coords,integrationPointIndex);

//					  double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);//unused

						Mat<3,3,Real> inverseJacobian;
						sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

						//get the shape function derivatives in global coordinates
						Mat<10,3,Real> shapeFunctionDerivativesGlobal = this->m_ShapeFunctionDerivatives[integrationPointIndex];

						//transform shape function derivatives in local coordinates
						//fixme

						Mat<10,3,Real> shapeFunctionDerivativesLocal;// = shapeFunctionDerivativesGlobal*inverseJacobian;
						shapeFunctionDerivativesLocal.clear();
						for(unsigned int i=0;i<10;i++)
						{
							for(unsigned int j=0;j<3;j++)
							{
								for(unsigned int k=0;k<3;k++)
								{
									shapeFunctionDerivativesLocal[i][j]+=shapeFunctionDerivativesGlobal[i][k]*inverseJacobian[k][j];
								}
							}
						}

						this->m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[integrationPointIndex]=shapeFunctionDerivativesLocal;
					}
				}

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::OldInitializeIntegrationPointsAndWeights(unsigned int numberOfIntegrationPoints)
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
				double beta3 = 0.100596423833201;
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

				switch(numberOfIntegrationPoints) {
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

			/*****************************************************************************
			 *
			 *  build global stiffness matrix
			 *
			 *****************************************************************************/

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::AddKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset)
			{

				kFactor = kFactor*(-1);

				unsigned int nElements=this->m_Topology->getNbQuadTetrahedra();

//			ElementStiffnessMatrixVectorType  elementStiffnessMatrixVector = this->m_ElementStiffnessMatrixVector.getValue();

				/*
				 * TODO mistake here?
				 *   the loop calculate the global stiffness matrix, so the size of the matrix is 3*#element X 3*#element
				 */
				ElementStiffnessMatrixType elementStiffnessMatrix;

				QuadraticTetraType tetraIndices;

				unsigned int posRow, posColumn, posRowElement, posColumnElement;

				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
				{
//			  elementStiffnessMatrix = elementStiffnessMatrixVector[iterElement];
					elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
//			  BuildElementMatrix(elementStiffnessMatrix,iterElement);
					tetraIndices = m_Topology->getQuadTetrahedron(iterElement);

					/*
					 * FIXME
					 *   different from the old version
					 */
					for(unsigned int m=0; m<10;m++)
					{
						for(unsigned int n=0; n<3;n++)
						{
							posColumn = 3*tetraIndices[m]+n + offset;
							posColumnElement = 3*m+n;

							for(unsigned int i=0; i<10;i++)
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

//			this->WriteMatrixToCVSFile(mat, "StiffnessMatrixQuadTetra.csv");
			}

			/*****************************************************************************
			 *
			 *  Stiffness interface
			 *
			 *****************************************************************************/

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::AddForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v)
			{

				//assume that addForce is always called before addDForce and addKToMatrix
				unsigned int nElements=m_Topology->getNbQuadTetrahedra();
				QuadraticTetraType tetraIndices;
				ElementStiffnessMatrixType elementStiffnessMatrix;
				ElementStiffnessMatrixType elementStiffnessMatrixX0;
				PrecomputedStiffnessBlockType currentStiffnessBlock;
				PrecomputedStiffnessBlockType currentStiffnessBlockX0;
				PrecomputedStiffnessBlockMatrixType stiffnessBlockMatrix;
				helper::fixed_array<PrecomputedStiffnessBlockMatrixType,4> stiffnessBlockMatrixArray;

				int mn, ij;
				RotationMatrixType S, R, Rtransp;
				Mat<3,3,Real> phi;
				Mat<10,3,Real> shapeFunctionDerivatives;
				Mat<3,10,Real> currentCoords;

				const VecCoord *X0=this->mstate->getX0();
				//this->mstate->

				sofa::helper::AdvancedTimer::stepBegin("ExtractRotation");

				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
				{
					tetraIndices = m_Topology->getQuadTetrahedron(iterElement);
					stiffnessBlockMatrixArray = m_QuadTetrahedronInfos[iterElement].precomputedElementStiffnessBlockMatrixArray;

					//first rebuild the elementStiffnessMatrix and elementStiffnessMatrixX0;
					//delete the temp matrices
					elementStiffnessMatrix.clear();
					elementStiffnessMatrixX0.clear();

					//loop over all integration points
					for(unsigned int iterIntPoint=0;iterIntPoint<numberOfIntegrationPoints;iterIntPoint++)
					{
						stiffnessBlockMatrix = stiffnessBlockMatrixArray[iterIntPoint];
						//loop over all i,m
						for(unsigned int m=0;m<10;m++)
						{
							for(unsigned int i=0;i<10;i++)
							{
								//extract 3x3 precomputed stiffness block Fim
								currentStiffnessBlock = stiffnessBlockMatrix[i][m];

								//determine the rotation matrix and transpose

								//the shapefunction is just for one interpoliert piont
								shapeFunctionDerivatives = this->m_QuadTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobal[iterIntPoint];

								for(int iter1=0;iter1<10;iter1++) {
									for(int iter2=0;iter2<3;iter2++) {
										currentCoords[iter2][iter1] = x[tetraIndices[iter1]][iter2];
									}
								}

								phi = currentCoords * shapeFunctionDerivatives;

								helper::Decompose<Real>::polarDecomposition(phi, R, S);
								Rtransp = R.transposed();

								this->m_QuadTetrahedronInfos[iterElement].elementRotationMatrixArray[iterIntPoint] = R;
								this->m_QuadTetrahedronInfos[iterElement].elementRotationMatrixTransposedArray[iterIntPoint] = Rtransp;

								//calculate R*Fim and save as currentStiffnessBlockX0
								currentStiffnessBlockX0 = R*currentStiffnessBlock;

								//calculate currentStiffnessBlockX0*RT (=R*Fim*RT) and save as currentStiffnessBlock
								currentStiffnessBlock = currentStiffnessBlockX0*Rtransp;

								//add the blocks into elementStiffnessMatrix and elementStiffnessMatrixX0
								for(unsigned int n=0;n<3;n++)
								{
									for(unsigned int j=0;j<3;j++)
									{
										mn = 3*m+n;
										ij = 3*i+j;

										elementStiffnessMatrix[ij][mn] += currentStiffnessBlock[j][n];
										elementStiffnessMatrixX0[ij][mn] += currentStiffnessBlockX0[j][n];

									}
								}

							}
						}

					}

					//revert to linear
					if(this->m_LinearMode.getValue())
					{
						elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
						elementStiffnessMatrixX0 = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
					}

					//save element stiffness matrix in tetrahedron datastructure
					m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix = elementStiffnessMatrix;
					//  elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
					//now use the element stiffness matrices to calculate force
					for(unsigned int m=0; m<10;m++)
					{
						for(unsigned int n=0; n<3;n++)
						{
							int posColumn = tetraIndices[m];
							int posColumnElement = 3*m+n;

							for(unsigned int i=0; i<10;i++)
							{
								//for(unsigned int j=0; j<3;j++)
								//{
								int posRow = tetraIndices[i];
//							  int posRowElement = 3*i;//unused

								//double test1 = (dx[posColumn])[n];
								//double test2 = elementStiffnessMatrix[posRowElement][posColumnElement];

								double test1 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement]-(*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+0][posColumnElement];
								double test2 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement]-(*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+1][posColumnElement];
								double test3 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement]-(*X0)[posColumn][n]*elementStiffnessMatrixX0[3*i+2][posColumnElement];

								f[posRow] += Deriv(-1*test1, -1*test2, -1*test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
								// }
							}
						}
					}
				}

				sofa::helper::AdvancedTimer::stepEnd("ExtractRotation");

				//in the linear case the same as addforce
				//must probably be changed for co-rotational version!!!!!

				//running Time calculation
//	          clock_t begin, end;
//	          begin = clock();

//#ifdef SMALL
//			  addDForce(f,x,1,1);
//			  char derMatrix[] = "DerivativeForceMatrix_small_addForce";
//			  char disMatrix[] = "DisplacementMatrix_small_addForce";
//#endif
				//addDForce(f,x,1,1);
				//unsigned int nElements=m_Topology->getNbQuadTetrahedra();

//#ifdef POLAR
//			  //loop over all elements
//			  for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//			  {
//				  this->BuildCoRotationalElementMatrix(x, iterElement);
//				  computeForce(f,x,iterElement);
//			  }
//
//
//			  char derMatrix[] = "DerivativeForceMatrix_polar_addForce";
//			  char disMatrix[] = "DisplacementMatrix_polar_addForce";
//#endif

//#ifdef POLAR2
////			  RotatedInitialElementVectorType rotatedInitialElements = this->m_RotatedInitialElements.getValue();
////			  m_RotationMatrixVector.resize(nElements);
//
//			  for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//			  {
//				  QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(iterElement);
//				  RotatedInitialElementType rotatedInitialElement = m_QuadTetrahedronInfos[iterElement].rotatedInitialElement;
//
//				  Mat<3,3,Real> J;
//				  J[0] = x[tetraIndices[1]] - x[tetraIndices[0]];
//				  J[1] = x[tetraIndices[2]] - x[tetraIndices[0]];
//				  J[2] = x[tetraIndices[3]] - x[tetraIndices[0]];
//				  Mat<3,3,Real> S;
//				  Mat<3,3,Real> R;
//				  polar_decomp(J, R, S);
//
//				  m_QuadTetrahedronInfos[iterElement].rotation.transpose(R);
//
//				  helper::fixed_array<Coord,10> deforme;
//				  VecCoord displacement;
//				  displacement.resize(10);
//
//				  for(int i=0;i<10;i++){
//					  deforme[i] = R*x[tetraIndices[i]];
//					  for(int j=0;j<3;j++){
//						  displacement[i][j] = rotatedInitialElement[i][j] - deforme[i][j];
//					  }
//				  }
//
//				  VecCoord F;
//				  F.resize(10);
//				  computeForce(F,displacement,iterElement);
//				  for(int i=0;i<10;i++){
//					  f[tetraIndices[i]] += m_QuadTetrahedronInfos[iterElement].rotation * F[i];
//				  }
//			  }
//
//			  char derMatrix[] = "DerivativeForceMatrix_polar2_addForce";
//			  char disMatrix[] = "DisplacementMatrix_polar2_addForce";
//#endif

				//FIXME calculate the running time
//			  end = clock();
//			  QString name = "AddForce at loop ";
//			  name += QString::number(loopCounter);
//			  this->WriteRunningTimeToCVSFile(begin, end, name);

//	  		if(loopCounter==100){
////	  		    this->WriteVectorsToCVSFile(df, derMatrix, loopCounter);
//	  		    this->WriteVectorsToCVSFile(x, disMatrix, loopCounter);
//	  		}

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::AddDForce (VecDeriv& df, const VecDeriv& x, double kFactor, double bFactor)
			{
				//implement addMDx on a per element basis - it will be faster to just use the complete mass matrix and

				kFactor = kFactor*(-1);

				unsigned int nElements=m_Topology->getNbQuadTetrahedra();
				//get the begin coordinates of all vertex of the net
//	  	  const VecCoord *X0=this->mstate->getX0();
				// QuadraticTetraType tetraIndices;
				//  ElementStiffnessMatrixType elementStiffnessMatrix;

//			if (kFactor == 1.0)
//			{
//
//				//loop over all elements
//				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//				{
//
//					computeForce(df,x,iterElement);
//				}
//			}
//			else {
//				//loop over all elements
//				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//				{
//				  elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;
//				  tetraIndices = m_Topology->getQuadTetrahedron(iterElement);
//
//				  for(unsigned int m=0; m<10;m++)
//				  {
//					  for(unsigned int n=0; n<3;n++)
//						{
//						  int posColumn = tetraIndices[m];
//						  int posColumnElement = 3*m+n;
//
//						  for(unsigned int i=0; i<10;i++)
//						  {
//							  for(unsigned int j=0; j<3;j++)
//							  {
//								  int posRow = tetraIndices[i];
//								  int posRowElement = 3*i+j;
//								  (df[posRow])[j] += (x[posColumn])[n]* elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor;
////								  (df[posRow])[j] += ((x[posColumn])[n]+(*X0)[posColumn][n])* elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor;
//							  }
//						  }
//						}
//					}
//				  }
//			}

				for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
				{
					ElementStiffnessMatrixType elementStiffnessMatrix = m_QuadTetrahedronInfos[iterElement].ElementStiffnessMatrix;

					//const VecCoord *X0=this->mstate->getX0();
					QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(iterElement);

					for(unsigned int m=0; m<10;m++)
					{

						//loop of the 3 freedom
						for(unsigned int n=0; n<3;n++)
						{
							//index of the vertex of Quadratic tetrahedra
							int posColumn = tetraIndices[m];
							int posColumnElement = 3*m+n;

							for(unsigned int i=0; i<10;i++)
							{
								//for(unsigned int j=0; j<3;j++)
								//{
								int posRow = tetraIndices[i];
//						  int posRowElement = 3*i;//unused

								//double test1 = (dx[posColumn])[n];
								//double test2 = elementStiffnessMatrix[posRowElement][posColumnElement];

								double test1 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement]*kFactor;
								double test2 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement]*kFactor;;
								double test3 = ((x[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement]*kFactor;;

//						  double test1 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+0][posColumnElement];
//						  double test2 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+1][posColumnElement];
//						  double test3 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+2][posColumnElement];

								df[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
								//}
							}
						}
					}
				}

//			char derMatrix[] = "DerivativeForceMatrix_small";
//			char disMatrix[] = "DisplacementMatrix_small";

			}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::computeForce(VecDeriv& df, const VecDeriv& dx, unsigned int elementID)
			{
				ElementStiffnessMatrixType elementStiffnessMatrix = m_QuadTetrahedronInfos[elementID].ElementStiffnessMatrix;

				//const VecCoord *X0=this->mstate->getX0();
				QuadraticTetraType tetraIndices = m_Topology->getQuadTetrahedron(elementID);

				//loop of the 10 vertex of each element
				for(unsigned int m=0; m<10;m++)
				{
					//loop of the 3 freedom
					for(unsigned int n=0; n<3;n++)
					{
						//index of the vertex of Quadratic tetrahedra
						int posColumn = tetraIndices[m];
						int posColumnElement = 3*m+n;

						for(unsigned int i=0; i<10;i++)
						{
							//for(unsigned int j=0; j<3;j++)
							//{
							int posRow = tetraIndices[i];
//							  int posRowElement = 3*i;//unused

							//double test1 = (dx[posColumn])[n];
							//double test2 = elementStiffnessMatrix[posRowElement][posColumnElement];

							double test1 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement];
							double test2 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement];
							double test3 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement];

							//						  double test1 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+0][posColumnElement];
							//						  double test2 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+1][posColumnElement];
							//						  double test3 = ((dx[posColumn])[n]-(*X0)[posColumn][n])*elementStiffnessMatrix[3*i+2][posColumnElement];

							df[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
							//}
						}

					}
				}
			}
			/*
			 * test code by Beibei
			 */

//template <class DataTypes>
//double QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::calculateVolume(helper::fixed_array<Coord,10> x){
//	Mat<4,4,Real> JacobiMatrix;
//	JacobiMatrix.clear();
//
//	for(unsigned int i=0; i< 4;i++)
//	{
//	  Mat<4,4,Real> Temp;
//	  Temp.clear();
//	  double r=m_IntPoints[3*i];
//	  double s=m_IntPoints[3*i+1];
//	  double t=m_IntPoints[3*i+2];
//	  double u = 1-r-s-t;
//
//	  /*
//	   * Jx1 = x1 (4ζ1 − 1) + 4x5 ζ2 + 4x7 ζ3 + 4x8 ζ4
//	   * Jx2 = x2 (4ζ2 − 1) + 4x6 ζ3 + 4x5 ζ1 + 4x9 ζ4
//	   * Jx3 = x3 (4ζ3 − 1) + 4x7 ζ1 + 4x6 ζ2 + 4x10 ζ4
//	   * Jx4 = x4 (4ζ4 − 1) + 4x8 ζ1 + 4x9 ζ2 + 4x10 ζ3
//	   */
//	  for(int row=0;row<3;row++){
//		  Temp[row+1][0]  = x[0][row]*(4*r-1) + x[4][row]*4*s + x[6][row]*4*t + x[7][row]*4*u;
//		  Temp[row+1][1]  = x[1][row]*(4*s-1) + x[5][row]*4*t + x[4][row]*4*r + x[8][row]*4*u;
//		  Temp[row+1][2]  = x[2][row]*(4*t-1) + x[6][row]*4*r + x[5][row]*4*s + x[9][row]*4*u;
//		  Temp[row+1][3]  = x[3][row]*(4*u-1) + x[7][row]*4*r + x[8][row]*4*s + x[9][row]*4*t;
//	  }
//	  JacobiMatrix = JacobiMatrix + Temp * m_IntWeights[i];
//	}
//    JacobiMatrix[0][0] = JacobiMatrix[0][1] = JacobiMatrix[0][2] = JacobiMatrix[0][3] = 1;
//
//    double determin = 0;
//    determin += JacobiMatrix[1][0]*JacobiMatrix[2][1]*JacobiMatrix[3][2];
//    determin += JacobiMatrix[1][1]*JacobiMatrix[2][2]*JacobiMatrix[3][3];
//    determin += JacobiMatrix[1][2]*JacobiMatrix[2][3]*JacobiMatrix[3][0];
//    determin += JacobiMatrix[1][3]*JacobiMatrix[2][0]*JacobiMatrix[3][1];
//
//    determin -= JacobiMatrix[1][0]*JacobiMatrix[2][3]*JacobiMatrix[3][2];
//    determin -= JacobiMatrix[1][1]*JacobiMatrix[2][0]*JacobiMatrix[3][3];
//    determin -= JacobiMatrix[1][2]*JacobiMatrix[2][1]*JacobiMatrix[3][0];
//    determin -= JacobiMatrix[1][3]*JacobiMatrix[2][2]*JacobiMatrix[3][1];
//
//    return determin;
//}
//template <class DataTypes>
//double QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::calculateVolume(helper::fixed_array<Coord,10> x){
//	Mat<3,3,Real> JacobiMatrix;
//	JacobiMatrix.clear();
//
//	for(unsigned int i=0; i< 4;i++)
//	{
//	  Mat<3,3,Real> Temp;
//	  Temp.clear();
//	  Mat<10,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivatives[i];
//
//	  for(int m=0;m<3;m++){
//		  for(int n=0;n<3;n++){
//			  Temp[m][n] = 0;
//			  for(int k=0;k<10;k++){
//				  Temp[m][n] += x[k][m] * currentShapeFunctionDerivatives[k][n];
//			  }
//		  }
//	  }
//
//	  JacobiMatrix = JacobiMatrix + Temp * m_IntWeights[i];
//	}
//
//    double determin = sofa::defaulttype::determinant(JacobiMatrix);
//
//    return fabs(determin/6.0);
//}

			/*****************************************************************************
			 *
			 *  Test Out Put
			 *
			 *****************************************************************************/
//template <class DataTypes>
//void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::WriteRunningTimeToCVSFile(clock_t begin, clock_t end, QString name){
////	QString completeFilename("/home/mediassi/MediAssistData/Temp/RunningTime.csv");
////	QFile file(completeFilename);
////
////	if(!file.open(QFile::WriteOnly | QFile::Append))
////		return;
////
////	QTextStream out(&file);
////	out<<name<<"    ";
////	out<<end<<"   "<<begin<<"   "<<end-begin<<"\n";
//}

			template <class DataTypes>
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename)
			{
				//open file and generate textstream
				QString completeFilename(MEDIASSIST_DATA_PATH);
				completeFilename += "Temp/";
				completeFilename += filename;

				QFile file(completeFilename);

				if (!file.open(QFile::WriteOnly | QFile::Truncate))
				return;

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
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::WriteElementMatrixToCVSFile(Mat<30,30,Real> mat,char* filename, int elementNumber)
			{
				//open file and generate textstream
				QString completeFilename("/home/laugisch/Desktop/");
				//QString elementNumberString(elementNumber);
				completeFilename += "Temp/";
				completeFilename += filename;
				completeFilename += QString::number(elementNumber);
				completeFilename +=".csv";

				QFile file(completeFilename);

				if (!file.open(QFile::WriteOnly | QFile::Truncate))
				return;

				QTextStream out(&file);

				//matrix line by line
				unsigned int rowSize = 30;
				unsigned int columnSize = 30;

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
			void QuadraticTetrahedralCorotationalFEMForceField<DataTypes>::WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber)
			{
				//open file and generate textstream
				QString completeFilename("/home/mediassi/MediAssistData/");
				completeFilename += "Temp/";
				completeFilename += filename;
				completeFilename += QString::number(elementNumber);
				completeFilename +=".csv";

				QFile file(completeFilename);

				if (!file.open(QFile::WriteOnly | QFile::Truncate))
				return;

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

		} // namespace forcefield

	} // namespace component

} // namespace sofa

#endif
