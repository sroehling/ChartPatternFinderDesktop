#include "SettingsHelper.h"

#define SETTING_APP_NAME "ChartPatternFinder"
#define SETTING_ORGANIZATION_NAME "Resultra"

namespace settingsHelper {

QSettingsPtr openUserSettings()
{
    return QSettingsPtr(new QSettings(QSettings::NativeFormat,
                           QSettings::UserScope,QString(SETTING_APP_NAME),
                           QString(SETTING_ORGANIZATION_NAME)));
}

} // namespace
