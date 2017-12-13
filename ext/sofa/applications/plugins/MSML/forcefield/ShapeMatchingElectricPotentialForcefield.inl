/*=========================================================================

  Program:   MediAssist Computer Assisted Surgery System
  Module:    MediAssist Examples, ShapeMatchingElectricPotentialForcefield
  Author:    Stefan Suwelack

=========================================================================*/

// ****************************************************************************
// Includes
// ****************************************************************************
#include "ShapeMatchingElectricPotentialForcefield.h"
#include <sofa/helper/AdvancedTimer.h>
#include <sofa/helper/decompose.h>
#include <sofa/component/topology/TetrahedronSetGeometryAlgorithms.h>
#include <sofa/defaulttype/Quat.h>

#include <sofa/defaulttype/Vec.h>

// just for debugging
#include <QString>
#include <QFile>
#include <QTextStream>
#include "MediAssistConfig.h"

//#include <SOLID/SOLID.h>
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkSTLReader.h"
#include "vtkPolyData.h"
#include "vtkStructuredGridWriter.h"
#include "vtkStructuredGrid.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

#include "vtkUnstructuredGridWriter.h"
#include "vtkUnstructuredGrid.h"
#include "vtkCellArray.h"
#include "vtkVertexGlyphFilter.h"

#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"




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
	   ShapeMatchingElectricPotentialForcefield<DataTypes>::ShapeMatchingElectricPotentialForcefield()
    : InternalData(new ShapeMatchingElectricPotentialForcefieldInternalData<DataTypes>)
	, m_Charge(core::objectmodel::BaseObject::initData(&m_Charge,(Real)1,"charge",""))
    , m_DeformedMeshFilename(initData(&m_DeformedMeshFilename,std::string("unnamed"),"deformedMeshFilename","name of the deformed stl mesh"))
    , m_PrecomputedPotentialFieldFilename(initData(&m_PrecomputedPotentialFieldFilename,std::string("unnamed"),"precomputedPotentialFieldFilename","name of the precomputed VTK unstructured grid"))
    , m_UsePrecomputedPotentialField(core::objectmodel::BaseObject::initData(&m_UsePrecomputedPotentialField,false,"usePrecomputedPotentialField",""))
    , m_SavePrecomputedPotentialField(core::objectmodel::BaseObject::initData(&m_SavePrecomputedPotentialField,false,"savePrecomputedPotentialField",""))
    , m_Resolution(core::objectmodel::BaseObject::initData(&m_Resolution,20,"resolution","Resolution of the potential field"))
    {
//	m_PotentialFieldTopology = 0;
	counter = 0;

    }

template <class DataTypes>
	   ShapeMatchingElectricPotentialForcefield<DataTypes>::~ShapeMatchingElectricPotentialForcefield()
{
	//cleanup here
	delete InternalData;
//	if(m_PotentialFieldTopology )
//		delete m_PotentialFieldTopology;

}

