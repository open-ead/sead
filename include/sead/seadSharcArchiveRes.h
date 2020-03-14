#ifndef SEAD_SHARC_ARCHIVE_RES_H_
#define SEAD_SHARC_ARCHIVE_RES_H_

#include <sead/seadArchiveRes.h>
#include <sead/seadFileDevice.h>
#include <sead/seadSafeString.h>

namespace sead {

class SharcArchiveRes : public ArchiveRes
{
    SEAD_RTTI_OVERRIDE(SharcArchiveRes, ArchiveRes)

public:
    struct ArchiveBlockHeader
    {
        char magic[4];
        u16 headerSize;
        u16 bom;
        u32 fileSize;
        u32 dataOffset;
        u16 version;
        u16 reserved;
    };

    struct FATBlockHeader
    {
        char magic[4];
        u16 headerSize;
        u16 nodeCount;
        u32 hashKey;
    };

    struct FATEntry
    {
        u32 hash;
        u32 nameOffset;
        u32 dataStartOffset;
        u32 dataEndOffset;
    };

    struct FNTBlockHeader
    {
        char magic[4];
        u16 headerSize;
        u16 reserved;
    };

public:
    SharcArchiveRes();
    virtual ~SharcArchiveRes();

    virtual void* getFileImpl_(const SafeString& file_path, FileInfo* file_info=NULL);
    virtual void* getFileFastImpl_(s32 entry_id, FileInfo* file_info);
    virtual s32 convertPathToEntryIDImpl_(const SafeString& file_path);
    virtual bool setCurrentDirectoryImpl_(const SafeString&);
    virtual bool openDirectoryImpl_(u32* handle, const SafeString&);                    // openDirectoryImpl_(SafeArray<u8, 32>*, const SafeString&)
    virtual bool closeDirectoryImpl_(u32* handle);                                      // closeDirectoryImpl_(SafeArray<u8, 32>*)
    virtual u32 readDirectoryImpl_(u32* handle, DirectoryEntry* entry, u32 num);        // readDirectoryImpl_(SafeArray<u8, 32>*, DirectoryEntry*, u32)
    virtual bool prepareArchive_(const void* archive);

    static const u32 cArchiveVersion = 0x100;
    static const u32 cArchiveEntryMax = 0x3fff;
    static const u32 cFileNameTableAlign = 4;

    const ArchiveBlockHeader* mArchiveBlockHeader;
    const FATBlockHeader* mFATBlockHeader;
    const char* mFNTBlock;
    Buffer<const FATEntry> mFATEntrys;
    const u8* mDataBlock;
    Endian::Types mEndianType;
};

} // namespace sead

#endif // SEAD_SHARC_ARCHIVE_RES_H_
