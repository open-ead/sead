#pragma once

// Forward declarations for Vector types.
// Most users should #include <seadVector.h> instead.

#include <basis/seadTypes.h>

namespace sead
{
template <typename T>
struct Vector2;

template <typename T>
struct Vector3;

template <typename T>
struct Vector4;

using Vector2i = Vector2<s32>;
using Vector2u = Vector2<u32>;
using Vector2f = Vector2<f32>;

using Vector3i = Vector3<s32>;
using Vector3u = Vector3<u32>;
using Vector3f = Vector3<f32>;

using Vector4i = Vector4<s32>;
using Vector4u = Vector4<u32>;
using Vector4f = Vector4<f32>;

}  // namespace sead