// ****************************************************************************
// Public Methods
// ****************************************************************************
template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::init()
	   {
		//init superclass
		this->core::behavior::ForceField<DataTypes>::init();
		m_Topology = (BaseMeshTopology*)this->getContext()->getMeshTopology();


		//load mesh into solid
		vtkSTLReader* reader = vtkSTLReader::New();
		reader->SetFileName(m_DeformedMeshFilename.getValue().c_str());
		reader->Update();

		vtkPolyData* surface = reader->GetOutput();

		std::cerr<<"Surface Mesh Loaded for CD, no of points: "<<surface->GetNumberOfPoints()<<"\n";
		std::cerr<<"Surface Mesh Loaded for CD, no of cells: "<<surface->GetNumberOfCells()<<"\n";

		//load surface model to solid
		DT_ShapeHandle meshShapeHandle = DT_NewComplexShape(0);
		vtkIdType* currentCellPoints;
		vtkIdType numberOfNodes=3;
		float currentVertex[3];
		double currentVTKVertex[3];

		surface->BuildCells();

		 for(int i=0; i<surface->GetNumberOfCells(); i++)
		 {
			 surface->GetCellPoints(i, numberOfNodes,currentCellPoints);
			 if(numberOfNodes != 3)
				 std::cerr<<"WTF:Number of nodes not 3 \n";

			 DT_Begin();

			 for(int j=0; j<numberOfNodes; j++)
			 {
				 surface->GetPoint(currentCellPoints[j], currentVTKVertex);
				 currentVertex[0] = currentVTKVertex[0];
				 currentVertex[1] = currentVTKVertex[1];
				 currentVertex[2] = currentVTKVertex[2];
				 DT_Vertex(currentVertex);
			 }

			 DT_End();

		 }

		 DT_EndComplexShape();

		 DT_ObjectHandle meshObjectHandle = DT_CreateObject(0,meshShapeHandle);

		 double bounds[6];
		 surface->GetBounds(bounds);

		//create topology and set extend and resolution
		this->m_PotentialFieldTopology = sofa::core::objectmodel::New<sofa::component::topology::RegularGridTopology>();
		this->m_PotentialFieldTopology->setSize(m_Resolution.getValue(), m_Resolution.getValue(), m_Resolution.getValue());
		this->m_PotentialFieldTopology->setPos(1.5*bounds[0]-0.5*bounds[1],1.5*bounds[1]-0.5*bounds[0],1.5*bounds[2]-0.5*bounds[3],1.5*bounds[3]-0.5*bounds[2],1.5*bounds[4]-0.5*bounds[5],1.5*bounds[5]-0.5*bounds[4]);
		//this->m_PotentialFieldTopology->setPos(bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5]);

		dx = this->m_PotentialFieldTopology->getDx()[0];
		dy = this->m_PotentialFieldTopology->getDy()[1];
		dz = this->m_PotentialFieldTopology->getDz()[2];

		//iterate over all points in rectangular grid
		unsigned int nPoints = m_Resolution.getValue()*m_Resolution.getValue()*m_Resolution.getValue();
		helper::vector<DT_ShapeHandle> pointShapeHandles;
		pointShapeHandles.resize(nPoints);
		helper::vector<DT_ObjectHandle> pointObjectHandles;
		pointObjectHandles.resize(nPoints);
		DT_ShapeHandle currentShapeHandle;
		DT_ObjectHandle currentObjectHandle;

		Vector3 currentPoint;

		for(int i=0; i<nPoints; i++)
		 {
			currentPoint = this->m_PotentialFieldTopology->getPoint(i);
			currentVertex[0]=currentPoint[0];
			currentVertex[1]=currentPoint[1];
			currentVertex[2]=currentPoint[2];

			pointShapeHandles[i] = DT_NewPoint(currentVertex);
			DT_EndComplexShape();
			pointObjectHandles[i] = DT_CreateObject(0,pointShapeHandles[i]);

		 }


		float currentPointOnMesh[3];
		float currentTempPoint[3];
		Vector3 currentDistance;

		m_PotentialFieldValues.resize(nPoints);

		for(int i=0; i<nPoints; i++)
		 {
			currentObjectHandle = pointObjectHandles[i];

			DT_GetClosestPair(meshObjectHandle,currentObjectHandle,currentPointOnMesh,currentTempPoint);
			currentDistance[0] = currentTempPoint[0]-currentPointOnMesh[0];
			currentDistance[1] = currentTempPoint[1]-currentPointOnMesh[1];
			currentDistance[2] = currentTempPoint[2]-currentPointOnMesh[2];
			m_PotentialFieldValues[i] = currentDistance.norm();//*currentDistance.norm(); //!! square the potentialfieldvalue!!
		 }

		if(m_SavePrecomputedPotentialField.getValue())
		{
			//save as vtk structured grid
			vtkStructuredGrid *sgrid = vtkStructuredGrid::New();
			sgrid->SetDimensions(m_Resolution.getValue(),m_Resolution.getValue(),m_Resolution.getValue());

			vtkFloatArray *vectors = vtkFloatArray::New();
			vectors->SetNumberOfComponents(1);
			vectors->SetNumberOfTuples(nPoints);
			vtkPoints *points = vtkPoints::New();
			points->Allocate(nPoints);

			for(int i=0; i<nPoints; i++)
			 {
				//fill vtk datastructure
				currentPoint = this->m_PotentialFieldTopology->getPoint(i);
				currentVertex[0]=currentPoint[0];
				currentVertex[1]=currentPoint[1];
				currentVertex[2]=currentPoint[2];

				points->InsertPoint(i,currentVertex);
				vectors->SetValue(i,m_PotentialFieldValues[i]);

			 }

			sgrid->SetPoints(points);
			sgrid->GetPointData()->SetScalars(vectors);


			vtkStructuredGridWriter *myWriter = vtkStructuredGridWriter::New();
			myWriter->SetFileName( m_PrecomputedPotentialFieldFilename.getValue().c_str() );
			myWriter->SetInput( sgrid );
			myWriter->Update();
			myWriter->Delete();

			vectors->Delete();
			points->Delete();
			sgrid->Delete();

		}

		//cleanup
		reader->Delete();

		//extract global and local coordinates of surface triangles
		unsigned int nTriangles = m_Topology->getNbTriangles();
		std::cout<<"Number of triangles:"<<nTriangles<<"\n";
		LinearTriangleType currentTriangle;
		//const sofa::helper::vector<Triangle> &triangleArray=fromModel->getTriangles();
		//sofa::helper::vector< TrianglesInTetrahedron> trianglesInTetrahedron = m_Topology->getTrianglesInTetrahedronArray();

		sofa::core::topology::BaseMeshTopology::TrianglesInTetrahedron currentTrianglesInTetrahedron;
		sofa::helper::vector< unsigned int >  currentTetrahedraAroundTriangle;

		Deriv vector1;
		Deriv vector2;
		Deriv normal;

		const VecCoord *X0=this->mstate->getX0();

		for (unsigned int i=0; i<nTriangles; ++i)
		{
			currentTetrahedraAroundTriangle = m_Topology->getTetrahedraAroundTriangle(i);

			if (currentTetrahedraAroundTriangle.size()==1) {
				m_SurfaceTrianglesGlobalIndices.push_back(i);

//				currentTriangle = m_Topology->getLocalTrianglesInTetrahedron(i);
//
//				if((currentTriangle[0]=0) && (currentTriangle[1]=1) && (currentTriangle[2]=2))
//					m_SurfaceTrianglesLocalIndices.push_back(0);
//				else if((currentTriangle[0]=0) && (currentTriangle[1]=1) && (currentTriangle[2]=3))
//					m_SurfaceTrianglesLocalIndices.push_back(1);
//				else if((currentTriangle[0]=1) && (currentTriangle[1]=2) && (currentTriangle[2]=3))
//					m_SurfaceTrianglesLocalIndices.push_back(2);
//				else if((currentTriangle[0]=2) && (currentTriangle[1]=0) && (currentTriangle[2]=3))
//					m_SurfaceTrianglesLocalIndices.push_back(3);
//				else
//					std::cerr<<"WTF triangle has wrong local index\n";
				currentTrianglesInTetrahedron = m_Topology->getTrianglesInTetrahedron(currentTetrahedraAroundTriangle[0]);

				m_SurfaceTrianglesLocalIndices.push_back(m_Topology->getTriangleIndexInTetrahedron(currentTrianglesInTetrahedron,i));

				//calculate volume of triangle
				currentTriangle = m_Topology->getTriangle(i);

				vector1 = (*X0)[currentTriangle[1]] - (*X0)[currentTriangle[0]];
				vector2 = (*X0)[currentTriangle[2]] - (*X0)[currentTriangle[0]];
				normal = vector1.cross(vector2);


				m_SurfaceTrianglesVolume.push_back(normal.norm());





			}
			
		}


		this->reinit();

	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::reinit()
	   {
		unsigned int nElements=m_Topology->getNbTetrahedra();
		unsigned int nNodes = m_Topology->getNbTetrahedra();

		//this->m_MeanRotationMatricesPerNode.resize(nNodes);

		this->m_LinearTetrahedronInfos.resize(nElements);
		InitializeIntegrationPointsAndWeights();
		InitializeIntegrationPointsAndWeights2D();
		InitializeShapeFunctionDerivatives();
		InitializeShapeFunctions();
		InitializeElementData();
//		InitializePrecomputedStiffnessBlocks();
	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::addForce (VecDeriv& f, const VecCoord& x, const VecDeriv& v)
	   {
			//just 4 testing: save gradients at integration points as vtk unstructured grid

			std::cout<<"Save Debug data..\n";
			const VecCoord *X0=this->mstate->getX0();

			const char* filename = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/ShapeMatching/LiverRegisteredMeshDebugData.vtk";
			//save as vtk unstructured grid
			vtkPoints* pointContainer = vtkPoints::New();
			pointContainer->SetDataTypeToDouble();

			vtkCellArray* cellArray = vtkCellArray::New();

			double currentVTKPoint[3];
			vtkIdType currentVTKTriangle[3];
			LinearTriangleType currentTriangle;
			Coord currentPoint;

			for(unsigned int i=0;i<this->m_SurfaceTrianglesGlobalIndices.size();i++)
			{
				currentTriangle = m_Topology->getTriangle(this->m_SurfaceTrianglesGlobalIndices[i]);

				for(int j=0;j<3;j++)
				{
					currentVTKPoint[0] = (*X0)[currentTriangle[j]][0];
					currentVTKPoint[1] = (*X0)[currentTriangle[j]][1];
					currentVTKPoint[2] = (*X0)[currentTriangle[j]][2];

					currentVTKTriangle[j] = pointContainer->InsertNextPoint( currentVTKPoint);
				}
//				currentVTKTriangle[0] = currentTriangle[0];
//				currentVTKTriangle[1] = currentTriangle[1];
//				currentVTKTriangle[2] = currentTriangle[2];

				cellArray->InsertNextCell(3,currentVTKTriangle);
			}


			vtkUnstructuredGrid* myGrid = vtkUnstructuredGrid::New();
			myGrid->SetPoints(pointContainer);
			myGrid->SetCells(VTK_TRIANGLE, cellArray);

			vtkUnstructuredGridWriter* writer = vtkUnstructuredGridWriter::New();

			writer->SetInput(myGrid);
			writer->SetFileName(filename);
			//writer->SetDataModeToAscii();
			//writer->EncodeAppendedDataOff();
			writer->Write();

			pointContainer->Delete();
			cellArray->Delete();
			myGrid->Delete();
			writer->Delete();


			//////////////////////////////////////////////
			//const char* filename2 = "/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/ShapeMatching/LiverRegisteredMeshDebugDataIntegrationPoints.vtk";
			QString filename2("/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/ShapeMatching/LiverRegisteredMeshDebugDataIntegrationPoints_");
			//QString elementNumberString(elementNumber);
			filename2 += QString::number(counter);
			filename2 += ".vtk";

			//save as vtk unstructured grid
			vtkPoints* pointContainer2 = vtkPoints::New();
			pointContainer2->SetDataTypeToDouble();

			vtkFloatArray *scalars= vtkFloatArray::New();
			scalars->SetNumberOfComponents(1);
			//scalars->SetNumberOfTuples(nPoints);

			vtkFloatArray *vectors= vtkFloatArray::New();
			vectors->SetNumberOfComponents(3);
			vectors->SetName("potentialGradient");


			//Coord currentPoint;
			//LinearTriangleType currentTriangle;
			LinearTetraType currentTetra;
			unsigned int currentTetraId;
			unsigned int localSurfaceId;
			//Vec<4,Real> currentShapeFunctionValues;
			Mat<3,4,Real> currentCoords;
			Vec<3,Real> currentSOFAPoint;
			LinearHexaType currentHexa;
			Real dr, ds, dt;
			unsigned int currentHexaId;
			Vec<8,Real> currentPotentialValues;
			Vec<8,Real> currentShapeFunctionValues;
			Vec<4,Real> currentTetraShapeFunctionValues;
			Mat<3,8,Real> currentShapeFunctionDerivativesValues;

			int nx = m_PotentialFieldTopology->getNx();
			int ny = m_PotentialFieldTopology->getNy();
			int nz = m_PotentialFieldTopology->getNz();

			Vector3 hexaCoordinates;

			Real currentPotentialValue;
			Vector3 currentFilteredPotentialDerivativeValue;
			Vector3 currentPotentialDerivativeValue;


			for(int iterTriangle=0;iterTriangle<this->m_SurfaceTrianglesGlobalIndices.size();iterTriangle++)
			{
					currentTetraId = (m_Topology->getTetrahedraAroundTriangle(m_SurfaceTrianglesGlobalIndices[iterTriangle]))[0]; //is unique for surface triangles
					currentTetra = m_Topology->getTetrahedron(currentTetraId);
					localSurfaceId = m_SurfaceTrianglesLocalIndices[iterTriangle];
//					if(localSurfaceId == 0)
//					{

						for(int iter1=0;iter1<4;iter1++){
						for(int iter2=0;iter2<3;iter2++){
						  currentCoords[iter2][iter1] = (*X0)[currentTetra[iter1]][iter2];
						 }
						}


						  //loop over all integration points
						   for(unsigned int iterIntPoint=0;iterIntPoint<NUMBER_OF_SURFACE_INTEGRATION_POINTS;iterIntPoint++)
						   {
							   currentTetraShapeFunctionValues = this->m_ShapeFunctionsSurface[localSurfaceId][iterIntPoint];
							   //  std::cout<<"shape function values:"<<currentShapeFunctionValues[0]<<","<<currentShapeFunctionValues[1]<<","<<currentShapeFunctionValues[2]<<","<<currentShapeFunctionValues[3]<<"\n";
							   currentSOFAPoint = currentCoords*currentTetraShapeFunctionValues;
							   currentVTKPoint[0] = currentSOFAPoint[0];
							   currentVTKPoint[1] = currentSOFAPoint[1];
							   currentVTKPoint[2] = currentSOFAPoint[2];

							   pointContainer2->InsertNextPoint( currentVTKPoint);

							   //save potential field and potential field gradient per integration point

							   //get id of hexaeder and local coords per point
							   currentHexaId = m_PotentialFieldTopology->findHexa(currentSOFAPoint,dr,ds,dt);

							   if(currentHexaId == -1)
							   {
								   std::cout<<"Could not find point in potential field! CurrentPoint: "<<currentSOFAPoint<<"\n";
								   currentHexaId = m_PotentialFieldTopology->findHexa(currentSOFAPoint,dr,ds,dt);
							   }

							   //get 3D coordinate of hexa
							   hexaCoordinates = m_PotentialFieldTopology->getCubeCoordinate(currentHexaId);


							   //build local coords, hexaeder = (i i+1 i+dy*1 i+dy*1+1 i+dz i+dz+1 i+dz+dy i+dz+dy+1)
							   currentPotentialValues[0] = m_PotentialFieldValues[hexaCoordinates[0]+nx*(hexaCoordinates[1]+ny*hexaCoordinates[2])];
							   currentPotentialValues[1] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*(hexaCoordinates[1]+ny*hexaCoordinates[2])];
							   currentPotentialValues[2] = m_PotentialFieldValues[hexaCoordinates[0]+nx*((hexaCoordinates[1]+1)+ny*hexaCoordinates[2])];
							   currentPotentialValues[3] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*((hexaCoordinates[1]+1)+ny*hexaCoordinates[2])];
							   currentPotentialValues[4] = m_PotentialFieldValues[hexaCoordinates[0]+nx*(hexaCoordinates[1]+ny*(hexaCoordinates[2]+1))];
							   currentPotentialValues[5] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*(hexaCoordinates[1]+ny*(hexaCoordinates[2]+1))];
							   currentPotentialValues[6] = m_PotentialFieldValues[hexaCoordinates[0]+nx*((hexaCoordinates[1]+1)+ny*(hexaCoordinates[2]+1))];
							   currentPotentialValues[7] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*((hexaCoordinates[1]+1)+ny*(hexaCoordinates[2]+1))];

							   //get shape function values (inline function)
							   GetHexaShapeFunctionValues(currentShapeFunctionValues,dr,ds,dt);

							   //get shape function derivatives (inline function)
							   GetHexaShapeFunctionDerivativeValues(currentShapeFunctionDerivativesValues,dr,ds,dt);

							   //multiply to get values
							   currentPotentialValue = currentPotentialValues*currentShapeFunctionValues;
							   scalars->InsertNextTuple1(currentPotentialValue);

							   currentPotentialDerivativeValue = currentShapeFunctionDerivativesValues * currentPotentialValues;
							   vectors->InsertNextTuple3( -1*currentPotentialDerivativeValue[0],  -1*currentPotentialDerivativeValue[1],  -1*currentPotentialDerivativeValue[2]);

						   }
