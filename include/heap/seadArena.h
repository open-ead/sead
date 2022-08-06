#ifndef SEAD_ARENA_H_
#define SEAD_ARENA_H_

#include <basis/seadTypes.h>

namespace sead
{
class Arena
{
public:
    Arena();
    ~Arena();

    void initialize(size_t size);
    void destroy();

    u8* mStart = nullptr;
    size_t mSize = 0;
    bool mInitWithStartAddress = false;
};

}  // namespace sead

#endif  // SEAD_ARENA_H_
