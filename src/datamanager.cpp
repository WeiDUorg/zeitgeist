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

#include "datamanager.h"
#include "gamelistmodel.h"
#include "availablemodsmodel.h"
#include "installedmodsmodel.h"
#include "game.h"

#include <QSettings>
#include <QList>
#include <QString>
#include <QDir>
#include <QtDebug>

DataManager::DataManager(QObject* parent) : QObject(parent), currentGame(0)
{
  settings = new QSettings("zeitgeist", "zeitgeist", this);
  gameListModel = new GameListModel(this);
  availableModsModel = new AvailableModsModel(this);
  installedModsModel = new InstalledModsModel(this);

  qDebug() << "Restoring state";
  restoreState();
  qDebug() << "Finished restoring state";
}

void DataManager::saveState()
{
  settings->setValue("currentGame", currentGame->path);
  saveGameList();
}

void DataManager::restoreState()
{
  restoreGameList();
  QString settingsGame = settings->value("currentGame").toString();
  if (QDir(settingsGame).exists()) {
    qDebug() << "Restoring game:" << settingsGame;
    useGame(settingsGame);
  }
}

void DataManager::useGame(const QString& path)
{
  loadGame(path);
}

void DataManager::saveGameList()
{
  if (gameListModel) {
    QList<GameListDataEntry> gameList = gameListModel->exportData();
    settings->remove(gameListSettingsName);
    settings->beginWriteArray(gameListSettingsName);
    for (int i = 0; i < gameList.length(); ++i) {
      settings->setArrayIndex(i);
      settings->setValue("name", gameList.at(i).name);
      settings->setValue("path", gameList.at(i).path);
    }
    settings->endArray();
  }
}

void DataManager::restoreGameList()
{
  int length = settings->beginReadArray(gameListSettingsName);
  QList<GameListDataEntry> list;
  list.reserve(length);
  for (int i = 0; i < length; ++i) {
    settings->setArrayIndex(i);
    GameListDataEntry entry;
    entry.name = settings->value("name").toString();
    entry.path = settings->value("path").toString();
    if (QDir(entry.path).exists()) {
      qDebug() << "Restoring game list entry:" << entry.path;
      list.append(entry);
    }
  }
  settings->endArray();
  gameListModel->importData(list);
}

void DataManager::loadGame(const QString& path)
{
  const QString new_path = path;
  delete currentGame;
  qDebug() << "Loading game from path " << new_path;
  currentGame = new Game(new_path, this);

  identifyCurrentGame();
  availableModsModel->populate(currentGame);
  installedModsModel->populate(currentGame->installedMods);
}

void DataManager::identifyCurrentGame()
{
  emit identityOfCurrentGame(gameListModel->identifierOfPath(currentGame->path));
}

void DataManager::refreshCurrentGame()
{
  loadGame(currentGame->path);
}
