/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2015 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/

#include <SofaSimpleFem/HexahedronFEMForceField.h>
#include <SofaTest/ForceField_test.h>

namespace sofa {

using namespace modeling;

/**  Test suite for HexahedronFEMForceField: we check if the accurate forces are computed
  */
template <typename _HexahedronFEMForceField>
struct HexahedronFEMForceField_test : public ForceField_test<_HexahedronFEMForceField>
{

    typedef _HexahedronFEMForceField ForceType;
    typedef ForceField_test<_HexahedronFEMForceField> Inherited;
    typedef typename ForceType::DataTypes DataTypes;

    typedef typename ForceType::VecCoord VecCoord;
    typedef typename ForceType::VecDeriv VecDeriv;
    typedef typename ForceType::Coord Coord;
    typedef typename ForceType::Deriv Deriv;
    typedef typename Coord::value_type Real;
    typedef helper::Vec<3,Real> Vec3;

    typedef ForceType Spring;
    typedef component::container::MechanicalObject<DataTypes> DOF;

    VecCoord x;
    VecDeriv v,f;

    HexahedronFEMForceField_test():Inherited::ForceField_test(std::string(SOFASIMPLEFEM_TEST_SCENES_DIR) + "/" + "HexahedronFEMForceField.scn")
    {
        //Position
        x.resize(8);
        DataTypes::set( x[0], 0,0,0);
        DataTypes::set( x[1], 1,0,0);
        DataTypes::set( x[2], 1,1,0);
        DataTypes::set( x[3], 0,1,0);
        // Apply an extension along z axis
        Vec3 xTmp(0,1,1.1);
        DataTypes::set( x[4], xTmp[0],xTmp[0],xTmp[2]);
        DataTypes::set( x[5], xTmp[1],xTmp[0],xTmp[2]);
        DataTypes::set( x[6], xTmp[1],xTmp[1],xTmp[2]);
        DataTypes::set( x[7], xTmp[0],xTmp[1],xTmp[2]);
        //Velocity
        v.resize(8);
        DataTypes::set( v[0], 0,0,0);
        DataTypes::set( v[1], 0,0,0);
        DataTypes::set( v[2], 0,0,0);
        DataTypes::set( v[3], 0,0,0);
        DataTypes::set( v[4], 0,0,0);
        DataTypes::set( v[5], 0,0,0);
        DataTypes::set( v[6], 0,0,0);
        DataTypes::set( v[7], 0,0,0);
        //Expected force
        f.resize(8);
        Vec3 fdown(0,0,0.25);
        Vec3 fup(0,0,-0.25);
        DataTypes::set( f[0],  fdown[0], fdown[1], fdown[2]);
        DataTypes::set( f[1],  fdown[0], fdown[1], fdown[2]);
        DataTypes::set( f[2],  fdown[0], fdown[1], fdown[2]);
        DataTypes::set( f[3],  fdown[0], fdown[1], fdown[2]);
        DataTypes::set( f[4],  fup[0], fup[1], fup[2]);
        DataTypes::set( f[5],  fup[0], fup[1], fup[2]);
        DataTypes::set( f[6],  fup[0], fup[1], fup[2]);
        DataTypes::set( f[7],  fup[0], fup[1], fup[2]);

        // Set force parameters
        Inherited::force->f_poissonRatio.setValue(0);
        Inherited::force->f_youngModulus.setValue(10);
        Inherited::force->setMethod(2); // small method
        Inherited::force->isCompliance.setValue(0);

        // Init simulation
        sofa::simulation::getSimulation()->init(Inherited::node.get());

    }

    //Test the value of the force it should be equal for each vertex to Pressure*area/4
    void test_valueForce()
    {
        // run the forcefield_test
        Inherited::run_test( x, v, f );
    }
};

// ========= Define the list of types to instanciate.
//using testing::Types;
typedef testing::Types<
component::forcefield::HexahedronFEMForceField<defaulttype::Vec3Types>
> TestTypes; // the types to instanciate.



// ========= Tests to run for each instanciated type
TYPED_TEST_CASE(HexahedronFEMForceField_test, TestTypes);

// test case
TYPED_TEST( HexahedronFEMForceField_test , extension )
{
    this->errorMax *= 100;
    this->deltaRange = std::make_pair( 1, this->errorMax * 10 );
    this->debug = false;

    // run test
    this->test_valueForce();
}

} // namespace sofa
