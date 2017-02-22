#ifndef QUEUEDMODSMODEL_H
#define QUEUEDMODSMODEL_H

#include <QList>
#include <QStandardItemModel>

class WeiduLog;

class QueuedModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  QueuedModsModel(QObject* parent);

public slots:
  void add(WeiduLog* componentList);

private slots:
  void clear();

private:
  QList<WeiduLog*> queue;
};

#endif // QUEUEDMODSMODEL_H
