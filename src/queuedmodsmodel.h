#ifndef QUEUEDMODSMODEL_H
#define QUEUEDMODSMODEL_H

#include <QList>
#include <QStandardItemModel>

class WeiduLog;

enum dataRole { Number = Qt::UserRole + 1, Language };

class QueuedModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  QueuedModsModel(QObject* parent);
  QList<int> queuedComponents(const QString& tp2) const;

public slots:
  void add(WeiduLog* componentList);
  void unqueue(const QModelIndexList& indices);
  WeiduLog* queue();

private slots:
  void clear();

private:
};

#endif // QUEUEDMODSMODEL_H
