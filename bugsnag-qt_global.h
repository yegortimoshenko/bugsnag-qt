#pragma once
#include <QtCore/qglobal.h>

#if defined(BUGSNAGQT_LIBRARY)
#define BUGSNAGQTSHARED_EXPORT Q_DECL_EXPORT
#else
#define BUGSNAGQTSHARED_EXPORT Q_DECL_IMPORT
#endif