//					}







			}

			vtkPolyData* myGrid2 = vtkPolyData::New();
			myGrid2->SetPoints(pointContainer2);

			myGrid2->GetPointData()->SetScalars(scalars);
			myGrid2->GetPointData()->SetVectors(vectors);

			  vtkSmartPointer<vtkVertexGlyphFilter> vertexGlyphFilter =
			    vtkSmartPointer<vtkVertexGlyphFilter>::New();
			  vertexGlyphFilter->AddInput(myGrid2);
			  vertexGlyphFilter->Update();

			vtkPolyDataWriter* writer2 = vtkPolyDataWriter::New();

			writer2->SetInput(vertexGlyphFilter->GetOutput());
			writer2->SetFileName(filename2.toStdString().c_str());
			//writer->SetDataModeToAscii();
			//writer->EncodeAppendedDataOff();
			writer2->Write();

			pointContainer2->Delete();
			myGrid2->Delete();
			writer2->Delete();
			scalars->Delete();
			vectors->Delete();

			std::cout<<"Debug data saved..\n";

			////////////////////here we start the real add force method/////////////////////////////
			//declare variables
			ElementStiffnessMatrixType elementStiffnessMatrix ;
			Coord currentIntegrationPoint;

			unsigned int nElements=m_Topology->getNbTetrahedra();



			if(counter==0)
		   {
			   std::cout<<"Calculate forces \n";

			//loop over all elements and clear stiffness matrices
			for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
			{
				m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix.clear();
			}



			//loop over all surfaces
			for(int iterTriangle=0;iterTriangle<this->m_SurfaceTrianglesGlobalIndices.size();iterTriangle++)
			{
				//extract current element
				currentTetraId = (m_Topology->getTetrahedraAroundTriangle(m_SurfaceTrianglesGlobalIndices[iterTriangle]))[0]; //is unique for surface triangles
				currentTetra = m_Topology->getTetrahedron(currentTetraId);
				localSurfaceId = m_SurfaceTrianglesLocalIndices[iterTriangle];

				//extract current positions x
				for(int iter1=0;iter1<4;iter1++){
					for(int iter2=0;iter2<3;iter2++){
					  currentCoords[iter2][iter1] = (x)[currentTetra[iter1]][iter2];
					  //currentCoords[iter2][iter1] = (*X0)[currentTetra[iter1]][iter2];
					 }
				}

				//loop over all integration points
				for(unsigned int iterIntPoint=0;iterIntPoint<NUMBER_OF_SURFACE_INTEGRATION_POINTS;iterIntPoint++)
				{
				//calculate current position of int point
				currentTetraShapeFunctionValues = this->m_ShapeFunctionsSurface[localSurfaceId][iterIntPoint];
			    currentIntegrationPoint = currentCoords*currentTetraShapeFunctionValues;

			    //get potential derivative at that point
			       //get id of hexaeder and local coords per point
				   currentHexaId = m_PotentialFieldTopology->findHexa(currentIntegrationPoint,dr,ds,dt);

				   //get 3D coordinate of hexa
				   hexaCoordinates = m_PotentialFieldTopology->getCubeCoordinate(currentHexaId);

				   if(currentHexaId == -1)
				   {
					   std::cout<<"Could not find point in potential field! CurrentPoint: "<<currentIntegrationPoint<<"\n";
					   currentHexaId = m_PotentialFieldTopology->findHexa(currentIntegrationPoint,dr,ds,dt);
				   }

				   //build local coords, hexaeder = (i i+1 i+dy*1 i+dy*1+1 i+dz i+dz+1 i+dz+dy i+dz+dy+1)
				   currentPotentialValues[0] = m_PotentialFieldValues[hexaCoordinates[0]+nx*(hexaCoordinates[1]+ny*hexaCoordinates[2])];
				   currentPotentialValues[1] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*(hexaCoordinates[1]+ny*hexaCoordinates[2])];
				   currentPotentialValues[2] = m_PotentialFieldValues[hexaCoordinates[0]+nx*((hexaCoordinates[1]+1)+ny*hexaCoordinates[2])];
				   currentPotentialValues[3] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*((hexaCoordinates[1]+1)+ny*hexaCoordinates[2])];
				   currentPotentialValues[4] = m_PotentialFieldValues[hexaCoordinates[0]+nx*(hexaCoordinates[1]+ny*(hexaCoordinates[2]+1))];
				   currentPotentialValues[5] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*(hexaCoordinates[1]+ny*(hexaCoordinates[2]+1))];
				   currentPotentialValues[6] = m_PotentialFieldValues[hexaCoordinates[0]+nx*((hexaCoordinates[1]+1)+ny*(hexaCoordinates[2]+1))];
				   currentPotentialValues[7] = m_PotentialFieldValues[hexaCoordinates[0]+1+nx*((hexaCoordinates[1]+1)+ny*(hexaCoordinates[2]+1))];

				   //get shape function values (inline function)
				   GetHexaShapeFunctionValues(currentShapeFunctionValues,dr,ds,dt);

				   //get shape function derivatives (inline function)
				   GetHexaShapeFunctionDerivativeValues(currentShapeFunctionDerivativesValues,dr,ds,dt);

				   //multiply to get values
				   currentPotentialValue = currentPotentialValues*currentShapeFunctionValues;

				   //multiply to get values
				   currentPotentialDerivativeValue = currentShapeFunctionDerivativesValues * currentPotentialValues;

				   currentPotentialDerivativeValue.normalize();

				   currentFilteredPotentialDerivativeValue = currentPotentialDerivativeValue*currentPotentialValue;

					Mat<4,3,Real> shapeFunctionDerivativesGlobal = this->m_LinearTetrahedronInfos[currentTetraId].shapeFunctionDerivativesGlobalSurface[4*iterIntPoint+localSurfaceId];


				   int posColumnElement, posRowElement;


				   //add term to stiffness matrix
				   //m_LinearTetrahedronInfos[currentTetraId].ElementStiffnessMatrix.clear();
				   for(unsigned int m=0; m<4;m++)
				  {
					  for(unsigned int n=0; n<3;n++)
						{
						  //int posColumn = tetraIndices[m];
						  posColumnElement = 3*m+n;

						  for(unsigned int p=0; p<4;p++)
						  {
							  for(unsigned int i=0; i<3;i++)
							  {
								  posRowElement = 3*p+i;
//								  m_LinearTetrahedronInfos[currentTetraId].ElementStiffnessMatrix[posRowElement][posColumnElement] +=
//										  m_Charge.getValue()*
//										  currentPotentialDerivativeValue[n]*shapeFunctionDerivativesGlobal[m][i]* currentTetraShapeFunctionValues[p]*m_SurfaceTrianglesVolume[iterTriangle]
//										  *this->m_IntWeights2D[localSurfaceId][iterIntPoint];
								  if(n == i)
								  {
									  if(p==n)
									  {
										  m_LinearTetrahedronInfos[currentTetraId].ElementStiffnessMatrix[posRowElement][posColumnElement] +=
										  m_Charge.getValue()*
										  (-1)*currentFilteredPotentialDerivativeValue[n];//*currentTetraShapeFunctionValues[p];
													// *m_SurfaceTrianglesVolume[iterTriangle]*this->m_IntWeights2D[localSurfaceId][iterIntPoint];
									  }
								  }
//								  double temp = m_Charge.getValue()*
//										  currentPotentialDerivativeValue[n]*shapeFunctionDerivativesGlobal[m][i]* currentTetraShapeFunctionValues[p]*m_SurfaceTrianglesVolume[iterTriangle]
//										  *this->m_IntWeights2D[localSurfaceId][iterIntPoint];
//								  if(temp != 0)
//									  std::cout<<"Stiffness Value OK";
//								  else
//								  {
//									  std::cout<<"StiffnessValue = 0, charge:"<< m_Charge.getValue()<<" ,potderiv:"<<currentPotentialDerivativeValue[n]<<", shapederiv:"<<shapeFunctionDerivativesGlobal[m][i]
//									                     <<", tetraShape:"<<currentTetraShapeFunctionValues[p]<<", volume:"<<m_SurfaceTrianglesVolume[iterTriangle]<<", intWeight: "<<this->m_IntWeights2D[localSurfaceId][iterIntPoint]<<"\n";
//								  }


							  }
						  }
						}
					}

				}
				}
			}



			 //now use the element stiffness matrices to calculate force
			VecDeriv fDebug;
			unsigned int nNodes = m_Topology->getNbPoints();
			fDebug.resize(nNodes);
			for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
			{
				  currentTetra = m_Topology->getTetrahedron(iterElement);
				 // m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix.clear();

				  for(unsigned int m=0; m<4;m++)
				  {
					  for(unsigned int n=0; n<3;n++)
						{
						  int posColumn = currentTetra[m];
						  int posColumnElement = 3*m+n;

						  for(unsigned int p=0; p<4;p++)
						  {
								  int posRow = currentTetra[p];

//								  Real test1 = (*X0)[posColumn][n]*m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+0][posColumnElement];
//								  Real test2 = (*X0)[posColumn][n]*m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+1][posColumnElement];
//								  Real test3 = (*X0)[posColumn][n]*m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+2][posColumnElement];

								  Real test1 = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+0][posColumnElement];
								  Real test2 = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+1][posColumnElement];
								  Real test3 = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix[3*p+2][posColumnElement];

								  f[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
								  fDebug[posRow] += Deriv(test1, test2, test3);
						  }
						}
				}
			}

			///// print debug vector field ///////////////////////
			//////////////////////////////////////////////
			QString filename3("/org/share/home/mediassi/MediAssistData/Modelle/MIC/Leber/ShapeMatching/LiverRegisteredMeshDebugVectorField_");
			//QString elementNumberString(elementNumber);
			filename3 += QString::number(counter);
			filename3 += ".vtk";

			counter++;


			//save as vtk unstructured grid
			pointContainer2 = vtkPoints::New();
			pointContainer2->SetDataTypeToDouble();

