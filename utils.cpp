/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */

#include "utils.h"

QPixmap*
load_qpixmap (const char* name)
{
    QString tmp = "/usr/share/qamix/icons/";
    tmp.append (name);
    return new QPixmap (tmp);
}
