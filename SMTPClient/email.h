#ifndef EMAIL_H
#define EMAIL_H

#include "emailaddress.h"

/**
 * @brief The Email class is responsible for storing the email information.
 */
class Email
{
public:
    Email();
    Email(EmailAddress sender, EmailAddress receiver, QString subject, QString content);

    QString content() const;
    void setContent(const QString &content);

    QString subject() const;
    void setSubject(const QString &subject);

    EmailAddress sender() const;
    void setSender(const EmailAddress &sender);

    EmailAddress receiver() const;
    void setReceiver(const EmailAddress &receiver);

private:
    QString content_;
    QString subject_;

    EmailAddress sender_;
    EmailAddress receiver_;
};

#endif // EMAIL_H
