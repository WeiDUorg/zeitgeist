#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Coordinator;
class DataManager;
class GameWindow;
class SettingsWindow;
class MainCentralWidget;

class QCloseEvent;
class QLabel;
class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(Coordinator* coordinator);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);

private slots:
  void showGameWindow();
  void showSettingsWindow();
  void createEnqueueModWindow(const QString& tp2);
  void createTerminalWindow();
  void updateNameOfCurrentGame(const QString& name);
  void availableModSelected(const bool& selected);
  void installedModSelected(const bool& selected);
  void queuedModSelected(const bool& selected);
  void queuedModAvailable(const bool& available);
  void gameAvailable(const bool& haveGot);

signals:
  void saveState();

private:
  void createStatusBar();
  void createActions();
  void createMenus();

  const Coordinator* coordinator;
  const DataManager* dataManager;
  MainCentralWidget* mainCentralWidget;
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

  const QString gameInstallActionEnabled = tr("Queue a mod for installation");
  const QString gameInstallActionDisabled =
    tr("You must select an available mod before you can enqueue it");
  const QString gameUnqueueActionEnabled = tr("Remove components from the queue");
  const QString gameUnqueueActionDisabled =
    tr("You can only unqueue selected components in the queues");
  const QString gameUninstallActionEnabled = tr("Queue a mod for uninstallation");
  const QString gameUninstallActionDisabled =
    tr("You must select an installed mod before you can uninstall it");
  const QString gameProcessActionEnabled = tr("Process queue");
  const QString gameProcessActionDisabled =
    tr("There is no queue to process");
  const QString gameRefreshActionEnabled = tr("Refresh game");
  const QString gameRefreshActionDisabled = tr("No game is loaded");
};

#endif // MAINWINDOW_H
