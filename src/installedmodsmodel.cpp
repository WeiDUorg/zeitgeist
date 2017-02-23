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

#include "installedmodsmodel.h"
#include "weidulog.h"

#include <algorithm>
#include <QDebug>
#include <QList>
#include <QListIterator>
#include <QModelIndex>
#include <QModelIndexList>
#include <QStandardItem>
#include <QString>

InstalledModsModel::InstalledModsModel(QObject* parent) :
  QStandardItemModel(parent)
{

}

void InstalledModsModel::clear()
{
  beginResetModel();
  removeRows(0, rowCount());
  endResetModel();
}

void InstalledModsModel::populate(const WeiduLog* logFile)
{
  /* logFile belongs to currentGame */
  clear();
  partitionedData = logFile->data;
  QList<QString> partitionNames = logFile->getPartitionNames();
  QList<QStandardItem*> parentItems;
  parentItems.reserve(partitionNames.size());
  foreach (QString name, partitionNames) {
    parentItems.append(new QStandardItem(name));
  }
  appendColumn(parentItems);
  QList<QList<WeiduLogComponent>>::const_iterator i;
  for (i = partitionedData.constBegin(); i != partitionedData.constEnd(); ++i) {
    int index = i - partitionedData.constBegin();
    QStandardItem* parent = parentItems.at(index);
    QList<QStandardItem*> children = getChildList(*i);
    parent->appendColumn(children);
  }
  lookup = populateLookup();
}

QList<QStandardItem*> InstalledModsModel::getChildList(const QList<WeiduLogComponent>& componentList) const
{
  QList<QStandardItem*> childList;
  foreach (WeiduLogComponent component, componentList) {
    childList.append(new QStandardItem(component.comment));
  }
  return childList;
}

QHash<QString, QList<int>> InstalledModsModel::populateLookup()
{
  QHash<QString, QList<int>> result;
  foreach (const QList<WeiduLogComponent> data, partitionedData) {
    foreach (const WeiduLogComponent c, data) {
      QString modName = c.modName.toUpper();
      QList<int> components = result.value(modName);
      components.append(c.number);
      result.insert(modName, components);
    }
  }
  return result;
}

QList<int> InstalledModsModel::installedComponents(const QString& tp2) const
{
  return lookup.value(tp2.toUpper());
}

WeiduLog* InstalledModsModel::selectedComponents(const QModelIndexList& indexList) const
{
  // Accumulate the component indices by block (parent index)
  QHash<int, QList<int>> acc;
  foreach (QModelIndex index, indexList) {
    QModelIndex parent = index.parent();
    if (parent.isValid()) {
      int i = parent.row();
      QList<int> block = acc.value(i);
      block.append(index.row());
      acc.insert(i, block);
    }
  }
  // Sort the component indices
  QHash<int, QList<int>>::const_iterator i;
  for (i = acc.constBegin(); i != acc.constEnd(); ++i) {
    QList<int> list = i.value();
    std::sort(list.begin(), list.end(), [](const int i, const int j) {
        return i < j;
      });
    acc.insert(i.key(), list);
  }
  // Sort the mod indices
  QList<int> modIndices = acc.keys();
  std::sort(modIndices.begin(), modIndices.end(), [](const int i, const int j) {
      return i < j;
    });
  // Build the result
  QList<QList<WeiduLogComponent>> result;
  foreach (int modIndex, modIndices) {
    QList<WeiduLogComponent> components = partitionedData[modIndex];
    QList<WeiduLogComponent> innerResult;
    foreach (int componentIndex, acc.value(modIndex)) {
      innerResult.append(components.at(componentIndex));
    }
    result.append(innerResult);
  }
  return new WeiduLog(0, result);
}
