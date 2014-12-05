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

#include "maincentralwidget.h"
#include "datamanager.h"
#include "availablemodsmodel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QTreeView>
#include <QAbstractItemView>

MainCentralWidget::MainCentralWidget(const DataManager* dataManager,
                                     QWidget* parent) :
  QWidget(parent), dataManager(dataManager)
{
  QLabel* availableModsLabel = new QLabel(tr("Available Mods"), this);
  QLabel* queueLabel = new QLabel(tr("Queue"), this);
  QLabel* installedModsLabel = new QLabel(tr("Installed Mods"), this);

  availableModsView = new QListView(this);
  availableModsView->setSelectionMode(QAbstractItemView::SingleSelection);
  availableModsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  queueView = new QTreeView(this);
  installedModsView = new QTreeView(this);

  availableModsModel = dataManager->availableModsModel;
  availableModsView->setModel(availableModsModel);

  QVBoxLayout* availableModsLayout = new QVBoxLayout;
  availableModsLayout->addWidget(availableModsLabel);
  availableModsLayout->addWidget(availableModsView);
  QVBoxLayout* queueLayout = new QVBoxLayout;
  queueLayout->addWidget(queueLabel);
  queueLayout->addWidget(queueView);
  QVBoxLayout* installedModsLayout = new QVBoxLayout;
  installedModsLayout->addWidget(installedModsLabel);
  installedModsLayout->addWidget(installedModsView);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addLayout(availableModsLayout);
  layout->addLayout(queueLayout);
  layout->addLayout(installedModsLayout);

  setLayout(layout);
}
