#ifndef QTOPC_GLOBAL_H
#define QTOPC_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QTOPC_LIBRARY)
#  define QTOPCSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTOPCSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTOPC_GLOBAL_H
