#pragma once

#include "basis/seadTypes.h"
#include "prim/seadEndian.h"
#include "prim/seadSafeString.h"

namespace sead
{
class StreamSrc;

class StreamFormat
{
public:
    virtual u8 readU8(StreamSrc* src, Endian::Types endian) = 0;
    virtual u16 readU16(StreamSrc* src, Endian::Types endian) = 0;
    virtual u32 readU32(StreamSrc* src, Endian::Types endian) = 0;
    virtual u64 readU64(StreamSrc* src, Endian::Types endian) = 0;
    virtual s8 readS8(StreamSrc* src, Endian::Types endian) = 0;
    virtual s16 readS16(StreamSrc* src, Endian::Types endian) = 0;
    virtual s32 readS32(StreamSrc* src, Endian::Types endian) = 0;
    virtual s64 readS64(StreamSrc* src, Endian::Types endian) = 0;
    virtual f32 readF32(StreamSrc* src, Endian::Types endian) = 0;
    virtual void readBit(StreamSrc* src, void* data, u32 bits) = 0;
    virtual void readString(StreamSrc* src, BufferedSafeString* str, u32 size) = 0;
    virtual u32 readMemBlock(StreamSrc* src, void* buffer, u32 size) = 0;
    virtual void writeU8(StreamSrc* src, Endian::Types endian, u8 value) = 0;
    virtual void writeU16(StreamSrc* src, Endian::Types endian, u16 value) = 0;
    virtual void writeU32(StreamSrc* src, Endian::Types endian, u32 value) = 0;
    virtual void writeU64(StreamSrc* src, Endian::Types endian, u64 value) = 0;
    virtual void writeS8(StreamSrc* src, Endian::Types endian, s8 value) = 0;
    virtual void writeS16(StreamSrc* src, Endian::Types endian, s16 value) = 0;
    virtual void writeS32(StreamSrc* src, Endian::Types endian, s32 value) = 0;
    virtual void writeS64(StreamSrc* src, Endian::Types endian, s64 value) = 0;
    virtual void writeF32(StreamSrc* src, Endian::Types endian, f32 value) = 0;
    virtual void writeBit(StreamSrc* src, const void* data, u32 bits) = 0;
    virtual void writeString(StreamSrc* src, const SafeString& str, u32 size) = 0;
    virtual void writeMemBlock(StreamSrc* src, const void* buffer, u32 size) = 0;
    virtual void writeDecorationText(StreamSrc* src, const SafeString& text) = 0;
    virtual void writeNullChar(StreamSrc* src) = 0;
    virtual void skip(StreamSrc* src, u32 offset) = 0;
    virtual void flush(StreamSrc* src) = 0;
    virtual void rewind(StreamSrc* src) = 0;
};

class BinaryStreamFormat : public StreamFormat
{
public:
    u8 readU8(StreamSrc* src, Endian::Types endian) override;
    u16 readU16(StreamSrc* src, Endian::Types endian) override;
    u32 readU32(StreamSrc* src, Endian::Types endian) override;
    u64 readU64(StreamSrc* src, Endian::Types endian) override;
    s8 readS8(StreamSrc* src, Endian::Types endian) override;
    s16 readS16(StreamSrc* src, Endian::Types endian) override;
    s32 readS32(StreamSrc* src, Endian::Types endian) override;
    s64 readS64(StreamSrc* src, Endian::Types endian) override;
    f32 readF32(StreamSrc* src, Endian::Types endian) override;
    void readBit(StreamSrc* src, void* data, u32 bits) override;
    void readString(StreamSrc* src, BufferedSafeString* str, u32 size) override;
    u32 readMemBlock(StreamSrc* src, void* buffer, u32 size) override;
    void writeU8(StreamSrc* src, Endian::Types endian, u8 value) override;
    void writeU16(StreamSrc* src, Endian::Types endian, u16 value) override;
    void writeU32(StreamSrc* src, Endian::Types endian, u32 value) override;
    void writeU64(StreamSrc* src, Endian::Types endian, u64 value) override;
    void writeS8(StreamSrc* src, Endian::Types endian, s8 value) override;
    void writeS16(StreamSrc* src, Endian::Types endian, s16 value) override;
    void writeS32(StreamSrc* src, Endian::Types endian, s32 value) override;
    void writeS64(StreamSrc* src, Endian::Types endian, s64 value) override;
    void writeF32(StreamSrc* src, Endian::Types endian, f32 value) override;
    void writeBit(StreamSrc* src, const void* data, u32 bits) override;
    void writeString(StreamSrc* src, const SafeString& str, u32 size) override;
    void writeMemBlock(StreamSrc* src, const void* buffer, u32 size) override;
    void writeDecorationText(StreamSrc* src, const SafeString& text) override;
    void writeNullChar(StreamSrc* src) override;
    void skip(StreamSrc* src, u32 offset) override;
    void flush(StreamSrc* src) override;
    void rewind(StreamSrc* src) override;
};

class TextStreamFormat : public StreamFormat
{
public:
    TextStreamFormat();

    u8 readU8(StreamSrc* src, Endian::Types endian) override;
    u16 readU16(StreamSrc* src, Endian::Types endian) override;
    u32 readU32(StreamSrc* src, Endian::Types endian) override;
    u64 readU64(StreamSrc* src, Endian::Types endian) override;
    s8 readS8(StreamSrc* src, Endian::Types endian) override;
    s16 readS16(StreamSrc* src, Endian::Types endian) override;
    s32 readS32(StreamSrc* src, Endian::Types endian) override;
    s64 readS64(StreamSrc* src, Endian::Types endian) override;
    f32 readF32(StreamSrc* src, Endian::Types endian) override;
    void readBit(StreamSrc* src, void* data, u32 bits) override;
    void readString(StreamSrc* src, BufferedSafeString* str, u32 size) override;
    u32 readMemBlock(StreamSrc* src, void* buffer, u32 size) override;
    void writeU8(StreamSrc* src, Endian::Types endian, u8 value) override;
    void writeU16(StreamSrc* src, Endian::Types endian, u16 value) override;
    void writeU32(StreamSrc* src, Endian::Types endian, u32 value) override;
    void writeU64(StreamSrc* src, Endian::Types endian, u64 value) override;
    void writeS8(StreamSrc* src, Endian::Types endian, s8 value) override;
    void writeS16(StreamSrc* src, Endian::Types endian, s16 value) override;
    void writeS32(StreamSrc* src, Endian::Types endian, s32 value) override;
    void writeS64(StreamSrc* src, Endian::Types endian, s64 value) override;
    void writeF32(StreamSrc* src, Endian::Types endian, f32 value) override;
    void writeBit(StreamSrc* src, const void* data, u32 bits) override;
    void writeString(StreamSrc* src, const SafeString& str, u32 size) override;
    void writeMemBlock(StreamSrc* src, const void* buffer, u32 size) override;
    void writeDecorationText(StreamSrc* src, const SafeString& text) override;
    void writeNullChar(StreamSrc* src) override;
    void skip(StreamSrc* src, u32 offset) override;
    void flush(StreamSrc* src) override;
    void rewind(StreamSrc* src) override;

private:
    void getNextData_(sead::StreamSrc* src);

    FixedSafeString<128> mEntryTerminator;
};

}  // namespace sead
