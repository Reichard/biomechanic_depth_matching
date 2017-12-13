/*
 * UnstructuredTetrahedralMeshSampler.cpp
 *
 *  Created on: Feb 19, 2013
 *      Author: milschew
 */

#include "UnstructuredTetrahedralMeshSampler.h"

#include "vtkCell.h"
#include "vtkPointData.h"

UnstructuredTetrahedralMeshSampler::UnstructuredTetrahedralMeshSampler(void)
{
	reader = NULL;
	grid = NULL;
}

UnstructuredTetrahedralMeshSampler::~UnstructuredTetrahedralMeshSampler()
{
	cleanUp();
}

void UnstructuredTetrahedralMeshSampler::cleanUp(void)
{
	if(reader != NULL)
	{
		reader->Delete();
		reader = NULL;
	}

	// TODO clean up tree nodes
}

void UnstructuredTetrahedralMeshSampler::loadFile(const char * filePath)
{
	cleanUp();

	reader = vtkUnstructuredGridReader::New();
	reader->SetFileName(filePath);
	reader->Update();
	grid = reader->GetOutput();

	// KD-Tree disabled!!!
//	initKDTree();

	computeSmallestCellBoundingBox();
}

void UnstructuredTetrahedralMeshSampler::initKDTree(void)
{
	double bounds[6];
	grid->GetBounds(bounds);

	rootNode.min[0] = bounds[0];
	rootNode.min[1] = bounds[2];
	rootNode.min[2] = bounds[4];

	rootNode.max[0] = bounds[1];
	rootNode.max[1] = bounds[3];
	rootNode.max[2] = bounds[5];

	for(int i = 0; i < grid->GetNumberOfCells(); i++)
	{
		rootNode.cells.push_back(i);
	}

	buildKDTree(&rootNode, grid);
}

bool UnstructuredTetrahedralMeshSampler::getCellBoundingBox(double bounds[6], Vector3 globalPoint)
{
	vtkCell * cell = NULL;
	int subId;
	double tempCoords[3];
	double weights[4];

	cell = grid->FindAndGetCell(globalPoint.ptr(), NULL, 0, 1e-6, subId, tempCoords, weights);
	if(cell == NULL)
	{
		bounds[0] = 0.0;
		bounds[1] = 0.0;
		bounds[2] = 0.0;
		bounds[3] = 0.0;
		bounds[4] = 0.0;
		bounds[5] = 0.0;

		return false;
	} else
	{
		cell->GetBounds(bounds);
		return true;
	}
}

bool UnstructuredTetrahedralMeshSampler::getSample(double & sample, vtkIdType & cellId, Vec<3, double> globalPoint)
{
	return getSampleVTK(sample, cellId, globalPoint);
}

bool UnstructuredTetrahedralMeshSampler::getPotentialValuesAndLocalCoords(Vector4 & potentialValues, Vector3 & localCoords, vtkIdType & cellId, Vector3 globalPoint)
{
	return getPotentialValuesAndLocalCoordsVTK(potentialValues, localCoords, cellId, globalPoint);
}

bool UnstructuredTetrahedralMeshSampler::getPotentialValuesAndLocalCoordsVTK(Vector4 & potentialValues, Vector3 & localCoords, vtkIdType & cellId, Vector3 globalPoint)
{
	vtkCell * cell = NULL;
	cellId = -1;
	int subId;
	double tempCoords[3];
	double weights[4];

	cellId = grid->FindCell(globalPoint.ptr(), NULL, 0, 1e-6, subId, tempCoords, weights);

	if(cellId == -1)
	{
		potentialValues.clear();
		localCoords.clear();
		return false;
	}

	cell = grid->GetCell(cellId);

	if(cell == NULL)
	{
		potentialValues.clear();
		localCoords.clear();
		return false;
	}

	Vector4 weightsVec(weights);
	vtkIdList *pointIdList = cell->GetPointIds();

	// r s t
	localCoords[0] = 1.0 - tempCoords[0] - tempCoords[1] - tempCoords[2];
	localCoords[1] = tempCoords[0];
	localCoords[2] = tempCoords[2];

	Vector4 localCoordsWeights(localCoords[0],
								localCoords[1],
								1.0 - localCoords[0] - localCoords[1] - localCoords[2],
								localCoords[2]);

	if(weightsVec != localCoordsWeights)
	{
		std::cout << "Weights do not match in getSample" << '\n';
		std::cout << "Weights: " << weightsVec << '\n';
		std::cout << "Local Coords Weights: " << localCoordsWeights << '\n';
	}

	potentialValues[0] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(0));
	potentialValues[1] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(1));
	potentialValues[2] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(2));
	potentialValues[3] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(3));

