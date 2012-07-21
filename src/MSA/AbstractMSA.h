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
#ifndef ABSTRACTMSA_H
#define ABSTRACTMSA_H

#include <QByteArray>
#include <QObject>

namespace MSA
{

class AbstractMSA : public QObject
{
    Q_OBJECT
public:
    AbstractMSA(QObject *parent);
    virtual ~AbstractMSA();
    virtual void sendMail(const QByteArray &from, const QList<QByteArray> &to, const QByteArray &data) = 0;
    virtual bool supportsBurl() const;
    virtual void sendBurl(const QByteArray &from, const QList<QByteArray> &to, const QByteArray &imapUrl);
public slots:
    virtual void cancel() = 0;
signals:
    void connecting();
    void sending();
    void sent();
    void error(const QString &message);
    void progressMax(int max);
    void progress(int num);
};

}

#endif // ABSTRACTMSA_H
