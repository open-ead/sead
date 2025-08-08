#pragma once

#include <math/seadVector.h>

namespace sead
{
template <typename T>
struct Ray
{
    Ray();

    T position;
    T direction;
};
namespace Geometry
{
float calcSquaredDistancePointToRay(Vector2<float>* point, Ray<Vector2<float>>* ray, float* scalar);
// float calcSquaredDistancePointToLine(Vector2<float>* const, Line<sead::Vector2<float>>* const,
// float*); float calcSquaredDistancePointToSegment(Vector3<float>* const, Segment<Vector3<float>>*
// const, float*); float calcSquaredDistancePointToSegment();
// float calcSquaredDistanceSegmentToSegment(Segment<Vector2<float>>* const,
//                                           Segment<Vector2<float>>* const, float*, float*);
// float calcSquaredDistanceRayToSegment(Ray<Vector2<float>> const&, Segment<Vector2<float>>* const,
//                                       float*, float*);
}  // namespace Geometry
}  // namespace sead
