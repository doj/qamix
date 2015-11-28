// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#ifndef MYCONFIG_H
#define MYCONFIG_H


#ifdef WITHKDE
#include <kconfig.h>
#else
#include <qsettings.h>
#endif


class MyConfig
{
public:

#ifdef WITHKDE
    MyConfig (KConfig* kconfig) : kconfig (kconfig) { }
#else
    MyConfig (QSettings* qsettings) : qsettings (qsettings) { }
#endif

#ifdef WITHKDE
    KConfig* get_kconfig () { return kconfig; }
#else
    QSettings* get_qsettings () { return qsettings; }
#endif

    QString readString (const char*, const char*, const char*);
    void writeString (const char*, const char*, const char*);

    QString readString (const char*, const char*, const QString&);
    void writeString (const char*, const char*, const QString&);

    int readNum (const char*, const char*, int);
    void writeNum (const char*, const char*, int);

    bool readBool (const char*, const char*, bool);
    void writeBool (const char*, const char*, bool);

    void sync ();

private:

#ifdef WITHKDE
    KConfig* kconfig;
#else
    QSettings* qsettings;
    QString mkbigkey (const char*, const char*) const;
#endif
};


#endif
