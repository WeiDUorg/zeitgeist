#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QMutex>
#include <QObject>
#include <QThread>

class WeiduLog;
class WeiduManager;

class QJsonDocument;
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
  void quacks(bool quacks);
  void weiduCheck() const;

  void weiduVersion(int version);
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, int index);
  void processQueues(WeiduLog* install, WeiduLog* uninstall);

signals:
  void weiduFailedValidation(const QString& weiduPath);
  void terminateManager();
  void terminateReader();
  void doesItQuack();
  void newWeiduManager(const WeiduManager* manager);
  void confirmedWeiduPath(const QString& path) const;

  void getVersion();
  void weiduVersionSignal(int version) const;
  void weiduListLanguages(const QString& tp2);
  void languageList(const QStringList& list);
  void weiduListComponents(const QString& tp2, const int& index);
  void componentList(const QString& tp2, int lang,
                     const QJsonDocument& componentList);
  void weiduInstall(WeiduLog* modList);
  void weiduUninstall(WeiduLog* modList);
  void installTaskStarted();
  void installTaskEnded();
  void processOutput(const QString& text);
  void processInput(const QString& text);

  void readLog(const QString& path);
  void logFile(WeiduLog* logFile);

private:
  QMutex weiduLog;
  QThread* workerThread;
  QThread* readerThread;
  QString currentWeidu;
  int currentWeiduVersion;
  WeiduManager* weiduManager;
};

#endif // CONTROLLER_H
