#ifndef AVAILABLEMODSMODEL_H
#define AVAILABLEMODSMODEL_H

#include <QStringListModel>
#include <QList>

class QObject;
class QString;

class Game;

class AvailableModsModel : public QStringListModel
{
  Q_OBJECT

public:
  AvailableModsModel(QObject* parent = 0);
  void populate(const Game* game);

private:
  void clear();

  QList<QString> names;
  QList<QString> paths;
};

#endif // AVAILABLEMODSMODEL_H
