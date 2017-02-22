#ifndef WEIDULOG_H
#define WEIDULOG_H

#include <QObject>
#include <QList>

class QString;

struct WeiduLogComponent
{
  QString modName;
  int language;
  int number;
  QString comment;
};

class WeiduLog : public QObject // There's also inheriting QList<WeiduLogComponent>, but it seems bothersome
{
  Q_OBJECT

public:
  WeiduLog(QObject* parent, QList<WeiduLogComponent> data);
  bool isEmpty();
  const QList<WeiduLogComponent> data;

  static QString logPath(const QString& gamePath);
};

#endif // WEIDULOG_H
