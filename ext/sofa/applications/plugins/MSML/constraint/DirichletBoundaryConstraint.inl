/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This library is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This library is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this library; if not, write to the Free Software Foundation,     *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.          *
*******************************************************************************
*                               SOFA :: Modules                               *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#ifndef SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_DirichletBoundaryConstraint_INL
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_DirichletBoundaryConstraint_INL
#include "DirichletBoundaryConstraint.h"

#include <sofa/simulation/common/MechanicalVisitor.h>
#include <sofa/simulation/common/MechanicalOperations.h>
#include <sofa/simulation/common/VectorOperations.h>

#include <sofa/component/topology/TopologyData.inl>
#include <sofa/core/behavior/MechanicalState.h>



namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{
using component::projectiveconstraintset::DirichletBoundaryConstraint;

//template <class DataTypes>
//	DirichletBoundaryConstraint<DataTypes>::DirichletBoundaryConstraint()
//	:FixedConstraint<DataTypes>()
//	, InternalData(new DirichletBoundaryConstraintInternalData<DataTypes>)
//	, _displacement( initData(&_displacement,Coord(),"displacement","The imposed displacement") )
//	, _box( initData( &_box, "box", "DOFs in the box defined by xmin,ymin,zmin, xmax,ymax,zmax are fixed") )
//	, _step( initData( &_step, (Real)1, "step", "Distance at each time step") )
//	{
//
//	}

//template <class DataTypes>
//	void DirichletBoundaryConstraint<DataTypes>::PointHandler::applyCreateFunction(unsigned int pointId, PointInformation &t, const sofa::core::topology::Topology::Point & point,
//            const sofa::helper::vector<unsigned int> & ancestorIds,
//            const sofa::helper::vector<double> & coeffs )
//{
//
//	if (dbc) {
//
//		if(!ancestorIds.size())
//		{
//			//in this case add new index to indexMap
//			helper::vector<unsigned int> tempIndices;
//			tempIndices.push_back(pointId);
//			dbc->m_IndicesMap[pointId] = tempIndices;//.insert( pair<unsigned int, helper::vector<unsigned int> > (pointId,tempIndices) );
//			t.ancestorId = pointId;
//			//std::cout<<"Add "<<pointId <<" to ancestor "<<pointId <<"\n";
//		}
//
//		else
//		{
//			unsigned int ancestorId;
//			for(int i=0;i<4;i++)
//			{
//				if(coeffs[i]==1.0)
//				{
//					ancestorId = ancestorIds[i];
//				}
//			}
//			helper::vector<unsigned int> tempIndices = dbc->m_IndicesMap[ancestorId];
//			tempIndices.push_back(pointId);
//			dbc->m_IndicesMap[ancestorId] = tempIndices;
//			t.ancestorId = ancestorId;
//			//std::cout<<"Add "<<pointId <<" to ancestor "<<ancestorId <<"\n";
//		}
//	}
//
//
//}
//
//
//
//template <class DataTypes>
//	void DirichletBoundaryConstraint<DataTypes>::PointHandler::applyDestroyFunction(unsigned int pointId, PointInformation& p)
//{
//
//
//	int ancestorId = p.ancestorId;
//	helper::vector<unsigned int> tempIndices = dbc->m_IndicesMap[ancestorId];
//	//find the current Id
//	int size = tempIndices.size();
//	for(int i=0;i<size;i++)
//	{
//		if(tempIndices[i]==pointId)
//		{
//			tempIndices.erase(tempIndices.begin()+i);
//			break;
//		}
//	}
//
//	//std::cout<<"Delete "<<pointId<<" with ancestor"<<ancestorId<<"\n";
//
//
//	dbc->m_IndicesMap[ancestorId] = tempIndices;
//
//
//
//
//}
//
//template <class DataTypes>
//	void DirichletBoundaryConstraint<DataTypes>::PointHandler::swap( unsigned int i1, unsigned int i2 )
//{
//	//swap from i2 to i1 (from last to index[i]
//	helper::vector<typename DirichletBoundaryConstraint<DataTypes>::PointInformation>& pointInf = *(dbc->m_PointInfos.beginEdit());
//	//helper::vector<typename DGLinearTetrahedralCorotationalFEMForceField<DataTypes>::TetrahedronInformation>& tetrahedronInf = *(this->m_LinearTetrahedronInfos.beginEdit());
//	int ancestorId = pointInf[i2].ancestorId;
//
//	//std::cout<<"Move "<<i2 <<" to "<<i1<<"\n";
//
//
//	helper::vector<unsigned int> tempIndices = dbc->m_IndicesMap[ancestorId];
//
//	//find the current Id
//	int size = tempIndices.size();
//	for(int i=0;i<size;i++)
//	{
//		if(tempIndices[i]==i2)
//		{
//			tempIndices[i] = i1;
//			break;
//		}
//	}
//
//	dbc->m_IndicesMap[ancestorId] = tempIndices;
//
//	dbc->m_PointInfos.endEdit();
//
//}

template <class DataTypes>
	DirichletBoundaryConstraint<DataTypes>::DirichletBoundaryConstraint()
	:FixedConstraint<DataTypes>()
	// , m_PointInfos(core::objectmodel::BaseObject::initData(&m_PointInfos, "pointInfo", "Internal point data"))
    , InternalData(new DirichletBoundaryConstraintInternalData<DataTypes>)
//	, _displacement( initData(&_displacement,Coord(),"displacement","The imposed displacement") )
//	, _box( initData( &_box, "box", "DOFs in the box defined by xmin,ymin,zmin, xmax,ymax,zmax are fixed") )
//	, _step( initData( &_step, (Real)1, "step", "Distance at each time step") )
//	, m_PointHandler(NULL)
	{
			//m_PointHandler = new PointHandler(this,&m_PointInfos);

			Coord dispValue= Coord();//0,0,0);
			_displacementValues.push_back(dispValue);
			//_displacementValues;
			m_PositionsValid = false;
			m_IndicesValid = false;

//		init();
	}


template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::init()
	{
		FixedConstraint<DataTypes>::init();

		//m_Topology = (BaseMeshTopology*)this->getContext()->getMeshTopology();

		//x0.resize(0);

//		if (!mstate.get())
//				mstate.set(dynamic_cast< MechanicalState<DataTypes>* >(getContext()->getMechanicalState()));

		/*const Vec6& vb=_box.getValue();
		if( vb != Vec6() )
		{
			this->removeConstraint( 0 );
			vector <unsigned> indices;

			this->mstate->getIndicesInSpace( indices, vb[0],vb[3],vb[1],vb[4],vb[2],vb[5] );
			for(unsigned int i = 0; i < indices.size(); i++)
			{
				this->addConstraint(indices[i]);
				_finalPos.push_back( (*this->mstate->getX())[indices[i]]+_displacement.getValue() );
			}
		}*/

		this->reinit();
	}

template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::reinit()
	{
		//FixedConstraint<DataTypes>::reinit();
//		unsigned int nNodes = m_Topology->getNbPoints();
//
//		helper::vector<typename DirichletBoundaryConstraint<DataTypes>::PointInformation>& pointInf = *(this->m_PointInfos.beginEdit());
//
//
//		pointInf.resize(nNodes);
//		sofa::component::projectiveconstraintset::DirichletBoundaryConstraint<DataTypes>::PointInformation tempPointInformation;
//		std::vector< unsigned int > tempAncestors;
//
//		for (int i=0;i<nNodes;i++) {
//				m_PointHandler->applyCreateFunction(i, pointInf[i],
//				(const unsigned int)0,  tempAncestors,
//				(const std::vector< double >)0);
//		}
//
//		int test = this->m_IndicesMap.size();
//
//		this->m_PointInfos.createTopologicalEngine(m_Topology,m_PointHandler);
//		this->m_PointInfos.registerTopologicalData();
//
//		this->m_PointInfos.endEdit();
	}


template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::addDisplacementBC(unsigned int index, Coord displacementValue)
	{
		//get the indices from map
//		int test = this->m_IndicesMap.size();
//
//		helper::vector<unsigned int> tempIndices = this->m_IndicesMap[index];
//		int tempIndicesSize = tempIndices.size();
//
//		int currentIndex;
//		for (int i=0;i<tempIndicesSize;i++) {
//				currentIndex = tempIndices[i];
//
//				FixedConstraint<DataTypes>::addConstraint(currentIndex);
//				_displacementValues.push_back( displacementValue );
//		}

//		FixedConstraint<DataTypes>::addConstraint(index);
//		_displacementValues.push_back( displacementValue );

//	m_PositionsValid = false;
//	m_IndicesValid = false;


	}


template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::addMarkerDisplacementBC(unsigned int index, Coord displacementValue)
	{


			FixedConstraint<DataTypes>::addConstraint(index);
			_displacementValues.push_back( displacementValue );
			m_PositionsValid = false;
			m_IndicesValid = false;
	}

template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::clearDisplacementBCs()
	{
		FixedConstraint<DataTypes>::clearConstraints();
		_displacementValues.clear();
		//const SetIndexArray & indices = this->f_indices.getValue().getArray();
			//		int theSize = indices.size();
		m_PositionsValid = false;
		m_IndicesValid = false;

	}


template <class DataTypes>
	void DirichletBoundaryConstraint<DataTypes>::projectPosition(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecCoord& resData)
	{

	if(!m_IndicesValid)
			updateIndices();

	if(!m_PositionsValid)
			updatePositions(mparams);


		helper::WriteAccessor<DataVecDeriv> res ( mparams, resData );
		int indicesSize = this->m_ValidIndices.size();

	    for(int i=0; i<indicesSize; i++)
		{
			res[this->m_ValidIndices[i]] = this->m_ValidPositions[i];// = (*X0)[this->m_ValidIndices[i]]+_displacementValues[i];
		}

	}

template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::updatePositions(const core::MechanicalParams* mparams )
{
	if(!m_IndicesValid)
		updateIndices();

	//try to get x0
	//get the mechanicalconatiner from context
	MechanicalState<DataTypes>* mstate;
	mstate = dynamic_cast< MechanicalState<DataTypes>* >(this->getContext()->getMechanicalState());

	const VecCoord *X0=mstate->getX0();

	int indicesSize = this->m_ValidIndices.size();
	this->m_ValidPositions.resize(indicesSize );

	for(int i=0; i<indicesSize; i++)
	{
		this->m_ValidPositions[i] = (*X0)[this->m_ValidIndices[i]]+_displacementValues[i];
	}

	m_PositionsValid = true;
}


template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::updateIndices()
{
	const SetIndexArray & indices = this->f_indices.getValue();
	int indicesSize = indices.size();

	this->m_ValidIndices.clear();
	this->m_ValidIndices.resize(indicesSize);

	int i=0;
	for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it,++i)
	{
		this->m_ValidIndices[i] = *it;
	}

	m_IndicesValid = true;

}