//			scalars= vtkFloatArray::New();
//			scalars->SetNumberOfComponents(1);
			//scalars->SetNumberOfTuples(nPoints);

			vectors= vtkFloatArray::New();
			vectors->SetNumberOfComponents(3);
			vectors->SetName("forces");

			Vector3 currentForce;

			for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
			{
				currentSOFAPoint = (x)[iterNode];
				currentVTKPoint[0] = currentSOFAPoint[0];
			   currentVTKPoint[1] = currentSOFAPoint[1];
			   currentVTKPoint[2] = currentSOFAPoint[2];

			   pointContainer2->InsertNextPoint( currentVTKPoint);

				 currentForce = fDebug[iterNode];
				  vectors->InsertNextTuple3( currentForce[0],  currentForce[1],  currentForce[2]);

			}


			myGrid2 = vtkPolyData::New();
			myGrid2->SetPoints(pointContainer2);

			myGrid2->GetPointData()->SetVectors(vectors);

			  vertexGlyphFilter =
			    vtkSmartPointer<vtkVertexGlyphFilter>::New();
			  vertexGlyphFilter->AddInput(myGrid2);
			  vertexGlyphFilter->Update();

			writer2 = vtkPolyDataWriter::New();

			writer2->SetInput(vertexGlyphFilter->GetOutput());
			writer2->SetFileName(filename3.toStdString().c_str());
			//writer->SetDataModeToAscii();
			//writer->EncodeAppendedDataOff();
			writer2->Write();

			pointContainer2->Delete();
			myGrid2->Delete();
			writer2->Delete();
			vectors->Delete();

			std::cout<<"Debug vector force field data saved..\n";


	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::addDForce (VecDeriv& df, const VecDeriv& dx, double kFactor, double bFactor)
	   {

//	    kFactor = kFactor*(-1);
//
//		unsigned int nElements=m_Topology->getNbTetrahedra();
//
////		WriteVectorsToCVSFile(df, "ForceFieldDf", counter);
////		counter++;
//
//
//
//		 //add in the element matrix
//		for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//		{
//			ElementStiffnessMatrixType elementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix;
//
//			LinearTetraType tetraIndices = m_Topology->getTetrahedron(iterElement);
//
//			for(unsigned int m=0; m<4;m++)
//			{
//
//			  for(unsigned int n=0; n<3;n++)
//				{
//
//				  int posColumn = tetraIndices[m];
//				  int posColumnElement = 3*m+n;
//
//				  for(unsigned int i=0; i<4;i++)
//				  {
//					int posRow = tetraIndices[i];
//
//					double test1 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+0][posColumnElement]*kFactor;
//					double test2 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+1][posColumnElement]*kFactor;
//					double test3 = ((dx[posColumn])[n])*elementStiffnessMatrix[3*i+2][posColumnElement]*kFactor;
//
//					df[posRow] += Deriv(test1, test2, test3);//dx[posColumn] * elementStiffnessMatrix[posRowElement][posColumnElement];
//
//				  }
//				}
//			}
//		}



		///////////////////////////////////////////////////////////////////////////////

	   }

