#ifndef WEIDUMANAGER_H
#define WEIDUMANAGER_H

#include <QObject>

class QString;

class WeiduManager : public QObject
{
  Q_OBJECT

public:
  WeiduManager(const QString& weiduPath);
  bool valid() const;

public slots:
  void version();

signals:
  void versionSignal(const QString& version);

private:
  const QString weiduPath;

};

#endif // WEIDUMANAGER_H
