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
#include "installedmodsmodel.h"

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
  installedModsView->setHeaderHidden(true);
  installedModsView->setSelectionMode(QAbstractItemView::MultiSelection);
  installedModsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  // TODO: if you select a parent, all children should be selected; same for deselection
  // TODO: if you select a child, the parent should also be selected; if you deselect all children, the parent should also be deselected

  availableModsModel = dataManager->availableModsModel;
  installedModsModel = dataManager->installedModsModel;
  availableModsView->setModel(availableModsModel);
  installedModsView->setModel(installedModsModel);

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
