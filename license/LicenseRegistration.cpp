#include "LicenseRegistration.h"
#include "LicenseKey.h"
#include <QDebug>
#include "SettingsHelper.h"

#define LICENSE_SETTINGS_EMAIL_KEY "LICENSE_EMAIL"
#define LICENSE_SETTINGS_LICENSE_KEY "LICENSE_KEY"

LicenseRegistration::LicenseRegistration()
{
    licenseSettings_ = settingsHelper::openUserSettings();
    qDebug() << "License settings file: " << licenseSettings_->fileName();
}

bool LicenseRegistration::fullVersionLicenseRegistered() const
{
    if(!licenseSettings_->contains(LICENSE_SETTINGS_EMAIL_KEY))
    {
        return false;
    }
    QString licenseEmail = licenseSettings_->value(LICENSE_SETTINGS_EMAIL_KEY).toString();

    if(!licenseSettings_->contains(LICENSE_SETTINGS_LICENSE_KEY))
    {
        return false;
    }
    QString licenseKeyStr = licenseSettings_->value(LICENSE_SETTINGS_LICENSE_KEY).toString();

    if(!licenseKey::validLicenseKey(licenseEmail,licenseKeyStr))
    {
        return false;
    }
    return true;

}

void LicenseRegistration::registerLicenseKey(const QString &emailAddress, const QString &licenseKeyStr)
{
    if(licenseKey::validLicenseKey(emailAddress,licenseKeyStr))
    {
        licenseSettings_->setValue(LICENSE_SETTINGS_EMAIL_KEY,QVariant(emailAddress));
        licenseSettings_->setValue(LICENSE_SETTINGS_LICENSE_KEY,QVariant(licenseKeyStr));
        licenseSettings_->sync();
    }
    emit licenseRegistrationComplete();
}



