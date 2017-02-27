/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2017  Fredrik Lindgren

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

#include <QDebug>
#include <QDirIterator>
#include <QDir>
#include <QFileInfo>
#include <QStandardItem>
#include <QString>
#include <QStringList>
#include <QVariant>

GameListModel::GameListModel(QObject* parent) : QStandardItemModel(parent)
{
  QStringList headerLabels;
  headerLabels << tr("Game Name") << tr("Game Location");
  setHorizontalHeaderLabels(headerLabels);
}

void GameListModel::addRow(const QString& gameName, const QString& gamePath,
                           const GameType& gameType)
{
  QList<QStandardItem*> row;
  QStandardItem* pathItem = new QStandardItem(gamePath);
  pathItem->setData(QVariant(stringOfType(gameType)), GAME_TYPE);
  row << new QStandardItem(gameName) << pathItem;
  appendRow(row);
}

void GameListModel::addGame(const QString& path)
{
  QString gamePath = findKeyFileDirectory(path);
  if (gamePath.isEmpty()) {
    emit notAGameDirectory(path);
  } else if (!duplicate(gamePath)) {
    GameType gameType = fingerprintGameDirectory(gamePath);
    QString gameName = prettyPrintGameType(gameType);
    addRow(gameName, gamePath, gameType);
  }
}

void GameListModel::removeGame(const QModelIndex& index)
{
  int row = index.row();
  if (index.isValid() && row < rowCount()) {
    emit gameRemoved(pathOfIndex(index));
    removeRows(row, 1);
  }
}

QList<GameListDataEntry> GameListModel::exportData() const
{
  int rows = rowCount();
  QList<GameListDataEntry> result;
  result.reserve(rows);
  for (int i = 0; i < rows; ++i) {
    GameListDataEntry row;
    row.name = item(i, 0)->text();
    row.path = item(i, 1)->text();
    result.append(row);
  }
  return result;
}

void GameListModel::importData(const QList<GameListDataEntry>& dataList)
{
  int rows = dataList.length();
  for (int i = 0; i < rows; ++i) {
    GameListDataEntry row = dataList.at(i);
    QDir dir(row.path);
    if (validGame(row.path)) {
      GameType type = fingerprintGameDirectory(row.path);
      addRow(row.name, row.path, type);
    }
  }
}

QString GameListModel::pathOfIndex(const QModelIndex& index) const
{
  int row = index.row();
  if (index.isValid() && row < rowCount()) {
    return item(row, 1)->text();
  }
  return QString();
}

QString GameListModel::identifierOfPath(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, 1)->text()) == 0) {
      return item(i, 0)->text();
    }
  }
  qDebug() << "Could not find an identity for path" << path;
  return QString();
}

/*
 * Return the directory which contains the file chitin.key
 * and (dialog.tlk or lang/en_us/dialog.tlk). The matching
 * against dialog.tlk (or equivalent) is necessary to avoid
 * false positives on chitin.key files located in backup
 * directories.
 */
QString GameListModel::findKeyFileDirectory(const QString& path) const
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
GameType GameListModel::fingerprintGameDirectory(const QString& path) const
{
  QDir dir(path);
  if (dir.exists("bgmain2.exe")) {
    return GameType::BG;
  } else if (dir.exists("baldur.exe") && dir.exists("bgconfig.exe")) {
    return GameType::BG2;
  } else if (dir.exists("idmain.exe")) {
    return GameType::IWD;
  } else if (dir.exists("iwd2.exe")) {
    return GameType::IWD2;
  } else if (dir.exists("torment.exe")) {
    return GameType::PST;
  } else if (dir.exists("movies/bgenter.wbm")) {
    return GameType::BGEE;
  } else if (dir.exists("movies/melissan.wbm")) {
    return GameType::BG2EE;
  } else if (dir.exists("movies/howseer.wbm")) {
    return GameType::IWDEE;
  }
  return GameType::UNKNOWN;
}

QString GameListModel::prettyPrintGameType(const GameType& type) const
{
  switch (type) {
  case GameType::BG:
    return QString("Baldur's Gate");
  case GameType::BG2:
    return QString("Baldur's Gate II");
  case GameType::IWD:
    return QString("Icewind Dale");
  case GameType::IWD2:
    return QString("Icewind Dale II");
  case GameType::PST:
    return QString("Planescape: Torment");
  case GameType::BGEE:
    return QString("Baldur's Gate: Enhanced Edition");
  case GameType::BG2EE:
    return QString("Baldur's Gate II: Enhanced Edition");
  case GameType::IWDEE:
    return QString("Icewind Dale: Enhanced Edition");
  default: // GameType::UNKNOWN
    return QString("IE game");
  }
}

bool GameListModel::duplicate(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, 1)->text()) == 0) {
      return true;
    }
  }
  return false;
}

//QMetaEnum (4.8) seems to lack a typeOfString equivalent so I ain't bothering
QString GameListModel::stringOfType(const GameType& type) const
{
  switch (type) {
  case GameType::BG:
    return "BG";
  case GameType::BG2:
    return "BG2";
  case GameType::IWD:
    return "IWD";
  case GameType::IWD2:
    return "IWD2";
  case GameType::PST:
    return "PST";
  case GameType::BGEE:
    return "BGEE";
  case GameType::BG2EE:
    return "BG2EE";
  case GameType::IWDEE:
    return "IWDEE";
  default: // GameType::UNKNOWN
    return "UNKNOWN";
  }
}

GameType GameListModel::typeOfString(const QString& name) const
{
  if (name == "BG") {
    return GameType::BG;
  } else if (name == "BG2") {
    return GameType::BG2;
  } else if (name == "IWD") {
    return GameType::IWD;
  } else if (name == "IWD2") {
    return GameType::IWD2;
  } else if (name == "PST") {
    return GameType::PST;
  } else if (name == "BGEE") {
    return GameType::BGEE;
  } else if (name == "BG2EE") {
    return GameType::BG2EE;
  } else if (name == "IWDEE") {
    return GameType::IWDEE;
  } else {
    return GameType::UNKNOWN;
  }
}

bool GameListModel::eeGame(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, 1)->text()) == 0) {
      GameType type = typeOfString(item(i, 1)->data(GAME_TYPE).toString());
      switch (type) {
      case GameType::BGEE:
      case GameType::BG2EE:
      case GameType::IWDEE:
        return true;
      default:
        return false;
      }
    }
  }
  return false;
}

bool GameListModel::validGame(const QString& path) const
{
  return QDir(path).exists() && !findKeyFileDirectory(path).isEmpty();
}
