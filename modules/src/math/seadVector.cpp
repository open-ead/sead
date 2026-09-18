#include <math/seadVector.h>

namespace sead
{
template <>
const Vector2f Vector2f::zero(0.0f, 0.0f);

template <>
const Vector2f Vector2f::ex(1.0f, 0.0f);

template <>
const Vector2f Vector2f::ey(0.0f, 1.0f);

template <>
const Vector2f Vector2f::ones(1.0f, 1.0f);

template <>
const Vector3f Vector3f::zero(0.0f, 0.0f, 0.0f);

template <>
const Vector3f Vector3f::ex(1.0f, 0.0f, 0.0f);

template <>
const Vector3f Vector3f::ey(0.0f, 1.0f, 0.0f);

template <>
const Vector3f Vector3f::ez(0.0f, 0.0f, 1.0f);

template <>
const Vector3f Vector3f::ones(1.0f, 1.0f, 1.0f);

template <>
const Vector4f Vector4f::zero(0.0f, 0.0f, 0.0f, 0.0f);

template <>
const Vector4f Vector4f::ex(1.0f, 0.0f, 0.0f, 0.0f);

template <>
const Vector4f Vector4f::ey(0.0f, 1.0f, 0.0f, 0.0f);

template <>
const Vector4f Vector4f::ez(0.0f, 0.0f, 1.0f, 0.0f);

template <>
const Vector4f Vector4f::ew(0.0f, 0.0f, 0.0f, 1.0f);

template <>
const Vector4f Vector4f::ones(1.0f, 1.0f, 1.0f, 1.0f);

template <>
const Vector2i Vector2i::zero(0, 0);

template <>
const Vector2i Vector2i::ex(1, 0);

template <>
const Vector2i Vector2i::ey(0, 1);

template <>
const Vector2i Vector2i::ones(1, 1);

template <>
const Vector3i Vector3i::zero(0, 0, 0);

template <>
const Vector3i Vector3i::ex(1, 0, 0);

template <>
const Vector3i Vector3i::ey(0, 1, 0);

template <>
const Vector3i Vector3i::ez(0, 0, 1);

template <>
const Vector3i Vector3i::ones(1, 1, 1);

template <>
const Vector4i Vector4i::zero(0, 0, 0, 0);

template <>
const Vector4i Vector4i::ex(1, 0, 0, 0);

template <>
const Vector4i Vector4i::ey(0, 1, 0, 0);

template <>
const Vector4i Vector4i::ez(0, 0, 1, 0);

template <>
const Vector4i Vector4i::ew(0, 0, 0, 1);

template <>
const Vector4i Vector4i::ones(1, 1, 1, 1);

template <>
const Vector2u Vector2u::zero(0u, 0u);

template <>
const Vector2u Vector2u::ex(1u, 0u);

template <>
const Vector2u Vector2u::ey(0u, 1u);

template <>
const Vector2u Vector2u::ones(1u, 1u);

template <>
const Vector3u Vector3u::zero(0u, 0u, 0u);

template <>
const Vector3u Vector3u::ex(1u, 0u, 0u);

template <>
const Vector3u Vector3u::ey(0u, 1u, 0u);

template <>
const Vector3u Vector3u::ez(0u, 0u, 1u);

template <>
const Vector3u Vector3u::ones(1u, 1u, 1u);

template <>
const Vector4u Vector4u::zero(0u, 0u, 0u, 0u);

template <>
const Vector4u Vector4u::ex(1u, 0u, 0u, 0u);

template <>
const Vector4u Vector4u::ey(0u, 1u, 0u, 0u);

template <>
const Vector4u Vector4u::ez(0u, 0u, 1u, 0u);

template <>
const Vector4u Vector4u::ew(0u, 0u, 0u, 1u);

template <>
const Vector4u Vector4u::ones(1u, 1u, 1u, 1u);

}  // namespace sead
