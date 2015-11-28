// -*- c++ -*-

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */

#pragma once

#include <QtCore/QSettings>

class MyConfig
{
public:

    MyConfig (QSettings* qsettings) : qsettings (qsettings) { }

    QSettings* get_qsettings () { return qsettings; }

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

    QSettings* qsettings;
    QString mkbigkey (const char*, const char*) const;
};