template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::projectJacobianMatrix(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataMatrixDeriv& cData)
{
//    helper::WriteAccessor<DataMatrixDeriv> c ( mparams, cData );
//    const SetIndexArray & indices = this->f_indices.getValue(mparams);
//
//    MatrixDerivRowIterator rowIt = c->begin();
//    MatrixDerivRowIterator rowItEnd = c->end();
//
//
//	while (rowIt != rowItEnd)
//	{
//		for (SetIndexArray::const_iterator it = indices.begin();
//			 it != indices.end();
//			 ++it)
//		{
//			rowIt.row().erase(*it);
//		}
//		++rowIt;
//	}

}

// projectVelocity applies the same changes on velocity vector as projectResponse on position vector :
// Each fixed point received a null velocity vector.
// When a new fixed point is added while its velocity vector is already null, projectVelocity is not usefull.
// But when a new fixed point is added while its velocity vector is not null, it's necessary to fix it to null. If not, the fixed point is going to drift.
template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::projectVelocity(const core::MechanicalParams* /*mparams*/ /* PARAMS FIRST */, DataVecDeriv& /*vData*/)
{
//#if 0 /// @TODO ADD A FLAG FOR THIS
//    const SetIndexArray & indices = f_indices.getValue().getArray();
//  //serr<<"FixedConstraint<DataTypes>::projectVelocity, res.size()="<<res.size()<<sendl;
//  	if( f_fixAll.getValue()==true ) {  // fix everyting
//		for( unsigned i=0; i<res.size(); i++ )
//			res[i] = Deriv();
//	}
//	else {
//    for (SetIndexArray::const_iterator it = indices.begin();
//        it != indices.end();
//        ++it)
//    {
//        res[*it] = Deriv();
//    }
//	}
//#endif
}

