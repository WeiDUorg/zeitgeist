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

#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

GameWindow::GameWindow(QWidget* parent) : QWidget(parent)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Edit Games"));

  gameList = new QTableView;
  gameList->verticalHeader()->hide();

  addGameButton = new QPushButton(tr("Add"), this);
  removeGameButton = new QPushButton(tr("Remove"), this);
  selectGameButton = new QPushButton(tr("Select"), this);
  QVBoxLayout* buttonLayout = new QVBoxLayout;
  buttonLayout->addWidget(addGameButton);
  buttonLayout->addWidget(removeGameButton);
  buttonLayout->addWidget(selectGameButton);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(gameList);
  layout->addLayout(buttonLayout);
  setLayout(layout);
}
