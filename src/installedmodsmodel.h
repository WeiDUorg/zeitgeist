#ifndef INSTALLEDMODSMODEL_H
#define INSTALLEDMODSMODEL_H

#include <QStandardItemModel>
#include <QList>

class QObject;
class QString;
class QStandardItem;

class LogFile;
struct LogFileComponent;

class InstalledModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  InstalledModsModel(QObject*  parent);
  void populate(const LogFile* logFile);

private:
  void clear();
  QList<QList<LogFileComponent>> partitionData(const QList<LogFileComponent>& data) const;
  QList<LogFileComponent> getContiguousBlock(const QList<LogFileComponent>& data, const int& index, const QString& name) const;
  QList<QString> getPartitionNames(const QList<QList<LogFileComponent>>& partitionedData) const;
  QList<QStandardItem*> getChildList(const QList<LogFileComponent>& componentList) const;
};

#endif // INSTALLEDMODSMODEL_H
