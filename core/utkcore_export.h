#ifndef UTKWIDGET_EXPORT_H
#define UTKWIDGET_EXPORT_H

#include <QtCore/qglobal.h>

#define BUILD_SHARED_LIBS

#if defined(BUILD_SHARED_LIBS)
#  if defined(UTKCORE_LIBRARY)
#    define UTKCORE_EXPORT Q_DECL_EXPORT
#  else
#    define UTKCORE_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define UTKCORE_EXPORT
#endif

#endif // UTKWIDGET_EXPORT_H
