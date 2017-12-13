/*
 * CollisionDetectionMeshSampler.h
 *
 *  Created on: Mar 6, 2013
 *      Author: milschew
 */

#ifndef COLLISIONDETECTIONMESHSAMPLER_H_
#define COLLISIONDETECTIONMESHSAMPLER_H_

#include "MeshSampler.h"
#include "vtkSTLReader.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkCellLocator.h"
//#include <SOLID/SOLID.h>

class CollisionDetectionMeshSampler : public MeshSampler
{
public:
	CollisionDetectionMeshSampler(void);
	virtual ~CollisionDetectionMeshSampler();

	virtual bool getSample(double & sample, vtkIdType & cellId,Vector3 globalPoint);

	bool getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint)
	{
		// not supported!
		return false;
	}
	virtual void getSmallestCellBoundingBox(double bounds[6])
	{
		// not supported
	}
	virtual bool getCellBoundingBox(double bounds[6], Vector3 globalPoint)
	{
		// not supported
		return false;
	}

	virtual void loadFile(const char * filePath);

	virtual bool isValidMesh(void)
	{
		return false;
	}

	virtual bool getSampleCD(double & sample, Vector3 & collisionPointToGlobalPoint, Vector3 globalPoint);

protected:
	vtkSTLReader * reader;
	vtkPolyData * grid;
	vtkSmartPointer<vtkCellLocator> cellLocatorRef;
	//DT_ObjectHandle deformedMeshSurfaceObjectHandle;

	void cleanUp(void);
};

#endif /* COLLISIONDETECTIONMESHSAMPLER_H_ */
