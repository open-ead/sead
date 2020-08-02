project(sead CXX ASM)

option(SEAD_DEBUG "Enable debug assertions and warnings" OFF)
set(SEAD_PLATFORM "nin" CACHE STRING "Target platform (default: nin)")
set_property(
  CACHE SEAD_PLATFORM
  PROPERTY STRINGS "nin"
)

add_library(sead OBJECT
  include/basis/seadNew.h
  include/basis/seadRawPrint.h
  include/basis/seadTypes.h
  modules/src/basis/seadNew.cpp

  include/codec/seadBase64.h
  include/codec/seadHashCRC16.h
  include/codec/seadHashCRC32.h
  modules/src/codec/seadHashCRC16.cpp
  modules/src/codec/seadHashCRC32.cpp

  include/container/seadBuffer.h
  include/container/seadListImpl.h
  include/container/seadOffsetList.h
  include/container/seadPtrArray.h
  include/container/seadSafeArray.h
  include/container/seadTList.h
  include/container/seadTreeNode.h
  modules/src/container/seadListImpl.cpp
  modules/src/container/seadPtrArray.cpp
  modules/src/container/seadTreeNode.cpp

  include/devenv/seadAssertConfig.h
  modules/src/devenv/seadAssertConfig.cpp

  include/filedevice/seadFileDevice.h
  include/filedevice/seadFileDeviceMgr.h
  include/filedevice/seadMainFileDevice.h
  include/filedevice/seadPath.h
  # modules/src/filedevice/seadFileDevice.cpp
  # modules/src/filedevice/seadFileDeviceMgr.cpp
  # modules/src/filedevice/seadMainFileDevice.cpp
  modules/src/filedevice/seadPath.cpp

  include/framework/seadFramework.h
  include/framework/seadHeapPolicies.h
  include/framework/seadMethodTree.h
  include/framework/seadMethodTreeMgr.h
  include/framework/seadTaskBase.h
  include/framework/seadTaskID.h
  include/framework/seadTaskMgr.h

  include/gfx/seadCamera.h
  include/gfx/seadColor.h
  include/gfx/seadPrimitiveRenderer.h
  include/gfx/seadPrimitiveRendererUtil.h
  include/gfx/seadProjection.h
  include/gfx/seadTexture.h
  modules/src/gfx/seadColor.cpp
  # modules/src/gfx/seadPrimitiveRenderer.cpp
  # modules/src/gfx/seadPrimitiveRendererUtil.cpp
  modules/src/gfx/seadProjection.cpp

  include/heap/seadArena.h
  include/heap/seadDisposer.h
  include/heap/seadHeap.h
  include/heap/seadHeapMgr.h
  modules/src/heap/seadDisposer.cpp
  modules/src/heap/seadHeap.cpp
  modules/src/heap/seadHeapMgr.cpp

  include/math/seadBoundBox.h
  include/math/seadMathCalcCommon.h
  include/math/seadMathCalcCommon.hpp
  include/math/seadMathPolicies.h
  include/math/seadMatrix.h
  include/math/seadMatrixCalcCommon.h
  include/math/seadMatrixCalcCommon.hpp
  include/math/seadQuat.h
  include/math/seadVector.h
  modules/src/math/seadBoundBox.cpp
  modules/src/math/seadMatrix.cpp
  modules/src/math/seadQuat.cpp
  modules/src/math/seadVector.cpp

  include/mc/seadCoreInfo.h
  modules/src/mc/seadCoreInfo.cpp

  include/prim/seadBitFlag.h
  include/prim/seadBitUtil.h
  include/prim/seadDelegate.h
  include/prim/seadDelegateEventSlot.h
  include/prim/seadEndian.h
  include/prim/seadEnum.h
  include/prim/seadLongBitFlag.h
  include/prim/seadMemUtil.h
  include/prim/seadNamable.h
  include/prim/seadPtrUtil.h
  include/prim/seadRuntimeTypeInfo.h
  include/prim/seadSafeString.h
  include/prim/seadSafeString.hpp
  include/prim/seadScopedLock.h
  include/prim/seadStringBuilder.h
  include/prim/seadStringUtil.h
  modules/src/prim/seadBitFlag.cpp
  modules/src/prim/seadEndian.cpp
  modules/src/prim/seadEnum.cpp
  modules/src/prim/seadMemUtil.cpp
  modules/src/prim/seadSafeString.cpp
  modules/src/prim/seadStringBuilder.cpp
  modules/src/prim/seadStringUtil.cpp

  include/random/seadGlobalRandom.h
  include/random/seadRandom.h
  modules/src/random/seadGlobalRandom.cpp
  modules/src/random/seadRandom.cpp

  include/resource/seadArchiveRes.h
  include/resource/seadDecompressor.h
  include/resource/seadResource.h
  include/resource/seadResourceMgr.h
  include/resource/seadSharcArchiveRes.h
  include/resource/seadSZSDecompressor.h
  modules/src/resource/seadArchiveRes.cpp
  # modules/src/resource/seadResource.cpp
  modules/src/resource/seadResourceMgr.cpp
  # modules/src/resource/seadSharcArchiveRes.cpp
  # modules/src/resource/seadSZSDecompressor.cpp

  include/thread/seadAtomic.h
  include/thread/seadCriticalSection.h
  include/thread/seadDelegateThread.h
  include/thread/seadEvent.h
  include/thread/seadMessageQueue.h
  include/thread/seadMutex.h
  include/thread/seadReadWriteLock.h
  include/thread/seadSemaphore.h
  include/thread/seadSpinLock.h
  include/thread/seadThread.h
  include/thread/seadThreadLocalStorage.h
  include/thread/seadThreadUtil.h
  modules/src/thread/seadDelegateThread.cpp
  modules/src/thread/seadReadWriteLock.cpp
  modules/src/thread/seadSpinLock.cpp
  modules/src/thread/seadThread.cpp
  modules/src/thread/seadThreadUtil.cpp

  include/time/seadCalendarSpan.h
  include/time/seadCalendarTime.h
  include/time/seadDateSpan.h
  include/time/seadDateTime.h
  include/time/seadDateUtil.h
  include/time/seadTickSpan.h
  include/time/seadTickTime.h
  modules/src/time/seadCalendarTime.cpp
  modules/src/time/seadDateSpan.cpp
  modules/src/time/seadDateTime.cpp
  modules/src/time/seadDateTimeUtc.cpp
  modules/src/time/seadDateUtil.cpp
  modules/src/time/seadTickSpan.cpp
)

