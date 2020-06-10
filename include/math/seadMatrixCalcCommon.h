#ifndef SEAD_MATRIXCALCCOMMON_H_
#define SEAD_MATRIXCALCCOMMON_H_

#include <math/seadMathPolicies.h>

namespace sead {

template <typename T>
class Matrix44CalcCommon
{
public:
    static void multiply(typename Policies<T>::Mtx44Base& out, const typename Policies<T>::Mtx44Base& b, const typename Policies<T>::Mtx34Base& a);
};

}  // namespace sead

#endif // SEAD_MATRIXCALCCOMMON_H_
