#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>

class DataManager;
class Controller;

class Coordinator : public QObject
{
  Q_OBJECT

public:
  Coordinator(QObject* parent = 0);

  DataManager* dataManager;
  Controller* controller;
};

#endif // COORDINATOR_H
