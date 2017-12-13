/******************************************************************************
   *Data definition for Quadratic Tetrahedron
******************************************************************************/
#ifndef SOFA_COMPONENT_TOPOLOGY_QUADRATICTETRAHEDRONDATA_H
#define SOFA_COMPONENT_TOPOLOGY_QUADRATICTETRAHEDRONDATA_H

#include <QuadraticBaseMeshTopology.h>
#include <sofa/helper/vector.h>

namespace sofa
{

namespace component
{

namespace topology
{
//	using core::topology::BaseMeshTopology;
//	typedef BaseMeshTopology::Tetra Tetra;
//	typedef Tetra Tetrahedron;

	using core::topology::QuadraticBaseMeshTopology;
//	typedef QuadraticBaseMeshTopology::QuadTetra QuadTetra;
	//test!!!
	typedef QuadraticBaseMeshTopology::QuadTetra QuadTetra;
	typedef QuadTetra QuadTetrahedron;

	typedef QuadraticBaseMeshTopology::Tetra Tetra;
	typedef Tetra Tetrahedron;

	/** \brief Basic creation function for element of type T : simply calls default constructor.
	*
	*/
	//TODO renamed the td_basicCreateFunc & td_basicDestroyFunc
	template < typename T >
	inline void Mytd_basicCreateFunc(int ,
									void* , T& t,
									const Tetrahedron& ,
									const sofa::helper::vector< unsigned int > &,
									const sofa::helper::vector< double >&)
	{
		t = T();
		//return;
	}

	/** \brief Basic destruction function for element of type T : does nothing.
	*
	*/
	template < typename T >
	inline void Mytd_basicDestroyFunc(int , void* , T& )
	{
		return;
	}

   /** \brief Topological Engine which will handle all TetrahedronData */
//   class TetrahedronSetTopologyEngine;
   class QuadraticTetrahedronSetTopologyEngine;

	/** \brief A class for storing Tetrahedron related data. Automatically manages topology changes.
	*
	* This class is a wrapper of class helper::vector that is made to take care transparently of all topology changes that might
	* happen (non exhaustive list: Tetrahedra added, removed, fused, renumbered).
	*/
	template< class T, class Alloc = helper::CPUMemoryManager<T> >
	class QuadraticTetrahedronData : public sofa::core::objectmodel::Data<sofa::helper::vector<T, Alloc> >
	{
      friend class QuadraticTetrahedronSetTopologyEngine;

	public:
		/// size_type
		typedef typename sofa::helper::vector<T,Alloc>::size_type size_type;
		/// reference to a value (read-write)
		typedef typename sofa::helper::vector<T,Alloc>::reference reference;
		/// const reference to a value (read only)
		typedef typename sofa::helper::vector<T,Alloc>::const_reference const_reference;
		/// const iterator
		typedef typename sofa::helper::vector<T,Alloc>::const_iterator const_iterator;

      /// Creation function, called when adding elements.
      typedef void (*t_createFunc)(int, void*, T&, const Tetrahedron&, const sofa::helper::vector< unsigned int >&, const sofa::helper::vector< double >& );
      /// Destruction function, called when deleting elements.
      typedef void (*t_destroyFunc)(int, void*, T&);
      /// Creation function, called when adding hexahedra elements.
      typedef void (*t_createHexahedronFunc)(const sofa::helper::vector<unsigned int> &, void*, helper::vector< T > &);
      /// Destruction function, called when removing hexahedra elements.
      typedef void (*t_destroyHexahedronFunc)(const sofa::helper::vector<unsigned int> &, void*, helper::vector< T > &);


	public:
//		/// Constructor
//        QuadraticTetrahedronData( const typename sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >::InitData& data,
//								  void (*createFunc) (int, void*, T&, const Tetrahedron &,const sofa::helper::vector< unsigned int >&, const sofa::helper::vector< double >&) = Mytd_basicCreateFunc,
//								  void* createParam  = (void*)NULL,
//								  void (*destroyFunc)(int, void*, T&) = Mytd_basicDestroyFunc,
//								  void* destroyParam = (void*)NULL )
//								  : sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(data),
//								    m_createFunc(createFunc), m_destroyFunc(destroyFunc),
//								    m_createHexahedronFunc(0), m_destroyHexahedronFunc(0),
//								    m_createParam(createParam), m_destroyParam(destroyParam)
//		{}
//
//		/// Constructor
//	    QuadraticTetrahedronData(size_type n, const T& value) : sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->resize(n, value);
//			this->endEdit();
//		}
//		/// Constructor
//	    QuadraticTetrahedronData(int n, const T& value) : sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->resize(n, value);
//			this->endEdit();
//		}
//		/// Constructor
//	    QuadraticTetrahedronData(long n, const T& value): sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->resize(n, value);
//			this->endEdit();
//		}
//		/// Constructor
//		explicit QuadraticTetrahedronData(size_type n): sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->resize(n);
//			this->endEdit();
//		}
//		/// Constructor
//		QuadraticTetrahedronData(const sofa::helper::vector<T, Alloc>& x): sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			(*data) = x;
//			this->endEdit();
//		}
//
//#ifdef __STL_MEMBER_TEMPLATES
//		/// Constructor
//		template <class InputIterator>
//		QuadraticTetrahedronData(InputIterator first, InputIterator last): sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->assign(first, last);
//			this->endEdit();
//		}
//#else /* __STL_MEMBER_TEMPLATES */
//		/// Constructor
//		QuadraticTetrahedronData(const_iterator first, const_iterator last): sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false)
//		{
//			sofa::helper::vector<T, Alloc>* data = this->beginEdit();
//			data->assign(first, last);
//			this->endEdit();
//		}
//#endif /* __STL_MEMBER_TEMPLATES */

