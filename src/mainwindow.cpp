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
#include "maincentralwidget.h"
#include "gamewindow.h"
#include "settingswindow.h"
#include "coordinator.h"
#include "datamanager.h"

#include <QApplication>
#include <QLabel>
#include <QStatusBar>
#include <QAction>
#include <QMenuBar>
#include <QMenu>

MainWindow::MainWindow(Coordinator* coordinator) :
  coordinator(coordinator),
  dataManager(coordinator->dataManager), gameWindow(0), settingsWindow(0)
{
  setCentralWidget(new MainCentralWidget(this, dataManager));

  createActions();

  createStatusBar();
  createMenus();

  connect(this, SIGNAL(saveState()),
          dataManager, SLOT(saveState()));
  connect(dataManager, SIGNAL(identityOfCurrentGame(const QString&)),
          this, SLOT(updateNameOfCurrentGame(const QString&)));
  dataManager->identifyCurrentGame(); // Side-effect

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

  gameRefreshAction = new QAction(tr("Refresh"), this);
  gameRefreshAction->setStatusTip(tr("Refresh game"));
  connect(gameRefreshAction, SIGNAL(triggered()),
          dataManager, SLOT(refreshCurrentGame()));

  gameInstallAction = new QAction(tr("Install"), this);
  gameInstallAction->setStatusTip(tr("Queue a mod for installation"));

  gameUnqueueAction = new QAction(tr("Unqueue"), this);
  gameUnqueueAction->setStatusTip(tr("Remove a mod from the queue"));

  gameUninstallAction = new QAction(tr("Uninstall"), this);
  gameUninstallAction->setStatusTip(tr("Queue a mod for uninstallation"));

  gameProcessAction = new QAction(tr("Process"), this);
  gameProcessAction->setStatusTip(tr("Process queue"));
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

void MainWindow::updateNameOfCurrentGame(const QString& name)
{
  statusBarCurrentGame->setText(name);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  emit saveState();
  QMainWindow::closeEvent(event);
}
