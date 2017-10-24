///******************************************************************************
//*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
//*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
//*                                                                             *
//* This library is free software; you can redistribute it and/or modify it     *
//* under the terms of the GNU Lesser General Public License as published by    *
//* the Free Software Foundation; either version 2.1 of the License, or (at     *
//* your option) any later version.                                             *
//*                                                                             *
//* This library is distributed in the hope that it will be useful, but WITHOUT *
//* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
//* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
//* for more details.                                                           *
//*                                                                             *
//* You should have received a copy of the GNU Lesser General Public License    *
//* along with this library; if not, write to the Free Software Foundation,     *
//* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
//*******************************************************************************
//*                               SOFA :: Modules                               *
//*                                                                             *
//* Authors: The SOFA Team and external contributors (see Authors.txt)          *
//*                                                                             *
//* Contact information: contact@sofa-framework.org                             *
//******************************************************************************/
//// Author: Hadrien Courtecuisse
////
// Copyright: See COPYING file that comes with this distribution
#include "MumpsSolver.h"
//#include "ExternalTypesUtils.h"

#include <sofa/core/ObjectFactory.h>
#include <sofa/helper/AdvancedTimer.h>

namespace sofa {

namespace component {

namespace linearsolver {

using std::cerr;
using std::endl;


template<class TMatrix, class TVector>
MumpsSolver<TMatrix,TVector>::MumpsSolver()
:
  f_ordering( initData(&f_ordering,2,"ordering","ose ordering 0=data->identity/1=tree/2=metis/3=natural/4=genmmd/5=md/6=mmd/7=amd") )
, f_dropTol( initData(&f_dropTol,(double) 0.0,"ic_dropTol","Drop tolerance use for incomplete factorization") )
, f_modified_flag( initData(&f_modified_flag,true,"ic_modifiedFlag","Modified ICC : maintains rowsums") )
,f_symmetric(initData(&f_symmetric, 1, "symmetric", "0 = nonsymmetric arbitrary matrix, 1 = symmetric positive definite, 2 = structurally symmetric matrix"))
{

	
}

template<class TMatrix, class TVector>
void MumpsSolver<TMatrix,TVector>::invert(Matrix& M) {
    //M.compress();

    MumpsSolverInvertData * data = (MumpsSolverInvertData *) this->getMatrixInvertData(&M);
    
    //1. set MUMPS options
	//initialization
	data->id.job = -1; 
	data->id.par = 1; 
	//matrix is not symmetrical positive definite
	data->id.sym = f_symmetric.getValue();
	data->id.comm_fortran = 1;			
	dmumps_c(&data->id);			 
	//streams    
	  data->id.icntl[0] = 0; //6; 
	  data->id.icntl[1] = 0; //6; 
	  data->id.icntl[2] = 0; //6; 
	  data->id.icntl[3] = 1; //3;
	//ordering metis (5), or pord (4), or AMD (0), AMF (2), QAMD (6)	
	  data->id.icntl[6] = 5;
	  
	  //2. convert matrix to mumps representation
	  
	  
	data->Mfiltered.clear();	
	
	if ( (f_symmetric.getValue() == 2) || (f_symmetric.getValue() == 1) ) {
		data->Mfiltered.copyUpperNonZeros(M);
		data->Mfiltered.fullDiagonal();
		//sout << "Filtered upper part of M, nnz = "
		//	<< data->Mfiltered.getRowBegin().back() << sendl;
	} else {
		data->Mfiltered.copyNonZeros(M);
		data->Mfiltered.fullRows();
		data->Mfiltered.fullDiagonal();
		//sout << "Filtered M, nnz = " << data->Mfiltered.getRowBegin().back()
		//	<< sendl;
		//td::cout << "Non-symmetric, arbitrary matrix" << std::endl;
	}
			
	//data->Mfiltered.resize(M.rowSize(), M.colSize());
	//data->Mfiltered.copyNonZeros(M);
	//data->Mfiltered.fullDiagonal();
	//data->Mfiltered.fullRows();	
	data->Mfiltered.shiftIndices(1);
	
	//output.compress();
    //ExternalTypesUtils::copyCRSMatrix(M, data->Mfiltered); //check for symmetry?
    

    
    int n= data->Mfiltered.rowSize();
    int numberOfEntries = data->Mfiltered.getColsValue().size();
    
    //decompress row indices
    helper::vector<int> rowIndices;
    //rowIndices.resize(numberOfEntries);
    int currentRowIndex = 1;//data->Mfiltered.getRowIndex()[0];
    //std::cout<<"First row index is "<<currentRowIndex<<"\n";
    //std::cout<<"RowSize is "<<n<<", number of entries on compressed rowIndices "<<data->Mfiltered.getRowBegin().size()<<"\n";
    
    for(unsigned int i=1; i<data->Mfiltered.getRowBegin().size(); i++) // for fortran compatibility
    {
		int nnzRow =  data->Mfiltered.getRowBegin()[i]-data->Mfiltered.getRowBegin()[i-1];
		
		for(unsigned int j=0;j<nnzRow; j++)
		{
			rowIndices.push_back(i);
		}
		
	
		
		
	}
	
	//std::cout<<"Number of entries: "<<data->Mfiltered.getColsValue().size()<<", no of row indices"<<rowIndices.size()<<", no of col indices"<<data->Mfiltered.getColsIndex().size()<<"\n";
	
	
	
	data->id.n = data->Mfiltered.rowSize();
	data->id.nz = data->Mfiltered.getColsValue().size();
	
	//array of row indices
	data->id.irn = (int *) &(rowIndices[0]);
	//array of col indices
	data->id.jcn = (int *) &(data->Mfiltered.getColsIndex()[0]);
	//array of values
	data->id.a = (Real*) &(data->Mfiltered.getColsValue()[0]);
	
	//std::cout<<"Number of entries: "<<data->Mfiltered.getColsValue().size()<<", no of row indices"<<rowIndices.size()<<", no of col indices"<<data->Mfiltered.getColsIndex().size()<<"\n";
	
	
	
	// 3) LU factorization	
	data->id.job = 1;
	dmumps_c(&data->id);	
	data->id.job = 2;
	dmumps_c(&data->id);
		
	
}

template<class TMatrix, class TVector>
void MumpsSolver<TMatrix,TVector>::solve (Matrix& M, Vector& z, Vector& r) {
    MumpsSolverInvertData * data = (MumpsSolverInvertData *) this->getMatrixInvertData(&M);
       
    // 4) Reading RHS and setting up MUMPS
	//fill solution vector with rhs
	z.clear();
	for(unsigned int i=0; i<r.size();i++)
	{
		z[i] = r[i];
	}
	
	Real *rhsptr = z.ptr();
	data->id.rhs = rhsptr;
	data->id.nrhs = 1;
	data->id.lrhs = data->id.n;
	
	// 5) Back substitution	
	data->id.job = 3;
	dmumps_c(&data->id);
	
	    
    
    
}

template class sofa::component::linearsolver::MumpsSolver< sofa::component::linearsolver::CompressedRowSparseMatrix<double>,sofa::component::linearsolver::FullVector<double> >;
//template class sofa::component::linearsolver::MumpsSolver< sofa::component::linearsolver::CompressedRowSparseMatrix<float>,sofa::component::linearsolver::FullVector<float> >;

SOFA_DECL_CLASS(MumpsSolver)

int MumpsSolverClass = core::RegisterObject("Awesome solvers from the Mumps library")
.add< MumpsSolver< CompressedRowSparseMatrix<double>,FullVector<double> > >(true)
//.add< MumpsSolver< CompressedRowSparseMatrix<float>,FullVector<float> > >(false)
//.add< MumpsSolver< CompressedRowSparseMatrix<defaulttype::Mat<3,3,double> >,FullVector<double> > >(true)
;

} // namespace linearsolver

} // namespace component

} // namespace sofa

