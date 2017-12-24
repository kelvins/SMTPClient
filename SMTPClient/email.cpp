#include "email.h"

Email::Email()
{
}

Email::Email(EmailAddress sender, EmailAddress receiver, QString subject, QString content)
{
    sender_   = sender;
    receiver_ = receiver;
    subject_  = subject;
    content_  = content;
}

QString Email::content() const
{
    return content_;
}

void Email::setContent(const QString &content)
{
    content_ = content;
}

QString Email::subject() const
{
    return subject_;
}

void Email::setSubject(const QString &subject)
{
    subject_ = subject;
}

EmailAddress Email::sender() const
{
    return sender_;
}

void Email::setSender(const EmailAddress &sender)
{
    sender_ = sender;
}

EmailAddress Email::receiver() const
{
    return receiver_;
}

void Email::setReceiver(const EmailAddress &receiver)
{
    receiver_ = receiver;
}
