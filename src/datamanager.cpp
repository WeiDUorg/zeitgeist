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

DataManager::DataManager(QObject* parent) : QObject(parent)
{
  settings = new QSettings("zeitgeist", "zeitgeist", this);
  gameListModel = new GameListModel(this);

  restoreState();
}

void DataManager::saveState()
{
  saveGameList();
}

void DataManager::restoreState()
{
  restoreGameList();
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
    list.append(entry);
  }
  settings->endArray();
  gameListModel->importData(list);
}
