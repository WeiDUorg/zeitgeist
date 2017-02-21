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

#include "logreader.h"
#include "weidulog.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QRegExp>
#include <QTextStream>

WeiduLog* LogReader::read(QObject* parent, const QString& path)
{
  qDebug() << "Attempting to read log" << path;
  QList<WeiduLogComponent> dummy;
  QFile file(path);
  if (file.exists()) {
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QByteArray logData = file.readAll();
      file.close();
      return read(parent, logData);
    } else {
      qDebug() << "Unable to open log file";
    }
  } else {
    qDebug() << "Log file does not exist";
  }
  return new WeiduLog(parent, dummy);
}

WeiduLog* LogReader::read(QObject* parent, const QByteArray& data)
{
  QList<WeiduLogComponent> list;
  QTextStream in(data);
  while (!in.atEnd()) {
    QString line = in.readLine();
    if (validLine(line)) {
      const WeiduLogComponent comp = parseLine(line);
      list << comp;
    }
  }
  return new WeiduLog(parent, list);
}

bool LogReader::validLine(const QString& line)
{
  if (line.isEmpty()) {
    return false;
  }
  QRegExp isComment("^[ \t]*//.*");
  QRegExp pattern("^~.+~ #[0-9]+ #[0-9]+.*");
  if (isComment.exactMatch(line)) {
    return false;
  }
  if (pattern.exactMatch(line)) {
    return true;
  }
  qDebug() << "Unmatched log line:" << line;
  return false;
}

WeiduLogComponent LogReader::parseLine(const QString& line)
{
  // ~(FOO/\\)?(SETUP-)?FOO.TP2~ #[0-9]+ #[0-9]+ // Comment
  int firstTilde = line.indexOf("~") + 1;
  int secondTilde = line.indexOf("~", firstTilde);
  int firstNumber = line.indexOf("#") + 1;
  int firstNumberEnd = line.indexOf(" ", firstNumber);
  int secondNumber = line.indexOf("#", firstNumber) + 1;
  int secondNumberEnd = line.indexOf(" ", secondNumber);
  int commentStart = line.indexOf("//") + 3;
  QString tp2Name = line.mid(firstTilde, secondTilde - firstTilde);
  QString languageNumber = line.mid(firstNumber,
                                    firstNumberEnd - firstNumber);
  QString componentNumber = line.mid(secondNumber,
                                     secondNumberEnd - secondNumber);
  QString comment = line.mid(commentStart);
  const WeiduLogComponent c = {tp2Name, languageNumber.toInt(),
                               componentNumber.toInt(), comment};
  return c;
}
