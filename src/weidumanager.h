#ifndef WEIDUMANAGER_H
#define WEIDUMANAGER_H

#include <QObject>

class QByteArray;
class QProcess;
class QString;
class QStringList;

class WeiduManager : public QObject
{
  Q_OBJECT

public:
  WeiduManager(const QString& weiduPath);
  bool valid() const;

private slots:
  void terminateManager();
  void quack();
  void newGamePath(const QString& path);

  void version();

signals:
  void quacks(const bool& quacks);
  void versionSignal(const int& version);
  /* Emitted when a game path is needed but this has none */
  void requestGamePath();
  /* Emitted when a game path was requested but this got an empty one */
  void emptyGamePath();

private:
  QByteArray run(const QStringList& arguments);

  const QString weiduPath;
  QString gamePath;
  QProcess* process;

};

#endif // WEIDUMANAGER_H
