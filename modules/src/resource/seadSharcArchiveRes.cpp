#include <cstring>

#include <container/seadBuffer.h>
#include <prim/seadSafeString.hpp>
#include <resource/seadSharcArchiveRes.h>

namespace
{
u32 calcHash32(const sead::SafeString& str, u32 key)
{
    const char* str_ = str.cstr();

    u32 result = 0;
    for (s32 i = 0; str_[i] != '\0'; i++)
        result = result * key + str_[i];

    return result;
}

s32 binarySearch_(u32 hash, const sead::SharcArchiveRes::FATEntry* buffer, s32 start, s32 end)
{
    s32 middle;

    for (;;)
    {
        middle = (start + end) / 2;

        u32 entryHash = buffer[middle].hash;
        if (entryHash == hash)
            return middle;

        else if (entryHash < hash)
        {
            if (start == middle)
                return -1;

            start = middle;
        }

        else
        {
            if (end == middle)
                return -1;

            end = middle;
        }
    }
}

}  // namespace

namespace sead
{
SharcArchiveRes::SharcArchiveRes()
    : ArchiveRes(), mArchiveBlockHeader(NULL), mFATBlockHeader(NULL), mFNTBlock(NULL),
      mDataBlock(NULL)
#ifdef cafe
      ,
      mEndianType(Endian::cBig)
#else
#error "Unknown platform"
#endif  // cafe
{
}

SharcArchiveRes::~SharcArchiveRes() {}

void* SharcArchiveRes::getFileImpl_(const SafeString& file_path, FileInfo* file_info)
{
    s32 id = convertPathToEntryIDImpl_(file_path);
    if (id < 0)
        return NULL;

    return getFileFastImpl_(id, file_info);
}

void* SharcArchiveRes::getFileFastImpl_(s32 entry_id, FileInfo* file_info)
{
    if (entry_id < 0 || entry_id >= mFATEntrys.mSize)
        return NULL;

    u32 start = mFATEntrys[entry_id].data_start_offset;

    if (file_info != NULL)
    {
        u32 end = mFATEntrys[entry_id].data_end_offset;
        if (start > end)
            return NULL;

        u32 length = end - start;

        file_info->mStartOffset = start;
        file_info->mLength = length;
    }

    return const_cast<u8*>(mDataBlock) + start;
}

s32 SharcArchiveRes::convertPathToEntryIDImpl_(const SafeString& file_path)
{
    u32 hash = calcHash32(file_path, mFATBlockHeader->hash_key);

    s32 start = 0;
    s32 end = mFATEntrys.mSize;

    s32 id = binarySearch_(hash, mFATEntrys.mBuffer, start, end);
    if (id == -1)
        return -1;

    u32 offset = mFATEntrys[id].name_offset;
    if (offset != 0)
    {
        id -= (offset >> 24) - 1;

        while (id < end)
        {
            const FATEntry* entry = mFATEntrys.unsafeGet(id);
            if (entry->hash != hash)
                return -1;

            else
            {
                u32 offset_ = entry->name_offset;

                if (reinterpret_cast<const u8*>(mFNTBlock + (offset_ & 0xffffff)) > mDataBlock)
                    return -1;

                if (file_path.isEqual(mFNTBlock + (offset_ & 0xffffff) * cFileNameTableAlign))
                    return id;
            }

            id++;
        }
    }

    return id;
}

bool SharcArchiveRes::setCurrentDirectoryImpl_(const SafeString&)
{
    return false;
}

bool SharcArchiveRes::openDirectoryImpl_(u32* handle, const SafeString&)
{
    *handle = 0;
    return true;
}

bool SharcArchiveRes::closeDirectoryImpl_(u32* handle)
{
    return true;
}

u32 SharcArchiveRes::readDirectoryImpl_(u32* handle, DirectoryEntry* entry, u32 num)
{
    u32 count = 0;

    while (*handle + count < mFATBlockHeader->file_num && count < num)
    {
        u32 id = *handle + count;

        u32 offset = mFATEntrys[id].name_offset;
        if (offset == 0)
            entry[count].name.format("%08x", mFATEntrys[id].hash);

        else
        {
            if (reinterpret_cast<const u8*>(mFNTBlock + (offset & 0xffffff)) > mDataBlock)
                entry[count].name.clear();

            else
                entry[count].name.copy(mFNTBlock + (offset & 0xffffff) * cFileNameTableAlign);
        }

        entry[count].is_directory = false;
        count++;
    }

    *handle += count;
    return count;
}

bool SharcArchiveRes::prepareArchive_(const void* archive)
{
    if (archive == NULL)
        return false;

    const u8* archive_ = reinterpret_cast<const u8*>(archive);

    mArchiveBlockHeader = reinterpret_cast<const ArchiveBlockHeader*>(archive_);
    if (std::strncmp(mArchiveBlockHeader->signature, "SARC", 4) != 0)
        return false;

    mEndianType = Endian::markToEndian(mArchiveBlockHeader->byte_order);
    if (mEndianType != Endian::getHostEndian())
        return false;

    if (mArchiveBlockHeader->version != cArchiveVersion)
        return false;

    if (mArchiveBlockHeader->header_size != sizeof(ArchiveBlockHeader))
        return false;

    mFATBlockHeader =
        reinterpret_cast<const FATBlockHeader*>(archive_ + mArchiveBlockHeader->header_size);
    if (std::strncmp(mFATBlockHeader->signature, "SFAT", 4) != 0)
        return false;

    if (mFATBlockHeader->header_size != sizeof(FATBlockHeader))
        return false;

    if (mFATBlockHeader->file_num > cArchiveEntryMax)
        return false;

    mFATEntrys.setBuffer(
        mFATBlockHeader->file_num,
        const_cast<FATEntry*>(reinterpret_cast<const FATEntry*>(
            archive_ + mArchiveBlockHeader->header_size + mFATBlockHeader->header_size)));

    const FNTBlockHeader* fnt_header = reinterpret_cast<const FNTBlockHeader*>(
        archive_ + mArchiveBlockHeader->header_size + mFATBlockHeader->header_size +
        mFATBlockHeader->file_num * sizeof(FATEntry));
    if (std::strncmp(fnt_header->signature, "SFNT", 4) != 0)
        return false;

    if (fnt_header->header_size != sizeof(FNTBlockHeader))
        return false;

    mFNTBlock = reinterpret_cast<const char*>(fnt_header) + fnt_header->header_size;
    if (static_cast<s32>(mArchiveBlockHeader->data_block_offset) <
        static_cast<s32>(reinterpret_cast<size_t>(mArchiveBlockHeader) -
                         reinterpret_cast<size_t>(mFNTBlock)))
        return false;

    mDataBlock = archive_ + mArchiveBlockHeader->data_block_offset;
    return true;
}

}  // namespace sead
