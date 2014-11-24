#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

class QSettings;
class GameListModel;

class DataManager : public QObject
{
  Q_OBJECT

public:
  DataManager(QObject* parent = 0);
  void saveState();
  void restoreState();

  QSettings* settings;
  GameListModel* gameListModel;

private:
  void saveGameList();
  void restoreGameList();

  const QString gameListSettingsName = "gameList";
};

#endif // DATAMANAGER_H
