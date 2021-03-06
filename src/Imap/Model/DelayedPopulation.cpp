/* Copyright (C) 2006 - 2013 Jan Kundrát <jkt@flaska.net>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License or (at your option) version 3 or any later version
   accepted by the membership of KDE e.V. (or its successor approved
   by the membership of KDE e.V.), which shall act as a proxy
   defined in Section 14 of version 3 of the license.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <QDebug>
#include <QTimer>
#include "DelayedPopulation.h"
#include "MailboxTree.h"
#include "Model.h"

namespace Imap
{

namespace Mailbox
{

DelayedAskForChildrenOfMailbox::DelayedAskForChildrenOfMailbox(Model *model, const QModelIndex &mailbox, const CacheMode cacheMode):
    QObject(model), m_model(model), m_mailbox(mailbox),
    // There's a catch with the meaning of "invalid index", see the documentation in the header
    m_topLevel(!mailbox.isValid()), m_cacheMode(cacheMode)
{
    QTimer::singleShot(0, this, SLOT(askNow()));
}

void DelayedAskForChildrenOfMailbox::askNow()
{
    Q_ASSERT(m_model);
    TreeItemMailbox *mailboxPtr = 0;
    if (m_topLevel) {
        // We're asked for the root, and we can find that
        mailboxPtr = m_model->m_mailboxes;
    } else {
        // The index was previously valid, so let's check if it remains so
        if (! m_mailbox.isValid()) {
            qDebug() << "DelayedAskForChildrenOfMailbox: lost mailbox";
            deleteLater();
            return;
        }
        mailboxPtr = dynamic_cast<TreeItemMailbox *>(static_cast<TreeItem *>(m_mailbox.internalPointer()));
    }
    Q_ASSERT(mailboxPtr);
    m_model->askForChildrenOfMailbox(mailboxPtr, m_cacheMode == CACHE_FORCE_RELOAD);
    // We're responsible for cleaning up
    deleteLater();
}


DelayedAskForMessagesInMailbox::DelayedAskForMessagesInMailbox(Model *model, const QModelIndex &list):
    QObject(model), m_model(model), m_list(list)
{
    QTimer::singleShot(0, this, SLOT(askNow()));
}

void DelayedAskForMessagesInMailbox::askNow()
{
    Q_ASSERT(m_model);
    if (! m_list.isValid()) {
        qDebug() << "DelayedAskForMessages: lost mailbox";
        deleteLater();
        return;
    }
    TreeItemMsgList *list = dynamic_cast<TreeItemMsgList *>(static_cast<TreeItem *>(m_list.internalPointer()));
    Q_ASSERT(list);
    m_model->askForMessagesInMailbox(list);
    // We're responsible for cleaning up
    deleteLater();
}

}

}
