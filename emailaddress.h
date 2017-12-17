#ifndef EMAILADDRESS_H
#define EMAILADDRESS_H

#include <QString>

/**
 * @brief The EmailAddress class is used to store a name and an address.
 */
class EmailAddress
{
public:
    EmailAddress();
    EmailAddress(QString name, QString address);

    QString name() const;
    void setName(const QString &name);

    QString address() const;
    void setAddress(const QString &address);

private:
    QString name_;
    QString address_;
};

#endif // EMAILADDRESS_H
