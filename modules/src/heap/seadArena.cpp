#include <heap/seadArena.h>
#include <nn/os.h>

namespace sead
{
Arena::Arena() {}
Arena::~Arena() = default;

void Arena::initialize(size_t size)
{
    nn::os::AllocateMemoryBlock(reinterpret_cast<u64*>(&mStart),
                                (size + 0x1FFFFF) & 0xFFFFFFFFFFE00000LL);
    mSize = size;
}

void Arena::destroy()
{
    if (!mInitWithStartAddress)
        nn::os::FreeMemoryBlock(reinterpret_cast<u64>(mStart),
                                (mSize + 0x1FFFFF) & 0xFFFFFFFFFFE00000LL);
    mInitWithStartAddress = false;
    mStart = nullptr;
    mSize = 0;
}

}  // namespace sead
