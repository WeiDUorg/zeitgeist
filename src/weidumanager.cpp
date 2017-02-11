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

#include "weidumanager.h"

#include <QDebug>
#include <QFile>
//#include <QFileDevice> // Qt 5
#include <QFileInfo>

// We will also need gamePath

WeiduManager::WeiduManager(const QString& weiduPath, QString gamePath) :
  weiduPath(weiduPath), gamePath(gamePath)
{

}

bool WeiduManager::valid() const
{
  QFileInfo info(weiduPath);
  if (!info.isExecutable() &&
      //      !QFile::setPermissions(path, QFileDevice::ExeOther)) // Qt 5
      !QFile::setPermissions(weiduPath, QFile::ExeOther))
    {
      return false;
    }
  return true;
}

void WeiduManager::version()
{
  const QString version = "latest";
  emit versionSignal(version);
}
