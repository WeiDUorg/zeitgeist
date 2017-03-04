#ifndef COMBOBOXDELEGATE_H
#define COMBOBOXDELEGATE_H

#include <QStyledItemDelegate>

class QAbstractItemModel;
class QModelIndex;
class QStyleOptionViewItem;
class QWidget;

class ComboBoxDelegate : public QStyledItemDelegate
{
  Q_OBJECT

public:
  ComboBoxDelegate(QWidget* parent);

  virtual QWidget* createEditor(QWidget* parent,
                                const QStyleOptionViewItem& option,
                                const QModelIndex& index) const;
  virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
  virtual void setModelData(QWidget* editor, QAbstractItemModel* model,
                            const QModelIndex& index) const;
};

#endif // COMBOBOXDELEGATE_H
