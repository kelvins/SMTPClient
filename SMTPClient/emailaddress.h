#ifndef EMAILADDRESS_H
#define EMAILADDRESS_H

#include <QString>

/**
 * @brief The EmailAddress class is used to store a email address
 * and a password when necessary.
 */
class EmailAddress
{
public:
    EmailAddress();
    EmailAddress(QString address, QString password = "");

    QString address() const;
    void setAddress(const QString &address);

    QString password() const;
    void setPassword(const QString &password);

private:
    QString address_;
    QString password_;
};

#endif // EMAILADDRESS_H
