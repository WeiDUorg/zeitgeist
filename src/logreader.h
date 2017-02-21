#ifndef LOGREADER_H
#define LOGREADER_H

#include <QObject>

class WeiduLog;
class WeiduLogComponent;

class QByteArray;
class QString;

class LogReader : QObject
{
  Q_OBJECT

public:
  static WeiduLog* read(QObject* parent, const QString& path);
  static WeiduLog* read(QObject* parent, const QByteArray& data);

private:
  static bool validLine(const QString& line);
  static WeiduLogComponent parseLine(const QString& line);
};

#endif // LOGREADER_H
