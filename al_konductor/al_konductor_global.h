#ifndef AL_KONDUCTOR_GLOBAL_H
#define AL_KONDUCTOR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AL_KONDUCTOR_LIBRARY)
#  define AL_KONDUCTORSHARED_EXPORT Q_DECL_EXPORT
#else
#  define AL_KONDUCTORSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // AL_KONDUCTOR_GLOBAL_H