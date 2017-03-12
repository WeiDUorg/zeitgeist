#ifndef WEIDUEXTRACTOR_H
#define WEIDUEXTRACTOR_H

#include <QObject>

class QByteArray;
class QJsonDocument;
class QStringList;

class WeiduExtractor : public QObject
{
  Q_OBJECT

public:
  static int version(const QByteArray& message);
  static QStringList languageList(const QByteArray& message);
  static QJsonDocument componentList(const QByteArray& message);
};

#endif // WEIDUEXTRACTOR_H
