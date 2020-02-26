#ifndef SEAD_ARENA_H_
#define SEAD_ARENA_H_

#include <types.h>

namespace sead {

class Arena
{
public:
    Arena();
    ~Arena();

    void initialize(size_t size);

    void* heap;
    u32 size;
    u8 _8;
};

} // namespace sead

#endif // SEAD_ARENA_H_
