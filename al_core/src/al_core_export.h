
#ifndef AL_CORE_EXPORT_H
#define AL_CORE_EXPORT_H

#ifdef AL_CORE_STATIC_DEFINE
#  define AL_CORE_EXPORT
#  define AL_CORE_NO_EXPORT
#else
#  ifndef AL_CORE_EXPORT
#    ifdef al_core_EXPORTS
        /* We are building this library */
#      define AL_CORE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define AL_CORE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef AL_CORE_NO_EXPORT
#    define AL_CORE_NO_EXPORT 
#  endif
#endif

#ifndef AL_CORE_DEPRECATED
#  define AL_CORE_DEPRECATED __declspec(deprecated)
#endif

#ifndef AL_CORE_DEPRECATED_EXPORT
#  define AL_CORE_DEPRECATED_EXPORT AL_CORE_EXPORT AL_CORE_DEPRECATED
#endif

#ifndef AL_CORE_DEPRECATED_NO_EXPORT
#  define AL_CORE_DEPRECATED_NO_EXPORT AL_CORE_NO_EXPORT AL_CORE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef AL_CORE_NO_DEPRECATED
#    define AL_CORE_NO_DEPRECATED
#  endif
#endif

#endif
