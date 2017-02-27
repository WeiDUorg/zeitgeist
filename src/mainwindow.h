#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>

class Coordinator;
class DataManager;
class GameWindow;
class MainCentralWidget;
class SettingsWindow;
class TerminalWindow;

class QAction;
class QCloseEvent;
class QLabel;
class QMenu;
class QProcess;

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
  void installerAvailable(const bool& available);
  void showAboutDialog() const;

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
  QAction* gameEnqueueAction;
  QAction* gameUnqueueAction;
  QAction* gameUninstallAction;
  QAction* gameProcessAction;
  QMenu* programMenu;
  QMenu* gameMenu;
  GameWindow* gameWindow;
  SettingsWindow* settingsWindow;
  QPointer<TerminalWindow> terminalWindow;

  bool installerToggle = false;
  bool enqueueToggle = false;
  bool processToggle = false;

  const QString gameEnqueueActionEnabled =
    tr("Queue a mod for installation");
  const QString gameEnqueueActionDisabled =
    tr("You must select an available mod before you can enqueue it");
  const QString gameUnqueueActionEnabled =
    tr("Remove components from the queue");
  const QString gameUnqueueActionDisabled =
    tr("You can only unqueue selected components in the queues");
  const QString gameUninstallActionEnabled =
    tr("Queue a mod for uninstallation");
  const QString gameUninstallActionDisabled =
    tr("You must select an installed mod before you can uninstall it");
  const QString gameProcessActionEnabled =
    tr("Process queue");
  const QString gameProcessActionDisabled =
    tr("There is no queue to process");
  const QString gameRefreshActionEnabled =
    tr("Refresh game");
  const QString gameRefreshActionDisabled =
    tr("No game is loaded");
  const QString installerUnavailable =
    tr("This action is unavailable until WeiDU has been configured");
};

#endif // MAINWINDOW_H
