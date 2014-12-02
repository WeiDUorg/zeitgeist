#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

class QSettings;
class QString;

class GameListModel;
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

private slots:
  void useGame(const QString& path);

private:
  void saveGameList();
  void restoreGameList();
  void loadGame(const QString& path);

  const QString gameListSettingsName = "gameList";
  QString currentGame;
  Game* game;
};

#endif // DATAMANAGER_H
