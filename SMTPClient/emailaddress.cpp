#include "emailaddress.h"

EmailAddress::EmailAddress()
{
}

EmailAddress::EmailAddress(QString address, QString password)
{
    this->address_  = address;
    this->password_ = password;
}

QString EmailAddress::address() const
{
    return address_;
}

void EmailAddress::setAddress(const QString &address)
{
    address_ = address;
}

QString EmailAddress::password() const
{
    return password_;
}

void EmailAddress::setPassword(const QString &password)
{
    password_ = password;
}
