#include "smtpclient.h"

/**
 * @brief Set the values, create the QSslSocket object and make the connections.
 */
SMTPClient::SMTPClient(QString host, int port, int conTimeout, int timeout)
{
    // Set host and port
    host_ = host;
    port_ = port;

    // Set the timeout variables
    conTimeout_ = conTimeout;
    timeout_ = timeout;

    textStream_ = NULL;

    // Create the socket object and connect it to the readyRead slot
    socket_ = new QSslSocket(this);

    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

SMTPClient::~SMTPClient()
{
    delete textStream_;

    socket_->abort();
    socket_->deleteLater();
}

/**
 * @brief Send the email using a SMTP protocol.
 * @param email - Contain the data used to send the email (e.g. credentials, subject, body).
 */
void SMTPClient::sendEmail(Email email)
{
    email_ = email;

    QString newContent = "";

    // Create a header used by the email server
    newContent.append("To: "   + email_.to().address()   + "\n"); // To
    newContent.append("From: " + email_.from().address() + "\n"); // From
    newContent.append("Subject: " + email_.subject() + "\n\n");   // Subject
    newContent.append(email_.content());                          // Content/Body

    newContent.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    newContent.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    email_.setContent(newContent);

    state_ = Init;

    // Connect to host
    socket_->connectToHostEncrypted(host_, port_);

    // Wait for the connection
    if ( !socket_->waitForConnected(conTimeout_) )
    {
        emit status(Status::Failed, socket_->errorString());
        return;
    }

    // Create the text stream object
    textStream_ = new QTextStream(socket_);

    // Call the abort function when reach the timeout
    QTimer::singleShot(timeout_, this, SLOT(abort()));
}

/**
 * @brief If the timeout is reached it will abort the sending
 * email process no matter what is happening.
 */
void SMTPClient::abort()
{
    if( socket_->isOpen() )
    {
        // Aborts the current connection and resets the socket
        socket_->abort();

        state_ = Close;

        emit status(Status::Failed, "Timeout reached, the procedure was canceled!");
    }
}

/**
 * @brief Get the responses from the server.
 *
 * SMTP server response codes:
 *
 * 250 – This SMTP server response simply means everything went
 * well and your message was delivered to the recipient server.
 *
 * 421 – Your message was temporarily deferred by the recipient
 * server. This is usually a result of too many connections in
 * a short timeframe or too many messages.
 *
 * 450 – Your message was not delivered because the other user
 * mailbox was not available. This can happen if the mailbox is
 * locked or is not routable.
 *
 * 451 – This response is sent when the message simply failed.
 * Often times this is not caused by you, but rather because of
 * a far-end server problem.
 *
 * 452 – This kind of response is sent back when there isn’t
 * enough system storage to send the message. Your message is
 * deferred until storage opens up and it can then be delivered.
 *
 * 550 – The message has failed because the other user’s mailbox
 * is unavailable or because the recipient server rejected your message.
 *
 * 551 – The mailbox your message was intended for does not exist on
 * the recipient server.
 *
 * 552 – The mailbox your message was sent to does not have enough
 * storage to accept your message.
 *
 * 553 – You message was not delivered because the name of the mailbox
 * you sent to does not exist.
 *
 * 554 – This is a very vague message failure response that can refer to
 * any number of problems either on your end or with the recipient server.
 *
 * References:
 * https://morf.lv/simple-tls-ssl-smtp-client-for-qt5
 * https://sendgrid.com/blog/smtp-server-response-codes-explained/
 * https://pt.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol
 */
void SMTPClient::readyRead()
{
    QString responseLine;

    do
    {
        responseLine = socket_->readLine();
        response_ += responseLine;
    } while ( socket_->canReadLine() && responseLine[3] != ' ' );

    responseLine.truncate(3);

    if ( state_ == Init && responseLine == "220" )
    {
        *textStream_ << "EHLO localhost" <<"\r\n";
        textStream_->flush();

        state_ = HandShake;
    }
    else if (state_ == HandShake && responseLine == "250")
    {
        socket_->startClientEncryption();
        socket_->waitForEncrypted(conTimeout_);

        *textStream_ << "EHLO localhost" << "\r\n";
        textStream_->flush();

        state_ = Auth;
    }
    else if (state_ == Auth && responseLine == "250")
    {
        *textStream_ << "AUTH LOGIN" << "\r\n";
        textStream_->flush();

        state_ = User;
    }
    else if (state_ == User && responseLine == "334")
    {
        // Use XOAUTH2 protocol, which means that password and username has to be sent in base64 coding
        *textStream_ << QByteArray().append(email_.credentials().address()).toBase64()  << "\r\n";
        textStream_->flush();

        state_ = Pass;
    }
    else if (state_ == Pass && responseLine == "334")
    {
        *textStream_ << QByteArray().append(email_.credentials().password()).toBase64() << "\r\n";
        textStream_->flush();

        state_ = Mail;
    }
    else if ( state_ == Mail && responseLine == "235" )
    {
        *textStream_ << "MAIL FROM:<" << email_.from().address() << ">\r\n";
        textStream_->flush();

        state_ = Rcpt;
    }
    else if ( state_ == Rcpt && responseLine == "250" )
    {
        *textStream_ << "RCPT TO:<" << email_.to().address() << ">\r\n";
        textStream_->flush();

        state_ = Data;
    }
    else if ( state_ == Data && responseLine == "250" )
    {
        *textStream_ << "DATA\r\n";
        textStream_->flush();

        state_ = Body;
    }
    else if ( state_ == Body && responseLine == "354" )
    {
        *textStream_ << email_.content() << "\r\n.\r\n";
        textStream_->flush();

        state_ = Quit;
    }
    else if ( state_ == Quit && responseLine == "250" )
    {
        *textStream_ << "QUIT\r\n";
        textStream_->flush();

        state_ = Close;

        emit status(Status::Success);
    }
    else if ( state_ == Close )
    {
        return;
    }
    else
    {
        state_ = Close;

        emit status(Status::Failed, response_);
    }
}
