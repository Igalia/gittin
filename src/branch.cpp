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
#include <QSharedPointer>

#include "branch.hpp"
#include "commit.hpp"
#include "repo.hpp"
#include "command.hpp"

namespace Gittin
{

class BranchPrivate : public QSharedData
{
public:
    Repo *repo;
    QString name;
};


Branch::Branch(Repo *repo, const QString &name)
      : d(new BranchPrivate)
{
    d->repo = repo;
    d->name = name;
}

Branch::Branch(const Branch &other)
      : d(other.d)
{
}

Branch::~Branch()
{
}

Branch &Branch::operator=(const Branch &other)
{
    d = other.d;
    return *this;
}

bool Branch::exists() const
{
    QSharedPointer<Command> cmd = d->repo->command("show-ref", QStringList() << "--verify" << "--quiet" << QString("refs/heads/%1").arg(d->name));
    return cmd->exitCode() == 0;
}

QString Branch::name() const
{
    return d->name;
}

Commit Branch::head(unsigned int n) const
{
    QSharedPointer<Command> cmd = d->repo->command("log", QStringList() << "-n1" << "--oneline" << "--no-abbrev-commit" << QString("%1~%2").arg(d->name).arg(QString::number(n)));
    return Commit(d->repo, cmd->stdout().split(' ').first());
}

}
