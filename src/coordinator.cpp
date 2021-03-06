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

#include "coordinator.h"
#include "controller.h"
#include "datamanager.h"
#include "enqueuemodmodel.h"
#include "weidumanager.h"

#include <QFile>

class QJsonDocument;

Coordinator::Coordinator(QObject* parent) :
  QObject(parent), dataManager(new DataManager(this)),
  controller(new Controller(this))
{
  connect(this, SIGNAL(weiduPathSignal(const QString&)),
          controller, SLOT(setupWeidu(const QString&)));
  connect(controller, SIGNAL(weiduFailedValidation(const QString&)),
          this, SLOT(weiduFailedValidation(const QString&)));
  connect(controller, SIGNAL(newWeiduManager(const WeiduManager*)),
          this, SLOT(newWeiduManager(const WeiduManager*)));
  connect(controller, SIGNAL(confirmedWeiduPath(const QString&)),
          dataManager, SLOT(confirmedWeiduPath(const QString&)));
  connect(dataManager, SIGNAL(storedWeiduPath(const QString&)),
          this, SLOT(weiduPath(const QString&)));
  connect(this, SIGNAL(restoreStateSignal()),
          dataManager, SLOT(restoreState()));
  connect(dataManager, SIGNAL(processQueues(WeiduLog*, WeiduLog*, WeiduLog*)),
          controller, SLOT(processQueues(WeiduLog*, WeiduLog*, WeiduLog*)));
  connect(dataManager, SIGNAL(getLog(const QString&)),
          controller, SIGNAL(readLog(const QString&)));
  connect(controller, SIGNAL(logFile(WeiduLog*)),
          dataManager, SLOT(logFile(WeiduLog*)));
  connect(controller, SIGNAL(componentList(const QString&, int,
                                           const QJsonDocument&)),
          dataManager, SLOT(componentList(const QString&, int,
                                          const QJsonDocument&)));
  connect(this, SIGNAL(createModDistArchive(const QString&)),
          dataManager, SLOT(createModDistArchive(const QString&)));
}

void Coordinator::weiduPath(const QString& path)
{
  emit weiduPathSignal(path);
}

void Coordinator::weiduFailedValidation(const QString& weiduPath)
{
  emit weiduFailedValidationSignal(weiduPath);
}

void Coordinator::newWeiduManager(const WeiduManager* manager)
{
  connect(dataManager, SIGNAL(newGamePath(const QString&, bool)),
          manager, SLOT(newGamePath(const QString&, bool)));
  connect(dataManager, SIGNAL(eeLang(const QString&)),
          manager, SLOT(eeLang(const QString&)));
  provideGamePath();
  emit installerAvailable(true);
}

void Coordinator::provideGamePath()
{
  dataManager->emitCurrentGamePath();
}

void Coordinator::restoreState()
{
  emit restoreStateSignal();
}

void Coordinator::initWeidu(const QString& weidu)
{
  QFile f(weidu);
  if (f.exists()) {
    weiduPath(weidu);
  } else {
    dataManager->restoreWeidu();
  }
}
