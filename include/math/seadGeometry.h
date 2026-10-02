#pragma once

#include <geom/seadLine.h>
#include <math/seadVector.h>

namespace sead
{
namespace Geometry
{
f32 calcSquaredDistancePointToRay(Vector2f* point, Ray2f* ray, f32* closestRayPos);
// float calcSquaredDistancePointToLine(Vector2f* const, Line2f* const, float*);
// float calcSquaredDistancePointToSegment(Vector3f* const, Segment3f* const, float*);
// float calcSquaredDistancePointToSegment();
// float calcSquaredDistanceSegmentToSegment(Segment2f* const, Segment2f* const, float*, float*);
// float calcSquaredDistanceRayToSegment(Ray2f const&, Segment2f* const, float*, float*);
}  // namespace Geometry
}  // namespace sead
