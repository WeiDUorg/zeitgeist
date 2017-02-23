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
#include "availablemodsmodel.h"
#include "coordinator.h"
#include "datamanager.h"
#include "installedmodsmodel.h"
#include "queuedmodsmodel.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QTreeView>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QItemSelection>
#include <QItemSelectionModel>
#include <QModelIndex>
#include <QModelIndexList>
#include <QList>
#include <QDebug>

MainCentralWidget::MainCentralWidget(QWidget* parent, const Coordinator* coordinator) :
  QWidget(parent), coordinator(coordinator)
{
  QLabel* availableModsLabel = new QLabel(tr("Available Mods"), this);
  QLabel* installQueueLabel = new QLabel(tr("Installation Queue"), this);
  QLabel* uninstallQueueLabel = new QLabel(tr("Uninstallation Queue"), this);
  QLabel* installedModsLabel = new QLabel(tr("Installed Mods"), this);

  availableModsView = new QListView(this);
  availableModsView->setSelectionMode(QAbstractItemView::SingleSelection);
  availableModsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  installQueueView = new QTreeView(this);
  installQueueView->setModel(coordinator->dataManager->inQueuedModsModel);
  installQueueView->setHeaderHidden(true);
  installQueueView->setSelectionMode(QAbstractItemView::MultiSelection);
  installQueueView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(installQueueView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,
                                                                      const QItemSelection&)),
          this, SLOT(handleInstallQueueSelection(const QItemSelection&,
                                                 const QItemSelection&)));
  connect(installQueueView->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
          this, SLOT(handleQueueAvailability(const QModelIndex, int, int)));
  connect(installQueueView->model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
          this, SLOT(handleQueueAvailability(const QModelIndex, int, int)));

  uninstallQueueView = new QTreeView(this);
  uninstallQueueView->setModel(coordinator->dataManager->outQueuedModsModel);
  uninstallQueueView->setHeaderHidden(true);
  uninstallQueueView->setSelectionMode(QAbstractItemView::MultiSelection);
  uninstallQueueView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(uninstallQueueView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,
                                                                        const QItemSelection&)),
          this, SLOT(handleUninstallQueueSelection(const QItemSelection&,
                                                   const QItemSelection&)));
  connect(uninstallQueueView->model(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
          this, SLOT(handleQueueAvailability(const QModelIndex, int, int)));
  connect(uninstallQueueView->model(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
          this, SLOT(handleQueueAvailability(const QModelIndex, int, int)));

  installedModsView = new QTreeView(this);
  installedModsView->setHeaderHidden(true);
  installedModsView->setSelectionMode(QAbstractItemView::MultiSelection);
  installedModsView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  availableModsView->setModel(coordinator->dataManager->availableModsModel);
  connect(availableModsView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,
                                                                       const QItemSelection&)),
          this, SLOT(handleAvailableSelection(const QItemSelection&, const QItemSelection&)));
  installedModsView->setModel(coordinator->dataManager->installedModsModel);
  connect(installedModsView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,
                                                                       const QItemSelection&)),
          this, SLOT(handleInstalledSelection(const QItemSelection&, const QItemSelection&)));

  QVBoxLayout* availableModsLayout = new QVBoxLayout;
  availableModsLayout->addWidget(availableModsLabel);
  availableModsLayout->addWidget(availableModsView);
  QVBoxLayout* queueLayout = new QVBoxLayout;
  queueLayout->addWidget(installQueueLabel);
  queueLayout->addWidget(installQueueView);
  queueLayout->addWidget(uninstallQueueLabel);
  queueLayout->addWidget(uninstallQueueView);
  QVBoxLayout* installedModsLayout = new QVBoxLayout;
  installedModsLayout->addWidget(installedModsLabel);
  installedModsLayout->addWidget(installedModsView);

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addLayout(availableModsLayout);
  layout->addLayout(queueLayout);
  layout->addLayout(installedModsLayout);

  setLayout(layout);
}

