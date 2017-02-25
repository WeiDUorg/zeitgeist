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
#include "weiduextractor.h"

#include <QByteArray>
#include <QDebug>
#include <QDir>
#include <QFile>
//#include <QFileDevice> // Qt 5
#include <QFileInfo>
#include <QStringList>

WeiduManager::WeiduManager(const QString& weiduPath) :
  weiduPath(weiduPath), gamePath(QString()), process(new QProcess(this))
{
  connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
          this, SLOT(endTask(int, QProcess::ExitStatus)));
  // should also monitor the error() signal
}

bool WeiduManager::executable() const
{
  QFileInfo info(weiduPath);
  return info.isExecutable() ||
    QFile::setPermissions(weiduPath,
                          QFile::permissions(weiduPath) |
                          QFile::ExeOwner | QFile::ExeUser |
                          QFile::ExeGroup | QFile::ExeOther);
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

void WeiduManager::doTask()
{
  if (!busy && !taskQueue.isEmpty()) {
    busy = true;
    const Task task = taskQueue.head();
    switch (task) {
    case Task::VERSION:
      qDebug() << "Starting VERSION task";
      versionTask();
      break;

    case Task::GAMEPATH:
      qDebug() << "Starting GAMEPATH task";
      qDebug() << "Setting working directory to" << gamePath;
      process->setWorkingDirectory(gamePath);
      endTask(0, QProcess::NormalExit); // I'm not so sure about this
      break;

    case Task::LISTLANGUAGES:
      qDebug() << "Starting LISTLANGUAGES task";
      listLanguagesTask();
      break;

    case Task::LISTCOMPONENTS:
      qDebug() << "Starting LISTCOMPONENTS task";
      listComponentsTask();
      break;

    case Task::INSTALL:
      qDebug() << "Starting INSTALL task";
      installTask();
      break;

    case Task::UNINSTALL:
      qDebug() << "Starting UNINSTALL task";
      uninstallTask();
      break;
    }
  }
}

void WeiduManager::startTask(const QStringList& arguments)
{
  process->start(weiduPath, arguments);
  bool started = process->waitForStarted();
  if (!started) {
    qDebug() << "Process did not start";
    // escalate
    // don't forget to dequeue
    // don't forget to unset busy
  }
}

void WeiduManager::endTask(int exitCode, QProcess::ExitStatus exitStatus)
{
  if (!exitCode == 0 || !exitStatus == 0) {
    qDebug() << "Abnormal return values from process";
    qDebug() << "Exit code:" << QString::number(exitCode) <<
      "Exit status:" << QString::number(exitStatus);
    // escalate
    // don't forget to dequeue
    // don't forget to unset busy
  }
  /* Only do stuff if we started stuff (finished() is emitted by quack()) */
  if (busy) {
    const Task task = taskQueue.head();
    switch (task) {
    case Task::VERSION:
      qDebug() << "Ending VERSION task";
      dequeue();
      emit versionSignal(WeiduExtractor::version(readStdOut()));
      break;

    case Task::GAMEPATH:
      qDebug() << "Ending GAMEPATH task";
      dequeue();
      break;

    case Task::LISTLANGUAGES:
      qDebug() << "Ending LISTLANGUAGES task";
      dequeue();
      emit languageList(WeiduExtractor::languageList(readStdOut()));
      break;

    case Task::LISTCOMPONENTS:
      qDebug() << "Ending LISTCOMPONENTS task";
      dequeue();
      emit componentList(WeiduExtractor::componentList(readStdOut()));
      break;

    case Task::INSTALL:
      qDebug() << "Ending INSTALL task";
      dequeue();
      emit modStackChanged();
      break;

    case Task::UNINSTALL:
      qDebug() << "Ending UNINSTALL task";
      dequeue();
      emit modStackChanged();
      break;
    }
  }
  busy = false;
  doTask();
}

void WeiduManager::dequeue()
{
  const Task task = taskQueue.dequeue();
  switch (task) {
  case Task::VERSION:
    qDebug() << "Dequeuing VERSION task";
    break;

  case Task::GAMEPATH:
    qDebug() << "Dequeuing GAMEPATH task";
    gamePathQueue.dequeue();
    break;

  case Task::LISTLANGUAGES:
    qDebug() << "Dequeuing LISTLANGUAGES task";
    listLanguagesQueue.dequeue();
    break;

  case Task::LISTCOMPONENTS:
    qDebug() << "Dequeuing LISTCOMPONENTS task";
    listComponentsQueue.dequeue();
    break;

  case Task::INSTALL:
    qDebug() << "Dequeuing INSTALL task";
    installQueue.dequeue();
    break;

  case Task::UNINSTALL:
    qDebug() << "Dequeueing UNINSTALL task";
    uninstallQueue.dequeue();
    break;
  }
}

QByteArray WeiduManager::readStdOut()
{
  return process->readAllStandardOutput();
}

QString WeiduManager::debugFile(const QString& gamePath, const QString& modName)
{
  if (!QFile::exists(gamePath + "/debugs")) {
    QDir dir(gamePath);
    dir.mkdir("debugs");
  }
  QFileInfo info(modName);
  QString logLoc = "debugs/";
  logLoc.append(info.baseName().toLower()).append(".debug");
  return logLoc;
}

void WeiduManager::enqueue(Task task, QQueue<QString>& queue, QString string)
{
  taskQueue.enqueue(task);
  queue.enqueue(string);
}

void WeiduManager::enqueue(Task task, QQueue<QPair<QString, int>>& queue, QPair<QString, int> tuple)
{
  taskQueue.enqueue(task);
  queue.enqueue(tuple);
}

void WeiduManager::enqueue(Task task, QQueue<QList<WeiduLogComponent>>& queue, WeiduLog* modList)
{
  if (!modList->isEmpty()) {
    foreach (QList<WeiduLogComponent> mod, modList->data) {
      if (!mod.isEmpty()) {
        taskQueue.enqueue(task);
        queue.enqueue(mod);
      }
    }
  }
  delete modList;
  modList = nullptr;
}

void WeiduManager::versionTask()
{
  qDebug() << "Attempting to get version";
  QStringList arguments;
  arguments << "--version";
  startTask(arguments);
}

void WeiduManager::listLanguagesTask()
{
  QString tp2 = listLanguagesQueue.head();
  qDebug() << "Attempting to list languages in" << tp2;
  QStringList arguments;
  arguments << "--list-languages" << tp2;
  startTask(arguments);
}

void WeiduManager::listComponentsTask()
{
  QPair<QString, int> tuple = listComponentsQueue.head();
  QString tp2 = tuple.first;
  int index = tuple.second;
  qDebug() << "Attempting to list components in" << tp2 << "for language" << index;
  QStringList arguments;
  arguments << "--list-components" << tp2 << QString::number(index);
  startTask(arguments);
}

void WeiduManager::installTask()
{
  QList<WeiduLogComponent> mod = installQueue.head();
  QString tp2 = mod[0].modName;
  qDebug() << "Attempting to install components for" << tp2;
  QStringList arguments;
  arguments << tp2;
  arguments << "--skip-at-view" << "--no-exit-pause";
  arguments << "--log" << debugFile(gamePath, tp2);
  arguments << "--language" << QString::number(mod[0].language);
  arguments << "--force-install-list";
  foreach (WeiduLogComponent comp, mod) {
    arguments << QString::number(comp.number);
  }
  startTask(arguments);
}

void WeiduManager::uninstallTask()
{
  QList<WeiduLogComponent> mod = uninstallQueue.head();
  QString tp2 = mod[0].modName;
  qDebug() << "Attempting to uninstall components for" << tp2;
  QStringList arguments;
  arguments << tp2;
  arguments << "--skip-at-view" << "--no-exit-pause";
  arguments << "--log" << debugFile(gamePath, tp2);
  arguments << "--force-uninstall-list";
  foreach (WeiduLogComponent comp, mod) {
    arguments << QString::number(comp.number);
  }
  startTask(arguments);
}

/* Not for general use */
QByteArray WeiduManager::run(const QStringList& arguments)
{
  process->start(weiduPath, arguments);
  process->waitForFinished();
  return process->readAllStandardOutput();
}

void WeiduManager::version()
{
  qDebug() << "Enqueuing VERSION task";
  taskQueue.enqueue(Task::VERSION);
  doTask();
}

void WeiduManager::newGamePath(const QString& path)
{
  qDebug() << "WeiduManager got game path" << path;
  if (!path.isEmpty()) {
    gamePath = path;
    enqueue(Task::GAMEPATH, gamePathQueue, gamePath);
    doTask();
  }
}

void WeiduManager::getLanguageList(const QString& tp2)
{
  qDebug() << "Enqueuing LISTLANGUAGES task";
  enqueue(Task::LISTLANGUAGES, listLanguagesQueue, tp2);
  doTask();
}

void WeiduManager::getComponentList(const QString& tp2, const int& index)
{
  qDebug() << "Enqueuing LISTCOMPONENTS task";
  QPair<QString, int> tuple;
  tuple.first = tp2;
  tuple.second = index;
  enqueue(Task::LISTCOMPONENTS, listComponentsQueue, tuple);
  doTask();
}

void WeiduManager::install(WeiduLog* modList)
{
  qDebug() << "Enqueuing INSTALL task(s)";
  enqueue(Task::INSTALL, installQueue, modList);
  doTask();
}

void WeiduManager::uninstall(WeiduLog* modList)
{
  qDebug() << "Enqueuing UNINSTALL task(s)";
  enqueue(Task::UNINSTALL, uninstallQueue, modList);
  doTask();
}
