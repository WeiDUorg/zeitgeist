#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>

class DataManager;
class Controller;
class WeiduManager;

class Coordinator : public QObject
{
  Q_OBJECT

public:
  Coordinator(QObject* parent = 0);

  const DataManager* dataManager;
  const Controller* controller;

public slots:
  void weiduPath(const QString& path);
  void weiduFailedValidation(const QString& path);
  void newWeiduManager(const WeiduManager* manager);
  void requestForGamePath();
  void restoreState();

signals:
  void setWeiduPath(const QString& path);
  void weiduFailedValidationSignal(const QString& weiduPath);
  void restoreStateSignal();
};

#endif // COORDINATOR_H
