#include "smtpclient.h"

SMTPClient::SMTPClient()
{
    initialize();
}

SMTPClient::SMTPClient(QString host, int port, int timeout)
{
    initialize();

    this->host_ = host;
    this->port_ = port;
    this->timeout_ = timeout;
}

void SMTPClient::initialize()
{
    this->port_ = 465;
    this->timeout_ = 30000;

    socket_ = new QSslSocket(this);

    connect(socket_, SIGNAL(readyRead()), this, SLOT(readyRead()));
}

SMTPClient::~SMTPClient()
{
    delete textStream_;
    delete socket_;
}

void SMTPClient::sendEmail(Email email)
{
    this->email_ = email;

    QString newContent = "To: " + email_.receiver().address() + "\n"; // To
    newContent.append("From: " + email_.sender().address() + "\n");   // From
    newContent.append("Subject: " + email_.subject() + "\n"); // Subject
    newContent.append(email_.content());                      // Content/Body

    newContent.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
    newContent.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));

    this->email_.setContent(newContent);

    state_ = Init;

    socket_->connectToHostEncrypted(host_, port_);

    if ( !socket_->waitForConnected(timeout_) )
    {
        emit status(Status::Failed, socket_->errorString());
        return;
    }

    textStream_ = new QTextStream(socket_);
}

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
        socket_->waitForEncrypted(timeout_);

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
        *textStream_ << QByteArray().append(email_.sender().address()).toBase64()  << "\r\n";
        textStream_->flush();

        state_ = Pass;
    }
    else if (state_ == Pass && responseLine == "334")
    {
        *textStream_ << QByteArray().append(email_.sender().password()).toBase64() << "\r\n";
        textStream_->flush();

        state_ = Mail;
    }
    else if ( state_ == Mail && responseLine == "235" )
    {
        *textStream_ << "MAIL FROM:<" << email_.sender().address() << ">\r\n";
        textStream_->flush();
        state_ = Rcpt;
    }
    else if ( state_ == Rcpt && responseLine == "250" )
    {
        *textStream_ << "RCPT TO:<" << email_.receiver().address() << ">\r\n"; //r
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
        deleteLater();
        return;
    }
    else
    {
        state_ = Close;
        emit status(Status::Failed, response_);
    }
}
