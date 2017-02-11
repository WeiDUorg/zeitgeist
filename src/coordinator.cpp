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

Coordinator::Coordinator(QObject* parent) :
  QObject(parent), dataManager(new DataManager(this)),
  controller(new Controller(this))
{
  connect(this, SIGNAL(setWeiduPath(const QString&, QString)),
          controller, SLOT(setupWeidu(const QString&, QString)));
}

void Coordinator::weiduPath(const QString& path)
{
  emit setWeiduPath(path, dataManager->getCurrentGamePath());
}
