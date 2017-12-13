/*
 * CollisionDetectionMeshSampler.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: milschew
 */

#include "CollisionDetectionMeshSampler.h"
#include "PoissonPotentialForcefieldUtil.h"

CollisionDetectionMeshSampler::CollisionDetectionMeshSampler()
{
	reader = NULL;
	grid = NULL;
	//deformedMeshSurfaceObjectHandle = NULL;
}

CollisionDetectionMeshSampler::~CollisionDetectionMeshSampler()
{
	cleanUp();
}

void CollisionDetectionMeshSampler::cleanUp(void)
{
	if(reader != NULL)
	{
		reader->Delete();
		//DT_DestroyObject(deformedMeshSurfaceObjectHandle);
		reader = NULL;
	}
}

void CollisionDetectionMeshSampler::loadFile(const char * filePath)
{
	cleanUp();

	reader = vtkSTLReader::New();
	reader->SetFileName(filePath);
	reader->Update();
	grid = reader->GetOutput();

	cellLocatorRef = vtkSmartPointer<vtkCellLocator>::New();
	cellLocatorRef->SetDataSet ( grid );
	cellLocatorRef->BuildLocator();
}

bool CollisionDetectionMeshSampler::getSample(double & sample, vtkIdType & cellId,Vector3 globalPoint)
{
	cellId = -1;
	sample = PoissonPotentialForcefieldUtil::getDistance(cellLocatorRef, globalPoint.ptr());
	return true;
}

bool CollisionDetectionMeshSampler::getSampleCD(double & sample, Vector3 & collisionPointToGlobalPoint, Vector3 globalPoint)
{
	double deformedMeshSurfacePoint[3];

	sample = PoissonPotentialForcefieldUtil::getDistance(cellLocatorRef, globalPoint.ptr(), deformedMeshSurfacePoint);

	collisionPointToGlobalPoint[0] = globalPoint[0] - deformedMeshSurfacePoint[0];
	collisionPointToGlobalPoint[1] = globalPoint[1] - deformedMeshSurfacePoint[1];
	collisionPointToGlobalPoint[2] = globalPoint[2] - deformedMeshSurfacePoint[2];

	return true;
}
