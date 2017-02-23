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
#include "availablemodsmodel.h"
#include "game.h"
#include "gamelistmodel.h"
#include "installedmodsmodel.h"
#include "queuedmodsmodel.h"
#include "weidulog.h"

#include <QFileInfo>
#include <QSettings>
#include <QList>
#include <QString>
#include <QDir>
#include <QtDebug>

DataManager::DataManager(QObject* parent) :
  QObject(parent), gameListModel(new GameListModel(this)),
  availableModsModel(new AvailableModsModel(this)),
  installedModsModel(new InstalledModsModel(this)),
  inQueuedModsModel(new QueuedModsModel(this)),
  outQueuedModsModel(new QueuedModsModel(this)),
  settings(new QSettings("zeitgeist", "zeitgeist", this)), currentGame(0)
{
  connect(gameListModel, SIGNAL(gameRemoved(const QString&)),
          this, SLOT(gameRemoved(const QString&)));
  connect(this, SIGNAL(clearModels()),
          availableModsModel, SLOT(clear()));
  connect(this, SIGNAL(clearModels()),
          installedModsModel, SLOT(clear()));
  connect(this, SIGNAL(clearModels()),
          inQueuedModsModel, SLOT(clear()));
  connect(this, SIGNAL(clearModels()),
          outQueuedModsModel, SLOT(clear()));
  connect(this, SIGNAL(clearQueues()),
          inQueuedModsModel, SLOT(clear()));
  connect(this, SIGNAL(clearQueues()),
          outQueuedModsModel, SLOT(clear()));
}

void DataManager::saveState()
{
  if (currentGame) {
    settings->setValue("currentGame", currentGame->path);
  } else {
    settings->setValue("currentGame", QString());
  }
  saveGameList();
}

void DataManager::restoreState()
{
  restoreGameList();
  QString settingsGame = settings->value("currentGame").toString();
  if (!settingsGame.isEmpty() && QDir(settingsGame).exists()) {
    qDebug() << "Restoring game:" << settingsGame;
    useGame(settingsGame);
  }
  QString settingsWeidu = settings->value("weiduPath").toString();
  if (!settingsWeidu.isEmpty() && QFileInfo(settingsWeidu).exists()) {
    qDebug() << "Attempting to restore WeiDU path" << settingsWeidu;
    emit restoreWeidu(settingsWeidu);
  }
  qDebug() << "Finished restoring state";
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
  const QString newPath = path;
  delete currentGame;
  qDebug() << "Loading game from path " << newPath;
  currentGame = new Game(this, newPath);

  identifyCurrentGame();
  availableModsModel->populate(currentGame);
  installedModsModel->populate(currentGame->getInstalledMods());
  emit newGamePath(newPath);
}

void DataManager::identifyCurrentGame() const
{
  QString path;
  if (currentGame) {
    path = currentGame->path;
  } else {
    path = QString();
  }
  emit identityOfCurrentGame(gameListModel->identifierOfPath(path));
}

void DataManager::refreshCurrentGame()
{
  if (currentGame) {
    loadGame(currentGame->path);
  }
}

QString DataManager::getCurrentGamePath() const
{
  QString path;
  if (currentGame) {
    path = currentGame->path;
  } else {
    path = QString();
  }
  return path;
}

void DataManager::emitCurrentGamePath() const
{
  QString path;
  if (currentGame) {
    path = currentGame->path;
  } else {
    path = QString();
  }
  emit newGamePath(path);
}

void DataManager::gameRemoved(const QString& path)
{
  if (currentGame && path == currentGame->path) {
    delete currentGame;
    currentGame = 0;
    emit clearModels();
    emit identityOfCurrentGame(QString());
  }
}

void DataManager::confirmedWeiduPath(const QString& path)
{
  settings->setValue("weiduPath", path);
}

void DataManager::enqueueComponents(WeiduLog* componentList)
{
  if (!componentList->isEmpty()) {
    componentList->setParent(inQueuedModsModel);
    inQueuedModsModel->add(componentList);
  } else { delete componentList; }
}

void DataManager::uninstallComponents(WeiduLog* componentList)
{
  if (!componentList->isEmpty()) {
    componentList->setParent(outQueuedModsModel);
    outQueuedModsModel->add(componentList);
  } else { delete componentList; }
}

void DataManager::unqueueInstallComponents(const QModelIndexList& indices)
{
  if (!indices.isEmpty()) {
    inQueuedModsModel->unqueue(indices);
  }
}

void DataManager::unqueueUninstallComponents(const QModelIndexList& indices)
{
  if (!indices.isEmpty()) {
    outQueuedModsModel->unqueue(indices);
  }
}

void DataManager::getQueues()
{
  WeiduLog* installQueue = inQueuedModsModel->queue();
  WeiduLog* uninstallQueue = outQueuedModsModel->queue();
  emit clearQueues();
  emit processQueues(installQueue, uninstallQueue);
}
