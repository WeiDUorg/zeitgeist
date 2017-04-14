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
#include "radiobuttondelegate.h"
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
  connect(this, SIGNAL(itemChanged(QStandardItem*)),
          this, SLOT(handleChangedState()));
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
      item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

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
          parent->setFlags(parent->flags() & ~Qt::ItemIsSelectable);
          subgroups.insert(subgroup, parent);
          itemList << parent;
        }
        item->setCheckable(false);
        item->setData(QVariant(true), RadioButtonDelegate::RADIO_ROLE);
        item->setData(QVariant(false), RadioButtonDelegate::RADIO_SELECTED);
        parent->setChild(parent->rowCount(), item);
        // FORCED_SUBCOMPONENT
        if (comp.contains("forced") && comp.value("forced").toBool() &&
            !forcedSubgroups.contains(subgroup)) {
          forcedSubgroups.append(subgroup);
          parent->child(0)->setData(QVariant(true),
                                    RadioButtonDelegate::RADIO_SELECTED);
        }
      } else { // Top-level component
        item->setCheckable(true);
        itemList << item;
      }
    }
  }
  appendColumn(itemList);
  emit componentsAvailable(!itemList.isEmpty());
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
    if (child->data(RadioButtonDelegate::RADIO_SELECTED).toBool()) {
      WeiduLogComponent c = {mod, child->data(COMPONENT_INDEX).toInt(),
                             lang, child->data(COMPONENT_NUMBER).toInt(),
                             parent->text() + QString(" -> ") + child->text()};
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

void EnqueueModModel::radioToggled(const QModelIndex& index)
{
  QModelIndex parentIndex = index.parent();
  if (parentIndex.isValid()) {
    QStandardItem* parent = itemFromIndex(parentIndex);
    QString name = parent->text();
    for (int i = 0; i < parent->rowCount(); ++i) {
      QStandardItem* childItem = parent->child(i, index.column());
      bool alreadySelected =
        childItem->data(RadioButtonDelegate::RADIO_SELECTED).toBool();
      QVariant select(i == index.row() &&
                      (!alreadySelected || forcedSubgroups.contains(name)));
      childItem->setData(select, RadioButtonDelegate::RADIO_SELECTED);
    }
  }
}

void EnqueueModModel::select(bool select)
{
  SelectionInProgress deferSignals(this);
  QStandardItem* root = invisibleRootItem();
  for (int i = 0; i < root->rowCount(); ++i) {
    QStandardItem* child = root->child(i);
    if (child->hasChildren() && !forcedSubgroups.contains(child->text())) {
      bool alreadySelected = false;
      for (int j = 0; j < child->rowCount() && !alreadySelected; ++j) {
        QStandardItem* childer = child->child(j);
        bool selected =
          childer->data(RadioButtonDelegate::RADIO_SELECTED).toBool();
        if (select && selected) {
          alreadySelected = true;
        } else {
          childer->setData(QVariant(false),
                           RadioButtonDelegate::RADIO_SELECTED);
        }
      }
      if (select && !alreadySelected) {
        child->child(0)->setData(QVariant(true),
                                 RadioButtonDelegate::RADIO_SELECTED);
      }
    }
    if (!child->hasChildren() && child->isEnabled()) {
      Qt::CheckState state;
      if (select) {
        state = Qt::Checked;
      } else {
        state = Qt::Unchecked;
      }
      child->setCheckState(state);
    }
  }
}

void EnqueueModModel::handleChangedState()
{
  bool anyChecked = false;
  bool anyUnchecked = false;
  QStandardItem* root = invisibleRootItem();
  for (int i = 0; i < root->rowCount(); ++i) {
    QStandardItem* child = root->child(i);
    if (child->hasChildren() && !forcedSubgroups.contains(child->text())) {
      bool unselected = true;
      for (int j = 0; j < child->rowCount(); ++j) {
        QStandardItem* childer = child->child(j);
        bool selected =
          childer->data(RadioButtonDelegate::RADIO_SELECTED).toBool();
        if (selected) {
          anyChecked = true;
          unselected = false;
        }
      }
      if (unselected) {
        anyUnchecked = true;
      }
    }
    if (!child->hasChildren() && child->isEnabled()) {
      switch (child->checkState()) {
      case Qt::Checked: {
        anyChecked = true;
        break;
      }

      case Qt::Unchecked: {
        anyUnchecked = true;
        break;
      }

      default: { }
      }
    }
  }
  if (anyChecked && !anyUnchecked) {
    emit stateChanged(Qt::Checked);
  }
  if (!anyChecked && anyUnchecked) {
    emit stateChanged(Qt::Unchecked);
  }
  if (anyChecked && anyUnchecked) {
    emit stateChanged(Qt::PartiallyChecked);
  }
  if (!anyChecked && !anyUnchecked) {
    emit stateChanged(Qt::Unchecked);
  }
}

SelectionInProgress::SelectionInProgress(EnqueueModModel* model) :
  model(model)
{
  disconnect(model, SIGNAL(itemChanged(QStandardItem*)),
             model, SLOT(handleChangedState()));
}

SelectionInProgress::~SelectionInProgress()
{
  connect(model, SIGNAL(itemChanged(QStandardItem*)),
          model, SLOT(handleChangedState()));
  model->handleChangedState();
}
