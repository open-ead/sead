#ifndef SEAD_ARCHIVE_RES_H_
#define SEAD_ARCHIVE_RES_H_

#include <basis/seadTypes.h>
#include <container/seadSafeArray.h>
#include <filedevice/seadFileDevice.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>
#include <resource/seadResource.h>

namespace sead
{
class ArchiveRes : public DirectResource
{
    SEAD_RTTI_OVERRIDE(ArchiveRes, DirectResource)

public:
    class FileInfo
    {
    public:
        explicit FileInfo() : mStartOffset(0), mLength(0) {}

        u32 mStartOffset;
        u32 mLength;
    };

public:
    ArchiveRes() : DirectResource(), mEnable(false) {}
    ~ArchiveRes() override = default;

    s32 getLoadDataAlignment() const override;
    void doCreate_(u8* buf, u32, Heap*) override;

protected:
    virtual const void* getFileImpl_(const SafeString& file_path,
                               FileInfo* file_info = nullptr) const = 0;
    virtual const void* getFileFastImpl_(s32 entry_id, FileInfo* file_info) const = 0;
    virtual s32 convertPathToEntryIDImpl_(const SafeString& file_path) const = 0;
    virtual bool setCurrentDirectoryImpl_(const SafeString&) = 0;
    virtual bool openDirectoryImpl_(HandleBuffer* handle, const SafeString& path) const = 0;
    virtual bool closeDirectoryImpl_(HandleBuffer* handle) const = 0;
    virtual u32 readDirectoryImpl_(HandleBuffer* handle, DirectoryEntry* entries, u32 num) const = 0;
    virtual bool isExistFileImpl_(const SafeString& path) const;
    virtual bool prepareArchive_(const void* archive) = 0;

    bool mEnable;
};

}  // namespace sead

#endif  // SEAD_ARCHIVE_RES_H_
