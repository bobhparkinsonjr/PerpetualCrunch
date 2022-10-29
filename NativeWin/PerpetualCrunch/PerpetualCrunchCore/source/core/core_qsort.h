#if !defined ( CORE_QSORT_H )
#define CORE_QSORT_H

namespace crunch { namespace core {

#if defined _CRUNCH_WINDOWS
extern void core_qsort( void *base, size_t num, size_t width, int (__cdecl *compare)(void *, const void *, const void *), void * context );
#else
extern void core_qsort( void *base, size_t num, size_t width, int (*compare)(void *, const void *, const void *), void * context );
#endif

} }

#endif
