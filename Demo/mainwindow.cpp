#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client_ = NULL;

    // Connect all signals and slots
    connect(ui->pushButtonSend, SIGNAL(clicked(bool)), this, SLOT(sendEmail()));
    connect(ui->pushButtonClear, SIGNAL(clicked(bool)), this, SLOT(clearFields()));
}

MainWindow::~MainWindow()
{
    delete client_;
    delete ui;
}

/**
 * @brief Slot used to send the message.
 */
void MainWindow::sendEmail()
{
    // Create the Sender
    EmailAddress sender(ui->lineEditEmailFrom->text(),
                        ui->lineEditPasswordFrom->text());

    // Create the receiver
    EmailAddress receiver(ui->lineEditEmailTo->text());

    // Create the email
    Email email(sender,
                receiver,
                ui->lineEditSubject->text(),
                ui->textEditContent->toPlainText());

    // Create the SMTPClient
    client_ = new SMTPClient(ui->lineEditHost->text(),
                             ui->spinBoxPort->value());

    // Connection used to receive the results
    connect(client_, SIGNAL(status(Status::e, QString)),
            this, SLOT(onStatus(Status::e, QString)), Qt::UniqueConnection);

    // Try to send the email
    client_->sendEmail(email);
}

/**
 * @brief Function responsible for clear all fields.
 * Note: it will not change the Host and Port fields.
 */
void MainWindow::clearFields()
{
    ui->lineEditEmailFrom->clear();
    ui->lineEditPasswordFrom->clear();
    ui->lineEditEmailTo->clear();
    ui->lineEditSubject->clear();
    ui->textEditContent->clear();
}

/**
 * @brief Slot the receives a result from the SMTPClient object.
 * @param status - The status (Success or Failed)
 * @param errorMessage - The error message if the status is Failed
 */
void MainWindow::onStatus(Status::e status, QString errorMessage)
{
    // Show a message based on the Status
    switch (status)
    {
    case Status::Success:
        QMessageBox::information(NULL, tr("SMTPClient"), tr("Message successfully sent!"));
        break;
    case Status::Failed:
    {
        QMessageBox::warning(NULL, tr("SMTPClient"), tr("Could not send the message!"));
        qCritical() << errorMessage;
    }
        break;
    default:
        break;
    }

    // Delete the client pointer
    delete client_;
    client_ = NULL;
}
