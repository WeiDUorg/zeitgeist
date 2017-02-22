#ifndef INSTALLEDMODSMODEL_H
#define INSTALLEDMODSMODEL_H

#include "weidulog.h"

#include <QHash>
#include <QList>
#include <QStandardItemModel>

class WeiduLog;
struct WeiduLogComponent;

class QObject;
class QStandardItem;
class QString;

class InstalledModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  InstalledModsModel(QObject*  parent);
  void populate(const WeiduLog* logFile);
  QList<int> installedComponents(const QString& tp2) const;

public slots:
  /* Returns the installed components selected by the user in the
     order they were installed */
  WeiduLog* selectedComponents(const QModelIndexList& indexList) const;

private slots:
  void clear();

private:
  QList<QList<WeiduLogComponent>> partitionData(const QList<WeiduLogComponent>& data) const;
  QList<WeiduLogComponent> getContiguousBlock(const QList<WeiduLogComponent>& data, const int& index, const QString& name) const;
  QList<QString> getPartitionNames(const QList<QList<WeiduLogComponent>>& partitionedData) const;
  QList<QStandardItem*> getChildList(const QList<WeiduLogComponent>& componentList) const;
  QHash<QString, QList<int>> populateLookup(const QList<WeiduLogComponent>& data);

  QList<QList<WeiduLogComponent>> partitionedData;
  QHash<QString, QList<int>> lookup;
};

#endif // INSTALLEDMODSMODEL_H
