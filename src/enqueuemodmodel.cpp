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

#include <QDebug>
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
  QList<QStandardItem*> itemList;
  QJsonArray componentList = components.array();
  for (int i = 0; i < componentList.size(); ++i) {
    QJsonObject comp = componentList.at(i).toObject();
    int number = comp.value("number").toInt();
    if (!installed.contains(number) && !queued.contains(number)) {
      QStandardItem* item = new QStandardItem(comp.value("name").toString());
      item->setData(QVariant(number), COMPONENT_NUMBER);
      item->setCheckable(true);
      itemList << item;
    }
  }
  appendColumn(itemList);
}

WeiduLog* EnqueueModModel::selected(const QString& mod, int lang)
{
  QList<WeiduLogComponent> result;
  QStandardItem* root = invisibleRootItem();
  for (int i = 0; i < root->rowCount(); ++i) {
    QStandardItem* item = root->child(i);
    if (item->checkState() == Qt::Checked) {
      WeiduLogComponent c =
        {mod, lang, item->data(COMPONENT_NUMBER).toInt(),
         item->text()};
      result << c;
    }
  }
  QList<QList<WeiduLogComponent>> data;
  data << result;
  return new WeiduLog(0, data); // WeiduLog intended for QueuedModsModel
}
