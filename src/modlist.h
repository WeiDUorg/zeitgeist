#ifndef MODLIST_H
#define MODLIST_H

#include <QWidget>

class QString;
class QListView;

class ModList : public QWidget
{
  Q_OBJECT

public:
  ModList(const QString& title, QWidget* parent = 0);

private:
  QListView* view;
};

#endif // MODLIST_H
