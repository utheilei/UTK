#ifndef UTKWIDGET_EXPORT_H
#define UTKWIDGET_EXPORT_H

#include <QtCore/qglobal.h>

#define BUILD_SHARED_LIBS

#if defined(BUILD_SHARED_LIBS)
#  if defined(UTKWIDGET_LIBRARY)
#    define UTKWIDGET_EXPORT Q_DECL_EXPORT
#  else
#    define UTKWIDGET_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define UTKWIDGET_EXPORT
#endif

#endif // UTKWIDGET_EXPORT_H
