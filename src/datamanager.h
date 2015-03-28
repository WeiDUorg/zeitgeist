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
  DataManager(QObject* parent = 0);
  void saveState();
  void restoreState();

  QSettings* settings;
  GameListModel* gameListModel;
  AvailableModsModel* availableModsModel;
  InstalledModsModel* installedModsModel;

private slots:
  void useGame(const QString& path);

public slots:
  void identifyCurrentGame();

signals:
  void identityOfCurrentGame(const QString& name);

private:
  void saveGameList();
  void restoreGameList();
  void loadGame(const QString& path);

  QHash<QString, Game*> games;
  Game* currentGame;

  const QString gameListSettingsName = "gameList";
};

#endif // DATAMANAGER_H
