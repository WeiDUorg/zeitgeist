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
  Game(QObject* parent, const QString& path);
  QList<QString> getModNames() const;
  QList<QString> getModPaths() const;
  LogFile* getInstalledMods() const;

  const QString path;

private:
  QList<Mod*> getModList(const QString& path);
  QStringList getTopLevelMods(const QString& path) const;
  QStringList getSubLevelMods(const QString& path) const;

  QList<Mod*> availableMods;
  LogFile* installedMods;
};

#endif // GAME_H
