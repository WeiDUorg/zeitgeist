#ifndef MODLIST_H
#define MODLIST_H

#include <QWidget>

class QString;
class QListView;

class ModList : public QWidget
{
  Q_OBJECT

public:
  ModList(QString title, QWidget* parent = 0);

private:
  QListView* view;
};

#endif // MODLIST_H
