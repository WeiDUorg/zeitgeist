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

public slots:
  void weiduPath(const QString& path);

signals:
  void setWeiduPath(const QString& path, QString gamePath);
};

#endif // COORDINATOR_H
