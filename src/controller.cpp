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

#include "controller.h"
#include "weidumanager.h"

#include <QDebug>

Controller::Controller(QObject* parent) :
  QObject(parent), workerThread(new QThread(this)), currentWeidu(QString()),
  weiduManager(0)
{

}

Controller::~Controller()
{
  emit terminateManager();
}

void Controller::setupWeidu(const QString& weiduPath)
{
  qDebug() << "weidu path is" << weiduPath;
  emit terminateManager();
  qDebug() << "Creating WeiduManager";
  currentWeidu = weiduPath;
  weiduManager = new WeiduManager(weiduPath);
  if (weiduManager->valid()) {
    qDebug() << "WeiduManager succeeded validation";
    weiduManager->moveToThread(workerThread);
    connect(this, SIGNAL(terminateManager()),
            weiduManager, SLOT(terminateManager()));
    connect(this, SIGNAL(doesItQuack()),
            weiduManager, SLOT(quack()));
    connect(weiduManager, SIGNAL(quacks(const bool&)),
            this, SLOT(quacks(const bool&)));
    connect(weiduManager, SIGNAL(versionSignal(const int&)),
            this, SLOT(weiduVersion(const int&)));
    connect(this, SIGNAL(getVersion()),
            weiduManager, SLOT(version()));
    workerThread->start();
    emit doesItQuack();
  } else {
    qDebug() << "WeiduManager failed validation";
    delete weiduManager;
    emit weiduFailedValidation(weiduPath);
  }
}

void Controller::quacks(const bool& quacks)
{
  if (quacks) {
    qDebug() << "File quacks like a WeiDU";
    emit newWeiduManager(weiduManager);
    emit getVersion();
  } else {
    qDebug() << "File fails to quack like a WeiDU";
    emit weiduFailedValidation(currentWeidu);
  }
}

void Controller::weiduVersion(const int& version)
{
  emit weiduVersionSignal(version);
}