void MainCentralWidget::handleTreeSelection(QItemSelectionModel* selectionModel, const QItemSelection& selected,
                         const QItemSelection& deselected)
{
  QModelIndex current = selectionModel->currentIndex();
  QItemSelection selectChildren;
  QItemSelection selectParent;
  QItemSelection deselectChildren;
  QItemSelection deselectParent;
  foreach (QModelIndex index, selected.indexes()) {
    if (index.isValid()) {
      const QAbstractItemModel* model = index.model();
      if (model->hasChildren(index)) {
        if (current.isValid() && current == index) {
          selectChildren.select(model->index(0, 0, index),
                                model->index(model->rowCount(index) - 1, 0, index));
        }
      } else {
        if (!selectionModel->isSelected(index.parent())) {
          QModelIndex parent = index.parent();
          selectParent.select(parent, parent);
        }
      }
    }
  }
  foreach (QModelIndex index, deselected.indexes()) {
    if (index.isValid()) {
      const QAbstractItemModel* model = index.model();
      if (model->hasChildren(index)) {
        QItemSelection selection;
        deselectChildren.select(model->index(0, 0, index),
                                model->index(model->rowCount(index) - 1, 0, index));
      } else {
        if (selectionModel->isSelected(index.parent())) {
          QModelIndex parent = index.parent();
          bool deselect = true;
          for (int i = 0; i < model->rowCount(parent); ++i) {
            QModelIndex childIndex = model->index(i, 0, parent);
            if (selectionModel->isSelected(childIndex)) {
              deselect = false;
              break;
            }
          }
          if (deselect) {
            deselectParent.select(parent, parent);
          }
        }
      }
    }
  }
  QItemSelection selection;
  if (!selected.isEmpty()) {
    selection.merge(selectChildren, QItemSelectionModel::Select);
    selection.merge(selectParent, QItemSelectionModel::Select);
    selectionModel->select(selection, QItemSelectionModel::Select);
  } else
  if (!deselected.isEmpty()) {
    selection.merge(deselectChildren, QItemSelectionModel::Select);
    selection.merge(deselectParent, QItemSelectionModel::Select);
    selectionModel->select(selection, QItemSelectionModel::Deselect);
  }
}

void MainCentralWidget::handleInstalledSelection(const QItemSelection& selected,
                                                 const QItemSelection& deselected)
{
  QItemSelectionModel* model = installedModsView->selectionModel();
  handleTreeSelection(model, selected, deselected);
  if (model->hasSelection()) {
    emit installedModSelected(true);
  } else {
    emit installedModSelected(false);
  }
}

void MainCentralWidget::handleAvailableSelection(const QItemSelection& selected,
                                                 const QItemSelection&)
{
  if (!selected.isEmpty()) {
    emit availableModSelected(true);
  } else {
    emit availableModSelected(false);
  }
}

void MainCentralWidget::handleInstallQueueSelection(const QItemSelection& selected,
                                                    const QItemSelection& deselected)
{
  QItemSelectionModel* model = installQueueView->selectionModel();
  handleTreeSelection(model, selected, deselected);
  if (model->hasSelection()) {
    emit queuedModSelected(true);
  } else if (!uninstallQueueView->selectionModel()->hasSelection()) {
    emit queuedModSelected(false);
  }
}

void MainCentralWidget::handleUninstallQueueSelection(const QItemSelection& selected,
                                                      const QItemSelection& deselected)
{
  QItemSelectionModel* model = uninstallQueueView->selectionModel();
  handleTreeSelection(model, selected, deselected);
  if (model->hasSelection()) {
    emit queuedModSelected(true);
  } else if (!installQueueView->selectionModel()->hasSelection()) {
    emit queuedModSelected(false);
  }
}

void MainCentralWidget::handleQueueAvailability(const QModelIndex&, int, int)
{
  emit queuedModAvailable((installQueueView->model()->rowCount() > 0) ||
                          (uninstallQueueView->model()->rowCount() > 0));
}

/*
 * Should really have sent a signal to dataManager with the index and
 * let things be handled there
 */
void MainCentralWidget::getSelectedAvailableMod()
{
  QModelIndex index = availableModsView->selectionModel()->currentIndex();
  QString tp2 = coordinator->dataManager->availableModsModel->data(index, Qt::DisplayRole).toString();
  emit selectedAvailableMod(tp2);
}

/* Ditto */
void MainCentralWidget::getSelectedInstalledMods()
{
  QModelIndexList list = installedModsView->selectionModel()->selectedIndexes();
  WeiduLog* componentList = coordinator->dataManager->installedModsModel->selectedComponents(list);
  emit selectedInstalledMods(componentList);
}

void MainCentralWidget::clearInstalledSelection()
{
  installedModsView->clearSelection();
}

void MainCentralWidget::getSelectedQueuedMods()
{
  QModelIndexList installQueue = installQueueView->selectionModel()->selectedIndexes();
  QModelIndexList uninstallQueue = uninstallQueueView->selectionModel()->selectedIndexes();
  emit selectedInstallQueuedMods(installQueue);
  emit selectedUninstallQueuedMods(uninstallQueue);
}

void MainCentralWidget::clearQueuedSelection()
{
  installQueueView->clearSelection();
  uninstallQueueView->clearSelection();
}
