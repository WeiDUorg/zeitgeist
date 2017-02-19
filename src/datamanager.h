#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QHash>

class QSettings;
class QString;

class GameListModel;
class AvailableModsModel;
class InstalledModsModel;
class Game;

class DataManager : public QObject
{
  Q_OBJECT

public:
  DataManager(QObject* parent);
  QString getCurrentGamePath() const;
  void emitCurrentGamePath() const;

  GameListModel* gameListModel;
  AvailableModsModel* availableModsModel;
  InstalledModsModel* installedModsModel;

private slots:
  void useGame(const QString& path);
  void restoreState();
  void saveState();
  void refreshCurrentGame();
  void gameRemoved(const QString& path);
  void confirmedWeiduPath(const QString& path);

public slots:
  void identifyCurrentGame() const;

signals:
  void identityOfCurrentGame(const QString& name) const;
  void clearModels();
  void newGamePath(const QString& gamePath) const;
  void restoreWeidu(const QString& weiduPath) const;

private:
  void saveGameList();
  void restoreGameList();
  void loadGame(const QString& path);

  QSettings* settings;
  Game* currentGame;

  const QString gameListSettingsName = "gameList";
};

#endif // DATAMANAGER_H
