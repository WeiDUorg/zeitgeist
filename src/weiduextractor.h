#ifndef WEIDUEXTRACTOR_H
#define WEIDUEXTRACTOR_H

#include <QObject>

class WeiduLog;

class QByteArray;
class QStringList;

class WeiduExtractor : public QObject
{
  Q_OBJECT

public:
  static int version(const QByteArray& message);
  static QStringList languageList(const QByteArray& message);
  static WeiduLog* componentList(const QByteArray& message);
};

#endif // WEIDUEXTRACTOR_H
