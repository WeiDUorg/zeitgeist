#ifndef GAMELISTMODEL_H
#define GAMELISTMODEL_H

#include <QStandardItemModel>
#include <QList>

class QModelIndex;
class QObject;
class QString;
class QStringList;
class QVariant;

enum class GameType {
  BG,
  BG2,
  IWD,
  IWD2,
  PST,
  BGEE,
  BG2EE,
  IWDEE,
  UNKNOWN
};

enum GameListMetaData {
  GAME_TYPE = Qt::UserRole + 1,
  LANG_LIST
};

enum GameListColumns {
  COLUMN_NAME,
  COLUMN_PATH,
  COLUMN_LANG
};

struct GameListDataEntry {
  QString name;
  QString path;
  QString lang;
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
  QString eeLang(const QString& path) const;
  bool validGame(const QString& path) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

private slots:
  void addGame(const QString& path);
  void removeGame(const QModelIndex& row);
  void handleDataChanged(const QModelIndex& topLeft,
                         const QModelIndex& bottomRight);

signals:
  void notAGameDirectory(const QString& path) const;
  void gameRemoved(const QString& path) const;
  void eeLangSignal(const QString& path, const QString& lang) const;

private:
  void addRow(const QString& gameName, const QString& gamePath,
              const GameType& gameType, const QString& eeLang);
  QString findKeyFileDirectory(const QString& path) const;
  GameType fingerprintGameDirectory(const QString& path) const;
  QString prettyPrintGameType(const GameType& type) const;
  bool duplicate(const QString& path) const;
  QString stringOfType(const GameType& type) const;
  GameType typeOfString(const QString& name) const;
  QStringList langDirs(const QString& gamePath) const;
  QString eeLangToDir(const QString& lang) const;
  QString eeDirToLang(const QString& dir) const;
};

#endif // GAMELISTMODEL_H
