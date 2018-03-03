# SMTPClient

A simple [SMTP (Simple Mail Transfer Protocol)][1] client to send e-mails using C++ and the Qt framework.

## How to use

Just include it in your `.PRO` file, like this:

```
include( $$PWD/SMTPClient.pri )
```

Then you can include the headers in your code:

```cpp
#include "SMTPClient/email.h"
#include "SMTPClient/smtpclient.h"
```

And send the e-mail, for example:

```cpp

/**
 * @brief Slot used to send the message.
 */
void MainWindow::sendEmail()
{
    // Create the email object
    Email email = createEmail();

    // Create the SMTPClient
    client_ = new SMTPClient(ui->lineEditHost->text(),
                             ui->spinBoxPort->value());

    // Connection used to receive the results
    connect(client_, SIGNAL(status(Status::e, QString)),
            this, SLOT(onStatus(Status::e, QString)), Qt::UniqueConnection);

    // Try to send the email
    client_->sendEmail(email);
}

/**
 * @brief Create and return an Email object based on the fields from the form.
 */
Email MainWindow::createEmail()
{
    // Create the credentials EmailAddress
    EmailAddress credentials(ui->lineEditEmailCredentials->text(),
                             ui->lineEditPasswordCredentials->text());

    // Create the from EmailAddress
    EmailAddress from(ui->lineEditEmailFrom->text());

    // Create the to EmailAddress
    EmailAddress to(ui->lineEditEmailTo->text());

    // Create the email
    Email email(credentials,
                from,
                to,
                ui->lineEditSubject->text(),
                ui->textEditContent->toPlainText());

    return email;
}
```

## License

This project was created under the [GNU General Public License v3.0][2].

 [1]: https://pt.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol
 [2]: LICENSE
