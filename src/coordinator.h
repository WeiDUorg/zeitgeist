#ifndef COORDINATOR_H
#define COORDINATOR_H

#include <QObject>

class DataManager;
class Controller;
class WeiduLog;
class WeiduManager;

class Coordinator : public QObject
{
  Q_OBJECT

public:
  Coordinator(QObject* parent = nullptr);

  const DataManager* dataManager;
  const Controller* controller;

private slots:
  void weiduPath(const QString& path);
  void weiduFailedValidation(const QString& path);
  void newWeiduManager(const WeiduManager* manager);
  void provideGamePath();

public slots:
  void restoreState();

signals:
  void weiduPathSignal(const QString& path);
  void weiduFailedValidationSignal(const QString& weiduPath);
  void restoreStateSignal();
  void installerAvailable(bool available);
  void createModDistArchive(const QString& targetName);
};

#endif // COORDINATOR_H
