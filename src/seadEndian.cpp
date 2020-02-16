#include <sead.h>

u8 swap8BE(u8 val)
{
    return val;
}

u8 swap8LE(u8 val)
{
    return val;
}

u16 swap16BE(u16 val)
{
    return (val << 8 | val >> 8) & 0xFFFF;
}

u16 swap16LE(u16 val)
{
    return val;
}

u32 swap32BE(u32 val)
{
    return val << 24 |
          (val & 0xFF00) << 8 |
           val >> 24 |
           val >> 8 & 0xFF00;
}

u32 swap32LE(u32 val)
{
    return val;
}

u64 swap64BE(u64 val)
{
    // Couldn't make an implementation that matches the original assembly
    // But this should be much more efficient
    return val << 56 |
          (val & 0xFF00) << 40 |
          (val & 0xFF0000) << 24 |
          (val & 0xFF000000) << 8 |
           val >> 56 |
           val >> 40 & 0xFF00 |
           val >> 24 & 0xFF0000 |
           val >> 8 & 0xFF000000;
}

u64 swap64LE(u64 val)
{
    return val;
}

namespace sead { namespace Endian {

Types cHostEndian = Type_Big; // Temporary
ConvFuncTable cConvFuncTable = { &swap8LE, &swap8BE, &swap16LE, &swap16BE, &swap32LE, &swap32BE, &swap64LE, &swap64BE };

} } // namespace sead::Endian
