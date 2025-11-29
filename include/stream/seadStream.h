#pragma once

#include "basis/seadTypes.h"
#include "prim/seadEndian.h"
#include "prim/seadSafeString.h"

namespace sead
{
class StreamFormat;
class StreamSrc;

class Stream
{
public:
    enum class Modes
    {
        Binary = 0,
        Text = 1,
    };

    Stream();
    Stream(StreamSrc* src, Modes mode);
    Stream(StreamSrc* src, StreamFormat* format);
    virtual ~Stream() = default;

    void skip(u32 bytes);
    void skip(u32 blockSize, u32 count);
    void rewind();
    bool isEOF();

    void setBinaryEndian(Endian::Types endian);
    void setMode(Modes mode);
    void setUserFormat(StreamFormat* format);

protected:
    static StreamFormat* BASIC_STREAM_FORMAT[2];

    void setSrc(StreamSrc* src) { mSrc = src; }

    StreamFormat* mFormat = nullptr;
    StreamSrc* mSrc = nullptr;
    Endian::Types mEndian = Endian::getHostEndian();
};

class ReadStream : public Stream
{
public:
    u8 readU8();
    void readU8(u8& value);
    u16 readU16();
    void readU16(u16& value);
    u32 readU32();
    void readU32(u32& value);
    u64 readU64();
    void readU64(u64& value);
    s8 readS8();
    void readS8(s8& value);
    s16 readS16();
    void readS16(s16& value);
    s32 readS32();
    void readS32(s32& value);
    s64 readS64();
    void readS64(s64& value);
    f32 readF32();
    void readF32(f32& value);
    void readBit(void* data, u32 bits);
    void readString(BufferedSafeString* str, u32 size);
    u32 readMemBlock(void* buffer, u32 size);

private:
    f32 readF32BitImpl_(u32 integerBits, u32 fractionalBits);
    f64 readF64BitImpl_(u32 integerBits, u32 fractionalBits);
};

class WriteStream : public ReadStream
{
public:
    ~WriteStream() override
    {
        if (mSrc)
            flush();
    }

    void writeU8(u8 value);
    void writeU16(u16 value);
    void writeU32(u32 value);
    void writeU64(u64 value);
    void writeS8(s8 value);
    void writeS16(s16 value);
    void writeS32(s32 value);
    void writeS64(s64 value);
    void writeF32(f32 value);
    void writeBit(const void* data, u32 bits);
    void writeString(const SafeString& str, u32 size);
    void writeMemBlock(const void* buffer, u32 size);
    void writeComment(const SafeString& comment);
    void writeLineComment(const SafeString& comment);
    void writeDecorationText(const SafeString& text);
    void writeNullChar();
    void flush();

private:
    void writeF32BitImpl_(f32 value, u32 integerBits, u32 fractionalBits);
    void writeF64BitImpl_(f64 value, u32 integerBits, u32 fractionalBits);
};
}  // namespace sead
