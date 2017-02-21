#ifndef INSTALLEDMODSMODEL_H
#define INSTALLEDMODSMODEL_H

#include <QStandardItemModel>
#include <QList>

class WeiduLog;
struct WeiduLogComponent;

class QObject;
class QString;
class QStandardItem;

class InstalledModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  InstalledModsModel(QObject*  parent);
  void populate(const WeiduLog* logFile);

private slots:
  void clear();

private:
  QList<QList<WeiduLogComponent>> partitionData(const QList<WeiduLogComponent>& data) const;
  QList<WeiduLogComponent> getContiguousBlock(const QList<WeiduLogComponent>& data, const int& index, const QString& name) const;
  QList<QString> getPartitionNames(const QList<QList<WeiduLogComponent>>& partitionedData) const;
  QList<QStandardItem*> getChildList(const QList<WeiduLogComponent>& componentList) const;
};

#endif // INSTALLEDMODSMODEL_H
