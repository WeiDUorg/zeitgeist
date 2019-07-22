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
  void populate(WeiduLog* logFile);
  WeiduLog* logFile() const;
  QList<int> installedComponents(const QString& tp2) const;

public slots:
  /* Returns the installed components selected by the user in the
     order they were installed */
  WeiduLog* selectedComponents(const QModelIndexList& indexList) const;

private slots:
  void clear();

private:
  QList<QStandardItem*> getChildList(const QList<WeiduLogComponent>&
                                     componentList) const;
  QHash<QString, QList<int>> populateLookup();

  QList<QList<WeiduLogComponent>> partitionedData;
  QHash<QString, QList<int>> lookup;
};

#endif // INSTALLEDMODSMODEL_H