//	Vector3 point0, point1, point2, point3, reconstructedGlobalPoint;
//	GetPoint(cell->GetPoints(), 0, point0);
//	GetPoint(cell->GetPoints(), 1, point1);
//	GetPoint(cell->GetPoints(), 2, point2);
//	GetPoint(cell->GetPoints(), 3, point3);
//	reconstructedGlobalPoint = point0 * weightsVec[0]
//	                           + point1 * weightsVec[1]
//	                           + point2 * weightsVec[2]
//	                           + point3 * weightsVec[3];

	return true;
}

bool UnstructuredTetrahedralMeshSampler::getSampleVTK(double & sample, vtkIdType & cellId, Vec<3, double> globalPoint)
{
	Vector4 potentialValues;
	Vector3 localCoords;

	bool cellExists = getPotentialValuesAndLocalCoordsVTK(potentialValues, localCoords, cellId, globalPoint);
	if(!cellExists)
	{
		sample = 0;
		return false;
	}

	Vector4 localCoordsWeights(localCoords[0], localCoords[1],
								1.0 - localCoords[0] - localCoords[1] - localCoords[2],
								localCoords[2]);

	// shape functions are given by the local coordinates
	sample = potentialValues * localCoordsWeights;

	return true;
}

bool UnstructuredTetrahedralMeshSampler::getPotentialGradientFEM(Vector3 & potentialGradient, Vector3 globalPoint)
{
	bool cellExists = false;
	vtkIdType cellId = -1;
	Vector4 potentialValues;
	Vector3 localCoords;
	vtkCell * tetrahedronCell = NULL;
	Mat<4, 3, double> localTetrahedronShapeFunctionDerivatives;
	Mat<3, 3, double> invertedTetrahedronJacobian;
	Mat<4, 3, double> globalTetrahedronShapeFunctionDerivatives;

	cellExists = getPotentialValuesAndLocalCoords(potentialValues, localCoords, cellId, globalPoint);
	if(!cellExists)
	{
		potentialGradient.clear();
		return false;
	}

	tetrahedronCell = grid->GetCell(cellId);

	/*
	 * Formal computation of gradient [dPhi(x) / dx]
	 */
	Mat<3, 4, double> tetrahedronBasis;
	Mat<3, 3, double> tetrahedronJacobian;

	for(int i = 0; i < 4; i++)
	{
		double * point = grid->GetPoint(tetrahedronCell->GetPointIds()->GetId(i));
		tetrahedronBasis[0][i] = point[0];
		tetrahedronBasis[1][i] = point[1];
		tetrahedronBasis[2][i] = point[2];
	}

	GetLocalTetrahedronShapeFunctionDerivatives(localTetrahedronShapeFunctionDerivatives, localCoords[0], localCoords[1], localCoords[2]);
	tetrahedronJacobian = tetrahedronBasis * localTetrahedronShapeFunctionDerivatives;
	tetrahedronJacobian.transpose();
	invertMatrix(invertedTetrahedronJacobian, tetrahedronJacobian);
	globalTetrahedronShapeFunctionDerivatives = localTetrahedronShapeFunctionDerivatives * invertedTetrahedronJacobian.transposed();

	potentialGradient = globalTetrahedronShapeFunctionDerivatives.transposed() * potentialValues;

	return true;
}

bool UnstructuredTetrahedralMeshSampler::getSampleKDTree(double & sample, Vec<3, double> globalPoint)
{
	Vector3 localPointRST;
	bool cellFound = false;
	vector<vtkIdType> candidates;
	Vector3 basis1;
	Vector3 basis2;
	Vector3 basis3;
	Vector3 basis4;

	findCandidates(&rootNode, &globalPoint, &candidates);

	for(int candidateId = 0; candidateId < candidates.size(); candidateId++)
	{
		vtkIdType cellId = candidates.at(candidateId);

		vtkCell * cell = grid->GetCell(cellId);
		vtkPoints * gridPoints = cell->GetPoints();
		vtkIdList *pointIdList = cell->GetPointIds();

		GetPoint(gridPoints, 0, basis1);
		GetPoint(gridPoints, 1, basis2);
		GetPoint(gridPoints, 2, basis3);
		GetPoint(gridPoints, 3, basis4);

		computeLocalPoint(globalPoint, basis1, basis2, basis3, basis4, localPointRST);

		Vector4 localPointVec4(localPointRST[0], localPointRST[1], 1.0 - localPointRST[0] - localPointRST[1] - localPointRST[2], localPointRST[2]);

		if(checkLocalPoint(localPointVec4))
		{
			cellFound = true;

			Vector4 potentialValues;
			potentialValues[0] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(0));
			potentialValues[1] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(1));
			potentialValues[2] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(2));
			potentialValues[3] = grid->GetPointData()->GetScalars()->GetTuple1(pointIdList->GetId(3));

