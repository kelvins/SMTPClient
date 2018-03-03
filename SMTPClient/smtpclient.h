#ifndef SMTPCLIENT_H
#define SMTPCLIENT_H

#include <QTimer>
#include <QObject>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QAbstractSocket>

#include "email.h"

namespace Status {
    enum e {
        Success,
        Failed
    };
}

/**
 * @brief The SMTPClient class is used to
 */
class SMTPClient : public QObject
{
    Q_OBJECT

public:
    // Default port 25 or 465 for cryptography connection through SSL
    SMTPClient(QString host,
               int port = 465,
               int conTimeout = 30000,
               int timeout = 60000);

    ~SMTPClient();

    void sendEmail(Email email);

signals:
    void status(Status::e status, QString errorMessage = "");

private slots:
    void readyRead();
    void abort();

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

    QString host_;
    int port_;
    int conTimeout_;
    int timeout_;

    Email email_;

    QSslSocket *socket_;
    QTextStream *textStream_;

    int state_;
    QString response_;
};

#endif // SMTPCLIENT_H
