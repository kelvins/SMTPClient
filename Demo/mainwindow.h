#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void onStatus(Status::e status, QString errorMessage);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