//			Vector3 reconstructedGlobalPoint = basis1 * localPointVec4[0]
//					                           + basis2 * localPointVec4[1]
//					                           + basis3 * localPointVec4[2]
//					                           + basis4 * localPointVec4[3];

			// shape functions are given by the local coordinates
			sample = potentialValues * localPointVec4;
			break;
		}
	}

	if(!cellFound)
	{
		sample = 0.0;
		return false;
	} else
	{
		return true;
	}
}

void UnstructuredTetrahedralMeshSampler::computeSmallestCellBoundingBox(void)
{
	double tempBoundingBox[6];
	double tempVolume = 0.0;

	double smallestVolume = 0.0;
	bool first = true;

	for(int i = 0; i < grid->GetNumberOfCells(); i++)
	{
		grid->GetCell(i)->GetBounds(tempBoundingBox);
		tempVolume = (tempBoundingBox[1] - tempBoundingBox[0])
		             * (tempBoundingBox[3] - tempBoundingBox[2])
			         * (tempBoundingBox[5] - tempBoundingBox[4]);

		if((tempVolume < smallestVolume)||(first))
		{
			smallestVolume = tempVolume;

			smallestBounds[0] = tempBoundingBox[0];
			smallestBounds[1] = tempBoundingBox[1];
			smallestBounds[2] = tempBoundingBox[2];
			smallestBounds[3] = tempBoundingBox[3];
			smallestBounds[4] = tempBoundingBox[4];
			smallestBounds[5] = tempBoundingBox[5];
		}

		if(first)
		{
			first = false;
		}
	}
}

void UnstructuredTetrahedralMeshSampler::getSmallestCellBoundingBox(double bounds[6])
{
	for(int i = 0; i < 6; i++)
	{
		bounds[i] = smallestBounds[i];
	}
}

void UnstructuredTetrahedralMeshSampler::GetLocalTetrahedronShapeFunctionDerivatives(Mat<4,3,double> & values, double r, double s, double t)
{
	// dN1/dr, dN1/ds, dN1/dt
	values[0][0] = 1;
	values[0][1] = 0;
	values[0][2] = 0;

	// dN2/dr, dN2/ds, dN2/dt
	values[1][0] = 0;
	values[1][1] = 1;
	values[1][2] = 0;

	values[2][0] = -1;
	values[2][1] = -1;
	values[2][2] = -1;

	values[3][0] = 0;
	values[3][1] = 0;
	values[3][2] = 1;
}

void UnstructuredTetrahedralMeshSampler::GetPoint(vtkPoints * points, vtkIdType id, Vec<3, double> & point)
{
	double p[3];
	points->GetPoint(id, p);

	point[0] = p[0];
	point[1] = p[1];
	point[2] = p[2];
}

bool UnstructuredTetrahedralMeshSampler::checkLocalPoint(Vector4 localPointVec4)
{
	for(unsigned int i = 0; i < localPointVec4.size(); i++)
	{
		if((localPointVec4[i] < -0.0001) || (localPointVec4[i] > 1.0001))
		{
			return false;
		}
	}

	return true;
}

void UnstructuredTetrahedralMeshSampler::computeLocalPoint(Vector3 globalPoint, Vector3 basis1, Vector3 basis2, Vector3 basis3, Vector3 basis4, Vector3 & localPoint)
{
	Matrix3 matrix;
	Matrix3 invertedMatrix;

	matrix[0][0] = basis1[0] - basis3[0];
	matrix[0][1] = basis2[0] - basis3[0];
	matrix[0][2] = basis4[0] - basis3[0];

	matrix[1][0] = basis1[1] - basis3[1];
	matrix[1][1] = basis2[1] - basis3[1];
	matrix[1][2] = basis4[1] - basis3[1];

	matrix[2][0] = basis1[2] - basis3[2];
	matrix[2][1] = basis2[2] - basis3[2];
	matrix[2][2] = basis4[2] - basis3[2];

	invertMatrix(invertedMatrix, matrix);
	localPoint = invertedMatrix * (globalPoint - basis3);
}

