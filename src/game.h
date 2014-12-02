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
  Game(const QString& path, QObject* parent = 0);

private:
  QList<Mod*> getModList(const QString& path);
  QStringList getTopLevelMods(const QString& path) const;
  QStringList getSubLevelMods(const QString& path) const;

  const QString path;
  QList<Mod*> availableMods;
  LogFile* installedMods;
};

#endif // GAME_H