//template <class DataTypes>
//void DirichletBoundaryConstraint<DataTypes>::projectPosition(const core::MechanicalParams* /*mparams*/ /* PARAMS FIRST */, DataVecCoord& /*xData*/)
//{
//	//
//}

// Matrix Integration interface
template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::applyConstraint(defaulttype::BaseMatrix *mat, unsigned int offset)
{
	if(!m_IndicesValid)
				updateIndices();

	//std::cout<<"Apply constraint to matrix, No of constraints: "<<this->m_ValidIndices.size();

    //sout << "applyConstraint in Matrix with offset = " << offset << sendl;
    const unsigned int N = Deriv::size();
//    const SetIndexArray & indices = this->f_indices.getValue();
//
//    for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
//    {
//        // Reset Fixed Row and Col
//        for (unsigned int c=0;c<N;++c)
//            mat->clearRowCol(offset + N * (*it) + c);
//        	//mat->clearRow(offset + N * (*it) + c);
//        // Set Fixed Vertex
//        for (unsigned int c=0;c<N;++c)
//            mat->set(offset + N * (*it) + c, offset + N * (*it) + c, 1.0);
//    }
//
    for(int i=0; i<this->m_ValidIndices.size(); i++)
	{
    	// Reset Fixed Row and Col
		for (unsigned int c=0;c<N;++c)
			mat->clearRowCol(offset + N * this->m_ValidIndices[i] + c);
			//mat->clearRow(offset + N * (*it) + c);
		// Set Fixed Vertex
		for (unsigned int c=0;c<N;++c)
			mat->set(offset + N * this->m_ValidIndices[i] + c, offset + N * this->m_ValidIndices[i] + c, 1.0);
	}

}

