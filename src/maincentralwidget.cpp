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
#include "coordinator.h"
#include "datamanager.h"
#include "availablemodsmodel.h"
#include "installedmodsmodel.h"

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
#include <QList>
#include <QDebug>

MainCentralWidget::MainCentralWidget(QWidget* parent, const Coordinator* coordinator) :
  QWidget(parent), coordinator(coordinator)
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

void MainCentralWidget::handleInstalledSelection(const QItemSelection& selected,
                                                 const QItemSelection& deselected)
{
  QItemSelectionModel* selectionModel = installedModsView->selectionModel();
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
          qDebug() << "Selecting all children";
          selectChildren.select(model->index(0, 0, index),
                                model->index(model->rowCount(index) - 1, 0, index));
        }
      } else {
        if (!selectionModel->isSelected(index.parent())) {
          qDebug() << "Selecting the parent";
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
        qDebug() << "Deselecting all children";
        QItemSelection selection;
        deselectChildren.select(model->index(0, 0, index),
                                model->index(model->rowCount(index) - 1, 0, index));
      } else {
        if (selectionModel->isSelected(index.parent())) {
          qDebug() << "Maybe deselecting the parent";
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
            qDebug() << "Deselecting the parent";
            deselectParent.select(parent, parent);
          }
        }
      }
    }
  }
  QItemSelection selection;
  if (!selected.isEmpty()) {
    qDebug() << "Merging selection";
    selection.merge(selectChildren, QItemSelectionModel::Select);
    selection.merge(selectParent, QItemSelectionModel::Select);
    selectionModel->select(selection, QItemSelectionModel::Select);
  } else
  if (!deselected.isEmpty()) {
    qDebug() << "Merging deselection";
    selection.merge(deselectChildren, QItemSelectionModel::Select);
    selection.merge(deselectParent, QItemSelectionModel::Select);
    selectionModel->select(selection, QItemSelectionModel::Deselect);
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

void MainCentralWidget::getSelectedAvailableMod()
{
  QModelIndex index = availableModsView->selectionModel()->currentIndex();
  QString tp2 = coordinator->dataManager->availableModsModel->data(index, Qt::DisplayRole).toString();
  emit selectedAvailableMod(tp2);
}
