/*
 * StructuredHexahedralMeshSampler.h
 *
 *  Created on: Feb 20, 2013
 *      Author: milschew
 */

#ifndef STRUCTUREDHEXAHEDRALMESHSAMPLER_H_
#define STRUCTUREDHEXAHEDRALMESHSAMPLER_H_

#include "MeshSampler.h"

#include "vtkStructuredGridReader.h"
#include "vtkStructuredGrid.h"

class StructuredHexahedralMeshSampler : public MeshSampler
{
public:
	StructuredHexahedralMeshSampler(void);
	virtual ~StructuredHexahedralMeshSampler();

	virtual bool getSample(double & sample, vtkIdType & cellId,Vector3 globalPoint);
	bool getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint);
	virtual void getSmallestCellBoundingBox(double bounds[6]);
	virtual bool getCellBoundingBox(double bounds[6], Vector3 globalPoint);

	virtual void loadFile(const char * filePath);

protected:
	vtkStructuredGridReader * reader;
	vtkStructuredGrid * grid;

	void GetHexahedronShapeFunctionValues(Vec<8, double> & values, double r, double s, double t);
	void cleanUp(void);
};

#endif /* STRUCTUREDHEXAHEDRALMESHSAMPLER_H_ */
