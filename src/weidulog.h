#ifndef WEIDULOG_H
#define WEIDULOG_H

#include <QDebug>
#include <QObject>
#include <QList>

class QString;

struct WeiduLogComponent
{
  QString modName;
  int index;
  int language;
  int number;
  QString comment;

  bool operator==(const WeiduLogComponent& r) const
  {
    return (this->modName == r.modName) && (this->number == r.number);
  }

  bool operator<(const WeiduLogComponent& r) const
  {
    return (this->index < r.index);
  }
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
