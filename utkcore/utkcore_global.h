#ifndef UTKCORE_GLOBAL_H
#define UTKCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(UTKCORE_LIBRARY)
#  define UTKCORE_EXPORT Q_DECL_EXPORT
#else
#  define UTKCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // UTKCORE_GLOBAL_H
