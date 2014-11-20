#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>

class QTableView;
class QPushButton;

class GameWindow : public QWidget
{
  Q_OBJECT

public:
  GameWindow(QWidget* parent);

private:
  QTableView* gameList;
  QPushButton* addGameButton;
  QPushButton* removeGameButton;
  QPushButton* selectGameButton;
};

#endif // GAMEWINDOW_H
