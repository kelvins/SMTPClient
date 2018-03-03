#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QString>
#include <QMainWindow>
#include <QMessageBox>

#include "../SMTPClient/email.h"
#include "../SMTPClient/smtpclient.h"
#include "../SMTPClient/emailaddress.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sendEmail();
    void clearFields();

    void onStatus(Status::e status, QString errorMessage);

private:
    Email createEmail();

private:
    Ui::MainWindow *ui;
    SMTPClient *client_;
};

#endif // MAINWINDOW_H
