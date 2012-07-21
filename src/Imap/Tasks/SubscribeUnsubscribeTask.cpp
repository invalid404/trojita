/* Copyright (C) 2006 - 2012 Jan Kundrát <jkt@flaska.net>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/


#include "SubscribeUnsubscribeTask.h"
#include "GetAnyConnectionTask.h"
#include "ItemRoles.h"
#include "Model.h"
#include "MailboxTree.h"

namespace Imap
{
namespace Mailbox
{


SubscribeUnsubscribeTask::SubscribeUnsubscribeTask(Model *model, const QModelIndex &mailbox, SubscribeUnsubscribeOperation operation):
    ImapTask(model), operation(operation), mailboxIndex(mailbox)
{
    TreeItemMailbox *mailboxPtr = dynamic_cast<TreeItemMailbox *>(static_cast<TreeItem *>(mailbox.internalPointer()));
    Q_ASSERT(mailboxPtr);
    conn = model->m_taskFactory->createGetAnyConnectionTask(model);
    conn->addDependentTask(this);
}

void SubscribeUnsubscribeTask::perform()
{
    parser = conn->parser;
    markAsActiveTask();

    IMAP_TASK_CHECK_ABORT_DIE;

    if (! mailboxIndex.isValid()) {
        // FIXME: add proper fix
        log("Mailbox vanished before we could ask for number of messages inside");
        _completed();
        return;
    }
    TreeItemMailbox *mailbox = dynamic_cast<TreeItemMailbox *>(static_cast<TreeItem *>(mailboxIndex.internalPointer()));
    Q_ASSERT(mailbox);

    switch (operation) {
    case SUBSCRIBE:
        tag = parser->subscribe(mailbox->mailbox());
        break;
    case UNSUBSCRIBE:
        tag = parser->unSubscribe(mailbox->mailbox());
        break;
    default:
        Q_ASSERT(false);
    }
}

bool SubscribeUnsubscribeTask::handleStateHelper(const Imap::Responses::State *const resp)
{
    if (resp->tag.isEmpty())
        return false;

    if (resp->tag == tag) {
        if (resp->kind == Responses::OK) {
            TreeItemMailbox *mailbox = dynamic_cast<TreeItemMailbox *>(static_cast<TreeItem *>(mailboxIndex.internalPointer()));
            QString subscribed = QLatin1String("\\SUBSCRIBED");
            switch (operation) {
            case SUBSCRIBE:
                if (mailbox && !mailbox->m_metadata.flags.contains(subscribed)) {
                    mailbox->m_metadata.flags.append(subscribed);
                }
                break;
            case UNSUBSCRIBE:
                if (mailbox) {
                    mailbox->m_metadata.flags.removeOne(subscribed);
                }
            }
            _completed();
        } else {
            _failed("SUBSCRIBE/UNSUBSCRIBE has failed");
            // FIXME: error handling
        }
        return true;
    } else {
        return false;
    }
}

QString SubscribeUnsubscribeTask::debugIdentification() const
{
    if (! mailboxIndex.isValid())
        return QString::fromAscii("[invalid mailboxIndex]");

    TreeItemMailbox *mailbox = dynamic_cast<TreeItemMailbox *>(static_cast<TreeItem *>(mailboxIndex.internalPointer()));
    Q_ASSERT(mailbox);
    return QString::fromAscii("attached to %1").arg(mailbox->mailbox());
}

QVariant SubscribeUnsubscribeTask::taskData(const int role) const
{
    return role == RoleTaskCompactName ? QVariant(tr("Looking for messages")) : QVariant();
}

}
}