		/// Optionnaly takes 2 parameters, a creation and a destruction function that will be called when adding/deleting elements.
		//TODO this constructor will be called by the Igstk
		QuadraticTetrahedronData( void (*createFunc) (int, void*, T&,const Tetrahedron& , const sofa::helper::vector< unsigned int >&, const sofa::helper::vector< double >&) = Mytd_basicCreateFunc, void* createParam  = (void*)NULL,
                       void (*destroyFunc)(int, void*, T&                                     ) = Mytd_basicDestroyFunc,
                       void* destroyParam = (void*)NULL )
                          : sofa::core::objectmodel::Data< sofa::helper::vector<T, Alloc> >(0, false, false),
                          m_createFunc(createFunc), m_destroyFunc(destroyFunc),
                          m_createHexahedronFunc(0), m_destroyHexahedronFunc(0),
                          m_createParam(createParam), m_destroyParam(destroyParam)
		{}

		/// Handle TetrahedronSetTopology related events, ignore others.
//		void handleTopologyEvents( std::list< const core::topology::TopologyChange *>::const_iterator changeIt,
//                                 std::list< const core::topology::TopologyChange *>::const_iterator &end );


      /// Creation function, called when adding elements.
      void setCreateFunction(t_createFunc createFunc)
      {
         m_createFunc=createFunc;
      }
      /// Destruction function, called when deleting elements.
      void setDestroyFunction(t_destroyFunc destroyFunc)
      {
         m_destroyFunc=destroyFunc;
      }

      /// Creation function, called when adding hexahedra elements.
      void setCreateHexahedronFunction(t_createHexahedronFunc createHexahedronFunc)
      {
         m_createHexahedronFunc=createHexahedronFunc;
      }
      /// Destruction function, called when removing hexahedra elements.
      void setDestroyHexahedronFunction(t_destroyHexahedronFunc destroyHexahedronFunc)
      {
         m_destroyHexahedronFunc=destroyHexahedronFunc;
      }


      /// Creation function, called when adding parameter to those elements.
      void setDestroyParameter( void* destroyParam )
      {
         m_destroyParam=destroyParam;
      }
      /// Destruction function, called when removing parameter to those elements.
      void setCreateParameter( void* createParam )
      {
         m_createParam=createParam;
      }


      /** Public fonction to apply creation and destruction functions */
      /// Apply adding hexahedra elements.
//      void applyCreateHexahedronFunction(const sofa::helper::vector<unsigned int> & indices);
//      /// Apply removing hexahedra elements.
//      void applyDestroyHexahedronFunction(const sofa::helper::vector<unsigned int> & indices);


	private:
//		/// Swaps values at indices i1 and i2.
//		void swap( unsigned int i1, unsigned int i2 );
//
//		/// Add some values. Values are added at the end of the vector.
//		void add( unsigned int nbTetrahedra,
//                const sofa::helper::vector< QuadTetrahedron >& tetrahedron,
//                const sofa::helper::vector< sofa::helper::vector< unsigned int > > &ancestors,
//                const sofa::helper::vector< sofa::helper::vector< double > >& coefs);
//
//		/// Remove the values corresponding to the Tetrahedra removed.
//		void remove( const sofa::helper::vector<unsigned int> &index );


	private:
      t_createFunc m_createFunc;
      t_destroyFunc m_destroyFunc;
      t_createHexahedronFunc m_createHexahedronFunc;
      t_destroyHexahedronFunc m_destroyHexahedronFunc;


		/** Parameter to be passed to creation function.
		*
		* Warning : construction and destruction of this object is not of the responsibility of TetrahedronData.
		*/
		void* m_createParam;

		/** Parameter to be passed to destruction function.
		*
		* Warning : construction and destruction of this object is not of the responsibility of TetrahedronData.
		*/
		void* m_destroyParam;
	};

} // namespace topology

} // namespace component

} // namespace sofa

#endif // _QuadraticTetrahedronDATA_H_