//---------------------------------------------------------------------------
// ApplyRotation Function
//---------------------------------------------------------------------------
template <class DataTypes>
void ShapeMatchingElectricPotentialForcefield< DataTypes >::applyRotation(const sofa::core::MechanicalParams* mparams /* PARAMS FIRST */, sofa::core::MultiVecDerivId dxId, sofa::core::MultiVecDerivId dfId, bool invert )
{
//	unsigned int nElements=m_Topology->getNbTetrahedra();
//		unsigned int nNodes = m_Topology->getNbPoints();
//		//this->
//
//		//const DataVecDeriv & dxData = *mparams->readDx(this->mstate);
//
//		const DataVecDeriv & dxData = *dxId[(this->mstate).get(mparams)].read();
//
//		//*fId[mstate.get(mparams)].write()
//		//DataVecDeriv & dxData = *dxId[this->mstate.get(mparams)].read();
//
//		const VecDeriv& dx = dxData.getValue(mparams);
//
//		DataVecDeriv & dfData = *dfId[(this->mstate).get()].write();
//
//		VecDeriv& df =  *dfData.beginEdit(mparams);
//
//
//
//
//
//	RotationMatrixType RMean;
//	LinearTetraType tetraIndices;
//	int currentNodeId;
//
//
//
//	for(unsigned int iterNode=0;iterNode<nNodes;iterNode++)
//	{
//	   // get mean rotation matrix
//		RMean = this->m_MeanRotationMatricesPerNode[iterNode];
//
//		if(invert)
//			 RMean = RMean.transposed();
//
//
//		Deriv currentDx = (dx[iterNode]);
//		Deriv result;
//
//		result = RMean*currentDx;//RMean * currentDx;
//
//		//result = RMean.transposed()*result;
//
//		df[iterNode] = result;
//
//
//	  }
//
//
//
//	dfData.endEdit(mparams);
//	this->WriteVectorsToCVSFile(df, "ApplyRotationForecFieldAfter",counter);
//	counter++;

}

