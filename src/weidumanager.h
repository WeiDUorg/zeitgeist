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
  WeiduManager(const QString& weiduPath, QString gamePath);
  bool valid() const;

public slots:
  void terminateManager();
  void quack();
  void version();

signals:
  void quacks(const bool& quacks);
  void versionSignal(const int& version);

private:
  QByteArray run(const QStringList& arguments);

  const QString weiduPath;
  QString gamePath;
  QProcess* process;

};

#endif // WEIDUMANAGER_H
