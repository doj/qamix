

/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include "myconfig.h"


QString
MyConfig::readString (const char* group, const char* key, const char* dfl_value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    return kconfig->readEntry (key, QString::fromUtf8 (dfl_value));
#else
    return qsettings->readEntry (mkbigkey (group, key), QString::fromUtf8 (dfl_value));
#endif
}


void
MyConfig::writeString (const char* group, const char* key, const char* value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    kconfig->writeEntry (key, QString::fromUtf8 (value));
#else
    qsettings->writeEntry (mkbigkey (group, key), QString::fromUtf8 (value));
#endif
}


QString
MyConfig::readString (const char* group, const char* key, const QString& dfl_value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    return kconfig->readEntry (key, dfl_value);
#else
    return qsettings->readEntry (mkbigkey (group, key), dfl_value);
#endif
}


void
MyConfig::writeString (const char* group, const char* key, const QString& value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    kconfig->writeEntry (key, value);
#else
    qsettings->writeEntry (mkbigkey (group, key), value);
#endif
}


int
MyConfig::readNum (const char* group, const char* key, int dfl_value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    return kconfig->readNumEntry (key, dfl_value);
#else
    return qsettings->readNumEntry (mkbigkey (group, key), dfl_value);
#endif
}


void
MyConfig::writeNum (const char* group, const char* key, int value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    kconfig->writeEntry (key, value);
#else
    qsettings->writeEntry (mkbigkey (group, key), value);
#endif
}


bool
MyConfig::readBool (const char* group, const char* key, bool dfl_value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    return kconfig->readBoolEntry (key, dfl_value);
#else
    return qsettings->readBoolEntry (mkbigkey (group, key), dfl_value);
#endif
}


void
MyConfig::writeBool (const char* group, const char* key, bool value)
{
#ifdef WITHKDE
    kconfig->setGroup (group);
    kconfig->writeEntry (key, value);
#else
    qsettings->writeEntry (mkbigkey (group, key), value);
#endif
}


void
MyConfig::sync ()
{
#ifdef WITHKDE
    kconfig->sync ();
#else
    // What can I do?
#endif
}


#ifndef WITHKDE
QString
MyConfig::mkbigkey (const char* group, const char* key) const
{
    return QString ("/qinternet/") + group + "/" + key;
}
#endif
