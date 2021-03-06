/*
 * Copyright (C) 2014 Jolla Ltd.
 * Contact: Giulio Camuffo <giulio.camuffo@jollamobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
 */

#include <QSharedData>
#include <QString>
#include <QDebug>
#include <QSharedPointer>

#include "commit.hpp"
#include "command.hpp"
#include "repo.hpp"

namespace Gittin
{

class CommitPrivate : public QSharedData
{
public:
    CommitPrivate(Repo *r, const QByteArray &s)
        : repo(r)
        , sha(s)
    {
    }

    Repo *repo;
    QString message;
    QByteArray sha;
    QString authorName;
    QString authorEmail;
    QString diff;
};


Commit::Commit(Repo *repo, const QByteArray &sha)
      : d(new CommitPrivate(repo, sha))
{
    QSharedPointer<Command> cmd = repo->command("log", QStringList() << "-n1" << "--pretty=format:%s" << sha);
    d->message = cmd->stdout();

    cmd = repo->command("show", QStringList() << "--pretty=format:%an;%ce" << sha);
    QByteArray out = cmd->stdout();

    int commaPos = out.indexOf(';');
    int newlinePos = out.indexOf('\n');
    d->authorName = out.left(commaPos);
    d->authorEmail = out.mid(commaPos + 1, newlinePos - commaPos - 1);
    d->diff = out.mid(newlinePos + 1);
}

Commit::Commit(const Commit &other)
      : d(other.d)
{
}

Commit::~Commit()
{
}

Commit &Commit::operator=(const Commit &other)
{
    d = other.d;
    return *this;
}

QString Commit::message() const
{
    return d->message;
}

QByteArray Commit::sha() const
{
    return d->sha;
}

QString Commit::authorName() const
{
    return d->authorName;
}

QString Commit::authorEmail() const
{
    return d->authorEmail;
}

QString Commit::diff() const
{
    return d->diff;
}

void Commit::addNote(const QString &msg)
{
    d->repo->command("notes", QStringList() << "add" << "-m" << msg << d->sha);
}

QDebug operator<<(QDebug dbg, const Commit &t)
{
    dbg.nospace() << "Commit(" << t.sha() << " " << t.message() << ")";
    return dbg.space();
}

}
