add_library(sead OBJECT
  modules/src/basis/seadNew.cpp

  modules/src/container/seadListImpl.cpp
  modules/src/container/seadPtrArray.cpp

  # modules/src/filedevice/seadFileDevice.cpp
  # modules/src/filedevice/seadFileDeviceMgr.cpp
  # modules/src/filedevice/seadMainFileDevice.cpp
  modules/src/filedevice/seadPath.cpp

  modules/src/gfx/seadColor.cpp
  # modules/src/gfx/seadPrimitiveRenderer.cpp
  # modules/src/gfx/seadPrimitiveRendererUtil.cpp
  modules/src/gfx/seadProjection.cpp

  modules/src/heap/seadDisposer.cpp
  modules/src/heap/seadHeap.cpp
  modules/src/heap/seadHeapMgr.cpp

  modules/src/math/seadMatrix.cpp
  modules/src/math/seadVector.cpp

  modules/src/prim/seadBitFlag.cpp
  modules/src/prim/seadEndian.cpp
  modules/src/prim/seadEnum.cpp
  modules/src/prim/seadMemUtil.cpp
  modules/src/prim/seadSafeString.cpp
  modules/src/prim/seadStringUtil.cpp

  modules/src/resource/seadArchiveRes.cpp
  # modules/src/resource/seadResource.cpp
  modules/src/resource/seadResourceMgr.cpp
  # modules/src/resource/seadSharcArchiveRes.cpp
  # modules/src/resource/seadSZSDecompressor.cpp
)
target_compile_options(sead PRIVATE -fno-rtti -fno-exceptions)
target_include_directories(sead PUBLIC include/)
