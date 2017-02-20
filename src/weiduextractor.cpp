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

#include "weiduextractor.h"

#include <QByteArray>
#include <QDebug>
#include <QRegExp>
#include <QStringList>

int WeiduExtractor::version(const QByteArray& message)
{
  QByteArray weiduVersion("WeiDU version");
  int index = message.indexOf(weiduVersion);
  QByteArray version = message.mid(index + weiduVersion.size() + 1, 3);
  qDebug() << "Version is" << version;
  return version.toInt();
}

QStringList WeiduExtractor::languageList(const QByteArray& message)
{
  QStringList raw = QString(message).split("\n");
  QStringList list;
  while (!raw.isEmpty()) {
    QRegExp regexp("^[0-9]+:(.+)$");
    if (regexp.exactMatch(raw.takeFirst())) {
      list.append(regexp.capturedTexts()[1]);
    }
  }
  qDebug() << "Languages obtained were" << list;
  return list;
}
