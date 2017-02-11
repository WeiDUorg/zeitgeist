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

Controller::Controller(QObject* parent) : QObject(parent), workerThread(new QThread(this))
{

}

void Controller::setupWeidu(const QString& weiduPath, QString gamePath)
{
  qDebug() << "weidu path is" << weiduPath;
  qDebug() << "Game path is" << gamePath;
  // We need some way of cleaning WeiduManager up if this function is called more than once
  WeiduManager* weiduManager = new WeiduManager(weiduPath, gamePath);
  if (weiduManager->valid()) {
    qDebug() << "WeiduManager succeeded validation";
    weiduManager->moveToThread(workerThread);
    connect(weiduManager, SIGNAL(versionSignal(const QString&)),
            this, SLOT(weiduVersion(const QString&)));
    connect(this, SIGNAL(getVersion()),
            weiduManager, SLOT(version()));
    workerThread->start();
    emit getVersion();
  } else {
    qDebug() << "WeiduManager failed validation";
    emit weiduFailedValidation(weiduPath);
  }
}

void Controller::weiduVersion(const QString& version)
{
  qDebug() << "Version is" << version;
}