template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::applyConstraint(defaulttype::BaseVector *vect, unsigned int offset)
{
	if(!m_IndicesValid)
				updateIndices();

	std::cout<<"Apply constraint to vector, No of constraints: "<<this->m_ValidIndices.size();

    //sout << "applyConstraint in Vector with offset = " << offset << sendl;
    const unsigned int N = Deriv::size();

//    const SetIndexArray & indices = this->f_indices.getValue();
//    for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
//    {
//        for (unsigned int c=0;c<N;++c)
//            vect->clear(offset + N * (*it) + c);
//    }

    for(int i=0; i<this->m_ValidIndices.size(); i++)
	{
    	for (unsigned int c=0;c<N;++c)
    	     vect->clear(offset + N * this->m_ValidIndices[i] + c);
	}
}

template <class DataTypes>
void DirichletBoundaryConstraint<DataTypes>::projectResponse(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecDeriv& resData)
{
	if(!m_IndicesValid)
				updateIndices();

	helper::WriteAccessor<DataVecDeriv> res ( mparams, resData );


	for(int i=0; i<this->m_ValidIndices.size(); i++)
	{
		res[this->m_ValidIndices[i]] = Deriv();
	}

}
//	  helper::WriteAccessor<DataVecDeriv> res ( mparams, resData );
//
//	//initialize initial Dofs positions, if it's not done
//			if (x0.size() == 0)
//			{
//				const SetIndexArray & indices = this->f_indices.getValue().getArray();
//				int vectorSize = this->f_indices.getValue().getArray().size();
//				x0.resize(vectorSize); //???????????
//				int i=0;
//				for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it,++i)
//				{
//					//Deriv test;
//					//test = Deriv(0.5,0.0,0.0);
//					//res[*it] += test;
//					x0[i] = res[*it];//+test;
//					//i++;
//				}
//			}
//
//    sofa::simulation::common::VectorOperations vop( mparams, this->getContext() );
//    sofa::simulation::common::MechanicalOperations mop( mparams, this->getContext() );
//    MultiVecCoord posData(&vop, core::VecCoordId::position() );
//    MultiVecDeriv velData(&vop, core::VecDerivId::velocity() );
//   // posData.
////    helper::WriteAccessor<DataVecDeriv> pos(mparams,&posData);
////    helper::WriteAccessor<DataVecDeriv> vel(mparams,&velData);
//
//	double h = this->getContext()->getDt();
//
//
////
//	const Data<VecDeriv>* v = this->mstate->read(sofa::core::ConstVecDerivId::velocity());
//	VecDeriv vel = v->getValue();
//
//	const Data<VecCoord>* p = this->mstate->read(sofa::core::VecCoordId::position());
//		VecCoord pos = p->getValue();
////
////
////
//
//    const SetIndexArray & indices = this->f_indices.getValue(mparams).getArray();
//    int i=0;
//    for (SetIndexArray::const_iterator it = indices.begin();
//             it != indices.end();
//             ++it,++i)
//    {
//    	Coord test1 = x0[i]+_displacementValues[i];
//    	Coord test2 = pos[*it];
//    	Coord test3 =  vel[*it];
//    	//res[*it] = Deriv();//(test1-test2)/h-test3;
//    	//res[*it] = (x0[i]+_displacementValues[i]-pos[*it])/h-vel[*it];
//    }
//	double test =1;


//}

} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa

#endif
