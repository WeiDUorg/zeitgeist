#ifndef ENQUEUEMODMODEL_H
#define ENQUEUEMODMODEL_H

#include <QJsonDocument>
#include <QList>
#include <QStandardItemModel>

class WeiduLog;

struct WeiduLogComponent;

class QStandardItem;

class EnqueueModModel : public QStandardItemModel
{
  Q_OBJECT

public:
  EnqueueModModel(QObject* parent);
  WeiduLog* selected(const QString& mod, int lang) const;
  QList<WeiduLogComponent> checkChildren(const QString& mod, int lang,
                                         const QStandardItem* parent,
                                         QList<WeiduLogComponent> acc,
                                         QList<QStandardItem*> queue) const;

public slots:
  void clear();
  void populate(const QJsonDocument& components,
                QList<int> installed,
                QList<int> queued);

private:
  enum MetaData {
    COMPONENT_NUMBER = Qt::UserRole + 1,
    COMPONENT_INDEX
  };
};

#endif // ENQUEUEMODMODEL_H
