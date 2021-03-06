#ifndef ENQUEUEMODMODEL_H
#define ENQUEUEMODMODEL_H

#include <QJsonDocument>
#include <QList>
#include <QStandardItemModel>

class WeiduLog;

struct WeiduLogComponent;

class QModelIndex;
class QStandardItem;

class EnqueueModModel : public QStandardItemModel
{
  Q_OBJECT

public:
  EnqueueModModel(QObject* parent);
  WeiduLog* selected(const QString& mod, int lang) const;
  QList<WeiduLogComponent> checkChildren(const QString& mod, int lang,
                                         QList<WeiduLogComponent> acc,
                                         QList<QStandardItem*> queue) const;

public slots:
  void clear();
  void populate(const QJsonDocument& components,
                QList<int> installed,
                QList<int> queued);
  void radioToggled(const QModelIndex& index);
  void select(bool select);
  void handleChangedState();

signals:
  void componentsAvailable(bool available);
  void stateChanged(Qt::CheckState state);

private:
  QStringList forcedSubgroups;
  enum MetaData {
    COMPONENT_NUMBER = Qt::UserRole + 1,
    COMPONENT_INDEX
  };
};

class SelectionInProgress : public QObject
{
  Q_OBJECT

public:
  SelectionInProgress(EnqueueModModel* model);
  ~SelectionInProgress();

private:
  EnqueueModModel* model;
};

#endif // ENQUEUEMODMODEL_H
