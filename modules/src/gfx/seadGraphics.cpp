#include <gfx/seadGraphics.h>
#include <thread/seadThread.h>

namespace sead
{

Graphics* Graphics::sInstance = nullptr;

Graphics::DevicePosture Graphics::sDefaultDevicePosture = Graphics::cDevicePosture_Same;
f32 Graphics::sDefaultDeviceZScale = 1.0f;
f32 Graphics::sDefaultDeviceZOffset = 0.0f;

void Graphics::lockDrawContext()
{
    sead::Thread* current = ThreadMgr::instance()->getCurrentThread();
    if (current == mContextHolderThread && mContextRefCounter > 0)
    {
        mContextRefCounter++;
    }
    else
    {
        mContextCriticalSection.lock();
        // SEAD_ASSERT(mContextHolderThread == nullptr);
        // SEAD_ASSERT(mContextRefCounter == 0);
        lockDrawContextImpl();
        mContextHolderThread = current;
        mContextRefCounter = 1;
    }
}

void Graphics::unlockDrawContext()
{
    sead::Thread* current = ThreadMgr::instance()->getCurrentThread();
    // SEAD_ASSERT(mContextHolderThread == current);
    // SEAD_ASSERT(mContextRefCounter > 0);
    if (--mContextRefCounter == 0)
    {
        mContextHolderThread = nullptr;
        unlockDrawContextImpl();
        mContextCriticalSection.unlock();
    }
}

}  // namespace sead
