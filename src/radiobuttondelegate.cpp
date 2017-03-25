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
    int radioButtonWidth =
      QApplication::style()->pixelMetric(QStyle::PM_ExclusiveIndicatorWidth,
                                         &option);
    int spacing =
      QApplication::style()->pixelMetric(QStyle::PM_RadioButtonLabelSpacing,
                                         &option);
    QStyleOptionViewItem opt = option;
    int left = opt.rect.left();
    opt.rect.setLeft(left + spacing + radioButtonWidth);
    QStyledItemDelegate::paint(painter, opt, index);
    opt.rect.setLeft(left + spacing / 2);
    opt.rect.setWidth(radioButtonWidth);
    opt.state |= model->data(index, RADIO_SELECTED).toBool() ?
      QStyle::State_On : QStyle::State_Off;
    QApplication::style()->drawPrimitive(QStyle::PE_IndicatorRadioButton,
                                         &opt, painter);
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
