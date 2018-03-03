#include "email.h"

Email::Email()
{
}

Email::Email(EmailAddress credentials, EmailAddress from, EmailAddress to, QString subject, QString content)
{
    credentials_ = credentials;
    from_ = from;
    to_ = to;
    subject_ = subject;
    content_ = content;
}

EmailAddress Email::credentials() const
{
    return credentials_;
}

void Email::setCredentials(const EmailAddress &credentials)
{
    credentials_ = credentials;
}

EmailAddress Email::from() const
{
    return from_;
}

void Email::setFrom(const EmailAddress &from)
{
    from_ = from;
}

EmailAddress Email::to() const
{
    return to_;
}

void Email::setTo(const EmailAddress &to)
{
    to_ = to;
}

QString Email::subject() const
{
    return subject_;
}

void Email::setSubject(const QString &subject)
{
    subject_ = subject;
}

QString Email::content() const
{
    return content_;
}

void Email::setContent(const QString &content)
{
    content_ = content;
}
