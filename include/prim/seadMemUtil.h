#ifndef SEAD_MEM_UTIL_H_
#define SEAD_MEM_UTIL_H_

#ifdef __cplusplus

#ifdef cafe
#include <prim/cafe/seadMemUtilCafe.hpp>
#elif SWITCH
#else
#error "Unknown platform"
#endif // cafe

#endif // __cplusplus

#endif // SEAD_MEM_UTIL_H_
