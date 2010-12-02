/*
    Certain enhancements (www.xtuple.com/trojita-enhancements)
    are copyright © 2010 by OpenMFG LLC, dba xTuple.  All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    - Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
    - Neither the name of xTuple nor the names of its contributors may be used to
    endorse or promote products derived from this software without specific prior
    written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#ifndef MAILSYNCHRONIZER_H
#define MAILSYNCHRONIZER_H

#include <QObject>
#include <QModelIndex>

namespace Imap {
namespace Mailbox {
class Model;
}
}

namespace XtConnect {

class MailboxFinder;

/** @short Download messages into the DB */
class MailSynchronizer : public QObject
{
    Q_OBJECT
public:
    explicit MailSynchronizer( QObject *parent, Imap::Mailbox::Model *model, MailboxFinder *finder );
    void setMailbox( const QString &mailbox );
public slots:
    void slotRowsInserted( const QModelIndex &parent, int start, int end );
    void slotMailboxFound( const QString &mailbox, const QModelIndex &index );
private:
    /** @short Walk through the cached messages and store the new ones */
    void walkThroughMessages();

    Imap::Mailbox::Model* m_model;
    MailboxFinder *m_finder;
    QString m_mailbox;
    QPersistentModelIndex m_index;
};

}

#endif // MAILSYNCHRONIZER_H