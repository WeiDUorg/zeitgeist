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

#include "weidulog.h"

#include <QString>

WeiduLog::WeiduLog(QObject* parent, QList<QList<WeiduLogComponent>> data) :
  QObject(parent), data(data)
{

}

bool WeiduLog::isEmpty() {
  if (!data.isEmpty()) {
    foreach (QList<WeiduLogComponent> compList, data) {
      if (compList.isEmpty()) {
        return true;
      }
    }
  } else { return true; }
  return false;
}

QString WeiduLog::logPath(const QString& gamePath)
{
  return gamePath + "/weidu.log";
}

QList<QString> WeiduLog::getPartitionNames() const
{
  QList<QString> names;
  names.reserve(data.size());
  foreach (QList<WeiduLogComponent> list, data) {
    names.append(list.first().modName);
  }
  return names;
}
