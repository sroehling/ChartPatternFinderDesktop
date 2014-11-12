#include "WelcomeDialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QPushButton>
#include <QDebug>

WelcomeDialog::WelcomeDialog(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Welcome to Chart Pattern Finder");

    QLabel *welcomeLabel = new QLabel();
    welcomeLabel->setOpenExternalLinks(true);
    welcomeLabel->setWordWrap(true);
    welcomeLabel->setText("The first step with Chart Pattern Finder is to select a folder (directory) "
                           "to load quotes for pattern analysis.");

    QLabel *welcomeLabel2 = new QLabel();
    welcomeLabel2->setOpenExternalLinks(true);
    welcomeLabel2->setWordWrap(true);
    welcomeLabel2->setText("Please select a folder containing some CSV files formatted "
                          "with Open, High, Low, Close, and Volume columns. "
                          "You can also initially choose an empty folder, then later populate this folder with some CSV files.");

    QLabel *welcomeLabel3 = new QLabel();
    welcomeLabel3->setOpenExternalLinks(true);
    welcomeLabel3->setWordWrap(true);
    welcomeLabel3->setText("If you have further questions about feeding  "
                          " data into Chart Pattern Finder, please see the "
                           "<a href=\"https://www.chartpatternfinder.com/getting-started\">getting started documentation</a> on our web site.");

    QPushButton *selectQuotesButton = new QPushButton("Select Quotes Folder");
    QPushButton *quitButton = new QPushButton("Quit");
    quitButton->setDefault(false);
    selectQuotesButton->setDefault(true);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addSpacerItem(new QSpacerItem(150,10));
    buttonLayout->addWidget(quitButton,0,Qt::AlignRight);
    buttonLayout->addSpacerItem(new QSpacerItem(10,10));
    buttonLayout->addWidget(selectQuotesButton,0,Qt::AlignRight);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(welcomeLabel);
    mainLayout->addSpacerItem(new QSpacerItem(10,20));
    mainLayout->addWidget(welcomeLabel2);
    mainLayout->addSpacerItem(new QSpacerItem(10,20));
    mainLayout->addWidget(welcomeLabel3);
    mainLayout->addSpacerItem(new QSpacerItem(10,20));
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    setFixedHeight(sizeHint().height());

    connect(selectQuotesButton, SIGNAL(clicked()),this, SLOT(selectQuotesButtonClicked()));
    connect(quitButton, SIGNAL(clicked()),this, SLOT(quitButtonClicked()));

}

void WelcomeDialog::selectQuotesButtonClicked()
{
    qDebug() << "Select quotes button clicked";
    emit welcomeDialogSelectQuotesButtonPressed();
}

void WelcomeDialog::quitButtonClicked()
{
    qDebug() << "Quit button clicked";
    emit welcomeDialogQuitButtonPressed();
}


