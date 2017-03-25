#ifndef RADIOBUTTONDELEGATE_H
#define RADIOBUTTONDELEGATE_H

#include <QStyledItemDelegate>

class QModelIndex;
class QPainter;
class QSize;
class QStyleOptionViewItem;
class QWidget;

class RadioButtonDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  RadioButtonDelegate(QWidget* parent);

  void paint(QPainter* painter, const QStyleOptionViewItem& option,
             const QModelIndex& index) const;
  virtual QSize sizeHint(const QStyleOptionViewItem& option,
                         const QModelIndex& index) const;

  enum Role {
    RADIO_ROLE = Qt::UserRole + 100,
    RADIO_SELECTED
  };
};

#endif // RADIOBUTTONDELEGATE_H
