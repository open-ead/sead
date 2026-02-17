#ifndef SEAD_ARCHIVE_RES_H_
#define SEAD_ARCHIVE_RES_H_

#include <basis/seadRawPrint.h>
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
    struct FileInfo
    {
        u32 mStartOffset;
        u32 mLength;
    };

public:
    ArchiveRes() : DirectResource(), mEnable(false) {}
    ~ArchiveRes() override = default;

    s32 getLoadDataAlignment() const override;
    void doCreate_(u8* buf, u32, Heap*) override;

    const void* getFile(const SafeString& file_path,
                        FileInfo* info = nullptr) SEAD_ARCHIVERES_CONST_TOKEN
    {
        SEAD_ASSERT(mEnable);
        return getFileImpl_(file_path, info);
    }

    const void* getFileFast(const s32 entry_id, FileInfo* info) SEAD_ARCHIVERES_CONST_TOKEN
    {
        SEAD_ASSERT(mEnable);
        return getFileFastImpl_(entry_id, info);
    }

    s32 convertPathToEntryID(const SafeString& path) SEAD_ARCHIVERES_CONST_TOKEN
    {
        SEAD_ASSERT(mEnable);
        return convertPathToEntryIDImpl_(path);
    }

#if SEAD_ARCHIVERES_TRYGETFILEPATH
    bool tryGetFilePath(SafeString* out_file_path, s32 entry_id) SEAD_ARCHIVERES_CONST_TOKEN
    {
        SEAD_ASSERT(mEnable);
        return tryGetFilePathImpl_(out_file_path, entry_id);
    }
#endif

    bool setCurrentDirectory(const SafeString& dir)
    {
        SEAD_ASSERT(mEnable);
        return setCurrentDirectoryImpl_(dir);
    }

    bool openDirectory(HandleBuffer* handle, const SafeString& dir) SEAD_ARCHIVERES_CONST_TOKEN
    {
        return openDirectoryImpl_(handle, dir);
    }

    bool closeDirectory(HandleBuffer* handle) SEAD_ARCHIVERES_CONST_TOKEN
    {
        return closeDirectoryImpl_(handle);
    }

    u32 readDirectory(HandleBuffer* handle, DirectoryEntry* entries,
                      u32 num) SEAD_ARCHIVERES_CONST_TOKEN
    {
        return readDirectoryImpl_(handle, entries, num);
    }

#if SEAD_ARCHIVERES_ISEXISTFILEIMPL
    bool isExistFile(const SafeString& path) SEAD_ARCHIVERES_CONST_TOKEN
    {
        return isExistFileImpl_(path);
    }
#endif

protected:
    virtual const void* getFileImpl_(const SafeString& file_path,
                                     FileInfo* file_info = nullptr) SEAD_ARCHIVERES_CONST_TOKEN = 0;
    virtual const void* getFileFastImpl_(s32 entry_id,
                                         FileInfo* file_info) SEAD_ARCHIVERES_CONST_TOKEN = 0;
#if SEAD_ARCHIVERES_TRYGETFILEPATH
    virtual bool tryGetFilePathImpl_(SafeString* out_file_path,
                                     s32 entry_id) SEAD_ARCHIVERES_CONST_TOKEN = 0;
#endif
    virtual s32
    convertPathToEntryIDImpl_(const SafeString& file_path) SEAD_ARCHIVERES_CONST_TOKEN = 0;
    virtual bool setCurrentDirectoryImpl_(const SafeString&) = 0;
    virtual bool openDirectoryImpl_(HandleBuffer* handle,
                                    const SafeString& path) SEAD_ARCHIVERES_CONST_TOKEN = 0;
    virtual bool closeDirectoryImpl_(HandleBuffer* handle) SEAD_ARCHIVERES_CONST_TOKEN = 0;
    virtual u32 readDirectoryImpl_(HandleBuffer* handle, DirectoryEntry* entries,
                                   u32 num) SEAD_ARCHIVERES_CONST_TOKEN = 0;
#if SEAD_ARCHIVERES_ISEXISTFILEIMPL
    virtual bool isExistFileImpl_(const SafeString& path) SEAD_ARCHIVERES_CONST_TOKEN = 0;
#endif
    virtual bool prepareArchive_(const void* archive) = 0;

    bool mEnable;
};

}  // namespace sead

#endif  // SEAD_ARCHIVE_RES_H_
