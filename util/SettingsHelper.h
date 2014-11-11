#ifndef SETTINGSHELPER_H
#define SETTINGSHELPER_H

#include <QSettings>
#include <boost/shared_ptr.hpp>


typedef boost::shared_ptr<QSettings> QSettingsPtr;


namespace settingsHelper
{
    QSettingsPtr openUserSettings();
}

#endif // SETTINGSHELPER_H
