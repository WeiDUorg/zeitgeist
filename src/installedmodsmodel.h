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
  QList<int> installedComponents(const QString& tp2) const;

public slots:
  /* Returns the installed components selected by the user in the
     order they were installed */
  WeiduLog* selectedComponents(const QModelIndexList& indexList) const;

  /* Sorts component blocks in reverse install order and compacts
   * blocks belonging to the same installed block.
   * Blocks themselves are not sorted internally.
   * Deletes the argument object and returns a new one.
   */
  WeiduLog* sortForUninstall(const WeiduLog* logFile) const;

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
