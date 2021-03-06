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
  WeiduLog* queue();

private slots:
  void clear();

private:
  WeiduLog* deduplicate(WeiduLog*) const;

  enum MetaData {
    COMPONENT_NUMBER = Qt::UserRole + 1,
    COMPONENT_LANGUAGE,
    COMPONENT_INDEX
  };
};

#endif // QUEUEDMODSMODEL_H
