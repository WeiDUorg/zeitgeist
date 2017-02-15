#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

class QString;

class Controller : public QObject
{
  Q_OBJECT

public:
  Controller(QObject* parent);
  ~Controller();

public slots:
  void setupWeidu(const QString& weiduPath, QString gamePath);
  void quacks(const bool& quacks);
  void weiduVersion(const int& version);

signals:
  void weiduFailedValidation(const QString& weiduPath);
  void terminateManager();
  void doesItQuack();
  void getVersion();
  void weiduVersionSignal(const int& version);

private:
  QThread* workerThread;
  QString currentWeidu;
};

#endif // CONTROLLER_H
