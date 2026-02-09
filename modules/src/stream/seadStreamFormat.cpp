#include "stream/seadStreamFormat.h"

#include "math/seadMathCalcCommon.h"
#include "stream/seadStreamSrc.h"

namespace sead
{
u8 BinaryStreamFormat::readU8(StreamSrc* src, Endian::Types endian)
{
    u8 rawValue = 0;
    src->read(&rawValue, sizeof(u8));
    return Endian::toHostU8(endian, rawValue);
}

u16 BinaryStreamFormat::readU16(StreamSrc* src, Endian::Types endian)
{
    u16 rawValue = 0;
    src->read(&rawValue, sizeof(u16));
    return Endian::toHostU16(endian, rawValue);
}

u32 BinaryStreamFormat::readU32(StreamSrc* src, Endian::Types endian)
{
    u32 rawValue = 0;
    src->read(&rawValue, sizeof(u32));
    return Endian::toHostU32(endian, rawValue);
}

u64 BinaryStreamFormat::readU64(StreamSrc* src, Endian::Types endian)
{
    u64 rawValue = 0;
    src->read(&rawValue, sizeof(u64));
    return Endian::toHostU64(endian, rawValue);
}

s8 BinaryStreamFormat::readS8(StreamSrc* src, Endian::Types endian)
{
    s8 rawValue = 0;
    src->read(&rawValue, sizeof(s8));
    return Endian::toHostU8(endian, rawValue);
}

s16 BinaryStreamFormat::readS16(StreamSrc* src, Endian::Types endian)
{
    s16 rawValue = 0;
    src->read(&rawValue, sizeof(s16));
    return Endian::toHostU16(endian, rawValue);
}

s32 BinaryStreamFormat::readS32(StreamSrc* src, Endian::Types endian)
{
    s32 rawValue = 0;
    src->read(&rawValue, sizeof(s32));
    return Endian::toHostU32(endian, rawValue);
}

s64 BinaryStreamFormat::readS64(StreamSrc* src, Endian::Types endian)
{
    s64 rawValue = 0;
    src->read(&rawValue, sizeof(s64));
    return Endian::toHostU64(endian, rawValue);
}

f32 BinaryStreamFormat::readF32(StreamSrc* src, Endian::Types endian)
{
    u32 rawValue = 0;
    src->read(&rawValue, sizeof(f32));
    return Endian::toHostF32(endian, &rawValue);
}

void BinaryStreamFormat::readBit(StreamSrc* src, void* data, u32 bits)
{
    u8* dataU8 = static_cast<u8*>(data);

    u32 size = bits / 8;
    src->read(dataU8, size);
    bits -= size * 8;

    if (bits <= 0)
        return;

    u8 lastByte;
    src->read(&lastByte, 1);

    u8 mask = 0xFF << bits;
    dataU8[size] &= mask;
    dataU8[size] |= lastByte & ~mask;
}

// NON_MATCHING: Potential wrong imlementation https://decomp.me/scratch/jCek1
void BinaryStreamFormat::readString(StreamSrc* src, BufferedSafeString* str, u32 size)
{
    u32 remainingSize = 0;
    if (size > (u32)str->getBufferSize())
    {
        remainingSize = size - str->getBufferSize();
        size = str->getBufferSize();
    }

    src->read(str->getBuffer(), size);

    if (size + 1 < (u32)str->getBufferSize())
        str->trim(size);
    else
        str->trim(str->getBufferSize() - 1);

    if (remainingSize != 0)
    {
        // Note: what happens if remaining size is bigger than the buffer?
        src->read(str->getBuffer(), remainingSize);
    }
}

u32 BinaryStreamFormat::readMemBlock(StreamSrc* src, void* buffer, u32 size)
{
    return src->read(buffer, size);
}

void BinaryStreamFormat::writeU8(StreamSrc* src, Endian::Types endian, u8 value)
{
    u8 rawValue = Endian::fromHostU8(endian, value);
    src->write(&rawValue, sizeof(u8));
}

void BinaryStreamFormat::writeU16(StreamSrc* src, Endian::Types endian, u16 value)
{
    u16 rawValue = Endian::fromHostU16(endian, value);
    src->write(&rawValue, sizeof(u16));
}

void BinaryStreamFormat::writeU32(StreamSrc* src, Endian::Types endian, u32 value)
{
    u32 rawValue = Endian::fromHostU32(endian, value);
    src->write(&rawValue, sizeof(u32));
}

void BinaryStreamFormat::writeU64(StreamSrc* src, Endian::Types endian, u64 value)
{
    u64 rawValue = Endian::fromHostU64(endian, value);
    src->write(&rawValue, sizeof(u64));
}

void BinaryStreamFormat::writeS8(StreamSrc* src, Endian::Types endian, s8 value)
{
    s8 rawValue = Endian::fromHostS8(endian, value);
    src->write(&rawValue, sizeof(s8));
}

void BinaryStreamFormat::writeS16(StreamSrc* src, Endian::Types endian, s16 value)
{
    s16 rawValue = Endian::fromHostS16(endian, value);
    src->write(&rawValue, sizeof(s16));
}

void BinaryStreamFormat::writeS32(StreamSrc* src, Endian::Types endian, s32 value)
{
    s32 rawValue = Endian::fromHostS32(endian, value);
    src->write(&rawValue, sizeof(s32));
}

void BinaryStreamFormat::writeS64(StreamSrc* src, Endian::Types endian, s64 value)
{
    s64 rawValue = Endian::fromHostS64(endian, value);
    src->write(&rawValue, sizeof(s64));
}

void BinaryStreamFormat::writeF32(StreamSrc* src, Endian::Types endian, f32 value)
{
    u32 rawValue = Endian::fromHostF32(endian, &value);
    src->write(&rawValue, sizeof(f32));
}

void BinaryStreamFormat::writeBit(StreamSrc* src, const void* data, u32 bits)
{
    const u8* dataU8 = static_cast<const u8*>(data);

    u8 size = bits / 8;
    src->write(dataU8, size);

    if (size * 8 == bits)
        return;

    const u8& lastByte = dataU8[size];
    src->write(&lastByte, 1);
}

void BinaryStreamFormat::writeString(StreamSrc* src, const SafeString& str, u32 size)
{
    u32 strSize = str.calcLength();
    if (strSize > size)
        strSize = size;

    src->write(str.cstr(), strSize);

    char nullchar = '\0';
    for (; strSize < size; strSize++)
        src->write(&nullchar, 1);
}

void BinaryStreamFormat::writeMemBlock(StreamSrc* src, const void* buffer, u32 size)
{
    src->write(buffer, size);
}

void BinaryStreamFormat::writeDecorationText([[maybe_unused]] StreamSrc* src,
                                             [[maybe_unused]] const SafeString& text)
{
}

void BinaryStreamFormat::writeNullChar([[maybe_unused]] StreamSrc* src) {}

void BinaryStreamFormat::skip(StreamSrc* src, u32 offset)
{
    src->skip(offset);
}

void BinaryStreamFormat::flush([[maybe_unused]] StreamSrc* src) {}

void BinaryStreamFormat::rewind(StreamSrc* src)
{
    src->rewind();
}
}  // namespace sead
