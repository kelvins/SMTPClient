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
#include "SMTPClient/emailaddress.h"
```

And send the e-mail, for example:

```cpp
void MainWindow::sendEmail()
{
    // Create the Sender
    EmailAddress sender("test@gmail.com", "123");

    // Create the receiver
    EmailAddress receiver("test2@gmail.com");

    // Create the email
    Email email(sender,
                receiver,
                "Testing",
                "Lorem Ipsum");

    // Create the SMTPClient
    client_ = new SMTPClient("smtp.gmail.com", 465);

    // Connection used to receive the results
    connect(client_, SIGNAL(status(Status::e, QString)), this, SLOT(onStatus(Status::e, QString)), Qt::UniqueConnection);

    // Try to send the email
    client_->sendEmail(email);
}
```

 [1]: https://pt.wikipedia.org/wiki/Simple_Mail_Transfer_Protocol
