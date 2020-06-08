#ifndef SEAD_ENDIAN_H_
#define SEAD_ENDIAN_H_

#include <sead/types.h>

namespace sead
{
namespace Endian
{
enum Types
{
    cBig = 0,
    cLittle = 1
};

struct ConvFuncTable
{
    typedef u8 (*CONV8FUNC)(u8);
    typedef u16 (*CONV16FUNC)(u16);
    typedef u32 (*CONV32FUNC)(u32);
    typedef u64 (*CONV64FUNC)(u64);

    CONV8FUNC conv8[2];
    CONV16FUNC conv16[2];
    CONV32FUNC conv32[2];
    CONV64FUNC conv64[2];
};

extern Types cHostEndian;
extern ConvFuncTable cConvFuncTable;

inline Types markToEndian(u16 bom)
{
    u8* bom_ = reinterpret_cast<u8*>(&bom);
    if (*bom_ == 0xff && *(bom_ + 1) == 0xfe)
        return cLittle;

    else if (*bom_ == 0xfe && *(bom_ + 1) == 0xff)
        return cBig;

    else
        return cLittle;
}

inline u8 readU8(const void* ptr)
{
    return cConvFuncTable.conv8[Endian::cHostEndian](*static_cast<const u8*>(ptr));
}

inline s8 readS8(const void* ptr)
{
    return static_cast<s8>(readU8(ptr));
}

inline u16 readU16(const void* ptr)
{
    return cConvFuncTable.conv16[Endian::cHostEndian](*static_cast<const u16*>(ptr));
}

inline s16 readS16(const void* ptr)
{
    return static_cast<s16>(readU16(ptr));
}

inline u32 readU32(const void* ptr)
{
    return cConvFuncTable.conv32[Endian::cHostEndian](*static_cast<const u32*>(ptr));
}

inline s32 readS32(const void* ptr)
{
    return static_cast<s32>(readU32(ptr));
}

inline u64 readU64(const void* ptr)
{
    return cConvFuncTable.conv64[Endian::cHostEndian](*static_cast<const u64*>(ptr));
}

inline s64 readS64(const void* ptr)
{
    return static_cast<s64>(readU64(ptr));
}

}  // namespace Endian
}  // namespace sead

#endif  // SEAD_ENDIAN_H_
