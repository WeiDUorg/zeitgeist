#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QModelIndex> // Apparently forward-declaration is not good enough

class QTableView;
class QPushButton;
class GameListModel;
class QString;

class GameWindow : public QWidget
{
  Q_OBJECT

public:
  GameWindow(QWidget* parent);

private slots:
  void browse();
  void remove();
  void notAGameDirectory(QString path);

signals:
  void addGame(QString path);
  void removeGame(QModelIndex index);

private:
  QTableView* gameList;
  QPushButton* addGameButton;
  QPushButton* removeGameButton;
  QPushButton* selectGameButton;
  GameListModel* model;
};

#endif // GAMEWINDOW_H
