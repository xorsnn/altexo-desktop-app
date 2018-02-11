
#ifndef AL_UTILS_EXPORT_H
#define AL_UTILS_EXPORT_H

#ifdef AL_UTILS_STATIC_DEFINE
#  define AL_UTILS_EXPORT
#  define AL_UTILS_NO_EXPORT
#else
#  ifndef AL_UTILS_EXPORT
#    ifdef al_utils_EXPORTS
        /* We are building this library */
#      define AL_UTILS_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define AL_UTILS_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef AL_UTILS_NO_EXPORT
#    define AL_UTILS_NO_EXPORT 
#  endif
#endif

#ifndef AL_UTILS_DEPRECATED
#  define AL_UTILS_DEPRECATED __declspec(deprecated)
#endif

#ifndef AL_UTILS_DEPRECATED_EXPORT
#  define AL_UTILS_DEPRECATED_EXPORT AL_UTILS_EXPORT AL_UTILS_DEPRECATED
#endif

#ifndef AL_UTILS_DEPRECATED_NO_EXPORT
#  define AL_UTILS_DEPRECATED_NO_EXPORT AL_UTILS_NO_EXPORT AL_UTILS_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef AL_UTILS_NO_DEPRECATED
#    define AL_UTILS_NO_DEPRECATED
#  endif
#endif

#endif
