#include <gfx/seadGraphics.h>

namespace sead
{

Graphics* Graphics::sInstance = nullptr;

Graphics::DevicePosture Graphics::sDefaultDevicePosture = Graphics::cDevicePosture_Same;
f32 Graphics::sDefaultDeviceZScale = 1.0f;
f32 Graphics::sDefaultDeviceZOffset = 0.0f;

// TODO: lockDrawContext/unlockDrawContext unimpl'd; WiiU refcount lock design doesn't fit master's DrawLockContext.

}  // namespace sead
