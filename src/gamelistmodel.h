#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include <QStandardItemModel>

class QObject;
class QString;
class QModelIndex;

class GameListModel : public QStandardItemModel
{
  Q_OBJECT

private slots:
  void addGame(QString path);
  void removeGame(QModelIndex row);

signals:
  void notAGameDirectory(QString path);

public:
  GameListModel(QObject* parent = 0);

private:
  QString findKeyFileDirectory(QString path);
  QString fingerprintGameDirectory(QString path);
  bool duplicate(QString path);
};

#endif // GAMELISTMODEL_H