void UnstructuredTetrahedralMeshSampler::findCandidates(Node * currentNode, Vec<3, double> * point, vector<vtkIdType> *candidates)
{
	bool isInBounds = true;

	for(int i = 0; i < 3; i++)
	{
		if((*point)[i] < currentNode->min[i])
		{
			isInBounds = false;
			break;
		}

		if((*point)[i] > currentNode->max[i])
		{
			isInBounds = false;
			break;
		}
	}

	if(!isInBounds)
	{
		return;
	}

	if((currentNode->child1 == NULL) && (currentNode->child2 == NULL))
	{
		for(int j = 0; j < currentNode->cells.size(); j++)
		{
			candidates->push_back(currentNode->cells.at(j));
		}
		return;
	}

	if(currentNode->child1 != NULL)
	{
		findCandidates(currentNode->child1, point, candidates);
	}

	if(currentNode->child2 != NULL)
	{
		findCandidates(currentNode->child2, point, candidates);
	}
}

void UnstructuredTetrahedralMeshSampler::buildKDTree(Node * currentNode, vtkUnstructuredGrid * tetrahedralMesh)
{
	double cellBounds[6];
	Vec<3, double> delta;
	int largestComponent = 0;
	double largestComponentDelta = 0.0;
	bool firstLeft = true;
	bool firstRight = true;

	if(currentNode->cells.size() <= 5)
	{
		currentNode->child1 = NULL;
		currentNode->child2 = NULL;
		return;
	}

	Node * leftChild = new Node();
	Node * rightChild = new Node();

	leftChild->min.clear();
	rightChild->max.clear();

	delta = currentNode->max - currentNode->min;
	for(int j = 0;j < delta.size();j++)
	{
		if(delta[j] > largestComponentDelta)
		{
			largestComponent = j;
			largestComponentDelta = delta[j];
		}
	}

	double splittingPlane = currentNode->min[largestComponent] + 0.5 * largestComponentDelta;

	for(int i = 0; i < currentNode->cells.size(); i++)
	{
		vtkIdType cellId = currentNode->cells.at(i);
		vtkCell * cell = tetrahedralMesh->GetCell(cellId);

		cell->GetBounds(cellBounds);
		Vec<3, double> cellMin(cellBounds[0], cellBounds[2], cellBounds[4]);
		Vec<3, double> cellMax(cellBounds[1], cellBounds[3], cellBounds[5]);
		Vec<3, double> boundsCenter = (cellMin + cellMax) * 0.5;

		if(boundsCenter[largestComponent] < splittingPlane)
		{
			leftChild->cells.push_back(cellId);
			updateNodeBounds(leftChild, cellMin, cellMax, firstLeft);
			firstLeft = false;
		}
		else
		{
			// rightChild
			rightChild->cells.push_back(cellId);
			updateNodeBounds(rightChild, cellMin, cellMax, firstRight);
			firstRight = false;
		}
	}

	if((leftChild->cells.size() == 0) || (rightChild->cells.size() == 0))
	{
		delete leftChild;
		delete rightChild;

		currentNode->child1 = NULL;
		currentNode->child2 = NULL;
		return;
	}

	currentNode->child1 = leftChild;
	currentNode->child2 = rightChild;

//	std::cout << "size leftChild: " << leftChild->cells.size() << " size rightChild: " << rightChild->cells.size() << '\n';

	buildKDTree(leftChild, tetrahedralMesh);
	buildKDTree(rightChild, tetrahedralMesh);
}

void UnstructuredTetrahedralMeshSampler::updateNodeBounds(Node * node, Vec<3, double> cellMin, Vec<3, double> cellMax, bool first)
{
	for(int k = 0; k < 3; k++)
	{
		if(first)
		{
			node->min[k] = cellMin[k];
			node->max[k] = cellMax[k];
			continue;
		}

		if(cellMin[k] < node->min[k])
		{
			node->min[k] = cellMin[k];
		}

		if(cellMax[k] > node->max[k])
		{
			node->max[k] = cellMax[k];
		}
	}
}
