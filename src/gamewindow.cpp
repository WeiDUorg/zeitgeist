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

#include "gamewindow.h"
#include "gamelistmodel.h"
#include "datamanager.h"

#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemView>

GameWindow::GameWindow(DataManager* dataManager, QWidget* parent) : QWidget(parent), dataManager(dataManager)
{
  resize(640, 400);
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Edit Games"));

  gameList = new QTableView;
  gameList->verticalHeader()->hide();
  gameList->setSelectionMode(QAbstractItemView::SingleSelection);

  model = dataManager->gameListModel;
  gameList->setModel(model);

  addGameButton = new QPushButton(tr("Add"), this);
  removeGameButton = new QPushButton(tr("Remove"), this);
  selectGameButton = new QPushButton(tr("Select"), this);
  QVBoxLayout* buttonLayout = new QVBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(addGameButton);
  buttonLayout->addWidget(removeGameButton);
  buttonLayout->addWidget(selectGameButton);
  connect(addGameButton, SIGNAL(clicked()),
          this, SLOT(browse()));
  connect(this, SIGNAL(addGame(QString)),
          model, SLOT(addGame(QString)));
  connect(model, SIGNAL(notAGameDirectory(QString)),
          this, SLOT(notAGameDirectory(QString)));
  connect(removeGameButton, SIGNAL(clicked()),
          this, SLOT(remove()));
  connect(this, SIGNAL(removeGame(QModelIndex)),
          model, SLOT(removeGame(QModelIndex)));

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(gameList);
  layout->addLayout(buttonLayout);
  setLayout(layout);
}

void GameWindow::browse()
{
  QString path = QFileDialog::getExistingDirectory(this,
                                                   tr("Open Game Directory"),
                                                   "/home");
  // Guard against the user clicking cancel
  if (!path.isEmpty()) {
    emit addGame(path);
  }
}

void GameWindow::remove()
{
  QModelIndex index = gameList->currentIndex();
  if (index.isValid()) {
    emit removeGame(index);
  }
}

void GameWindow::notAGameDirectory(QString path)
{
  QMessageBox msg;
  msg.setText(path + " " + tr("does not contain a valid game directory."));
  msg.exec();
}
