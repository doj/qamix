// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */

#pragma once
#include <libintl.h>

#include <QtCore/QString>
#include <QtGui/QPixmap>

QPixmap*
load_qpixmap (const char* name);

inline QString i18n (const char* msgid)
{
    return QString::fromUtf8 (gettext(msgid));
}

inline QString i18n (const char* msgid1, const char* msgid2,
		     unsigned long int n)
{
    return QString::fromUtf8 (ngettext (msgid1, msgid2, n));
}
