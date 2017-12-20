#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sendEmail();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendEmail()
{
    EmailAddress from("", "");
    EmailAddress to("");

    Email email(from, to, "test", "test123345");

    SMTPClient *client = new SMTPClient("smtp.gmail.com", 465);
    connect(client, SIGNAL(status(Status::e, QString)), this, SLOT(onStatus(Status::e, QString)));
    client->sendEmail(email);
}

void MainWindow::onStatus(Status::e status, QString errorMessage)
{
    if( status == Status::Success )
    {
        QMessageBox::information(NULL, tr("SMTPClient"), tr("Message successfully sent."));
    }
    else if( status == Status::Failed )
    {
        QMessageBox::warning(NULL, tr("SMTPClient"), errorMessage);
    }
}
