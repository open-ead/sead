#include "stream/seadStreamFormat.h"

#include <stdio.h>

#include "codec/seadBase64.h"
#include "math/seadMathCalcCommon.h"
#include "prim/seadScopedLock.h"
#include "prim/seadStringUtil.h"
#include "stream/seadStreamSrc.h"
#include "thread/seadMutex.h"

namespace sead
{
static FixedSafeString<128> sTextData;
static Mutex sMutex;

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

// NOTE: Leaves higher bits of last byte of data at their previous value
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

// NOTE: If size > str->getBufferSize(), it wraps around and starts reading to the start again.
// if size > str->getBufferSize()*2, the second iteration continues writing out-of-bounds.
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
        src->read(str->getBuffer(), remainingSize);
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

// NOTE: Writes extra bits in last byte into stream normally
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

TextStreamFormat::TextStreamFormat() : mEntryTerminator(" \t\r\n") {}

u8 TextStreamFormat::readU8(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    u8 value = 0;
    getNextData_(src);
    StringUtil::tryParseU8(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

u16 TextStreamFormat::readU16(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    u16 value = 0;
    getNextData_(src);
    StringUtil::tryParseU16(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

u32 TextStreamFormat::readU32(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    u32 value = 0;
    getNextData_(src);
    StringUtil::tryParseU32(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

u64 TextStreamFormat::readU64(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    u64 value = 0;
    getNextData_(src);
    StringUtil::tryParseU64(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

s8 TextStreamFormat::readS8(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    s8 value = 0;
    getNextData_(src);
    StringUtil::tryParseS8(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

s16 TextStreamFormat::readS16(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    s16 value = 0;
    getNextData_(src);
    StringUtil::tryParseS16(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

s32 TextStreamFormat::readS32(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    s32 value = 0;
    getNextData_(src);
    StringUtil::tryParseS32(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

s64 TextStreamFormat::readS64(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    s64 value = 0;
    getNextData_(src);
    StringUtil::tryParseS64(&value, sTextData.cstr(), StringUtil::CardinalNumber::BaseAuto);
    return value;
}

f32 TextStreamFormat::readF32(StreamSrc* src, [[maybe_unused]] Endian::Types endian)
{
    ScopedLock<Mutex> lock(&sMutex);
    f32 value = 0.0f;
    getNextData_(src);

    if (sTextData.calcLength() != 0)
        sscanf(sTextData.cstr(), "%f", &value);

    return value;
}

// NON_MATCHING: https://decomp.me/scratch/PKNGF
void TextStreamFormat::readBit(StreamSrc* src, void* data, u32 bits)
{
    ScopedLock<Mutex> lock(&sMutex);
    u8* dest = static_cast<u8*>(data);
    getNextData_(src);

    SafeString bitStr = sTextData;
    if (!bitStr.comparen("0b", 2))
        bitStr = bitStr.getPart(2);

    u32 bitCount = 0;
    u8 currentByte = 0;
    u32 length = bitStr.calcLength();
    for (u32 i = 0; bitCount < bits && i < length + 1; i++)
    {
        currentByte <<= 1;
        if (bitStr.at(i) == '1')
            currentByte |= 1;

        if ((++bitCount & 7) == 0)
        {
            dest[bitCount / 8 - 1] = currentByte;
            currentByte = 0;
        }
    }

    u32 remainder = bitCount & 7;
    if (remainder != 0)
    {
        u8 mask = 0xff << remainder;
        dest[bitCount / 8] &= mask;
        dest[bitCount / 8] |= currentByte;
    }
}

void TextStreamFormat::readString(StreamSrc* src, BufferedSafeString* str,
                                  [[maybe_unused]] u32 size)
{
    ScopedLock<Mutex> lock(&sMutex);
    getNextData_(src);
    str->copy(sTextData);
}

u32 TextStreamFormat::readMemBlock(StreamSrc* src, void* buffer, u32 size)
{
    ScopedLock<Mutex> lock(&sMutex);
    getNextData_(src);
    u32 textSize = sTextData.calcLength();

    u64 readSize = 0;
    u64 tmpReadSize = 0;
    Base64::decode(buffer, size, sTextData.cstr(), textSize, &tmpReadSize);
    readSize = tmpReadSize;

    return readSize;
}

void TextStreamFormat::writeU8(StreamSrc* src, [[maybe_unused]] Endian::Types endian, u8 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%u", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeU16(StreamSrc* src, [[maybe_unused]] Endian::Types endian, u16 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%u", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeU32(StreamSrc* src, [[maybe_unused]] Endian::Types endian, u32 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%u", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeU64(StreamSrc* src, [[maybe_unused]] Endian::Types endian, u64 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%llu", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeS8(StreamSrc* src, [[maybe_unused]] Endian::Types endian, s8 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%d", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeS16(StreamSrc* src, [[maybe_unused]] Endian::Types endian, s16 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%d", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeS32(StreamSrc* src, [[maybe_unused]] Endian::Types endian, s32 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%d", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeS64(StreamSrc* src, [[maybe_unused]] Endian::Types endian, s64 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%lld", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeF32(StreamSrc* src, [[maybe_unused]] Endian::Types endian, f32 value)
{
    sead::FixedSafeString<32> tmp;
    tmp.format("%.8f", value);
    u32 length = tmp.calcLength();

    src->write(tmp.cstr(), length);
    src->write(mEntryTerminator.cstr(), 1);
}

void TextStreamFormat::writeBit(StreamSrc* src, const void* data, u32 bits)
{
    ScopedLock<Mutex> lock(&sMutex);
    const u8* dataU8 = static_cast<const u8*>(data);
    sTextData = "0b";

    for (u32 i = 0; i < (bits + 7) / 8; i++)
    {
        u32 bitsInByte = bits - i * 8;
        if (bitsInByte > 8)
            bitsInByte = 8;

        for (s32 j = bitsInByte - 1; j >= 0; j--)
        {
            if (dataU8[i] & (1 << j))
                sTextData.append('1');
            else
                sTextData.append('0');
        }
    }

    src->write(sTextData.cstr(), bits + 2);  // 0b + bits
    src->write(mEntryTerminator.cstr(), 1);
}

// NOTE: Quotes are sanitized with \"
void TextStreamFormat::writeString(StreamSrc* src, const SafeString& str, u32 size)
{
    u32 length = str.calcLength();
    if (size <= length)
        length = size;

    char quotes = '"';
    char backslash = '\\';
    src->write(&quotes, 1);
    for (u32 i = 0; i < length; i++)
    {
        if (str[i] == '"')
        {
            src->write(&backslash, 1);
        }
        else
        {
            if (i != 0)
                str.cstr();
            if (str[i] == '"')
                src->write(&backslash, 1);
        }

        src->write(&str.cstr()[i], 1);
    }
    src->write(&quotes, 1);
}

inline s32 toBase64Size(u32 size)
{
    s32 bytes = size / 3;
    if (size % 3 != 0)
        bytes++;
    return bytes * 4;
}

// NON_MATCHING: Bad toBase64Size calculation https://decomp.me/scratch/Xzhbw
void TextStreamFormat::writeMemBlock(StreamSrc* src, const void* buffer, u32 size)
{
    ScopedLock<Mutex> lock(&sMutex);
    sTextData.clear();

    if (toBase64Size(size) + 1 < sTextData.getBufferSize())
    {
        char* textBuffer = sTextData.getBuffer();

        textBuffer[toBase64Size(size)] = SafeString::cNullChar;
        Base64::encode(textBuffer, buffer, size, false);
        u32 length = sTextData.calcLength();

        src->write("\"", 1);
        src->write(sTextData.cstr(), length);
        src->write("\"", 1);
        src->write(mEntryTerminator.cstr(), 1);
    }
}

void TextStreamFormat::writeDecorationText(StreamSrc* src, const SafeString& text)
{
    u32 length = text.calcLength();
    src->write(text.cstr(), length);
}

void TextStreamFormat::writeNullChar(StreamSrc* src)
{
    char nullchar = '\0';
    src->write(&nullchar, 1);
}

// NOTE: It will skip the next field regardless of the specified offset
void TextStreamFormat::skip(StreamSrc* src, [[maybe_unused]] u32 offset)
{
    ScopedLock<Mutex> lock(&sMutex);
    getNextData_(src);
}

void TextStreamFormat::flush([[maybe_unused]] StreamSrc* src) {}

void TextStreamFormat::rewind(StreamSrc* src)
{
    src->rewind();
}

// NON_MATCHING: Bad implementation https://decomp.me/scratch/6wcQg
void TextStreamFormat::getNextData_(sead::StreamSrc* src)
{
    sTextData.clear();

    s32 txtLength = 0;
    char commentEndChar = '\0';
    bool inQuotes = false;

    char value;
    while (src->read(&value, 1) != 0)
    {
        // Skip comments
        if (commentEndChar != '\0')
        {
            if (value != commentEndChar)
            {
                if (commentEndChar == '/')
                    commentEndChar = '*';
                continue;
            }

            if (commentEndChar == '*')
            {
                commentEndChar = '/';
                continue;
            }

            commentEndChar = '\0';
            value = mEntryTerminator[0];
        }

        if (inQuotes)
        {
            if (value == '"')
            {
                if (txtLength >= 2 && sTextData[txtLength - 1] == '\\')
                {
                    // Since Quotes are sanitized we need to override the backslash with quotes
                    sTextData.copyAt(-2, "\"", 1);
                    continue;
                }
                return;
            }

            sTextData.append(value);
            txtLength++;
            continue;
        }

        if (txtLength == 0 && value == '"')
        {
            inQuotes = true;
            continue;
        }

        if (mEntryTerminator.include(value) || value == '\0')
        {
            if (!sTextData.isEmpty())
            {
                return;
            }
            continue;
        }

        sTextData.append(value);
        txtLength++;

        // Skip hash comments until new line
        if (txtLength >= 1 && sTextData[txtLength - 1] == '#')
        {
            commentEndChar = '\n';
            txtLength--;
            sTextData.trim(txtLength);
        }

        if (txtLength >= 2)
        {
            // Skip comments until new line
            if (sTextData[txtLength - 2] == '/' && sTextData[txtLength - 1] == '/')
            {
                commentEndChar = '\n';
                txtLength -= 2;
                sTextData.trim(txtLength);
                continue;
            }

            // Skip multiline comments until end of comment
            if (sTextData[txtLength - 2] == '/' && sTextData[txtLength - 1] == '*')
            {
                commentEndChar = '*';
                txtLength -= 2;
                sTextData.trim(txtLength);
                continue;
            }
        }
    }
}
}  // namespace sead
