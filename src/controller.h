#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QQueue>
#include <QThread>

class WeiduLog;
class WeiduManager;

class QString;
class QStringList;

class Controller : public QObject
{
  Q_OBJECT

public:
  Controller(QObject* parent);
  ~Controller();

private slots:
  void setupWeidu(const QString& weiduPath);
  void quacks(const bool& quacks);
  void weiduCheck() const;

  void weiduVersion(const int& version);
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, const int& index);
  void processQueues(WeiduLog* install, WeiduLog* uninstall);

signals:
  void weiduFailedValidation(const QString& weiduPath);
  void terminateManager();
  void doesItQuack();
  void newWeiduManager(const WeiduManager* manager);
  void confirmedWeiduPath(const QString& path) const;

  void getVersion();
  void weiduVersionSignal(const int& version) const;
  void weiduListLanguages(const QString& tp2);
  void languageList(const QStringList& list);
  void weiduListComponents(const QString& tp2, const int& index);
  void componentList(WeiduLog* componentList);

private:
  QThread* workerThread;
  QString currentWeidu;
  int currentWeiduVersion;
  WeiduManager* weiduManager;
  QQueue<WeiduLog*> installQueue;
  QQueue<WeiduLog*> uninstallQueue;
};

#endif // CONTROLLER_H
