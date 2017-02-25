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
#include "logreader.h"
#include "mod.h"
#include "weidulog.h"

#include <algorithm>
#include <QString>
#include <QStringList>
#include <QDirIterator>
#include <QtDebug>

Game::Game(QObject* parent, const QString& path) : QObject(parent), path(path)
{
  availableMods = getModList(path);
  installedMods = LogReader::read(this, WeiduLog::logPath(path)); // WeiduLog intended for us
}

QList<Mod*> Game::getModList(const QString& path)
{
  QStringList topLevelMods = getTopLevelMods(path);
  QStringList subLevelMods = getSubLevelMods(path);
  QStringList mods = topLevelMods + subLevelMods;
  QList<Mod*> result;
  foreach (QString tp2Path, mods) {
    qDebug() << "Found mod at:" << tp2Path;
    result << new Mod(this, tp2Path);
  }
  std::sort(result.begin(), result.end(), [](const Mod* l, const Mod* r) {
      return l->getName() < r->getName();
    });
  return result;
}

QStringList Game::getTopLevelMods(const QString& path) const
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

QStringList Game::getSubLevelMods(const QString& path) const
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

QList<QString> Game::getModNames() const
{
  QList<QString> result;
  result.reserve(availableMods.size());
  foreach (const Mod* mod, availableMods) {
    result.append(mod->getName());
  }
  return result;
}

QList<QString> Game::getModPaths() const
{
  QList<QString> result;
  result.reserve(availableMods.size());
  foreach (const Mod* mod, availableMods) {
    result.append(mod->tp2Path);
  }
  return result;
}

WeiduLog* Game::getInstalledMods() const
{
  return installedMods;
}

void Game::reloadLog()
{
  installedMods = LogReader::read(this, WeiduLog::logPath(path));
}
