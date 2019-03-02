#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(CONTEXTFREEGRAMMAR_LIB)
#  define CONTEXTFREEGRAMMAR_EXPORT Q_DECL_EXPORT
# else
#  define CONTEXTFREEGRAMMAR_EXPORT Q_DECL_IMPORT
# endif
#else
# define CONTEXTFREEGRAMMAR_EXPORT
#endif
