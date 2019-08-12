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

#include "gamewindow.h"
#include "comboboxdelegate.h"
#include "gamelistmodel.h"
#include "datamanager.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QModelIndex>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>

GameWindow::GameWindow(QWidget* parent, const DataManager* dataManager) :
  QWidget(parent), dataManager(dataManager)
{
  resize(640, 400);
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setWindowTitle(tr("Edit Games"));

  gameList = new QTableView(this);
  gameList->verticalHeader()->hide();
  gameList->setSelectionMode(QAbstractItemView::SingleSelection);
  gameList->setItemDelegate(new ComboBoxDelegate(gameList));

  model = dataManager->gameListModel;
  connect(model, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
          this, SLOT(rowsInserted(const QModelIndex&, int, int)));
  gameList->setModel(model);
  gameList->resizeColumnsToContents();

  foreach (QModelIndex index, model->specialIndexes()) {
    gameList->openPersistentEditor(index);
  }

  addGameButton = new QPushButton(tr("Add"), this);
  removeGameButton = new QPushButton(tr("Remove"), this);
  selectGameButton = new QPushButton(tr("Select"), this);
  closeWindowButton = new QPushButton(tr("Close"), this);
  QVBoxLayout* buttonLayout = new QVBoxLayout;
  buttonLayout->addStretch();
  buttonLayout->addWidget(addGameButton);
  buttonLayout->addWidget(removeGameButton);
  buttonLayout->addWidget(selectGameButton);
  buttonLayout->addWidget(closeWindowButton);
  connect(addGameButton, SIGNAL(clicked()),
          this, SLOT(browse()));
  connect(this, SIGNAL(addGame(const QString&)),
          model, SLOT(addGame(const QString&)));
  connect(model, SIGNAL(notAGameDirectory(const QString&)),
          this, SLOT(notAGameDirectory(const QString&)));
  connect(removeGameButton, SIGNAL(clicked()),
          this, SLOT(remove()));
  connect(this, SIGNAL(removeGame(const QModelIndex&)),
          model, SLOT(removeGame(const QModelIndex&)));
  connect(selectGameButton, SIGNAL(clicked()),
          this, SLOT(select()));
  connect(this, SIGNAL(useGame(const QString&)),
          dataManager, SLOT(useGame(const QString&)));
    connect(closeWindowButton, SIGNAL(clicked()),
	    this, SLOT(close()));

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(gameList);
  layout->addLayout(buttonLayout);
  setLayout(layout);
}

void GameWindow::browse()
{
  QString path = QFileDialog::getExistingDirectory(this,
                                                   tr("Open Game Directory"),
                                                   QDir::homePath());
  // Guard against the user clicking cancel
  if (!path.isEmpty()) {
    emit addGame(path);
    gameList->resizeColumnsToContents();
  }
}

void GameWindow::remove()
{
  QModelIndex index = gameList->currentIndex();
  if (index.isValid()) {
    emit removeGame(index);
    gameList->resizeColumnsToContents();
  }
}

void GameWindow::select()
{
  QModelIndex index = gameList->currentIndex();
  select(index);
}

void GameWindow::select(const QModelIndex& index)
{
  if (index.isValid()) {
    QString path = model->pathOfIndex(index);
    if (!path.isEmpty()) {
      emit useGame(path);
    } // else?
  }
}

void GameWindow::rowsInserted(const QModelIndex&, int start, int end)
{
  /* If the game list is empty and the user adds a game, automatically
   * select that game
   */
  if (model->rowCount() == 1) {
    select(model->index(0,0));
  }
  foreach (QModelIndex index, model->specialIndexes()) {
    if (index.row() >= start && index.row() <= end) {
      gameList->openPersistentEditor(index);
    }
  }
}

void GameWindow::notAGameDirectory(const QString& path)
{
  QMessageBox msg;
  msg.setText(path + " " + tr("does not contain a valid game directory."));
  msg.exec();
}
