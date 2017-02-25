#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include <QStandardItemModel>
#include <QList>

class QModelIndex;
class QObject;
class QString;
class QStringList;

enum class GameType {
  BG,
  BG2,
  IWD,
  IWD2,
  PST,
  BGEE,
  BG2EE,
  UNKNOWN
};

enum GameListMetaData {
  GAME_TYPE = Qt::UserRole + 1
};

struct GameListDataEntry {
  QString name;
  QString path;
  QString type;
};

class GameListModel : public QStandardItemModel
{
  Q_OBJECT

public:
  GameListModel(QObject* parent);
  QList<GameListDataEntry> exportData() const;
  void importData(const QList<GameListDataEntry>& dataList);
  QString pathOfIndex(const QModelIndex& index) const;
  QString identifierOfPath(const QString& path) const;
  bool eeGame(const QString& path) const;

private slots:
  void addGame(const QString& path);
  void removeGame(const QModelIndex& row);

signals:
  void notAGameDirectory(const QString& path) const;
  void gameRemoved(const QString& path) const;

private:
  QString findKeyFileDirectory(const QString& path) const;
  GameType fingerprintGameDirectory(const QString& path) const;
  QString prettyPrintGameType(const GameType& type) const;
  bool duplicate(const QString& path) const;
  QString stringOfType(const GameType& type) const;
  GameType typeOfString(const QString& name) const;
};

#endif // GAMELISTMODEL_H
