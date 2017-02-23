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

#include "queuedmodsmodel.h"
#include "weidulog.h"

#include <QDebug>
#include <QStandardItem>
#include <QString>

QueuedModsModel::QueuedModsModel(QObject* parent) :
  QStandardItemModel(parent)
{

}

void QueuedModsModel::clear()
{
  beginResetModel();
  removeColumns(0, columnCount());
  endResetModel();
}

void QueuedModsModel::add(WeiduLog* componentList)
{
  queue.append(componentList);
  QList<QStandardItem*> parentList = takeColumn(0);
  foreach (QList<WeiduLogComponent> list, componentList->data) {
    QStandardItem* parentItem = new QStandardItem(list.first().modName);
    QList<QStandardItem*> childItems;
    foreach (WeiduLogComponent comp, list) {
      childItems.append(new QStandardItem(comp.comment));
    }
    bool merged = false;
    if (!parentList.isEmpty()) {
      QStandardItem* lastParent = parentList.last();
      if (parentItem->text().compare(lastParent->text()) == 0) {
        QList<QStandardItem*> children = lastParent->takeColumn(0);
        children.append(childItems);
        lastParent->appendColumn(children);
        merged = true;
      }
    }
    if (!merged) {
      parentItem->appendColumn(childItems);
      parentList << parentItem;
    }
  }
  appendColumn(parentList);
}

QList<int> QueuedModsModel::queuedComponents(const QString& tp2) const
{
  QList<int> result;
  foreach (WeiduLog* modList, queue) {
    foreach (QList<WeiduLogComponent> compList, modList->data) {
      foreach (WeiduLogComponent comp, compList) {
        if (comp.modName.toUpper().compare(tp2, Qt::CaseInsensitive) == 0) {
          result.append(comp.number);
        }
      }
    }
  }
  return result;
}
