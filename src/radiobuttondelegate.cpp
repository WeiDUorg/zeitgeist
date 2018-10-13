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

#include "radiobuttondelegate.h"

#include <QAbstractItemModel>
#include <QApplication>
#include <QDebug>
#include <QRect>
#include <QSize>
#include <QStyle>
#include <QStyleOptionButton>
#include <QStyleOptionViewItem>

RadioButtonDelegate::RadioButtonDelegate(QWidget* parent) :
  QStyledItemDelegate(parent)
{

}

void RadioButtonDelegate::paint(QPainter* painter,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const
{
  const QAbstractItemModel* model = index.model();
  if (model->data(index, RADIO_ROLE).toBool()) {
    QStyleOptionButton opt;
    opt.rect = option.rect;
    opt.text = index.data().toString();
    opt.state = option.state;
    opt.state |= model->data(index, RADIO_SELECTED).toBool() ?
      QStyle::State_On : QStyle::State_Off;
    QStyle* style = option.widget ?
      option.widget->style() : QApplication::style();
    style->drawControl(QStyle::CE_RadioButton, &opt, painter, option.widget);
  } else {
    QStyledItemDelegate::paint(painter, option, index);
  }
}

QSize RadioButtonDelegate::sizeHint(const QStyleOptionViewItem& option,
                                    const QModelIndex& index) const
{
  const QAbstractItemModel* model = index.model();
  QSize s = QStyledItemDelegate::sizeHint(option, index);
  if (model->data(index, RADIO_ROLE).toBool()) {
    int radioButtonHeight =
      QApplication::style()->pixelMetric(QStyle::PM_ExclusiveIndicatorHeight,
                                         &option);
    s.setHeight(qMax(s.height(), radioButtonHeight));
  }
  return s;
}
