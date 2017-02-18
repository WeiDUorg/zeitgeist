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
#include "weidumanager.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
//#include <QFileDevice> // Qt 5
#include <QFileInfo>
#include <QProcess>
#include <QStringList>

WeiduManager::WeiduManager(const QString& weiduPath) :
  weiduPath(weiduPath), gamePath(QString()), process(new QProcess(this))
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

void WeiduManager::quack()
{
  QStringList arguments;
  arguments << "--version";
  QByteArray message = run(arguments);
  QByteArray auth("WeiDU version");
  emit quacks(message.contains(auth));
}

void WeiduManager::terminateManager()
{
  deleteLater();
}

void WeiduManager::newGamePath(const QString& path)
{
  qDebug() << "Got game path" << path;
  if (!gamePath.isEmpty()) {
    gamePath = path;
    //process->setWorkingDirectory(path);
  } else {
    /* this is currently left unconnected because there is no code path that
     * could trigger it (this triggers on the request path and requests can
     * only be triggered from some currentGame)
     */
    emit emptyGamePath();
  }
}

/* This implementation is not suitable for arguments which:
     may run WeiDU interactively
     may cause WeiDU to run for a long time (due to the time-out)
   We need to hook stdout up to a widget and allow for writing to stdin
   But perhaps only for select arguments, so it would be an alternate way
   of running WeiDU, rather than a replacement one
*/
QByteArray WeiduManager::run(const QStringList& arguments)
{
  process->start(weiduPath, arguments);
  process->waitForFinished();
  return process->readAllStandardOutput();
}

void WeiduManager::version()
{
  qDebug() << "Attempting to get version";
  QStringList arguments;
  arguments << "--version";
  QByteArray message = run(arguments);
  emit versionSignal(WeiduExtractor::version(message));
}
