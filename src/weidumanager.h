#ifndef WEIDUMANAGER_H
#define WEIDUMANAGER_H

#include <QObject>

class QString;

class WeiduManager : public QObject
{
  Q_OBJECT

public:
  WeiduManager(const QString& weiduPath, QString gamePath);
  bool valid() const;

public slots:
  void version();
  void terminateManager();

signals:
  void versionSignal(const QString& version);

private:
  const QString weiduPath;
  QString gamePath;

};

#endif // WEIDUMANAGER_H
