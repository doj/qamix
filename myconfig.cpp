/*
 *  Author: Arvin Schnell <arvin@suse.de>
 */


#include "myconfig.h"


QString
MyConfig::readString (const char* group, const char* key, const char* dfl_value)
{
    return qsettings->readEntry (mkbigkey (group, key), QString::fromUtf8 (dfl_value));
}


void
MyConfig::writeString (const char* group, const char* key, const char* value)
{
    qsettings->writeEntry (mkbigkey (group, key), QString::fromUtf8 (value));
}


QString
MyConfig::readString (const char* group, const char* key, const QString& dfl_value)
{
    return qsettings->readEntry (mkbigkey (group, key), dfl_value);
}


void
MyConfig::writeString (const char* group, const char* key, const QString& value)
{
    qsettings->writeEntry (mkbigkey (group, key), value);
}


int
MyConfig::readNum (const char* group, const char* key, int dfl_value)
{
    return qsettings->readNumEntry (mkbigkey (group, key), dfl_value);
}


void
MyConfig::writeNum (const char* group, const char* key, int value)
{
    qsettings->writeEntry (mkbigkey (group, key), value);
}


bool
MyConfig::readBool (const char* group, const char* key, bool dfl_value)
{
    return qsettings->readBoolEntry (mkbigkey (group, key), dfl_value);
}


void
MyConfig::writeBool (const char* group, const char* key, bool value)
{
    qsettings->writeEntry (mkbigkey (group, key), value);
}


void
MyConfig::sync ()
{
}


QString
MyConfig::mkbigkey (const char* group, const char* key) const
{
    return QString ("/qinternet/") + group + "/" + key;
}
