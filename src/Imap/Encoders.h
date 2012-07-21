/* Copyright (C) 2006 - 2012 Jan Kundrát <jkt@flaska.net>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or the version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef IMAP_ENCODERS_H
#define IMAP_ENCODERS_H

#include <QString>

namespace Imap {

typedef enum {
    DoubleQuoted,
    SquareBrackets,
    Parentheses
} QuotedStringStyle;

QByteArray quotedString(const QByteArray &unquoted, QuotedStringStyle style = DoubleQuoted);
QByteArray encodeRFC2047Phrase(const QString &text);

QByteArray encodeRFC2047String(const QString &text);
QByteArray encodeRFC2047String(const QByteArray &text, const QByteArray &encoding);

QString decodeRFC2047String(const QByteArray &raw);

QByteArray encodeImapFolderName(const QString &text);

QString decodeImapFolderName(const QByteArray &raw);

QByteArray quotedPrintableDecode(const QByteArray &raw);
QByteArray quotedPrintableEncode(const QByteArray &raw);

}

#endif // IMAP_ENCODERS_H
