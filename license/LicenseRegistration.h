#ifndef LICENSEREGISTRATION_H
#define LICENSEREGISTRATION_H

#include <QObject>
#include "SettingsHelper.h"

class LicenseRegistration : public QObject {

    Q_OBJECT

    private:
        QSettingsPtr licenseSettings_;
    public:
        LicenseRegistration();
        bool fullVersionLicenseRegistered() const;
        void registerLicenseKey(const QString &emailAddress, const QString &licenseKeyStr);
        virtual ~LicenseRegistration() {}
    signals:
        void licenseRegistrationComplete();
};

typedef boost::shared_ptr<LicenseRegistration> LicenseRegistrationPtr;


#endif // LICENSEREGISTRATION_H
