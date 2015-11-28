// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#ifndef UTILS_H
#define UTILS_H


#include <libintl.h>

#include <qstring.h>
#include <qpixmap.h>

#ifdef WITHKDE
#include <klocale.h>
#endif


QPixmap*
load_qpixmap (const char* name);


#ifndef WITHKDE

inline QString i18n (const char* msgid)
{
    return QString::fromUtf8 (gettext(msgid));
}

inline QString i18n (const char* msgid1, const char* msgid2,
		     unsigned long int n)
{
    return QString::fromUtf8 (ngettext (msgid1, msgid2, n));
}

#endif


#endif
