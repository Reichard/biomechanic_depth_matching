/*
 * ExternalTypesUtils.h
 *
 *  Created on: Jul 16, 2013
 *      Author: steverma
 */

#ifndef EXTERNALTYPESUTILS_H_
#define EXTERNALTYPESUTILS_H_

#include <cassert>

#include <sofa/component/linearsolver/CompressedRowSparseMatrix.h>
#include <sofa/component/linearsolver/FullVector.h>

#include <paralution.hpp>
//extern "C" {
//#include <petsc.h>
//}

namespace sofa {
namespace component {
namespace linearsolver {

using namespace paralution;

typedef enum {
	ARBITRARY, STRUCTURAL_SYMMETRIC, SYMMETRIC, SPD
} MatrixType;

class ExternalTypesUtils {
	public:
	template<class RealI, class RealO> static void copyCRSMatrix(
		CompressedRowSparseMatrix<RealI>& input,
		CompressedRowSparseMatrix<RealO>& output) {
		input.compress();
		output.resize(input.rowSize(), input.colSize());
		output.copyNonZeros(input);
		output.fullDiagonal();
		output.fullRows();
		output.compress();
	}

	

	template<class Real> static void initParalutionMatrix(
		CompressedRowSparseMatrix<Real>& input, LocalMatrix<Real>& output) {
		assert(input.getColsIndex().size() == input.getColsValue().size());
		output.Clear();
		output.AllocateCSR(input.Name(), input.getColsValue().size(),
			input.rowSize(), input.colSize());
		output.CopyFromCSR(&(input.getRowBegin()[0]),
			&(input.getColsIndex()[0]), &(input.getColsValue()[0]));
	}
	template<class Real> static void initParalutionVector(
		FullVector<Real>& input, LocalVector<Real>& output) {
		Real *data = input.ptr();
		output.SetDataPtr(&data, input.Name(), input.size());
	}
	template<class Real> static void destroyParalutionVector(LocalVector<Real>& input) {
		Real *temp;
		input.LeaveDataPtr(&temp);
	}

	private:
	ExternalTypesUtils();
	~ExternalTypesUtils();
};

} /* namespace linearsolver */
} /* namespace component */
} /* namespace sofa */
#endif /* EXTERNALTYPESUTILS_H_ */
