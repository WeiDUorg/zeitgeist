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

#include "comboboxdelegate.h"

#include <QAbstractItemModel>
#include <QComboBox>
#include <QDebug>
#include <QModelIndex>
#include <QString>
#include <QStringList>

ComboBoxDelegate::ComboBoxDelegate(QWidget* parent) :
  QStyledItemDelegate(parent)
{

}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                      const QModelIndex& index) const
{
  const QAbstractItemModel* model = index.model();
  const QStringList data = model->data(index).toStringList();
  if (data.isEmpty() || (data.size() == 1)) {
    return QStyledItemDelegate::createEditor(parent, option, index);
  }
  QComboBox* box = new QComboBox(parent);
  foreach (QString datum, data) {
    box->addItem(datum);
  }
  return box;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
  const QAbstractItemModel* model = index.model();
  const QStringList data = model->data(index).toStringList();
  if (data.size() > 1) {
    if (QComboBox* box = static_cast<QComboBox*>(editor)) {
      QString text = index.data(Qt::EditRole).toString();
      int boxIndex = box->findText(text);
      if (boxIndex >= 0 && boxIndex < data.size()) {
        box->setCurrentIndex(boxIndex);
      }
    }
  } else {
    QStyledItemDelegate::setEditorData(editor, index);
  }
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const
{
  const QStringList data = model->data(index).toStringList();
  if (data.size() > 1) {
    if (QComboBox* box = static_cast<QComboBox*>(editor)) {
      model->setData(index, box->currentText(), Qt::EditRole);
    }
  } else {
    QStyledItemDelegate::setModelData(editor, model, index);
  }
}
