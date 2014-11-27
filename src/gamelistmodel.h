#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include <QStandardItemModel>
#include <QList>

class QObject;
class QString;
class QModelIndex;

struct GameListDataEntry {
  QString name;
  QString path;
};

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
  QList<GameListDataEntry> exportData();
  void importData(QList<GameListDataEntry> dataList);
  QString pathOfIndex(QModelIndex index);

private:
  QString findKeyFileDirectory(QString path);
  QString fingerprintGameDirectory(QString path);
  bool duplicate(QString path);
};

#endif // GAMELISTMODEL_H
