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

#include "logfile.h"

#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QRegExp>
#include <QtDebug>

LogFile::LogFile(QObject* parent, const QString& gamePath) : QObject(parent)
{
  QString logFile = gamePath + "/weidu.log";
  qDebug() << "Looking for log file:" << logFile;
  if (QFileInfo(logFile).exists()) {
    data = parse(logFile);
  } else {
    qDebug() << "No log file found";
    // Should we do something with data?
  }
}

QList<LogFileComponent> LogFile::parse(const QString& logFilePath) const
{
  qDebug() << "Parsing log file:" << logFilePath;
  QList<LogFileComponent> list;
  QFile logFile(logFilePath);
  if (logFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&logFile);
    while (!in.atEnd()) {
      QString line = in.readLine();
      if (validLine(line)) {
        LogFileComponent c = parseLine(line);
        qDebug() << "Name:" << c.modName << "Language:" << c.language
                 << "Number:" << c.number;
        list << c;
      }
    }
  }
  qDebug() << "Done parsing log file";
  return list;
}

bool LogFile::validLine(const QString& line) const
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

LogFileComponent LogFile::parseLine(const QString& line) const
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
  LogFileComponent c;
  c.modName = tp2Name;
  c.language = languageNumber.toInt();
  c.number = componentNumber.toInt();
  c.comment = comment;
  return c;
}

QList<LogFileComponent> LogFile::getData() const
{
  return data;
}
