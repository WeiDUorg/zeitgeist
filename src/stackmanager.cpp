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

#include "stackmanager.h"
#include "logreader.h"
#include "weidulog.h"

#include <algorithm>
#include <QDebug>
#include <QString>

WeiduLog* StackManager::uninstall(WeiduLog* toUninstall,
                                  const WeiduLog* logFile,
                                  QStack<QList<WeiduLogComponent>>& reinstall)
{
  WeiduLog* sorted = sortForUninstall(toUninstall);
  delete toUninstall; toUninstall = nullptr;
  QStack<QStack<WeiduLogComponent>> log = toStack(logFile);
  const QList<QList<WeiduLogComponent>> modList = sorted->data;
  QList<QList<WeiduLogComponent>> result;

  QList<QList<WeiduLogComponent>>::const_iterator mod;
  for (mod = modList.constBegin(); mod != modList.constEnd(); ++mod) {
    QString blockName = (*mod).first().modName;
    bool modDone = false;
    while (!modDone && !log.isEmpty()) {
      QStack<WeiduLogComponent> logHead = log.pop();
      QString logHeadName = logHead.top().modName;
      if (blockName.compare(logHeadName, Qt::CaseInsensitive) == 0) {
        QList<WeiduLogComponent> subresult; QList<WeiduLogComponent> rb;
        QList<WeiduLogComponent>::const_iterator comp;
        for (comp = (*mod).constBegin(); comp != (*mod).constEnd(); ++comp) {
          bool compDone = false;
          while (!compDone && !logHead.isEmpty()) {
            WeiduLogComponent logComp = logHead.pop();
            if ((*comp) == logComp) {
              qDebug() << "Stack Manager is queuing component"
                       << logComp.comment << "of mod" << logHeadName
                       << "for uninstallation";
              subresult.append(logComp);
              /* logHead holds all components of (*mod) if it holds at
               * least one component, or something is wrong */
              compDone = true; modDone = true;
            } else {
              qDebug() << "Stack Manager is queuing component"
                       << logComp.comment << "of mod" << logHeadName
                       << "for reinstallation";
              subresult.append(logComp); rb.append(logComp);
            }
          }
        }
        /* if there are components left, put them back */
        if (!logHead.isEmpty()) { log.push(logHead); }

        result.append(subresult); if (!rb.isEmpty()) { reinstall.push(rb); }
      } else {
        qDebug() << "Stack Manager is queuing block" << logHeadName
                 << "for reinstallation";
        QList<WeiduLogComponent> subresult;
        foreach (WeiduLogComponent c, logHead) { subresult.append(c); }
        result.append(subresult); reinstall.push(subresult);
      }
    }
  }
  delete sorted; sorted = nullptr;
  return new WeiduLog(0, result);
}

WeiduLog* StackManager::sortForUninstall(const WeiduLog* modList)
{
  /* flatten would be better off as a WeiduLog method, right? ->
   * WeiduLog redo; data is currently const
   */
  QList<WeiduLogComponent> flat;
  foreach (QList<WeiduLogComponent> block, modList->data) {
    flat.append(block);
  }
  std::sort(flat.begin(), flat.end());
  std::reverse(flat.begin(), flat.end());
  WeiduLog* sorted = LogReader::read(0, flat);

  return sorted;
}

QStack<QStack<WeiduLogComponent>> StackManager::toStack(const WeiduLog* logFile)
{
  QStack<QStack<WeiduLogComponent>> result;
  foreach (QList<WeiduLogComponent> block, logFile->data) {
    QStack<WeiduLogComponent> subresult;
    foreach (WeiduLogComponent c, block) {
      subresult.push(c);
    }
    result.push(subresult);
  }
  return result;
}
