

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include <qstring.h>

#ifdef WITHKDE
#include <kstandarddirs.h>
#endif

#include "utils.h"


QPixmap*
load_qpixmap (const char* name)
{
#ifdef WITHKDE
    QString tmp = "kamix/icons/hicolor/22x22/";
    tmp.append (name);
    tmp = locate ("data", tmp);
#else
    QString tmp = "/usr/share/qamix/icons/";
    tmp.append (name);
#endif

    return new QPixmap (tmp);
}
