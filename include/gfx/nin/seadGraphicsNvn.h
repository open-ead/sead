#pragma once

#include <gfx/seadGraphics.h>
#include <thread/seadCriticalSection.h>
#include "nvn/nvn.h"

namespace sead
{
class GraphicsNvn : public Graphics
{
public:
    class CreateArg;

    GraphicsNvn(const CreateArg& arg);

    void initializeDrawLockContext(Heap*);
    void initializeImpl(Heap*);

    int getNewTextureId();
    NVNdevice* getNvnDevice() const { return mNvnDevice; }
    NVNtexturePool* getTexturePool() { return &mNvnTexturePool; }
    int getTextureSamplerID() const { return mTextureSamplerID; }
    CriticalSection* getCriticalSection2() { return &mCriticalSection2; }

private:
    NVNdevice* mNvnDevice;
    void* _38;
    void* _40;
    void* _48;
    void* _50;
    NVNtexturePool mNvnTexturePool;
    void* _78;
    void* _80;
    void* _88;
    void* _90;
    void* _98;
    void* _A0;
    void* _A8;
    void* _B0;
    void* _B8;
    void* _C0;
    void* _C8;
    void* _D0;
    void* _D8;
    void* _E0;
    void* _E8;
    void* _F0;
    int mTextureSamplerID;
    void* _100;
    int _108;
    int _10C;
    int _110;
    int _114;
    CriticalSection mCriticalSection1;
    CriticalSection mCriticalSection2;
    CriticalSection mCriticalSection3;
    void* _1D8;
    void* _1E0;
    void* _1E8;
    void* _1F0;
    void* _1F8;
    bool _200;
    bool _201;
    bool _202;
};
static_assert(sizeof(GraphicsNvn) == 0x208);

}  // namespace sead