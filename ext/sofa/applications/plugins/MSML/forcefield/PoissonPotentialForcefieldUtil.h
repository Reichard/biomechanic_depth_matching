/*
 * PoissonPotentialForcefieldUtil.h
 *
 *  Created on: Mar 6, 2013
 *      Author: milschew
 */

#ifndef POISSONPOTENTIALFORCEFIELDUTIL_H_
#define POISSONPOTENTIALFORCEFIELDUTIL_H_

//#include <SOLID/SOLID.h>
#include "vtkPolyData.h"
#include "vtkCellLocator.h"

#include <sofa/defaulttype/Mat.h>

using namespace sofa::defaulttype;

class PoissonPotentialForcefieldUtil
{
public:

	static float getDistance(vtkCellLocator* cellLocator, double point[3], double deformedMeshSurfacePoint[3])
	{
		/*DT_ShapeHandle pointShapeHandle;
		DT_ObjectHandle pointObjectHandle;
		float tempPoint[3];
		float distance = 0.0f;
		float floatDeformedMeshSurfacePoint[3];
		float floatPoint[3];

		floatPoint[0] = point[0];
		floatPoint[1] = point[1];
		floatPoint[2] = point[2];

		pointShapeHandle = DT_NewPoint(floatPoint);
		DT_EndComplexShape();
		pointObjectHandle = DT_CreateObject(0, pointShapeHandle);
		distance = DT_GetClosestPair(deformedMeshSurfaceObjectHandle, pointObjectHandle, floatDeformedMeshSurfacePoint, tempPoint);

		deformedMeshSurfacePoint[0] = floatDeformedMeshSurfacePoint[0];
		deformedMeshSurfacePoint[1] = floatDeformedMeshSurfacePoint[1];
		deformedMeshSurfacePoint[2] = floatDeformedMeshSurfacePoint[2];

		DT_DestroyObject(pointObjectHandle);
		DT_DeleteShape(pointShapeHandle);

		return distance;*/
		vtkIdType currentCellId;
        int currentSubId;
        double currentDistance;
		

		//first find closes point
		cellLocator->FindClosestPoint ( point, deformedMeshSurfacePoint,
										    currentCellId,currentSubId,currentDistance );

		
		
		
		return currentDistance;
	}

	static float getDistance( vtkCellLocator* cellLocator, double point[3])
	{
		double deformedMeshSurfacePoint[3];

		return getDistance( cellLocator, point, deformedMeshSurfacePoint);
	}

	//static void buildDeformedMeshSurfaceObjectHandle(vtkPolyData* deformedMeshSurface)
	//{
		/*DT_ShapeHandle meshShapeHandle = DT_NewComplexShape(0);

		vtkIdType* currentCellPoints;
		vtkIdType numberOfNodes=3;
		float currentVertex[3];
		double currentVTKVertex[3];

		// TODO performance???
		deformedMeshSurface->BuildCells();

		for(int i=0; i<deformedMeshSurface->GetNumberOfCells(); i++)
		{
			 deformedMeshSurface->GetCellPoints(i, numberOfNodes,currentCellPoints);
			 if(numberOfNodes != 3)
				 std::cerr<<"WTF:Number of nodes not 3 \n";

			 DT_Begin();

			 for(int j=0; j<numberOfNodes; j++)
			 {
				 deformedMeshSurface->GetPoint(currentCellPoints[j], currentVTKVertex);
				 currentVertex[0] = currentVTKVertex[0];
				 currentVertex[1] = currentVTKVertex[1];
				 currentVertex[2] = currentVTKVertex[2];
				 DT_Vertex(currentVertex);
			 }

			 DT_End();

		}

		DT_EndComplexShape();

		DT_ObjectHandle meshObjectHandle = DT_CreateObject(0,meshShapeHandle);
		return meshObjectHandle;*/

		// TODO delete meshShapeHandle
	//}

	static void GetHexahedronShapeFunctionValues(Vec<8,double> & values, double r, double s,double t)
	{
		/*
		 * Order of points in hexahedron (bottom face)
		 * 3--2
		 * |  |
		 * 0--1
		 */

		values[0] = (1.0 - r) * (1.0 - s) * (1.0 - t);
		values[1] = (r) * (1.0 - s) * (1.0 - t);
		values[2] = (r) * (s) * (1.0 - t);
		values[3] = (1.0 - r) * (s) * (1.0 - t);
		values[4] = (1.0 - r) * (1.0 - s) * (t);
		values[5] = (r) * (1.0 - s) * (t);
		values[6] = (r) * (s) * (t);
		values[7] = (1.0 - r) * (s) * (t);
	}

	static void GetLocalHexahedronShapeFunctionDerivatives(Mat<8,3,double> & values, double r, double s,double t)
	{
		// dN1/dr, dN1/ds, dN1/dt
		values[0][0] = - (1.0 - s) * (1.0 - t);
		values[0][1] = - (1.0 - r) * (1.0 - t);
		values[0][2] = - (1.0 - r) * (1.0 - s);

		// dN2/dr, dN2/ds, dN2/dt
		values[1][0] = (1.0 - s) * (1.0 - t);
		values[1][1] = - (r) * (1.0 - t);
		values[1][2] = - (r) * (1.0 - s);

		values[2][0] = (s) * (1.0 - t);
		values[2][1] = (r) * (1.0 - t);
		values[2][2] = - (r) * (s);

		values[3][0] = - (s) * (1.0 -t);
		values[3][1] = (1.0 - r) * (1.0 - t);
		values[3][2] = - (1.0 - r) * (s);

		values[4][0] = - (1.0 - s) * (t);
		values[4][1] = - (1.0 - r) * (t);
		values[4][2] = (1.0 - r) * (1.0 - s);

		values[5][0] = (1.0 - s) * (t);
		values[5][1] = - (r) * (t);
		values[5][2] = (r) * (1.0 - s);

		values[6][0] = (s) * (t);
		values[6][1] = (r) * (t);
		values[6][2] = (r) * (s);

		values[7][0] = - (s) * (t);
		values[7][1] = (1.0 - r) * (t);
		values[7][2] = (1.0 - r) * (s);
	}

	static void GetTriangleShapeFunctionValues(Vec<3,double> & values, double r, double s)
	{
		values[0] = r;
		values[1] = s;
		values[2] = 1 - r - s;
	}

	static void GetLocalTriangleShapeFunctionDerivatives(Mat<3,2,double> & values, double r, double s)
	{
		// dN1/dr, dN1/ds, dN1/dt
		values[0][0] = 1;
		values[0][1] = 0;

		// dN2/dr, dN2/ds, dN2/dt
		values[1][0] = 0;
		values[1][1] = 1;

		values[2][0] = -1;
		values[2][1] = -1;
	}
};

#endif /* POISSONPOTENTIALFORCEFIELDUTIL_H_ */
