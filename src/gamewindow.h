#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QModelIndex> // Apparently forward-declaration is not good enough

class DataManager;
class QTableView;
class QPushButton;
class GameListModel;
class QString;

class GameWindow : public QWidget
{
  Q_OBJECT

public:
  GameWindow(const DataManager* dataManager, QWidget* parent = 0);

private slots:
  void browse();
  void remove();
  void select();
  void notAGameDirectory(const QString& path);

signals:
  void addGame(const QString& path);
  void removeGame(const QModelIndex& index);
  void useGame(const QString& path);

private:
  const DataManager* dataManager;
  QTableView* gameList;
  QPushButton* addGameButton;
  QPushButton* removeGameButton;
  QPushButton* selectGameButton;
  GameListModel* model;
};

#endif // GAMEWINDOW_H
