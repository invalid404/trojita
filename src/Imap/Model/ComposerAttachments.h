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

#ifndef IMAP_COMPOSERATTACHMENTS_H
#define IMAP_COMPOSERATTACHMENTS_H

#include <QIODevice>
#include <QPointer>
#include <QSharedPointer>

namespace Imap {
namespace Mailbox {

class Model;
class FullMessageCombiner;
class TreeItemMessage;
class TreeItemPart;

/** @short A generic item to be used as an attachment */
class AttachmentItem {
public:
    virtual ~AttachmentItem();

    virtual QString caption() const = 0;
    virtual QString tooltip() const = 0;
    virtual QByteArray mimeType() const = 0;
    virtual QByteArray contentDispositionHeader() const = 0;

    /** @short Return shared pointer to QIODevice which is ready to return data for this part

    The underlying QIODevice MUST NOT be stored for future use.  It is no longer valid after the source
    AttachmentItem is destroyed.

    The QIODevice MAY support only a single reading pass.  If the caller wants to read data multiple times, they should
    obtain another copy through calling rawData again.

    This function MAY return a null pointer if the data is not ready yet. Always use isAvailable() to make sure that
    the function will return correct data AND check the return value due to a possible TOCTOU issue.

    When the event loop is renetered, the QIODevice MAY become invalid and MUST NOT be used anymore.

    (I really, really like the RFC way of expression constraints :). )
    */
    virtual QSharedPointer<QIODevice> rawData() const = 0;

    /** @short Return true if the data are already available from a local cache */
    virtual bool isAvailableLocally() const = 0;

    typedef enum {
        CTE_7BIT,
        CTE_8BIT,
        CTE_BINARY,
        CTE_BASE64
    } ContentTransferEncoding;
    virtual ContentTransferEncoding suggestedCTE() const = 0;

    /** @short Returns a valid IMAP URL or an empty QByteArray if the data are not available from an IMAP server */
    virtual QByteArray imapUrl() const = 0;

    /** @short Express interest in having the attachment data available locally */
    virtual void preload() const = 0;

    /** @short Different sorts of possible attachments */
    typedef enum {
        ATTACHMENT_IMAP_MESSAGE,
        ATTACHMENT_IMAP_PART,
        ATTACHMENT_FILE
    } AttachmentKind;

    /** @short Describe the data for use in a drag-and-drop operation */
    virtual void asDroppableMimeData(QDataStream &stream) const = 0;
};

/** @short Part of a message stored in an IMAP server */
class ImapMessageAttachmentItem: public AttachmentItem {
public:
    ImapMessageAttachmentItem(Model *model, const QString &mailbox, const uint uidValidity, const uint uid);
    ~ImapMessageAttachmentItem();

    virtual QString caption() const;
    virtual QString tooltip() const;
    virtual QByteArray mimeType() const;
    virtual QByteArray contentDispositionHeader() const;
    virtual QSharedPointer<QIODevice> rawData() const;
    virtual bool isAvailableLocally() const;
    virtual ContentTransferEncoding suggestedCTE() const;
    virtual QByteArray imapUrl() const;
    virtual void preload() const;
    virtual void asDroppableMimeData(QDataStream &stream) const;
private:
    FullMessageCombiner *fullMessageCombiner;
    TreeItemMessage *messagePtr() const;

    QPointer<Model> model;
    QString mailbox;
    uint uidValidity;
    uint uid;
};

/** @short Part of a message stored in an IMAP server */
class ImapPartAttachmentItem: public AttachmentItem {
public:
    ImapPartAttachmentItem(Model *model, const QString &mailbox, const uint uidValidity, const uint uid,
                           const QString &imapPartId, const QString &trojitaPath);
    ~ImapPartAttachmentItem();

    virtual QString caption() const;
    virtual QString tooltip() const;
    virtual QByteArray mimeType() const;
    virtual QByteArray contentDispositionHeader() const;
    virtual QSharedPointer<QIODevice> rawData() const;
    virtual bool isAvailableLocally() const;
    virtual ContentTransferEncoding suggestedCTE() const;
    virtual QByteArray imapUrl() const;
    virtual void preload() const;
    virtual void asDroppableMimeData(QDataStream &stream) const;
private:
    TreeItemPart *partPtr() const;

    QPointer<Model> model;
    QString mailbox;
    uint uidValidity;
    uint uid;
    QString imapPartId;
    QString trojitaPath;
};

/** @short On-disk file */
class FileAttachmentItem: public AttachmentItem {
public:
    explicit FileAttachmentItem(const QString &fileName);
    ~FileAttachmentItem();

    virtual QString caption() const;
    virtual QString tooltip() const;
    virtual QByteArray mimeType() const;
    virtual QByteArray contentDispositionHeader() const;
    virtual QSharedPointer<QIODevice> rawData() const;
    virtual bool isAvailableLocally() const;
    virtual ContentTransferEncoding suggestedCTE() const;
    virtual QByteArray imapUrl() const;
    virtual void preload() const;
    virtual void asDroppableMimeData(QDataStream &stream) const;
private:
    QString fileName;
    mutable QByteArray m_cachedMime;
};

}
}

#endif // IMAP_COMPOSERATTACHMENTS_H
