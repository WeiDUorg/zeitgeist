#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class QString;

class Controller : public QObject
{
  Q_OBJECT

public:
  Controller(QObject* parent = 0);

};

#endif // CONTROLLER_H
