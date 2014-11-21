#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QAction;
class QMenu;
class GameWindow;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

private slots:
  void showGameWindow();

private:
  void createStatusBar();
  void createActions();
  void createMenus();

  QLabel* statusBarGameLabel;
  QLabel* statusBarCurrentGame;
  QAction* programAboutAction;
  QAction* programSettingsAction;
  QAction* programQuitAction;
  QAction* gameEditAction;
  QAction* gameRefreshAction;
  QAction* gameInstallAction;
  QAction* gameUnqueueAction;
  QAction* gameUninstallAction;
  QAction* gameProcessAction;
  QMenu* programMenu;
  QMenu* gameMenu;
  GameWindow* gameWindow;
};

#endif // MAINWINDOW_H
