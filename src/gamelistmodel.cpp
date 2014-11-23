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

#include "gamelistmodel.h"

#include <QString>
#include <QStringList>
#include <QDirIterator>
#include <QDir>
#include <QFileInfo>
#include <QStandardItem>
#include <QList>

GameListModel::GameListModel(QObject* parent) : QStandardItemModel(parent)
{
  QStringList headerLabels;
  headerLabels << tr("Game Name") << tr("Game Location");
  setHorizontalHeaderLabels(headerLabels);
}

void GameListModel::addGame(QString path)
{
  QString gamePath = findKeyFileDirectory(path);
  if (gamePath.isEmpty()) {
    emit notAGameDirectory(path);
  } else if (!duplicate(gamePath)) {
    QString gameType = fingerprintGameDirectory(gamePath);
    QList<QStandardItem*> row;
    row << new QStandardItem(gameType) << new QStandardItem(gamePath);
    appendRow(row);
  }
}

void GameListModel::removeGame(QModelIndex index)
{
  int row = index.row();
  if (index.isValid() && row < rowCount()) {
    removeRows(row, 1);
  }
}

/*
 * Return the directory which contains the file chitin.key
 * and (dialog.tlk or lang/en_us/dialog.tlk). The matching
 * against dialog.tlk (or equivalent) is necessary to avoid
 * false positives on chitin.key files located in backup
 * directories.
 */
QString GameListModel::findKeyFileDirectory(QString path)
{
  QString chitinkey("chitin.key");
  QString dialogtlk("dialog.tlk");
  QString dialogtlkee("lang/en_us/" + dialogtlk);
  QStringList nameFilter(chitinkey);
  QDirIterator iterator(path, nameFilter, QDir::Files,
                        QDirIterator::Subdirectories);
  while (iterator.hasNext()) {
    iterator.next();
    QFileInfo info = iterator.fileInfo();
    QDir dir = info.dir();
    if (dir.exists(dialogtlk) ||
        dir.exists(dialogtlkee)) {
      return dir.absolutePath();
    }
  }
  return QString();
}

/*
 * Determine the generic game type of a game directory by
 * looking for characteristic files.
 */
QString GameListModel::fingerprintGameDirectory(QString path)
{
  QDir dir(path);
  if (dir.exists("bgmain2.exe")) {
    return QString("Baldur's Gate");
  } else if (dir.exists("baldur.exe") && dir.exists("bgconfig.exe")) {
    return QString("Baldur's Gate II");
  } else if (dir.exists("idmain.exe")) {
    return QString("Icewind Dale");
  } else if (dir.exists("iwd2.exe")) {
    return QString("Icewind Dale II");
  } else if (dir.exists("torment.exe")) {
    return QString("Planescape: Torment");
  } else if (dir.exists("movies/bgenter.wbm")) {
    return QString("Baldur's Gate: Enhanced Edition");
  } else if (dir.exists("movies/melissan.wbm")) {
    return QString("Baldur's Gate II: Enhanced Edition");
  }
  return QString("IE game");
}

bool GameListModel::duplicate(QString path)
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, 1)->text()) == 0) {
      return true;
    }
  }
  return false;
}
