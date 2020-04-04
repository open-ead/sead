#pragma once

#include "sead/seadPolicies.h"

namespace sead {

template <typename T>
class Matrix44CalcCommon
{
public:
    static void multiply(typename Policies<T>::Mtx44Base& out, const typename Policies<T>::Mtx44Base& b, const typename Policies<T>::Mtx34Base& a)
    {
        T a_00 = a.rows[0][0];
        T a_01 = a.rows[0][1];
        T a_02 = a.rows[0][2];
        T a_03 = a.rows[0][3];

        T a_10 = a.rows[1][0];
        T a_11 = a.rows[1][1];
        T a_12 = a.rows[1][2];
        T a_13 = a.rows[1][3];

        T a_20 = a.rows[2][0];
        T a_21 = a.rows[2][1];
        T a_22 = a.rows[2][2];
        T a_23 = a.rows[2][3];

        T b_00 = b.rows[0][0];
        T b_01 = b.rows[0][1];
        T b_02 = b.rows[0][2];
        T b_03 = b.rows[0][3];

        T b_10 = b.rows[1][0];
        T b_11 = b.rows[1][1];
        T b_12 = b.rows[1][2];
        T b_13 = b.rows[1][3];

        T b_20 = b.rows[2][0];
        T b_21 = b.rows[2][1];
        T b_22 = b.rows[2][2];
        T b_23 = b.rows[2][3];

        T b_30 = b.rows[2][0];
        T b_31 = b.rows[2][1];
        T b_32 = b.rows[2][2];
        T b_33 = b.rows[2][3];

        out.rows[0][0] = b_02 * a_20 + b_00 * a_00 + b_01 * a_10;
        out.rows[0][1] = b_02 * a_21 + b_00 * a_01 + b_01 * a_11;
        out.rows[0][2] = b_02 * a_22 + b_00 * a_02 + b_01 * a_12;
        out.rows[0][3] = b_02 * a_23 + b_00 * a_03 + b_01 * a_13 + b_03;

        out.rows[1][0] = b_12 * a_20 + b_10 * a_00 + b_11 * a_10;
        out.rows[1][1] = b_12 * a_21 + b_10 * a_01 + b_11 * a_11;
        out.rows[1][2] = b_12 * a_22 + b_10 * a_02 + b_11 * a_12;
        out.rows[1][3] = b_12 * a_23 + b_10 * a_03 + b_11 * a_13 + b_13;

        out.rows[2][0] = b_22 * a_20 + b_20 * a_00 + b_21 * a_10;
        out.rows[2][1] = b_22 * a_21 + b_20 * a_01 + b_21 * a_11;
        out.rows[2][2] = b_22 * a_22 + b_20 * a_02 + b_21 * a_12;
        out.rows[2][3] = b_22 * a_23 + b_20 * a_03 + b_21 * a_13 + b_23;

        out.rows[3][0] = b_32 * a_20 + b_30 * a_00 + b_31 * a_10;
        out.rows[3][1] = b_32 * a_21 + b_30 * a_01 + b_31 * a_11;
        out.rows[3][2] = b_32 * a_22 + b_30 * a_02 + b_31 * a_12;
        out.rows[3][3] = b_32 * a_23 + b_30 * a_03 + b_31 * a_13 + b_33;
    }
};

}
