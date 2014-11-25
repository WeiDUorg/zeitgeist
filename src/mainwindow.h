#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DataManager;
class GameWindow;
class SettingsWindow;
class QCloseEvent;
class QLabel;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(DataManager* dataManager, QWidget* parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);

private slots:
  void showGameWindow();
  void showSettingsWindow();

private:
  void createStatusBar();
  void createActions();
  void createMenus();

  DataManager* dataManager;
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
  SettingsWindow* settingsWindow;
};

#endif // MAINWINDOW_H
