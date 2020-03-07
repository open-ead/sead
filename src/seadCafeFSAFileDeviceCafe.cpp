#include <sead.h>

namespace sead {

CafeFSAFileDevice::CafeFSAFileDevice(const SafeString& name, const SafeString& devicePath)
    : FileDevice(name)
{
    this->devicePath = devicePath.c_str();
    _58 = 0;
    _5C = 0xDA;
    _60 = 0;
    _64 = 0;
    _68 = 0;
}

CafeContentFileDevice::CafeContentFileDevice()
    : CafeFSAFileDevice("content", "/vol/content")
{
}

} // namespace sead
