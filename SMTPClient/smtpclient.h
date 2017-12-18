#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QAbstractSocket>

#include "email.h"

namespace Status {
    enum e {
        Success,
        Failed
    };
};

/**
 * @brief The SMTPClient class
 */
class SMTPClient : public QObject
{
    Q_OBJECT
public:
    SMTPClient();
    SMTPClient(QString host, int port, int timeout = 30000);

    ~SMTPClient();

    void sendEmail(Email email);

private:
    void initialize();

signals:
    void status(Status::e status, QString errorMessage = "");

private slots:
    void readyRead();

private:
    enum states{
        Tls,
        HandShake,
        Auth,
        User,
        Pass,
        Rcpt,
        Mail,
        Data,
        Init,
        Body,
        Quit,
        Close
    };

    int port_;
    int timeout_;
    QString host_;

    Email email_;
    QTextStream *textStream_;

    QSslSocket *socket_;

    int state_;
    QString response_;
};

#endif // SMTPCLIENT_H
