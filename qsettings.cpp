#include "qsettings.h"

static QSettings* g_psettings = nullptr;

QSettings* settings()
{
    if(!g_psettings)
        g_psettings = new QSettings;
    return g_psettings;
}

QSettings::QSettings(QObject *parent) : QObject(parent)
{

}




