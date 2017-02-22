#ifndef QUEUEDMODSMODEL_H
#define QUEUEDMODSMODEL_H

#include <QStandardItemModel>

enum Queue { Install, Uninstall };

class QueuedModsModel : public QStandardItemModel
{
  Q_OBJECT

public:
  QueuedModsModel(QObject* parent);

private slots:
  void clear();
};

#endif // QUEUEDMODSMODEL_H
