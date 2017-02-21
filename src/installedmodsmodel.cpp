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

#include <QList>
#include <QListIterator>
#include <QStandardItem>
#include <QString>
#include <QtDebug>

InstalledModsModel::InstalledModsModel(QObject* parent) :
  QStandardItemModel(parent)
{

}

void InstalledModsModel::clear()
{
  beginResetModel();
  removeColumns(0, columnCount());
  endResetModel();
}

void InstalledModsModel::populate(const WeiduLog* logFile)
{
  clear();
  QList<WeiduLogComponent> data = logFile->data;
  QList<QList<WeiduLogComponent>> partitionedData = partitionData(data);
  QList<QString> partitionNames = getPartitionNames(partitionedData);
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
}

QList<QList<WeiduLogComponent>> InstalledModsModel::partitionData(const QList<WeiduLogComponent>& data) const
{
  QList<QList<WeiduLogComponent>> partitionedData;
  QList<WeiduLogComponent>::const_iterator i;
  for (i = data.constBegin(); i != data.constEnd(); ++i) {
    int distance = i - data.constBegin();
    QList<WeiduLogComponent> block = getContiguousBlock(data, distance, (*i).modName);
    partitionedData.append(block);
    i += (block.length() - 1);
  }
  qDebug() << "Playing back read log entries:";
  foreach (QList<WeiduLogComponent> list, partitionedData) {
    qDebug() << "New block";
    foreach (WeiduLogComponent comp, list) {
      qDebug() << comp.modName;
    }
  }
  qDebug() << "End of log entries";
  return partitionedData;
}

QList<WeiduLogComponent> InstalledModsModel::getContiguousBlock(const QList<WeiduLogComponent>& data, const int& index, const QString& name) const
{
  QList<WeiduLogComponent> block;
  QList<WeiduLogComponent>::const_iterator i;
  for (i = data.constBegin() + index; i != data.constEnd() && (*i).modName.compare(name) == 0; ++i) {
    block.append(*i);
  }
  return block;
}

QList<QString> InstalledModsModel::getPartitionNames(const QList<QList<WeiduLogComponent>>& partitionedData) const
{
  QList<QString> names;
  names.reserve(partitionedData.size());
  foreach (QList<WeiduLogComponent> list, partitionedData) {
    names.append(list.first().modName);
  }
  return names;
}

QList<QStandardItem*> InstalledModsModel::getChildList(const QList<WeiduLogComponent>& componentList) const
{
  QList<QStandardItem*> childList;
  foreach (WeiduLogComponent component, componentList) {
    childList.append(new QStandardItem(component.comment));
  }
  return childList;
}
