add_library(sead
  src/seadNew.cpp
  src/seadSafeString.cpp
  # FIXME: add the rest of the source files
)
target_compile_options(sead PRIVATE -fno-rtti -fno-exceptions)
target_include_directories(sead PUBLIC include/)
