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

#include <QSettings>
#include <QList>
#include <QString>
#include <QDir>
#include <QtDebug>

DataManager::DataManager(QObject* parent) : QObject(parent)
{
  settings = new QSettings("zeitgeist", "zeitgeist", this);
  gameListModel = new GameListModel(this);

  qDebug() << "Restoring state";
  restoreState();
  qDebug() << "Finished restoring state";
}

void DataManager::saveState()
{
  saveGameList();
}

void DataManager::restoreState()
{
  QString settingsGame = settings->value("currentGame").toString();
  if (QDir(settingsGame).exists()) {
    currentGame = settingsGame;
    qDebug() << "Restoring game:" << currentGame;
    loadGame(currentGame);
  }
  restoreGameList();
}

void DataManager::useGame(QString path)
{
  qDebug() << "Using game:" << path;
  currentGame = path;
  settings->setValue("currentGame", path);
  loadGame(currentGame);
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

void DataManager::loadGame(QString path)
{

}
