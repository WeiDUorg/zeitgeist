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
  QList<int> queuedComponents(const QString& tp2) const;

public slots:
  void add(WeiduLog* componentList);
  void unqueue(const QModelIndexList& indices);

private slots:
  void clear();

private:
  QList<WeiduLog*> queue;
};

#endif // QUEUEDMODSMODEL_H
