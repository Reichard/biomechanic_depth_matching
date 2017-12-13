/*
 * MeshSampler.h
 *
 *  Created on: Feb 19, 2012
 *      Author: milschew
 */

#ifndef __MESHSAMPLER_H
#define __MESHSAMPLER_H

#include <sofa/defaulttype/Vec.h>
#include <vtkCell.h>

using namespace sofa::defaulttype;

class MeshSampler
{
public:
	virtual ~MeshSampler() {};
	virtual void getSmallestCellBoundingBox(double bounds[6]) = 0;
	virtual bool getSample(double & sample, vtkIdType & cellId,Vector3 globalPoint) = 0;
	virtual bool getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint) = 0;
	virtual bool getCellBoundingBox(double bounds[6], Vector3 globalPoint) = 0;
	virtual void loadFile(const char * filePath) = 0;

	virtual bool isValidMesh(void)
	{
		return true;
	}

	virtual bool getSampleCD(double & sample, Vector3 & collisionPointToGlobalPoint, Vector3 globalPoint)
	{
		return false;
	}
};

#endif
