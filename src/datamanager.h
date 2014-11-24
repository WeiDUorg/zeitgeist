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

  QSettings* settings;
  GameListModel* gameListModel;
};

#endif // DATAMANAGER_H
