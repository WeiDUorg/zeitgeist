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

#include "enqueuemodmodel.h"
#include "weidulog.h"

#include <algorithm>
#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QStandardItem>
#include <QVariant>

EnqueueModModel::EnqueueModModel(QObject* parent) :
  QStandardItemModel(parent)
{

}

void EnqueueModModel::clear()
{
  beginResetModel();
  removeRows(0, rowCount());
  removeColumns(0, columnCount());
  endResetModel();
}

void EnqueueModModel::populate(const QJsonDocument& components,
                               QList<int> installed,
                               QList<int> queued)
{
  clear();
  QHash<QString, QStandardItem*> subgroups;
  QList<QStandardItem*> itemList;
  QJsonArray componentList = components.array();
  for (int i = 0; i < componentList.size(); ++i) {
    QJsonObject comp = componentList.at(i).toObject();
    int number = comp.value("number").toInt();
    if (!installed.contains(number) && !queued.contains(number)) {
      QStandardItem* item = new QStandardItem(comp.value("name").toString());
      item->setData(QVariant(number), COMPONENT_NUMBER);
      item->setData(QVariant(comp.value("index").toInt()), COMPONENT_INDEX);
      item->setCheckable(true);

      // InstallByDefault
      if (comp.value("forced").toBool() && !comp.contains("subgroup")) {
        item->setCheckState(Qt::Checked);
        item->setEnabled(false);
      }

      // Subgroups
      if (comp.contains("subgroup")) {
        QString subgroup = comp.value("subgroup").toString();
        QStandardItem* parent;
        if (subgroups.contains(subgroup)) {
          parent = subgroups.value(subgroup);
        } else {
          parent = new QStandardItem(subgroup);
          subgroups.insert(subgroup, parent);
          itemList << parent;
        }
        parent->setChild(parent->rowCount(), item);
      } else { // Top-level component
        itemList << item;
      }
    }
  }
  appendColumn(itemList);
}

WeiduLog* EnqueueModModel::selected(const QString& mod, int lang) const
{
  QStandardItem* root = invisibleRootItem();
  QList<WeiduLogComponent> init;
  QList<QStandardItem*> queue;
  QList<QList<WeiduLogComponent>> data;
  QList<WeiduLogComponent> result = checkChildren(mod, lang, root, init, queue);
  std::sort(result.begin(), result.end(),
            [](const WeiduLogComponent l, const WeiduLogComponent r) {
              return l.index < r.index;
            });
  data.append(result);
  return new WeiduLog(0, data); // WeiduLog intended for QueuedModsModel
}

QList<WeiduLogComponent> EnqueueModModel::checkChildren(const QString& mod,
                                                        int lang,
                                                        const QStandardItem* parent,
                                                        QList<WeiduLogComponent> acc,
                                                        QList<QStandardItem*> queue) const
{
  for (int i = 0; i < parent->rowCount(); ++i) {
    QStandardItem* child = parent->child(i);
    if (child->checkState() == Qt::Checked) {
      WeiduLogComponent c = {mod, child->data(COMPONENT_INDEX).toInt(),
                             lang, child->data(COMPONENT_NUMBER).toInt(),
                             child->text()};
      acc.append(c);
    }
    if (child->hasChildren()) {
      queue.append(child);
    }
  }
  if (queue.isEmpty()) {
    return acc;
  } else {
    return checkChildren(mod, lang, queue.takeFirst(), acc, queue.mid(1));
  }
}
