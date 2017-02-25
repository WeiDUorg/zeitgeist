/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2014  Fredrik Lindgren

 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "mainwindow.h"
#include "controller.h"
#include "coordinator.h"
#include "datamanager.h"
#include "enqueuemodwindow.h"
#include "gamewindow.h"
#include "maincentralwidget.h"
#include "settingswindow.h"
#include "terminalwindow.h"

#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QMenu>
#include <QDebug>

MainWindow::MainWindow(Coordinator* coordinator) :
  coordinator(coordinator),
  dataManager(coordinator->dataManager),
  mainCentralWidget(new MainCentralWidget(this, coordinator)),
  gameWindow(nullptr), settingsWindow(nullptr), terminalWindow(nullptr)
{
  setCentralWidget(mainCentralWidget);
  connect(mainCentralWidget, SIGNAL(availableModSelected(const bool&)),
          this, SLOT(availableModSelected(const bool&)));
  connect(mainCentralWidget, SIGNAL(installedModSelected(const bool&)),
          this, SLOT(installedModSelected(const bool&)));
  connect(mainCentralWidget, SIGNAL(queuedModSelected(const bool&)),
          this, SLOT(queuedModSelected(const bool&)));
  connect(mainCentralWidget, SIGNAL(queuedModAvailable(const bool&)),
          this, SLOT(queuedModAvailable(const bool&)));

  createActions();
  createStatusBar();
  createMenus();

  connect(this, SIGNAL(saveState()),
          dataManager, SLOT(saveState()));
  connect(dataManager, SIGNAL(identityOfCurrentGame(const QString&)),
          this, SLOT(updateNameOfCurrentGame(const QString&)));
  connect(dataManager, SIGNAL(gotGame(const bool&)),
          this, SLOT(gameAvailable(const bool&)));

  connect(coordinator->controller, SIGNAL(installTaskStarted()),
          this, SLOT(createTerminalWindow()));

  setMinimumSize(800, 600);
  setWindowTitle("Zeitgeist");
}

MainWindow::~MainWindow()
{

}

void MainWindow::createStatusBar()
{
  statusBarGameLabel = new QLabel(tr("Current Game:"), this);
  statusBarCurrentGame = new QLabel(tr("none"), this);
  statusBar()->addWidget(statusBarGameLabel);
  statusBar()->addWidget(statusBarCurrentGame);
  statusBar()->setSizeGripEnabled(false);
}

void MainWindow::createActions()
{
  programAboutAction = new QAction(tr("About"), this);
  programAboutAction->setStatusTip(tr("About Zeitgeist"));

  programSettingsAction = new QAction(tr("Settings"), this);
  programSettingsAction->setStatusTip(tr("Edit program settings"));
  connect(programSettingsAction, SIGNAL(triggered()),
          this, SLOT(showSettingsWindow()));

  programQuitAction = new QAction(tr("Quit"), this);
  connect(programQuitAction, SIGNAL(triggered()),
          qApp, SLOT(closeAllWindows()));

  gameEditAction = new QAction(tr("Edit"), this);
  gameEditAction->setStatusTip(tr("Edit games"));
  connect(gameEditAction, SIGNAL(triggered()),
          this, SLOT(showGameWindow()));

  /* Should only be enabled while there is a currentGame
   * Default: disabled (no game is loaded)
   */
  gameRefreshAction = new QAction(tr("Refresh"), this);
  gameRefreshAction->setStatusTip(gameRefreshActionDisabled);
  gameRefreshAction->setEnabled(false);
  connect(gameRefreshAction, SIGNAL(triggered()),
          dataManager, SLOT(refreshCurrentGame()));

  /* Should only be enabled while there is a selection in availableModsView
   * Default: disabled (no available mod is selected)
   */
  gameInstallAction = new QAction(tr("Enqueue"), this);
  gameInstallAction->setStatusTip(gameInstallActionDisabled);
  gameInstallAction->setEnabled(false);
  connect(gameInstallAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(getSelectedAvailableMod()));
  connect(gameInstallAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(clearAvailableSelection()));
  connect(mainCentralWidget, SIGNAL(selectedAvailableMod(const QString&)),
          this, SLOT(createEnqueueModWindow(const QString&)));

  /* Should only be enabled while there are selections in installQueueView
   * or uninstallQueueView
   * Default: disabled (no queued mod is selected)
   */
  gameUnqueueAction = new QAction(tr("Unqueue"), this);
  gameUnqueueAction->setStatusTip(gameUnqueueActionDisabled);
  gameUnqueueAction->setEnabled(false);
  connect(gameUnqueueAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(getSelectedQueuedMods()));
  connect(gameUnqueueAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(clearQueuedSelection()));
  connect(mainCentralWidget, SIGNAL(selectedInstallQueuedMods(const QModelIndexList&)),
          dataManager, SLOT(unqueueInstallComponents(const QModelIndexList&)));
  connect(mainCentralWidget, SIGNAL(selectedUninstallQueuedMods(const QModelIndexList&)),
          dataManager, SLOT(unqueueUninstallComponents(const QModelIndexList&)));

  /* Should only be enabled while there is a selection in installedModsView
   * Default: disabled (no installed mod is selected)
   */
  gameUninstallAction = new QAction(tr("Uninstall"), this);
  gameUninstallAction->setStatusTip(gameUninstallActionDisabled);
  gameUninstallAction->setEnabled(false);
  connect(gameUninstallAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(getSelectedInstalledMods()));
  connect(gameUninstallAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(clearInstalledSelection()));
  connect(mainCentralWidget, SIGNAL(selectedInstalledMods(WeiduLog*)),
          dataManager, SLOT(uninstallComponents(WeiduLog*)));

  /* Should only be enabled while there are mods in either queue
   * Default: disabled (no mods in either queue)
   */
  gameProcessAction = new QAction(tr("Process"), this);
  gameProcessAction->setStatusTip(gameProcessActionDisabled);
  gameProcessAction->setEnabled(false);
  connect(gameProcessAction, SIGNAL(triggered()),
          dataManager, SLOT(getQueues()));
  connect(gameProcessAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(clearQueuedSelection()));
}

