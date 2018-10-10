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

#include "logreader.h"
#include "weidulog.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QIODevice>
#include <QMutexLocker>
#include <QRegExp>
#include <QTextStream>

LogReader::LogReader(QMutex* weiduLog) :
weiduLog(weiduLog)
{

}

WeiduLog* LogReader::read(QObject* parent, const QString& path)
{
  qDebug() << "Attempting to read log" << path;
  QList<QList<WeiduLogComponent>> dummy;
  QFile file(path);
  if (file.exists()) {
    QMutexLocker locker(weiduLog);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
      QByteArray logData = file.readAll();
      file.close();
      locker.unlock();
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
  int index = 0;
  QTextStream in(data);
  while (!in.atEnd()) {
    QString line = in.readLine();
    if (validLine(line)) {
      const WeiduLogComponent comp = parseLine(index, line);
      ++index;
      list << comp;
    }
  }
  return new WeiduLog(parent, partitionData(list));
}

void LogReader::readLog(const QString& path)
{
  emit logFile(read(0, path));
}

void LogReader::terminateReader()
{
  deleteLater();
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
  return false;
}

WeiduLogComponent LogReader::parseLine(int index, const QString& line)
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
  QString comment;
  if (commentStart != 2) { // -1 + 3 = no comment
    comment = line.mid(commentStart);
  }
  const WeiduLogComponent c = {tp2Name, index, languageNumber.toInt(),
                               componentNumber.toInt(), comment};
  return c;
}

QList<QList<WeiduLogComponent>> LogReader::partitionData(const QList<WeiduLogComponent>& data)
{
  QList<QList<WeiduLogComponent>> partitionedData;
  QList<WeiduLogComponent>::const_iterator i;
  for (i = data.constBegin(); i != data.constEnd(); ++i) {
    int distance = i - data.constBegin();
    QList<WeiduLogComponent> block = getContiguousBlock(data,
                                                        distance,
                                                        (*i).modName);
    partitionedData.append(block);
    i += (block.length() - 1);
  }
  return partitionedData;
}

QList<WeiduLogComponent> LogReader::getContiguousBlock(const QList<WeiduLogComponent>& data,
                                                       int index, const QString& name)
{
  QList<WeiduLogComponent> block;
  QList<WeiduLogComponent>::const_iterator i;
  for (i = data.constBegin() + index; i != data.constEnd()
         && (*i).modName.compare(name) == 0; ++i) {
    block.append(*i);
  }
  return block;
}
