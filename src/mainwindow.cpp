/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2017  Fredrik Lindgren

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
#include "moddistwindow.h"
#include "settingswindow.h"
#include "terminalwindow.h"

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QStatusBar>
#include <QString>
#include <QStringList>

MainWindow::MainWindow(Coordinator* coordinator) :
  coordinator(coordinator),
  dataManager(coordinator->dataManager),
  mainCentralWidget(new MainCentralWidget(this, coordinator)),
  gameWindow(nullptr), settingsWindow(nullptr), modDistWindow(nullptr),
  terminalWindow(nullptr)
{
  setCentralWidget(mainCentralWidget);
  connect(mainCentralWidget, SIGNAL(availableModSelected(bool)),
          this, SLOT(availableModSelected(bool)));
  connect(mainCentralWidget, SIGNAL(installedModSelected(bool)),
          this, SLOT(installedModSelected(bool)));
  connect(mainCentralWidget, SIGNAL(queuedModSelected(bool)),
          this, SLOT(queuedModSelected(bool)));
  connect(mainCentralWidget, SIGNAL(queuedModAvailable(bool)),
          this, SLOT(queuedModAvailable(bool)));

  connect(coordinator, SIGNAL(installerAvailable(bool)),
          this, SLOT(installerAvailable(bool)));
  connect(this, SIGNAL(importModArchives(const QStringList&)),
          dataManager, SLOT(importModDistArchive(const QStringList&)));
  connect(dataManager, SIGNAL(importModDistArchiveSuccess(bool)),
          this, SLOT(importModDistArchiveSuccess(bool)));

  createActions();
  createStatusBar();
  createMenus();

  connect(this, SIGNAL(saveState()),
          dataManager, SLOT(saveState()));
  connect(dataManager, SIGNAL(identityOfCurrentGame(const QString&)),
          this, SLOT(updateNameOfCurrentGame(const QString&)));
  connect(dataManager, SIGNAL(gotGame(bool)),
          this, SLOT(gameAvailable(bool)));

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
  connect(programAboutAction, SIGNAL(triggered()),
          this, SLOT(showAboutDialog()));

  programSettingsAction = new QAction(tr("Settings"), this);
  programSettingsAction->setStatusTip(tr("Edit program settings"));
  connect(programSettingsAction, SIGNAL(triggered()),
          this, SLOT(showSettingsWindow()));

  programModDistAction = new QAction(tr("Create Mod Distribution"), this);
  programModDistAction->setStatusTip(tr("Package a mod in a distributable format"));
  connect(programModDistAction, SIGNAL(triggered()),
          this, SLOT(showModDistWindow()));

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

  gameImportModAction = new QAction(tr("Import Mod"), this);
  gameImportModAction->setStatusTip(gameImportModActionDisabled);
  gameImportModAction->setEnabled(false);
  connect(gameImportModAction, SIGNAL(triggered()),
          this, SLOT(browseForModArchives()));

  /* Should only be enabled while there is a selection in availableModsView
   * Default: disabled (no available mod is selected)
   */
  gameEnqueueAction = new QAction(tr("Enqueue"), this);
  gameEnqueueAction->setStatusTip(gameEnqueueActionDisabled);
  gameEnqueueAction->setEnabled(false);
  connect(gameEnqueueAction, SIGNAL(triggered()),
          mainCentralWidget, SLOT(getSelectedAvailableMod()));
  connect(gameEnqueueAction, SIGNAL(triggered()),
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
  connect(mainCentralWidget,
          SIGNAL(selectedInstallQueuedMods(const QModelIndexList&)),
          dataManager,
          SLOT(unqueueInstallComponents(const QModelIndexList&)));
  connect(mainCentralWidget,
          SIGNAL(selectedUninstallQueuedMods(const QModelIndexList&)),
          dataManager,
          SLOT(unqueueUninstallComponents(const QModelIndexList&)));

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
  programMenu->addAction(programModDistAction);
  programMenu->addAction(programAboutAction);
  programMenu->addAction(programQuitAction);

  gameMenu = menuBar()->addMenu(tr("Game"));
  gameMenu->addAction(gameEditAction);
  // Recent (games)
  gameMenu->addAction(gameRefreshAction);
  gameMenu->addAction(gameImportModAction);
  gameMenu->addAction(gameEnqueueAction);
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

void MainWindow::showModDistWindow()
{
  if (!modDistWindow) {
    modDistWindow = new ModDistWindow(this, coordinator);
  }
  modDistWindow->show();
  modDistWindow->activateWindow();
}

void MainWindow::createEnqueueModWindow(const QString& tp2)
{
  EnqueueModWindow* enqueueModWindow =
    new EnqueueModWindow(this, coordinator, tp2);
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

void MainWindow::availableModSelected(bool selected)
{
  enqueueToggle = selected;
  if (selected && installerToggle) {
    gameEnqueueAction->setEnabled(true);
    gameEnqueueAction->setStatusTip(gameEnqueueActionEnabled);
  } else {
    gameEnqueueAction->setEnabled(false);
    if (!selected) {
      gameEnqueueAction->setStatusTip(gameEnqueueActionDisabled);
    } else if (!installerToggle) {
      gameEnqueueAction->setStatusTip(installerUnavailable);
    }
  }
}

void MainWindow::installedModSelected(bool selected)
{
  if (selected) {
    gameUninstallAction->setEnabled(true);
    gameUninstallAction->setStatusTip(gameUninstallActionEnabled);
  } else {
    gameUninstallAction->setEnabled(false);
    gameUninstallAction->setStatusTip(gameUninstallActionDisabled);
  }
}

void MainWindow::queuedModSelected(bool selected)
{
  if (selected) {
    gameUnqueueAction->setEnabled(true);
    gameUnqueueAction->setStatusTip(gameUnqueueActionEnabled);
  } else {
    gameUnqueueAction->setEnabled(false);
    gameUnqueueAction->setStatusTip(gameUnqueueActionDisabled);
  }
}

void MainWindow::queuedModAvailable(bool available)
{
  processToggle = available;
  if (available && installerToggle) {
    gameProcessAction->setEnabled(true);
    gameProcessAction->setStatusTip(gameProcessActionEnabled);
  } else {
    gameProcessAction->setEnabled(false);
    if (!available) {
      gameProcessAction->setStatusTip(gameProcessActionDisabled);
    } else if (!installerToggle) {
      gameProcessAction->setStatusTip(installerUnavailable);
    }
  }
}

void MainWindow::gameAvailable(bool haveGot)
{
  if (haveGot) {
    gameRefreshAction->setEnabled(true);
    gameRefreshAction->setStatusTip(gameRefreshActionEnabled);
    gameImportModAction->setEnabled(true);
    gameImportModAction->setStatusTip(gameImportModActionEnabled);
  } else {
    gameRefreshAction->setEnabled(false);
    gameRefreshAction->setStatusTip(gameRefreshActionDisabled);
    gameImportModAction->setEnabled(false);
    gameImportModAction->setStatusTip(gameImportModActionDisabled);
  }
}

void MainWindow::installerAvailable(bool available)
{
  installerToggle = available;
  if (available && enqueueToggle) {
    gameEnqueueAction->setEnabled(true);
    gameEnqueueAction->setStatusTip(gameEnqueueActionEnabled);
  } else if (!enqueueToggle) {
    gameEnqueueAction->setEnabled(false);
    gameEnqueueAction->setStatusTip(gameEnqueueActionDisabled);
  } else if (!available) {
    gameEnqueueAction->setEnabled(false);
    gameEnqueueAction->setStatusTip(installerUnavailable);
  }
  if (available && processToggle) {
    gameProcessAction->setEnabled(true);
    gameProcessAction->setStatusTip(gameProcessActionEnabled);
  } else if (!processToggle) {
    gameProcessAction->setEnabled(false);
    gameProcessAction->setStatusTip(gameProcessActionDisabled);
  } else if (!available) {
    gameProcessAction->setEnabled(false);
    gameProcessAction->setStatusTip(installerUnavailable);
  }
}

void MainWindow::showAboutDialog() const
{
  QString title = "About Zeitgeist";
  QStringList body;
  body << "<p>Zeitgeist is a program for installing mods for Infinity Engine";
  body << " games using WeiDU.</p><p>Version: ";
  body << QString::number(123);
  body << "<p>Copyright (C) 2017 Fredrik Lindgren</p>";
  body << "</p><p>This is free software under the terms of the <a href='htt";
  body << "ps://www.gnu.org/licenses/gpl.html'>GNU GPL</a>, version 3 or any";
  body << " later version.</p><p>The source code is available at <a href='ht";
  body << "tps://github.com/WeiDUorg/zeitgeist'>https://github.com/WeiDUorg/";
  body << "zeitgeist</a></p>";
  body << "<p>This program makes use of the library:<br />";
  body << "<a href='https://stachenov.github.io/quazip/'>QuaZIP</a> by ";
  body << "Sergei Tachenov &ndash; <a href='https://www.gnu.org/licenses/old";
  body << "-licenses/lgpl-2.1.html'>GNU LGPL v2.1</a></p>";

  QMessageBox::about(0, title, body.join(""));
}

void MainWindow::browseForModArchives()
{
  QStringList mods =
    QFileDialog::getOpenFileNames(this, tr("Select mod archives"),
                                  QDir::homePath(),
                                  "IE Mod/ZIP (*.iemod *.zip)");
  qDebug() << "Selected mods were:" << mods;
  emit importModArchives(mods);
}

void MainWindow::importModDistArchiveSuccess(bool success) const
{
  if (success) {
    statusBar()->showMessage(importModSuccess, 5000);
  } else {
    statusBar()->showMessage(importModFailure, 5000);
  }
}
