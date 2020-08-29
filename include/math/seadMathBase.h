#pragma once

namespace sead
{
template <typename T>
struct BaseVec2
{
    T x;
    T y;
};

template <typename T>
struct BaseVec3
{
    T x;
    T y;
    T z;
};

template <typename T>
struct BaseVec4
{
    T x;
    T y;
    T z;
    T w;
};

template <typename T>
struct BaseQuat
{
    T x;
    T y;
    T z;
    T w;
};

template <typename T>
struct BaseMtx34
{
    union
    {
        T m[3][4];
        T a[12];
    };
};

template <typename T>
struct BaseMtx44
{
    union
    {
        T m[4][4];
        T a[16];
    };
};
}  // namespace sead
