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
#ifndef SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_DirichletBoundaryConstraint_H
#define SOFA_COMPONENT_PROJECTIVECONSTRAINTSET_DirichletBoundaryConstraint_H


#include <sofa/component/projectiveconstraintset/FixedConstraint.h>
#include <sofa/component/projectiveconstraintset/FixedConstraint.inl>
#include <sofa/helper/vector.h>

#include <sofa/component/topology/TopologyData.h>

#include <sofa/helper/fixed_array.h>
#include <sofa/helper/gl/template.h>

#include <map>


namespace sofa
{

namespace component
{

namespace projectiveconstraintset
{


using helper::vector;
using core::objectmodel::Data;
using namespace sofa::core::objectmodel;
using namespace std;

template< class DataTypes >
class DirichletBoundaryConstraintInternalData
{
};

/*
 *  Impose a displacement onto particles
 */
template <class DataTypes>
class DirichletBoundaryConstraint : public FixedConstraint<DataTypes>
{
public:
	SOFA_CLASS(SOFA_TEMPLATE(DirichletBoundaryConstraint,DataTypes),SOFA_TEMPLATE(FixedConstraint, DataTypes));

   typedef typename DataTypes::VecCoord VecCoord;
   typedef typename DataTypes::VecDeriv VecDeriv;
	typedef typename DataTypes::Coord Coord;
	typedef typename DataTypes::Deriv Deriv;
	typedef typename DataTypes::Real Real;
	//typedef topology::PointSubsetT SetIndex;
	typedef helper::vector<unsigned int> SetIndexArray;
	typedef defaulttype::Vec<6,Real> Vec6;


    typedef typename DataTypes::MatrixDeriv MatrixDeriv;
    typedef typename MatrixDeriv::RowIterator MatrixDerivRowIterator;
    typedef typename MatrixDeriv::RowType MatrixDerivRowType;
    typedef Data<VecCoord> DataVecCoord;
    typedef Data<VecDeriv> DataVecDeriv;
   typedef Data<MatrixDeriv> DataMatrixDeriv;

protected:
    DirichletBoundaryConstraintInternalData<DataTypes> *InternalData;
    friend class DirichletBoundaryConstraintInternalData<DataTypes>;

protected:
//	helper::vector<Coord> _displacementValues;



	VecCoord _displacementValues;

	VecCoord x0;
	
	bool m_PositionsValid;
	bool m_IndicesValid;

	VecCoord m_ValidPositions;
	helper::vector<int > m_ValidIndices;


	map<unsigned int,helper::vector<unsigned int> > m_IndicesMap;

	void updatePositions(const core::MechanicalParams* mparams );
	void updateIndices( );

public:
	// Used for initialization (constructor)
//	Data<Coord> _displacement;
//	Data<Vec6> _box;
//	Data<Real> _step;
//
//	class PointInformation
//	{
//	public:
//		unsigned int ancestorId;
//
//
//		PointInformation() {
//		}
//
//		/// Output stream
//		inline friend std::ostream& operator<< ( std::ostream& os, const PointInformation& /*tri*/ )
//		{
//			return os;
//		}
//
//		/// Input stream
//		inline friend std::istream& operator>> ( std::istream& in, PointInformation& /*tri*/ )
//		{
//			return in;
//		}
//	};


//    class PointHandler : public TopologyDataHandler<sofa::core::topology::Topology::Point , sofa::helper::vector<PointInformation> >
//    {
//    public :
//      typedef typename DirichletBoundaryConstraint<DataTypes>::PointInformation PointInformation;
//      PointHandler(DirichletBoundaryConstraint<DataTypes>* dbc,
//                         PointData<sofa::helper::vector<PointInformation> >* data)
//        :TopologyDataHandler<sofa::core::topology::Topology::Point, sofa::helper::vector<PointInformation> >(data)
//        ,dbc(dbc)
//      {
//
//      }
//
//      void applyCreateFunction(unsigned int, PointInformation &t, const sofa::core::topology::Topology::Point &,
//                               const sofa::helper::vector<unsigned int> &,
//                               const sofa::helper::vector<double> &);
//
//
//      void applyDestroyFunction(unsigned int, PointInformation& );
//
//      void swap( unsigned int i1, unsigned int i2 );
//
//    protected:
//      DirichletBoundaryConstraint<DataTypes>* dbc;
//
//    };






//	void projectPosition(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecDeriv& resData);
	void projectVelocity(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecDeriv& vData);
	void projectPosition(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecCoord& xData);
	void projectJacobianMatrix(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataMatrixDeriv& cData);

