#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "LicenseRegistration.h"

class RegisterDialog : public QDialog
{
    Q_OBJECT
private:
    QLineEdit *licenseKeyEntry_;
    QLineEdit *emailEntry_;
    QPushButton *registerButton_;
    QPushButton *cancelButton_;

    LicenseRegistrationPtr licenseRegistration_;

    void enableRegistration();
    void disableRegistration();
    bool currentLicenseKeyEntryValid();
    void updateRegistrationEnabled();
public:
    explicit RegisterDialog(const LicenseRegistrationPtr &licenseRegistration);

signals:

public slots:
private slots:
    void emailTextChanged(const QString &);
    void licenseTextChanged(const QString &);
    void registerButtonClicked();
    void cancelButtonClicked();


};

#endif // REGISTERDIALOG_H
