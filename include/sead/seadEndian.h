#ifndef SEAD_ENDIAN_H_
#define SEAD_ENDIAN_H_

struct ConvFuncTable
{
    typedef u8 (*swap8_t)(u8);
    typedef u16 (*swap16_t)(u16);
    typedef u32 (*swap32_t)(u32);
    typedef u64 (*swap64_t)(u64);

    swap8_t int8[2];
    swap16_t int16[2];
    swap32_t int32[2];
    swap64_t int64[2];
};

namespace sead { namespace Endian {

enum Types
{
    Type_Big    = 0,
    Type_Little = 1
};

extern Types cHostEndian;
extern ConvFuncTable cConvFuncTable;

inline u8 readU8(const void* ptr)
{
    return cConvFuncTable.int8[Endian::cHostEndian](*static_cast<const u8*>(ptr));
}

inline s8 readS8(const void* ptr)
{
    return static_cast<s8>(readU8(ptr));
}

inline u16 readU16(const void* ptr)
{
    return cConvFuncTable.int16[Endian::cHostEndian](*static_cast<const u16*>(ptr));
}

inline s16 readS16(const void* ptr)
{
    return static_cast<s16>(readU16(ptr));
}

inline u32 readU32(const void* ptr)
{
    return cConvFuncTable.int32[Endian::cHostEndian](*static_cast<const u32*>(ptr));
}

inline s32 readS32(const void* ptr)
{
    return static_cast<s32>(readU32(ptr));
}

inline u64 readU64(const void* ptr)
{
    return cConvFuncTable.int64[Endian::cHostEndian](*static_cast<const u64*>(ptr));
}

inline s64 readS64(const void* ptr)
{
    return static_cast<s64>(readU64(ptr));
}

} } // namespace sead::Endian

#endif // SEAD_ENDIAN_H_