if(SEAD_DEBUG)
  target_sources(sead PRIVATE
    include/hostio/seadHostIOEventListener.h
    include/hostio/seadHostIOMgr.h
    include/hostio/seadHostIONode.h
    include/hostio/seadHostIOReflexible.h
    include/hostio/seadHostIOThreadLock.h
    modules/src/hostio/seadHostIOEventListener.cpp
    modules/src/hostio/seadHostIONode.cpp
    modules/src/hostio/seadHostIOReflexible.cpp
    modules/src/hostio/seadHostIOThreadLock.cpp
  )
endif()

if(SEAD_PLATFORM STREQUAL "nin")
  target_sources(sead PRIVATE
    include/thread/nin/seadThreadLocalStorageNin.hpp
    modules/src/thread/nin/seadCriticalSectionNin.cpp
    modules/src/thread/nin/seadEventNin.cpp
    modules/src/thread/nin/seadMessageQueueNin.cpp
    modules/src/thread/nin/seadMutexNin.cpp
    modules/src/thread/nin/seadSemaphoreNin.cpp
    modules/src/thread/nin/seadThreadNin.cpp
  )
endif()

target_compile_options(sead PRIVATE -fno-exceptions)
target_compile_options(sead PRIVATE -Wno-invalid-offsetof)
target_include_directories(sead PUBLIC include/)

if(SEAD_DEBUG)
  target_compile_definitions(sead PUBLIC SEAD_DEBUG=1)
endif()
