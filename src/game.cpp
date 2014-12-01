/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2014  Fredrik Lindgren

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "game.h"
#include "mod.h"
#include "logfile.h"

#include <QString>
#include <QStringList>
#include <QDirIterator>
#include <QtDebug>

Game::Game(QString path, QObject* parent) :QObject(parent), path(path)
{
  availableMods = getModList(path);
  installedMods = new LogFile(path, this);
}

QList<Mod*> Game::getModList(QString path)
{
  QStringList topLevelMods = getTopLevelMods(path);
  QStringList subLevelMods = getSubLevelMods(path);
  QStringList mods = topLevelMods + subLevelMods;
  QList<Mod*> result;
  foreach (QString tp2Path, mods) {
    qDebug() << "Found mod at:" << tp2Path;
    result << new Mod(tp2Path, this);
  }
  // sort results by name
  return result;
}

QStringList Game::getTopLevelMods(QString path)
{
  QStringList result;
  QStringList nameFilter("*.tp2");
  QDirIterator iterator(path, nameFilter, QDir::Files);
  while (iterator.hasNext()) {
    iterator.next();
    result << iterator.fileName();
  }
  return result;
}

QStringList Game::getSubLevelMods(QString path)
{
  QStringList result;
  QDirIterator iterator(path, QDir::Dirs);
  while (iterator.hasNext()) {
    iterator.next();
    QString name = iterator.fileName();
    QString tp2 = name + ".tp2";
    QString setupTp2 = "setup-" + name + ".tp2";
    QString foundTp2 = QString();
    QDir dir(iterator.filePath());
    if (dir.exists(setupTp2)) {
      foundTp2 = setupTp2;
    } else if (dir.exists(tp2)) {
      foundTp2 = tp2;
    }
    if (!foundTp2.isEmpty()) {
      result << name + "/" + foundTp2;
    }
  }
  return result;
}
