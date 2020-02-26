/*
            libEDGE for GHS -PPC-(Custom Header)

        I literally do not know any other way to use
    this library than to manually define a header for it.
*/

#ifndef _PPC_EDG_H
#define _PPC_EDG_H

#if defined(__ppc) && defined(__EDG__)
#ifdef __cplusplus
extern "C" {
#endif

void* __memzero(void* __s, size_t __n);

#ifdef __cplusplus
}
#endif

#endif   /* __ppc */
#endif /* _PPC_EDG_H */
