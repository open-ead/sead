#include <sead.h>

namespace sead {

MainFileDevice::MainFileDevice(Heap* heap)
    : FileDevice("main")
    , mFileDevice(NULL)
{
    mFileDevice = new(heap, 4) CafeContentFileDevice();
}

MainFileDevice::~MainFileDevice()
{
    if (mFileDevice == NULL)
        return;

    delete mFileDevice;
    mFileDevice = NULL;
}

} // namespace sead
