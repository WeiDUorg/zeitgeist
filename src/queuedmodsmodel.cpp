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

#include "queuedmodsmodel.h"
#include "weidulog.h"

#include <algorithm>
#include <QDebug>
#include <QHash>
#include <QModelIndex>
#include <QStandardItem>
#include <QString>
#include <QVariant>

QueuedModsModel::QueuedModsModel(QObject* parent) :
  QStandardItemModel(parent)
{

}

void QueuedModsModel::clear()
{
  beginResetModel();
  removeRows(0, rowCount());
  endResetModel();
}

void QueuedModsModel::add(WeiduLog* componentList)
{
  /* our WeiduLog */
  WeiduLog* deduplicated = deduplicate(componentList);
  if (!deduplicated->isEmpty()) {
    QList<QStandardItem*> parentList = takeColumn(0);
    foreach (QList<WeiduLogComponent> list, deduplicated->data) {
      QStandardItem* parentItem = new QStandardItem(list.first().modName);
      QList<QStandardItem*> childItems;
      foreach (WeiduLogComponent comp, list) {
        QString compName;
        if (!comp.comment.isEmpty()) {
          compName = comp.comment;
        } else {
          compName = comp.modName + " #" + QString::number(comp.language)
            + " #" + QString::number(comp.number);
        }
        QStandardItem* child = new QStandardItem(compName);
        child->setData(QVariant(comp.number), COMPONENT_NUMBER);
        child->setData(QVariant(comp.language), COMPONENT_LANGUAGE);
        childItems.append(child);
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
  delete deduplicated;
}

QList<int> QueuedModsModel::queuedComponents(const QString& tp2) const
{
  QList<int> result;
  QStandardItem* root = invisibleRootItem();
  for (int i = 0; i < root->rowCount(); ++i) {
    QStandardItem* block = root->child(i);
    if (block->text().compare(tp2, Qt::CaseInsensitive) == 0) {
      for (int j = 0; j < block->rowCount(); ++j) {
        QStandardItem* component = block->child(j);
        result.append(component->data(COMPONENT_NUMBER).toInt());
      }
    }
  }
  return result;
}

void QueuedModsModel::unqueue(const QModelIndexList& indices)
{
  // Accumulate component indices by block (parent index)
  QHash<int, QList<int>> acc;
  foreach (QModelIndex index, indices) {
    QModelIndex parent = index.parent();
    if (parent.isValid()) {
      int i = parent.row();
      QList<int> block = acc.value(i);
      block.append(index.row());
      acc.insert(i, block);
    }
  }
  // Sort and reverse the component indices
  QHash<int, QList<int>>::const_iterator i;
  for (i = acc.constBegin(); i != acc.constEnd(); ++i) {
    QList<int> list = i.value();
    std::sort(list.begin(), list.end(), [](const int i, const int j) {
        return i < j;
      });
    std::reverse(list.begin(), list.end());
    acc.insert(i.key(), list);
  }
  // Sort and reverse the mod indices
  QList<int> modIndices = acc.keys();
  std::sort(modIndices.begin(), modIndices.end(), [](const int i, const int j) {
      return i < j;
    });
  std::reverse(modIndices.begin(), modIndices.end());
  // Remove the rows (in the reverse order to not invalidate any indices)
  foreach(int modIndex, modIndices) {
    QList<int> compList = acc.value(modIndex);
    QModelIndex parent = index(modIndex, 0);
    foreach (int compIndex, compList) {
      removeRow(compIndex, parent);
    }
    if (!parent.child(0, 0).isValid()) {
      removeRow(modIndex);
    }
  }
}

WeiduLog* QueuedModsModel::queue()
{
  QList<QList<WeiduLogComponent>> result;
  QStandardItem* root = invisibleRootItem();
  for (int i = 0; i < root->rowCount(); ++i) {
    QStandardItem* mod = root->child(i);
    QList<WeiduLogComponent> compList;
    for (int j = 0; j < mod->rowCount(); ++j) {
      QStandardItem* comp = mod->child(j);
      WeiduLogComponent c = { mod->text(),
                              comp->data(COMPONENT_LANGUAGE).toInt(),
                              comp->data(COMPONENT_NUMBER).toInt(),
                              comp->text() };
      compList << c;
    }
    if (!compList.isEmpty()) {
      result << compList;
    }
  }
  return new WeiduLog(0, result); // WeiduLog intended for Controller
}

WeiduLog* QueuedModsModel::deduplicate(WeiduLog* logFile) const
{
  /* our WeiduLog */
  QList<QList<WeiduLogComponent>> result;
  QStandardItem* root = invisibleRootItem();
  foreach (QList<WeiduLogComponent> modList, logFile->data) {
    QString modName = modList[0].modName;
    QList<int> queuedComps;
    QList<WeiduLogComponent> dedup;
    for (int i = 0; i < root->rowCount(); ++i) {
      QStandardItem* queuedMod = root->child(i);
      if (modName.compare(queuedMod->text(), Qt::CaseInsensitive) == 0) {
        // grab all the queued component numbers
        for (int j = 0; j < queuedMod->rowCount(); ++j) {
          int compNumber = queuedMod->child(j)->data(COMPONENT_NUMBER).toInt();
          queuedComps << compNumber;
        }
      }
    }
    foreach (WeiduLogComponent comp, modList) {
      if (!queuedComps.contains(comp.number)) {
        dedup << comp;
      }
    }
    if (!dedup.isEmpty()) {
      result << dedup;
    }
  }
  delete logFile;
  logFile = nullptr;
  return new WeiduLog(0, result);
}
