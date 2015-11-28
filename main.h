#pragma once

#include <QtCore/QStringList>

#ifdef WITHKDE
#include <kamixunique.h>
#else
#include <QtGui/QApplication>
#endif

#include "myconfig.h"


#ifdef WITHKDE
extern KAMixUnique* app;
#else
extern QApplication* app;
#endif

extern MyConfig* myconfig;

extern QStringList card_list;
