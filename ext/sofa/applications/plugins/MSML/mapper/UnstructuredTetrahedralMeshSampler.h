/*
 * UnstructuredTetrahedralMeshSampler.h
 *
 *  Created on: Feb 19, 2012
 *      Author: milschew
 */

#ifndef __UNSTRUCTUREDTETRAHEDRALMESHSAMPLER_H
#define __UNSTRUCTUREDTETRAHEDRALMESHSAMPLER_H

#include "MeshSampler.h"

#include "vtkType.h"
#include "vtkUnstructuredGrid.h"
#include "vtkUnstructuredGridReader.h"
#include "vtkPoints.h"

#include <sofa/defaulttype/Vec.h>
#include <sofa/helper/vector.h>
#include <sofa/defaulttype/Mat.h>

using namespace sofa::defaulttype;
using namespace sofa::helper;

struct Node {

	Vec<3, double> min;
	Vec<3, double> max;

	vector<vtkIdType> cells;
	Node * child1;
	Node * child2;
};

class UnstructuredTetrahedralMeshSampler : public MeshSampler
{
public:
	UnstructuredTetrahedralMeshSampler(void);
	virtual ~UnstructuredTetrahedralMeshSampler();

	virtual bool getSample(double & sample, vtkIdType & cellId,Vec<3, double> globalPoint);
	bool getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint);
	virtual void getSmallestCellBoundingBox(double bounds[6]);
	virtual bool getCellBoundingBox(double bounds[6], Vector3 globalPoint);
	virtual void loadFile(const char * filePath);

	vtkUnstructuredGrid * getGrid(void)
	{
		return grid;
	}

protected:
	void initKDTree(void);
	void buildKDTree(Node * currentNode, vtkUnstructuredGrid * tetrahedralMesh);
	void updateNodeBounds(Node * node, Vec<3, double> cellMin, Vec<3, double> cellMax, bool first);
	void findCandidates(Node * currentNode, Vec<3, double> *point,vector<vtkIdType> *candidates);

	void computeLocalPoint(Vector3 globalPoint, Vector3 basis1, Vector3 basis2, Vector3 basis3, Vector3 basis4, Vector3 & localPoint);
	bool checkLocalPoint(Vector4 localPoint);

	void GetPoint(vtkPoints * points, vtkIdType id, Vec<3, double> & point);
	void computeSmallestCellBoundingBox(void);
	bool getSampleKDTree(double & sample, Vec<3, double> globalPoint);
	bool getSampleVTK(double & sample, vtkIdType & cellId, Vec<3, double> globalPoint);

	void cleanUp(void);

	bool getPotentialValuesAndLocalCoords(Vector4 & potentialValues, Vector3 & localCoords, vtkIdType & cellId, Vector3 globalPoint);
	bool getPotentialValuesAndLocalCoordsVTK(Vector4 & potentialValues, Vector3 & localCoords, vtkIdType & cellId, Vector3 globalPoint);

	void GetLocalTetrahedronShapeFunctionDerivatives(Mat<4,3,double> & values, double r, double s, double t);

protected:
	vtkUnstructuredGrid * grid;
	vtkUnstructuredGridReader * reader;
	Node rootNode;
	double smallestBounds[6];
};

#endif