template <class DataTypes>
	   double ShapeMatchingElectricPotentialForcefield<DataTypes>:: getPotentialEnergy(const VecCoord& x) const
	   {
			return 1.0;
	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::addKToMatrix(sofa::defaulttype::BaseMatrix *mat, SReal kFactor, unsigned int &offset)
	   {
//	kFactor = kFactor*(-1);
//
//			unsigned int nElements=this->m_Topology->getNbTetrahedra();
//
//			ElementStiffnessMatrixType elementStiffnessMatrix;
//
//			LinearTetraType tetraIndices;
//
//			unsigned int posRow, posColumn, posRowElement, posColumnElement;
//
//			for(unsigned int iterElement=0;iterElement<nElements;iterElement++)
//			{
//			  elementStiffnessMatrix = m_LinearTetrahedronInfos[iterElement].ElementStiffnessMatrix;
//			  tetraIndices = m_Topology->getTetrahedron(iterElement);
//
//			  for(unsigned int m=0; m<4;m++)
//			  {
//				  for(unsigned int n=0; n<3;n++)
//					{
//					  posColumn = 3*tetraIndices[m]+n + offset;
//					  posColumnElement = 3*m+n;
//
//					  for(unsigned int i=0; i<4;i++)
//					  {
//						  for(unsigned int j=0; j<3;j++)
//						  {
//							  posRow = 3*tetraIndices[i]+j + offset;
//							  posRowElement = 3*i+j;
//
//							  mat->add(posRow, posColumn, elementStiffnessMatrix[posRowElement][posColumnElement]*kFactor);
//						  }
//					  }
//
//					}
//				}
//			}

	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::handleTopologyChange()
	   {

	   }

// ****************************************************************************
// Protected/Private Methods
// ****************************************************************************

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::GetHexaShapeFunctionValues(Vec<8,Real> & values, Real r, Real s,Real t)
	   {
			values[0]=(1-r)*(1-s)*(1-t);
			values[1]=(r)*(1-s)*(1-t);
			values[2]=(1-r)*(s)*(1-t);
			values[3]=(r)*(s)*(1-t);
			values[4]=(1-r)*(1-s)*(t);
			values[5]=(r)*(1-s)*(t);
			values[6]=(1-r)*(s)*(t);
			values[7]=(r)*(s)*(t);

	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::GetHexaShapeFunctionDerivativeValues(Mat<3,8,Real> & values, Real r, Real s,Real t)
		{
			values[0][0] = -1*(1-s)*(1-t) / dx;
			values[1][0] = -1*(1-r)*(1-t) / dy;
			values[2][0] = -1*(1-r)*(1-s) / dz;

			values[0][1] =  (1-s)*(1-t) / dx;
			values[1][1] = -1*(r)*(1-t) / dy;
			values[2][1] = -1*(r)*(1-s) / dz;

			values[0][2] = -1*(s)*(1-t) / dx;
			values[1][2] =  (1-r)*(1-t) / dy;
			values[2][2] = -1*(1-r)*(s) / dz;

			values[0][3] = (s)*(1-t) / dx;
			values[1][3] = (r)*(1-t) / dy;
			values[2][3] = -1*(r)*(s) / dz;

			values[0][4] = -1*(1-s)*(t) / dx;
			values[1][4] = -1*(1-r)*(t) / dy;
			values[2][4] = (1-r)*(1-s) / dz;

			values[0][5] =  (1-s)*(t) / dx;
			values[1][5] = -1*(r)*(t) / dy;
			values[2][5] =  (r)*(1-s) / dz;

			values[0][6] = -1*(s)*(t) / dx;
			values[1][6] = (1-r)*(t) / dy;
			values[2][6] = (1-r)*(s) / dz;

			values[0][7] = (s)*(t) / dx;
			values[1][7] = (r)*(t) / dy;
			values[2][7] = (r)*(s) / dz;
		}


template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializePrecomputedStiffnessBlocks()
	   {
		unsigned int nElements=m_Topology->getNbTetrahedra();
		for(unsigned int i=0; i<nElements;i++)
		{
			this->InitializePrecomputedStiffnessBlocks(i); //inline definition follows right after!
		}
	   }

//ElementStiffnessBlock is computed here for element with elementID!TODO: Thinking about rename to InitializePrecomputedElementStiffnessBlock(unsigned int elementID)
template <class DataTypes>
	   inline void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializePrecomputedStiffnessBlocks(unsigned int elementID)
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
	   inline double ShapeMatchingElectricPotentialForcefield<DataTypes>::GetElementVolume(unsigned int elementID, ElementCoordsType coords)
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
	   inline void ShapeMatchingElectricPotentialForcefield<DataTypes>::BuildStiffnessBlock(unsigned int elementID, PrecomputedStiffnessBlockMatrixType & stiffnessBlockMatrix,ElementCoordsType coords, unsigned int integrationPointIndex)
	   {

//		ElementStiffnessMatrixType stiffnessBlock;
//
//		stiffnessBlock.clear();
//
//		double mu = this->m_ShearModulus ;
//
//		double lambda = this->m_LamesFirstParameter;
//
//		Mat<3,3,Real> jacobianMatrix;
//
//		this->GetJacobian(jacobianMatrix,coords,integrationPointIndex);
//
//		double jacobianDeterminant = sofa::defaulttype::determinant(jacobianMatrix);
//
//		Mat<3,3,Real> inverseJacobian;
//		sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);
//
//		Mat<4,3,Real> shapeFunctionDerivativesGlobal = this->m_LinearTetrahedronInfos[elementID].shapeFunctionDerivativesGlobal[integrationPointIndex];
//
//		//Only used for debug
////		double debug11 = shapeFunctionDerivativesGlobal[0][0];
////		double debug12 = shapeFunctionDerivativesGlobal[0][1];
////		double debug13 = shapeFunctionDerivativesGlobal[0][2];
////
////		double debug21 = shapeFunctionDerivativesGlobal[1][0];
////		double debug22 = shapeFunctionDerivativesGlobal[1][1];
////		double debug23 = shapeFunctionDerivativesGlobal[1][2];
////
////		double debug31 = shapeFunctionDerivativesGlobal[2][0];
////		double debug32 = shapeFunctionDerivativesGlobal[2][1];
////		double debug33 = shapeFunctionDerivativesGlobal[2][2];
////
////		double debug41 = shapeFunctionDerivativesGlobal[3][0];
////		double debug42 = shapeFunctionDerivativesGlobal[3][1];
////		double debug43 = shapeFunctionDerivativesGlobal[3][2];
//
//		unsigned int mn,ij;
//
//		//build stiffness block (force derivative \partial F _ij over \partial U _mn)
//		for(unsigned int m=0;m<4;m++)
//		{
//			for(unsigned int n=0;n<3;n++)
//			{
//			  for(unsigned int i=0;i<4;i++)
//			  {
//				  for(unsigned int j=0;j<3;j++)
//				  {
//					  mn = 3*m+n;
//					  ij = 3*i+j;
//
//					  stiffnessBlock[ij][mn]= 0;
//
//					  //factor (-1) is due to sofa convention
//					  stiffnessBlock[ij][mn] += (lambda *shapeFunctionDerivativesGlobal[m][n]*shapeFunctionDerivativesGlobal[i][j]
//													 + mu*shapeFunctionDerivativesGlobal[m][j]*shapeFunctionDerivativesGlobal[i][n])*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
//					  if(j==n)
//					  {
//						  for(unsigned int k=0;k<3;k++)
//						  {
//							  stiffnessBlock[ij][mn] += mu*shapeFunctionDerivativesGlobal[m][k]*shapeFunctionDerivativesGlobal[i][k]*jacobianDeterminant*this->m_IntWeights[integrationPointIndex];
//						  }
//					  }
//
//				  }
//			  }
//			}
//		}
//
//		//this->WriteElementMatrixToCVSFile(stiffnessBlock, "StiffnessElementMatrixNo_",elementID);
//
//		//copy stiffness block to new datastructure
//		PrecomputedStiffnessBlockType currentStiffnessBlock;
//
//		for(unsigned int m=0;m<4;m++)
//		{
//			for(unsigned int i=0;i<4;i++)
//			{
//			  currentStiffnessBlock.clear();
//
//			  for(unsigned int n=0;n<3;n++)
//			  {
//				  for(unsigned int j=0;j<3;j++)
//				  {
//					  mn = 3*m+n;
//					  ij = 3*i+j;
//					  currentStiffnessBlock[j][n] = stiffnessBlock[ij][mn];
//
//				  }
//			  }
//
//			  stiffnessBlockMatrix[i][m] = currentStiffnessBlock;
//
//			}
//		}


	   }


template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializeShapeFunctionDerivatives()
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

			//iterate over surfaces
			for(unsigned int j=0; j< 4;j++)
			{
				for(unsigned int i=0; i< NUMBER_OF_SURFACE_INTEGRATION_POINTS;i++)
				{
//				  DataTypes::set ( currentIntegrationPoint, this->m_NodalLocalCoordinates[3*i], this->m_NodalLocalCoordinates[3*i+1], this->m_NodalLocalCoordinates[3*i+2]);
//				  r=currentIntegrationPoint[0];
//				  s=currentIntegrationPoint[1];
//				  t=currentIntegrationPoint[2];
//
//				  tempExp = 1-r-s-t;


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

				  m_ShapeFunctionDerivativesSurface[j][i] = currentShapeFunctionDerivatives;

				}
			}

	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializeShapeFunctions()
	   {
			m_ShapeFunctions.resize(numberOfIntegrationPoints);

		  Vec<4,Real> currentShapeFunctions;
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

		//iterate over surfaces
		for(unsigned int j=0; j< 4;j++)
		{
			for(unsigned int i=0; i< NUMBER_OF_SURFACE_INTEGRATION_POINTS;i++)
			{
				  currentIntegrationPoint = m_IntPoints2D[j][i];// DataTypes::set ( currentIntegrationPoint, m_IntPoints2D[j][i][3*i], m_IntPoints[3*i+1], m_IntPoints[3*i+2]);
				  r=currentIntegrationPoint[0];
				  s=currentIntegrationPoint[1];
				  t=currentIntegrationPoint[2];

				  tempExp = 1-r-s-t;

				  currentShapeFunctions[0] = r;
				  currentShapeFunctions[1] = s;
				  currentShapeFunctions[2] = tempExp;
				  currentShapeFunctions[3] = t;

			      m_ShapeFunctionsSurface[j][i] = currentShapeFunctions;

			}
		}



	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializeIntegrationPointsAndWeights()
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
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializeIntegrationPointsAndWeights2D()
	   {
				double r11 = 0.333333333333333;

				double r13 = 0.166666666666667;
				double r23 = 0.666666666666667;
				double w13 = 0.333333333333333;

				double r17 = 0.1012865073235;
				double r27 = 0.7974269853531;
				double r47 = 0.4701420641051;
				double r67 = 0.0597158717898;
				double r77 = 0.333333333333333;
				double w17 = 0.1259391805448;
				double w47 = 0.1323941527885;
				double w77 = 0.225;

				Mat<NUMBER_OF_SURFACE_INTEGRATION_POINTS,2,Real> intPointsTemp;
				double intWeightsTemp[NUMBER_OF_SURFACE_INTEGRATION_POINTS];


				switch(NUMBER_OF_SURFACE_INTEGRATION_POINTS){
					  case 1:
						  intPointsTemp[0][0] = r11;
						  intPointsTemp[0][1] = r11;

						  intWeightsTemp[0] = 1.0;
						  break;
					  case 3:
						  intPointsTemp[0][0] = r13;
						  intPointsTemp[0][1] = r13;

						  intPointsTemp[1][0] = r23;
						  intPointsTemp[1][1] = r13;

						  intPointsTemp[2][0] = r13;
						  intPointsTemp[2][1] = r23;


						  intWeightsTemp[0] = w13;
						  intWeightsTemp[1] = w13;
						  intWeightsTemp[2] = w13;
						  break;
					  case 7:
						  intPointsTemp[0][0] = r17;
						  intPointsTemp[0][1] = r17;

						  intPointsTemp[1][0] = r27;
						  intPointsTemp[1][1] = r17;

						  intPointsTemp[2][0] = r17;
						  intPointsTemp[2][1] = r27;

						  intPointsTemp[3][0] = r47;
						  intPointsTemp[3][1] = r67;

						  intPointsTemp[4][0] = r47;
						  intPointsTemp[4][1] = r47;

						  intPointsTemp[5][0] = r67;
						  intPointsTemp[5][1] = r47;

						  intPointsTemp[6][0] = r77;
						  intPointsTemp[7][1] = r77;


						  intWeightsTemp[0] = w17;
						  intWeightsTemp[1] = w17;
						  intWeightsTemp[2] = w17;
						  intWeightsTemp[3] = w47;
						  intWeightsTemp[4] = w47;
						  intWeightsTemp[5] = w47;
						  intWeightsTemp[6] = w77;
					  		  break;
					  default:
						  serr << "Wrong number of 2D integration points" << sendl;

					  }

				//now fill in the local coordinates for each surface
				Mat<3,3,Real> currentMat;
				Coord currentLocalCoords;
				Coord currentShapeFunctionValues;

				for(unsigned int i=0; i<NUMBER_OF_SURFACE_INTEGRATION_POINTS;i++)
				{
					currentShapeFunctionValues[0] = 1-intPointsTemp[i][0]-intPointsTemp[i][1];
					currentShapeFunctionValues[1] = intPointsTemp[i][0];
					currentShapeFunctionValues[2] = intPointsTemp[i][1];

					//surface 1: 1,3,2
					currentMat[0][0] = 0;
					currentMat[1][0] = 1;
					currentMat[2][0] = 0;

					currentMat[0][1] = 0;
					currentMat[1][1] = 0;
					currentMat[2][1] = 1;

					currentMat[0][2] = 0;
					currentMat[1][2] = 0;
					currentMat[2][2] = 0;

					currentLocalCoords = currentMat*currentShapeFunctionValues;

					std::cout<<"Local coordinates surface 0: "<< currentLocalCoords[0] <<","<<currentLocalCoords[1] <<","<<currentLocalCoords[2] <<"\n";

					m_IntPoints2D[0][i] = currentLocalCoords;
					m_IntWeights2D[0][i] = 0.5*intWeightsTemp[i];

					//surface 2: 0,2,3
					currentMat[0][0] = 1;
					currentMat[1][0] = 0;
					currentMat[2][0] = 0;

					currentMat[0][1] = 0;
					currentMat[1][1] = 0;
					currentMat[2][1] = 0;

					currentMat[0][2] = 0;
					currentMat[1][2] = 0;
					currentMat[2][2] = 1;

					currentLocalCoords = currentMat*currentShapeFunctionValues;

					m_IntPoints2D[1][i] = currentLocalCoords;
					m_IntWeights2D[1][i] = 0.5*intWeightsTemp[i];

					//surface 3: 0,3,1
					currentMat[0][0] = 1;
					currentMat[1][0] = 0;
					currentMat[2][0] = 0;

					currentMat[0][1] = 0;
					currentMat[1][1] = 0;
					currentMat[2][1] = 1;

					currentMat[0][2] = 0;
					currentMat[1][2] = 1;
					currentMat[2][2] = 0;

					currentLocalCoords = currentMat*currentShapeFunctionValues;

					m_IntPoints2D[2][i] = currentLocalCoords;
					m_IntWeights2D[2][i] = 0.8660254037844386*intWeightsTemp[i];

					//surface 4: 0,1,2
					currentMat[0][0] = 1;
					currentMat[1][0] = 0;
					currentMat[2][0] = 0;

					currentMat[0][1] = 0;
					currentMat[1][1] = 1;
					currentMat[2][1] = 0;

					currentMat[0][2] = 0;
					currentMat[1][2] = 0;
					currentMat[2][2] = 0;

					currentLocalCoords = currentMat*currentShapeFunctionValues;

					m_IntPoints2D[3][i] = currentLocalCoords;
					m_IntWeights2D[3][i] = 0.5*intWeightsTemp[i];

				}




	   }

template <class DataTypes>
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::InitializeElementData()
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

			//initialize surface shape function derivatives
			//loop over all integration points
			for(unsigned int iterIntPoint=0;iterIntPoint<NUMBER_OF_SURFACE_INTEGRATION_POINTS;iterIntPoint++)
			{
				for(unsigned int iterSurface=0;iterSurface<4;iterSurface++)
				{
					 Mat<3,3,Real> jacobianMatrix;

					  this->GetSurfaceJacobian(jacobianMatrix,coords,iterSurface,iterIntPoint);

					  Mat<3,3,Real> inverseJacobian;
					  sofa::defaulttype::invertMatrix(inverseJacobian,jacobianMatrix);

					  //get the shape function derivatives in local coordinates
					  Mat<4,3,Real> shapeFunctionDerivativesSurfaceLocal = this->m_ShapeFunctionDerivativesSurface[iterSurface][iterIntPoint];

					  //debug output
					 // std::cout<<"ShapeFunctionDerivLocal: "<<shapeFunctionDerivativesSurfaceLocal <<"\n";

					  //transform shape function derivatives in global coordinates

					  Mat<4,3,Real> shapeFunctionDerivativesSurfaceGlobal;
					  shapeFunctionDerivativesSurfaceGlobal.clear();
					  for(unsigned int i=0;i<4;i++)
					  {
						  for(unsigned int j=0;j<3;j++)
						  {
							  for(unsigned int k=0;k<3;k++)
							  {
								  shapeFunctionDerivativesSurfaceGlobal[i][j]+=shapeFunctionDerivativesSurfaceLocal[i][k]*inverseJacobian[k][j];
							  }
						  }
					  }

					this->m_LinearTetrahedronInfos[iterElement].shapeFunctionDerivativesGlobalSurface[4*iterIntPoint+iterSurface]=shapeFunctionDerivativesSurfaceGlobal;


//
//					//calculate current position of int point
//					currentTetraShapeFunctionValues = this->m_ShapeFunctionsSurface[localSurfaceId][iterIntPoint];
//
//					m_ShapeFunctionDerivativesSurface[j][i]
				}
			}

		}




	   }

template <class DataTypes>
	   inline void ShapeMatchingElectricPotentialForcefield<DataTypes>::GetJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfIntegrationPoint)
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
	   inline void ShapeMatchingElectricPotentialForcefield<DataTypes>::GetSurfaceJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int surfaceIndex, int indexOfIntegrationPoint)
	   {
			Mat<4,3,Real> currentShapeFunctionDerivatives = this->m_ShapeFunctionDerivativesSurface[surfaceIndex][indexOfIntegrationPoint];
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
	   inline void ShapeMatchingElectricPotentialForcefield<DataTypes>::GetNodalJacobian(Mat<3,3,Real> &jacobianMatrix, ElementCoordsType coords, int indexOfNodalPoint)
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
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::WriteMatrixToCVSFile(defaulttype::BaseMatrix * mat,char* filename)
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
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::WriteElementMatrixToCVSFile(Mat<12,12,Real>  mat,char* filename, int elementNumber)
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
	   void ShapeMatchingElectricPotentialForcefield<DataTypes>::WriteVectorsToCVSFile(const VecDeriv& vec, char* filename, int elementNumber)
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
