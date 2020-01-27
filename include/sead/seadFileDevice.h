#ifndef SEAD_FILE_DEVICE_H_
#define SEAD_FILE_DEVICE_H_

#include <sead/seadDisposer.h>
#include <sead/seadListImpl.h>

namespace sead {

class FileDevice : public UnkList, public IDisposer
{
public:
    FileDevice()
        : UnkList(), IDisposer(), name(), _4C(1)
    {
    }

    FixedSafeString<32> name;
    u8 _4C;
    void* vtable;
};

} // namespace sead

#endif // SEAD_FILE_DEVICE_H_
