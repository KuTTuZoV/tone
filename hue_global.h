#ifndef HUE_GLOBAL_H
#define HUE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HUE_LIBRARY)
#  define HUE_EXPORT Q_DECL_EXPORT
#else
#  define HUE_EXPORT Q_DECL_IMPORT
#endif

#endif // HUE_GLOBAL_H
