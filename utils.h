// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */

#pragma once
#include <libintl.h>

#include <QtCore/QString>
#include <QtGui/QPixmap>

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