void MainWindow::createMenus()
{
  programMenu = menuBar()->addMenu(tr("Program"));
  programMenu->addAction(programSettingsAction);
  programMenu->addAction(programAboutAction);
  programMenu->addAction(programQuitAction);

  gameMenu = menuBar()->addMenu(tr("Game"));
  gameMenu->addAction(gameEditAction);
  // Recent (games)
  gameMenu->addAction(gameRefreshAction);
  gameMenu->addAction(gameInstallAction);
  gameMenu->addAction(gameUnqueueAction);
  gameMenu->addAction(gameUninstallAction);
  gameMenu->addAction(gameProcessAction);
}

void MainWindow::showGameWindow()
{
  if (!gameWindow) {
    gameWindow = new GameWindow(this, dataManager);
  }
  if (gameWindow->isHidden()) {
    gameWindow->show();
  } else {
    gameWindow->activateWindow();
  }
}

void MainWindow::showSettingsWindow()
{
  if (!settingsWindow) {
    settingsWindow = new SettingsWindow(this, coordinator);
  }
  if (settingsWindow->isHidden()) {
    settingsWindow->show();
  } else {
    settingsWindow->activateWindow();
  }
}

void MainWindow::createEnqueueModWindow(const QString& tp2)
{
  EnqueueModWindow* enqueueModWindow = new EnqueueModWindow(this, coordinator, tp2);
  enqueueModWindow->show();
}

void MainWindow::createTerminalWindow()
{
  if (!terminalWindow) {
    terminalWindow = new TerminalWindow(this, coordinator);
    terminalWindow->show();
  } // else the window is already up and connected
}

void MainWindow::updateNameOfCurrentGame(const QString& name)
{
  statusBarCurrentGame->setText(name);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  emit saveState();
  QMainWindow::closeEvent(event);
}

void MainWindow::availableModSelected(const bool& selected)
{
  if (selected) {
    gameInstallAction->setEnabled(true);
    gameInstallAction->setStatusTip(gameInstallActionEnabled);
  } else {
    gameInstallAction->setEnabled(false);
    gameInstallAction->setStatusTip(gameInstallActionDisabled);
  }
}

void MainWindow::installedModSelected(const bool& selected)
{
  if (selected) {
    gameUninstallAction->setEnabled(true);
    gameUninstallAction->setStatusTip(gameUninstallActionEnabled);
  } else {
    gameUninstallAction->setEnabled(false);
    gameUninstallAction->setStatusTip(gameUninstallActionDisabled);
  }
}

void MainWindow::queuedModSelected(const bool& selected)
{
  if (selected) {
    gameUnqueueAction->setEnabled(true);
    gameUnqueueAction->setStatusTip(gameUnqueueActionEnabled);
  } else {
    gameUnqueueAction->setEnabled(false);
    gameUnqueueAction->setStatusTip(gameUnqueueActionDisabled);
  }
}

void MainWindow::queuedModAvailable(const bool& available)
{
  if (available) {
    gameProcessAction->setEnabled(true);
    gameProcessAction->setStatusTip(gameProcessActionEnabled);
  } else {
    gameProcessAction->setEnabled(false);
    gameProcessAction->setStatusTip(gameProcessActionDisabled);
  }
}

void MainWindow::gameAvailable(const bool& haveGot)
{
  if (haveGot) {
    gameRefreshAction->setEnabled(true);
    gameRefreshAction->setStatusTip(gameRefreshActionEnabled);
  } else {
    gameRefreshAction->setEnabled(false);
    gameRefreshAction->setStatusTip(gameRefreshActionDisabled);
  }
}
