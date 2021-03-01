#ifndef NETSEND_GLOBAL_H
#define NETSEND_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NETSEND_LIBRARY)
#  define NETSENDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NETSENDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NETSEND_GLOBAL_H