	void projectResponse(const core::MechanicalParams* mparams /* PARAMS FIRST */, DataVecDeriv& resData);

	void applyConstraint(defaulttype::BaseMatrix *mat, unsigned int offset);
	void applyConstraint(defaulttype::BaseVector *vect, unsigned int offset);

	bool getAreIndicesValid(){return m_IndicesValid;}


	DirichletBoundaryConstraint();

	virtual ~DirichletBoundaryConstraint(){};
	
	virtual void init();
	virtual void reinit();
//	{
//		FixedConstraint<DataTypes>::init();
//
//		x0.resize(0);
//
//		/*const Vec6& vb=_box.getValue();
//		if( vb != Vec6() )
//		{
//			this->removeConstraint( 0 );
//			vector <unsigned> indices;
//
//			this->mstate->getIndicesInSpace( indices, vb[0],vb[3],vb[1],vb[4],vb[2],vb[5] );
//			for(unsigned int i = 0; i < indices.size(); i++)
//			{
//				this->addConstraint(indices[i]);
//				_finalPos.push_back( (*this->mstate->getX())[indices[i]]+_displacement.getValue() );
//			}
//		}*/
//	}

	// Now templated in inline file to use with FriendCudaDirichletBoundaryConstraint
	void addDisplacementBC(unsigned int index, Coord displacementValue);

	void addMarkerDisplacementBC(unsigned int index, Coord displacementValue);
//	{
//		FixedConstraint<DataTypes>::addConstraint(index);
//		_displacementValues.push_back( displacementValue );
//	}
//
	// Now templated in inline file to use with FriendCudaDirichletBoundaryConstraint
	void clearDisplacementBCs();
//	{
//		FixedConstraint<DataTypes>::clearConstraints();
//		_displacementValues.clear();
//		//const SetIndexArray & indices = this->f_indices.getValue().getArray();
//			//		int theSize = indices.size();
//
//	}
	// -- Constraint interface
	//virtual void projectPosition(VecCoord&x )
//	{
////		//initialize initial Dofs positions, if it's not done
////		if (x0.size() == 0)
////		{
////	        const SetIndexArray & indices = this->f_indices.getValue().getArray();
////	        int vectorSize = this->f_indices.getValue().getArray().size();
////	        x0.resize(vectorSize); //???????????
////	        int i=0;
////	        for (SetIndexArray::const_iterator it = indices.begin(); it != indices.end(); ++it)
////	        {
////	            x0[i] = x[*it];
////	            i++;
////	        }
////		}
////
////			const SetIndexArray & indices = this->f_indices.getValue().getArray();
////			//int theSize = indices.size();
////			int i=0;
////			for (SetIndexArray::const_iterator it = indices.begin();
////						it != indices.end();
////						++it,++i)
////			{
////				//if( x[*it] == _finalPos[i] ) continue; // already arrived
////
////
////				//Coord vecstep = _finalPos[i]-x[*it];
////
////				//Real distance = vecstep.norm();
////
////				x[*it] = x0[i] + _displacementValues[i];
////
////				/*if(distance<_step.getValue())
////				{
////					x[*it] = _finalPos[i];
////				}
////				else
////				{
////					x[*it] += vecstep*(_step.getValue()/distance);
////				}*/
////			}
//	}
	
	
	

//	virtual void draw()
//	{
//		if (!this->getContext()->
//				   getShowBehaviorModels()) return;
//		const VecCoord& x = *this->mstate->getX();
//		const VecCoord& x0 = *this->mstate->getX0();
//		glDisable (GL_LIGHTING);
//		glPointSize(10);
//		glColor4f (0.5,0.5,1.0,1);
//
//		const SetIndexArray & indices = this->f_indices.getValue().getArray();
//		for (SetIndexArray::const_iterator it = indices.begin();
//		it != indices.end();
//		++it)
//		{
//			glBegin (GL_POINTS);
//			gl::glVertexT(x[*it]);
//			gl::glVertexT(x0[*it]);
//			glEnd();
//
//				glBegin (GL_LINES);
//				gl::glVertexT(x[*it]);
//				gl::glVertexT(x0[*it]);
//				glEnd();
//
//		}
//
//
//	}
    protected:
//	sofa::core::topology::BaseMeshTopology* m_Topology;
//	  PointData<sofa::helper::vector<PointInformation> > m_PointInfos;
//        PointHandler* m_PointHandler;
	
	
};

} // namespace projectiveconstraintset

} // namespace component

} // namespace sofa


#endif
