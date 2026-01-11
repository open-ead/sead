#include <math/seadGeometry.h>

namespace sead
{

float Geometry::calcSquaredDistancePointToRay(Vector2<float>* point, Ray<Vector2<float>>* ray,
                                              float* scalar)
{
    auto diff = *point - ray->position;

    auto numerator = diff.dot(ray->direction);
    auto squaredLength = diff.squaredLength();
    if (numerator < 0.0f)
    {
        numerator = 0.0f;
        squaredLength = diff.squaredLength();
    }
    if (scalar != nullptr)
    {
        *scalar = numerator;
    }
    return squaredLength;
}

}  // namespace sead
