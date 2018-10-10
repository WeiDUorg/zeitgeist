#ifndef LOGREADER_H
#define LOGREADER_H

#include <QObject>
#include <QList>

class WeiduLog;
struct WeiduLogComponent;

class QByteArray;
class QMutex;
class QString;

class LogReader : public QObject
{
  Q_OBJECT

public:
  LogReader(QMutex* weiduLog);

  static WeiduLog* read(QObject* parent, const QByteArray& data);

private slots:
  void readLog(const QString& path);
  void terminateReader();

signals:
  void logFile(WeiduLog* logFile);

private:
  QMutex* weiduLog;

  WeiduLog* read(QObject* parent, const QString& path);
  static bool validLine(const QString& line);
  static WeiduLogComponent parseLine(int index, const QString& line);
  static QList<QList<WeiduLogComponent>> partitionData(const QList<WeiduLogComponent>& data);
  static QList<WeiduLogComponent> getContiguousBlock(const QList<WeiduLogComponent>& data,
                                                     int index, const QString& name);
};

#endif // LOGREADER_H
