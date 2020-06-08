add_library(sead
  src/seadArchiveRes.cpp
  # seadArena
  # seadFileDevice (platform specific)
  src/seadColor.cpp
  # src/seadCriticalSectionNx.cpp
  src/seadDisposer.cpp
  src/seadEndian.cpp
  # src/seadFileDevice.cpp
  # src/seadFileDeviceMgr.cpp
  src/seadHeap.cpp
  src/seadHeapMgr.cpp
  src/seadListImpl.cpp
  # src/seadMainFileDevice.cpp
  src/seadMatrix.cpp
  src/seadNew.cpp
  src/seadPath.cpp
  # src/seadPrimitiveRenderer.cpp
  # src/seadPrimitiveRendererUtil.cpp
  src/seadProjection.cpp
  src/seadPtrArray.cpp
  # src/seadResource.cpp
  src/seadResourceMgr.cpp
  src/seadSafeString.cpp
  # src/seadSharcArchiveRes.cpp
  src/seadStringUtil.cpp
  # src/seadSZSDecompressor.cpp
  src/seadVector.cpp
)
target_compile_options(sead PRIVATE -fno-rtti -fno-exceptions)
target_include_directories(sead PUBLIC include/)
