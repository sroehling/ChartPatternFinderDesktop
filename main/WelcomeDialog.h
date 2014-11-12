#ifndef WELCOMEDIALOG_H
#define WELCOMEDIALOG_H

#include <QDialog>

class WelcomeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit WelcomeDialog(QWidget *parent = 0);

signals:
    void welcomeDialogSelectQuotesButtonPressed();
    void welcomeDialogQuitButtonPressed();

private slots:
    void selectQuotesButtonClicked();
    void quitButtonClicked();

};

#endif // WELCOMEDIALOG_H
