#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QList>

class Mod;
class WeiduLog;

class QString;
class QStringList;

class Game : public QObject
{
  Q_OBJECT

public:
  Game(QObject* parent, const QString& path);
  QList<QString> getModNames() const;
  QList<QString> getModPaths() const;

  const QString path;

private:
  QList<Mod*> getModList(const QString& path);
  QStringList getTopLevelMods(const QString& path) const;
  QStringList getSubLevelMods(const QString& path) const;

  QList<Mod*> availableMods;
};

#endif // GAME_H
