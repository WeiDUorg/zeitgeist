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
  void addGame(const QString& path);
  void removeGame(const QModelIndex& row);

signals:
  void notAGameDirectory(const QString& path) const;

public:
  GameListModel(QObject* parent = 0);
  QList<GameListDataEntry> exportData() const;
  void importData(const QList<GameListDataEntry>& dataList);
  QString pathOfIndex(const QModelIndex& index) const;

private:
  QString findKeyFileDirectory(const QString& path) const;
  QString fingerprintGameDirectory(const QString& path) const;
  bool duplicate(const QString& path) const;
};

#endif // GAMELISTMODEL_H
