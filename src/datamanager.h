#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QHash>
#include <QModelIndex>

class AvailableModsModel;
class Game;
class GameListModel;
class InstalledModsModel;
class QueuedModsModel;
class WeiduLog;

class QSettings;
class QString;

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
  QueuedModsModel* inQueuedModsModel;
  QueuedModsModel* outQueuedModsModel;

private slots:
  void useGame(const QString& path);
  void restoreState();
  void saveState();
  void refreshCurrentGame();
  void gameRemoved(const QString& path);
  void confirmedWeiduPath(const QString& path);
  void enqueueComponents(WeiduLog* componentList);
  void uninstallComponents(WeiduLog* componentList);
  void unqueueInstallComponents(const QModelIndexList& indices);
  void unqueueUninstallComponents(const QModelIndexList& indices);
  void getQueues();
  void logFile(WeiduLog* logFile);

public slots:
  void identifyCurrentGame() const;

signals:
  void identityOfCurrentGame(const QString& name) const;
  void clearModels();
  void newGamePath(const QString& gamePath, const bool& eeGame) const;
  void restoreWeidu(const QString& weiduPath) const;
  void clearQueues();
  void processQueues(WeiduLog* installQueue,
                     WeiduLog* uninstallQueue);
  void gotGame(const bool& haveGot);
  void getLog(const QString& gamePath);

private:
  void saveGameList();
  void restoreGameList();
  void loadGame(const QString& path);

  QSettings* settings;
  Game* currentGame;

  const QString gameListSettingsName = "gameList";
};

#endif // DATAMANAGER_H
