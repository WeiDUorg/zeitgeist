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

/* There's also inheriting QList<WeiduLogComponent>, but it seems bothersome */
class WeiduLog : public QObject
{
  Q_OBJECT

public:
  WeiduLog(QObject* parent, QList<QList<WeiduLogComponent>> data);
  bool isEmpty();
  const QList<QList<WeiduLogComponent>> data;

  static QString logPath(const QString& gamePath);
  QList<QString> getPartitionNames() const;
};

#endif // WEIDULOG_H
