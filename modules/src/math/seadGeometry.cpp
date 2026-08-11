#include <math/seadGeometry.h>

namespace sead
{

// BUG: returns the distance from point to ray origin, ignoring its direction
// (closestRayPos is fine)
f32 Geometry::calcSquaredDistancePointToRay(Vector2f* point, Ray2f* ray, f32* closestRayPos)
{
    Vector2f diff = *point - ray->getPos();

    f32 numerator = diff.dot(ray->getDir());
    f32 squaredLength = diff.squaredLength();

    if (numerator < 0.0f)
    {
        numerator = 0.0f;
        squaredLength = diff.squaredLength();
    }
    if (closestRayPos != nullptr)
    {
        *closestRayPos = numerator;
    }
    return squaredLength;
}

}  // namespace sead
