#ifndef MODTREE_H
#define MODTREE_H

#include <QWidget>

class QString;
class QTreeView;

class ModTree : public QWidget
{
  Q_OBJECT

 public:
  ModTree(QString title);

 private:
  QTreeView* view;
};

#endif // MODTREE_H
