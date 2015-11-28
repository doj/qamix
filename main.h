

#include <qstringlist.h>

#ifdef WITHKDE
#include <kamixunique.h>
#else
#include <qapplication.h>
#endif

#include "myconfig.h"


#ifdef WITHKDE
extern KAMixUnique* app;
#else
extern QApplication* app;
#endif

extern MyConfig* myconfig;

extern QStringList card_list;

