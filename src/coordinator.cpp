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

#include "coordinator.h"
#include "controller.h"
#include "datamanager.h"
#include "weidumanager.h"

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
  connect(dataManager, SIGNAL(restoreWeidu(const QString&)),
          this, SLOT(weiduPath(const QString&)));
  connect(this, SIGNAL(restoreStateSignal()),
          dataManager, SLOT(restoreState()));
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
  connect(dataManager, SIGNAL(newGamePath(const QString&)),
          manager, SLOT(newGamePath(const QString&)));
  connect(manager, SIGNAL(requestGamePath()),
          this, SLOT(requestForGamePath()));
}

void Coordinator::requestForGamePath()
{
  dataManager->emitCurrentGamePath();
}

void Coordinator::restoreState()
{
  emit restoreStateSignal();
}
