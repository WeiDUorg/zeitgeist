#ifndef MODLIST_H
#define MODLIST_H

#include <QWidget>

class QString;

class ModList : public QWidget
{
  Q_OBJECT

 public:
  ModList(QString title);
};

#endif // MODLIST_H
