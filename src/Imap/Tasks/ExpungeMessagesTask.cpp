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


#include "ExpungeMessagesTask.h"
#include "ItemRoles.h"
#include "KeepMailboxOpenTask.h"
#include "Model.h"
#include "MailboxTree.h"

namespace Imap
{
namespace Mailbox
{


ExpungeMessagesTask::ExpungeMessagesTask(Model *model, ImapTask *parentTask, const QList<QPersistentModelIndex> &messages):
    ImapTask(model), conn(parentTask), messages(messages)
{
    if (messages.isEmpty()) {
        throw CantHappen("ExpungeMessagesTask called with empty message set");
    }
    Q_ASSERT(conn);
    conn->addDependentTask(this);
}

void ExpungeMessagesTask::perform()
{
    parser = conn->parser;
    markAsActiveTask();

    IMAP_TASK_CHECK_ABORT_DIE;

    Sequence seq;
    bool first = true;

    Q_FOREACH(const QPersistentModelIndex& index, messages) {
        if (! index.isValid()) {
            // That message is gone already
        } else {
            TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
            Q_ASSERT(item);
            TreeItemMessage *message = dynamic_cast<TreeItemMessage *>(item);
            Q_ASSERT(message);
            if (first) {
                seq = Sequence(message->uid());
                first = false;
            } else {
                seq.add(message->uid());
            }
        }
    }

    if (first) {
        // No valid messages
        _failed("All messages are gone already");
        return;
    }

    if (!model->accessParser(parser).capabilities.contains("UIDPLUS")) {
        _failed("The IMAP server doesn't support the UIDPLUS extension");
    }

    tag = parser->uidExpunge(seq);
}

bool ExpungeMessagesTask::handleStateHelper(const Imap::Responses::State *const resp)
{
    if (resp->tag.isEmpty())
        return false;

    if (resp->tag == tag) {
        if (resp->kind == Responses::OK) {
            _completed();
        } else {
            _failed("UID EXPUNGE failed");
        }
        return true;
    } else {
        return false;
    }
}

QVariant ExpungeMessagesTask::taskData(const int role) const
{
    return role == RoleTaskCompactName ? QVariant(tr("Removing some of the deleted messages")) : QVariant();
}

}
}
