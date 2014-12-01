#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>

class QString;
class QStringList;

class LogFile;
class Mod;

class Game : public QObject
{
  Q_OBJECT

public:
  Game(QString path, QObject* parent = 0);

private:
  QList<Mod*> getModList(QString path);
  QStringList getTopLevelMods(QString path);
  QStringList getSubLevelMods(QString path);

  const QString path;
  QList<Mod*> availableMods;
  LogFile* installedMods;
};

#endif // GAME_H
