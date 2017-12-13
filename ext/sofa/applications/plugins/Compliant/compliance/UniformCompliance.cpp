#include "UniformCompliance.inl"
#include <sofa/defaulttype/VecTypes.h>
#include <sofa/core/ObjectFactory.h>

namespace sofa
{
namespace component
{
namespace forcefield
{

using namespace sofa::defaulttype;

// Register in the Factory
int UniformComplianceClass = core::RegisterObject("Uniform compliance")
#ifndef SOFA_FLOAT
        .add< UniformCompliance< Vec1dTypes > >(true)
        .add< UniformCompliance< Vec2dTypes > >()
        .add< UniformCompliance< Vec3dTypes > >()
        .add< UniformCompliance< Vec6dTypes > >()
#endif
#ifndef SOFA_DOUBLE
#ifdef SOFA_FLOAT
        .add< UniformCompliance< Vec1fTypes > >(true)
#else
        .add< UniformCompliance< Vec1fTypes > >()
#endif
        .add< UniformCompliance< Vec2fTypes > >()
        .add< UniformCompliance< Vec3fTypes > >()
        .add< UniformCompliance< Vec6fTypes > >()
#endif
        ;

SOFA_DECL_CLASS(UniformCompliance)

#ifndef SOFA_FLOAT
template class SOFA_Compliant_API UniformCompliance<Vec1dTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec2dTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec3dTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec6dTypes>;
#endif
#ifndef SOFA_DOUBLE
template class SOFA_Compliant_API UniformCompliance<Vec1fTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec2fTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec3fTypes>;
template class SOFA_Compliant_API UniformCompliance<Vec6fTypes>;
#endif

}
}
}
