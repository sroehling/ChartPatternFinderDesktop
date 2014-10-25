#include "RegisterDialog.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include "LicenseKey.h"
#include "LicenseRegistration.h"

RegisterDialog::RegisterDialog(const LicenseRegistrationPtr &licenseRegistration) :
    QDialog(0)
{
    setModal(true);
    licenseRegistration_ = licenseRegistration;

    QLabel *helpLabel = new QLabel();
    helpLabel->setOpenExternalLinks(true);
    helpLabel->setWordWrap(true);
    helpLabel->setText("Enter your email address and the license key you received after purchasing "
                           "the full version upgrade of Chart Pattern Finder. "
                           "If you haven't purchased the upgrade yet, this can be  "
                           "<a href=\"http://www.chartpatternfinder.com/buy\">purchased through our web site</a>.");


    QLabel *emailLabel = new QLabel("Email:");
    emailEntry_ = new QLineEdit;
    QHBoxLayout *emailLayout = new QHBoxLayout;
    emailLayout->addWidget(emailLabel,0,Qt::AlignRight);
    emailLayout->addWidget(emailEntry_,0,Qt::AlignRight);
    emailEntry_->setMinimumWidth(300);

    QLabel *licenseKeyLabel = new QLabel("License Key:");
    licenseKeyEntry_ = new QLineEdit;
    licenseKeyEntry_->setMinimumWidth(300);
    QHBoxLayout *licenseKeyLayout = new QHBoxLayout;
    licenseKeyLayout->addWidget(licenseKeyLabel,0,Qt::AlignRight);
    licenseKeyLayout->addWidget(licenseKeyEntry_,0,Qt::AlignRight);

    cancelButton_ = new QPushButton("Cancel");
    registerButton_ = new QPushButton("Register");
    registerButton_->setEnabled(false);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addSpacerItem(new QSpacerItem(150,10));
    buttonLayout->addWidget(cancelButton_,0,Qt::AlignRight);
    buttonLayout->addSpacerItem(new QSpacerItem(10,10));
    buttonLayout->addWidget(registerButton_,0,Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(helpLabel);
    mainLayout->addSpacerItem(new QSpacerItem(10,20));

    mainLayout->addLayout(emailLayout);
    mainLayout->addLayout(licenseKeyLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setWindowTitle("Register Chart Pattern Finder");
    setFixedHeight(sizeHint().height());

    connect(emailEntry_, SIGNAL(textChanged(const QString &)),this, SLOT(emailTextChanged(const QString &)));
    connect(licenseKeyEntry_, SIGNAL(textChanged(const QString &)),this, SLOT(licenseTextChanged(const QString &)));
    connect(registerButton_, SIGNAL(clicked()),this, SLOT(registerButtonClicked()));
    connect(cancelButton_, SIGNAL(clicked()),this, SLOT(cancelButtonClicked()));

}

void RegisterDialog::enableRegistration()
{
    registerButton_->setEnabled(true);
    registerButton_->setDefault(true);
    cancelButton_->setDefault(false);
}

void RegisterDialog::disableRegistration()
{
    registerButton_->setEnabled(false);
    registerButton_->setDefault(false);
    cancelButton_->setDefault(true);
}

bool RegisterDialog::currentLicenseKeyEntryValid()
{
    if(licenseKey::validLicenseKey(emailEntry_->text(),licenseKeyEntry_->text()))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void RegisterDialog::updateRegistrationEnabled()
{
    if(currentLicenseKeyEntryValid())
    {
        enableRegistration();
    }
    else
    {
        disableRegistration();
    }
}


void RegisterDialog::emailTextChanged(const QString &emailText)
{
    qDebug() << "Email text changed:" << emailText;
    updateRegistrationEnabled();
}

void RegisterDialog::licenseTextChanged(const QString &licenseText)
{
    qDebug() << "License key text changed:" << licenseText;
    updateRegistrationEnabled();
}

void RegisterDialog::registerButtonClicked()
{
    qDebug() << "Register button clicked";
    assert(licenseKey::validLicenseKey(emailEntry_->text(),licenseKeyEntry_->text()));
    licenseRegistration_->registerLicenseKey(emailEntry_->text(),licenseKeyEntry_->text());
    close();

}

void RegisterDialog::cancelButtonClicked()
{
    qDebug() << "Cancel button clicked";
    close();
}

