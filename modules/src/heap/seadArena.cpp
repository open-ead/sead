#include <heap/seadArena.h>
#include <nn/os.h>

namespace sead
{
Arena::Arena() = default;
Arena::~Arena() = default;

void Arena::initialize(size_t size)
{
    nn::os::AllocateMemoryBlock(reinterpret_cast<uintptr_t*>(&mStart),
                                (size + 0x1FFFFF) & 0xFFFFFFFFFFE00000LL);
    mSize = size;
}

void Arena::destroy()
{
    if (!mInitWithStartAddress)
    {
        nn::os::FreeMemoryBlock(reinterpret_cast<uintptr_t>(mStart),
                                (mSize + 0x1FFFFF) & 0xFFFFFFFFFFE00000LL);
    }
    mInitWithStartAddress = false;
    mStart = nullptr;
    mSize = 0;
}

}  // namespace sead
