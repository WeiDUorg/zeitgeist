#ifndef ENQUEUEMODMODEL_H
#define ENQUEUEMODMODEL_H

#include <QJsonDocument>
#include <QList>
#include <QStandardItemModel>

class WeiduLog;

class EnqueueModModel : public QStandardItemModel
{
  Q_OBJECT

public:
  EnqueueModModel(QObject* parent);
  WeiduLog* selected(const QString& mod, int lang);

public slots:
  void clear();
  void populate(const QJsonDocument& components,
                QList<int> installed,
                QList<int> queued);

private:
  enum MetaData {
    COMPONENT_NUMBER = Qt::UserRole + 1
  };
};

#endif // ENQUEUEMODMODEL_H
