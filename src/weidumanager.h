#ifndef WEIDUMANAGER_H
#define WEIDUMANAGER_H

#include "weidulog.h"

#include <QObject>
#include <QPair>
#include <QProcess>
#include <QQueue>

class QByteArray;
class QJsonDocument;
class QMutex;
class QMutexLocker;
class QProcess;
class QString;
class QStringList;

enum class Task {
  VERSION,
  GAMEPATH,
  LISTLANGUAGES,
  LISTCOMPONENTS,
  INSTALL,
  UNINSTALL
};

class WeiduManager : public QObject
{
  Q_OBJECT

public:
  WeiduManager(const QString& weiduPath, QMutex* weiduLog);
  bool executable() const;

private slots:
  void terminateManager();
  void quack();
  void processInput(const QString& text);
  void readProcessOutput();

  /* Slots to queue up tasks */
  void version();
  void newGamePath(const QString& path, const bool& eeGame);
  void eeLang(const QString& lang);
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, const int& index);
  void install(WeiduLog* modList);
  void uninstall(WeiduLog* modList);

  /* Not for external use, not even as a slot */
  void endTask(int exitCode, QProcess::ExitStatus exitStatus);

signals:
  void quacks(const bool& quacks);

  /* Signals for communicating results */
  void versionSignal(const int& version);
  void languageList(const QStringList& languageList);
  void componentList(const QString& tp2, int lang,
                     const QJsonDocument& componentList);
  void modStackChanged(const QString& gamePath);
  void installTaskStarted();
  void installTaskEnded();
  void processOutput(const QString& text);

private:
  QByteArray run(const QStringList& arguments);
  void doTask();
  void startTask(const QStringList& arguments);
  void dequeue();
  QString debugFile(const QString& gamePath, const QString& modName);
  void enqueue(Task task,
               QQueue<QString>& queue,
               QString string);
  void enqueue(Task task,
               QQueue<QPair<QString, int>>& queue,
               QPair<QString, int> tuple);
  void enqueue(Task task,
               QQueue<QList<WeiduLogComponent>>& queue,
               WeiduLog* modList);

  /* Tasks */
  void versionTask();
  void listLanguagesTask();
  void listComponentsTask();
  void installTask();
  void uninstallTask();

  const QString weiduPath;
  QMutex* weiduLog;
  QMutexLocker* weiduLogLocker;
  QString gamePath;
  bool eeGame = false;
  QString eeLangDir = "en_us";
  QProcess* process;

  bool syncMethodsDisabled = false;
  bool busy = false;
  bool broadcast = false;
  QByteArray readBuffer;
  QQueue<Task> taskQueue;
  QQueue<QString> gamePathQueue;
  QQueue<QString> listLanguagesQueue;
  QQueue<QPair<QString, int>> listComponentsQueue;
  QQueue<QList<WeiduLogComponent>> installQueue;
  QQueue<QList<WeiduLogComponent>> uninstallQueue;
};

#endif // WEIDUMANAGER_H
