#include "emailaddress.h"

EmailAddress::EmailAddress()
{
}

EmailAddress::EmailAddress(QString name, QString address)
{
    this->name_ = name;
    this->address_ = address;
}

QString EmailAddress::name() const
{
    return name_;
}

void EmailAddress::setName(const QString &name)
{
    name_ = name;
}

QString EmailAddress::address() const
{
    return address_;
}

void EmailAddress::setAddress(const QString &address)
{
    address_ = address;
}
