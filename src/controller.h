#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QThread>

//class WeiduManager;

class QString;

class Controller : public QObject
{
  Q_OBJECT

public:
  Controller(QObject* parent = 0);

public slots:
  void setupWeidu(const QString& weiduPath, QString gamePath);
  void weiduVersion(const QString& version);

signals:
  void getVersion();

private:
  QThread* workerThread;

  //bool valid(const WeiduManager* manager) const;

};

#endif // CONTROLLER_H
