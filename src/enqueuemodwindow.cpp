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

#include "enqueuemodwindow.h"
#include "datamanager.h"
#include "coordinator.h"
#include "controller.h"
#include "enqueuemodmodel.h"
#include "installedmodsmodel.h"
#include "queuedmodsmodel.h"
#include "weidulog.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QListView>
#include <QLabel>
#include <QModelIndex>
#include <QPushButton>
#include <QStringListModel>
#include <QTreeView>
#include <QVBoxLayout>

EnqueueModWindow::EnqueueModWindow(QWidget* parent,
                                   const Coordinator* coordinator,
                                   const QString& tp2) :
  QWidget(parent), coordinator(coordinator), tp2(tp2)
{
  connect(this, SIGNAL(resetModel()),
          coordinator->dataManager->enqueueModModel, SLOT(clear()));
  emit resetModel();
  resize (640, 520); // Should ideally assume a size depending on parent's size
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(windowTitle + ": " + tp2);
  connect(this, SIGNAL(enqueueComponents(const QString&, int)),
          coordinator->dataManager,
          SLOT(enqueueComponents(const QString&, int)));

  QLabel* languageLabel = new QLabel(tr("Languages"), this);
  languageListModel = new QStringListModel(this);
  languageListView = new QListView(this);
  languageListView->setSelectionMode(QAbstractItemView::SingleSelection);
  languageListView->setModel(languageListModel);
  connect(this, SIGNAL(getLanguageList(const QString&)),
          coordinator->controller, SLOT(getLanguageList(const QString&)));
  connect(coordinator->controller, SIGNAL(languageList(const QStringList&)),
          this, SLOT(languageList(const QStringList&)));
  connect(languageListView->selectionModel(),
          SIGNAL(selectionChanged(const QItemSelection&,
                                  const QItemSelection&)),
          this, SLOT(handleLanguageSelection(const QItemSelection&,
                                             const QItemSelection)));
  connect(this, SIGNAL(getComponentList(const QString&, const int&)),
          coordinator->controller, SLOT(getComponentList(const QString&,
                                                         const int&)));

  QLabel* componentLabel = new QLabel(tr("Components"), this);
  componentListView = new QTreeView(this);
  componentListView->setModel(coordinator->dataManager->enqueueModModel);
  componentListView->setHeaderHidden(true);
  componentListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

  QGridLayout* paneLayout = new QGridLayout;
  paneLayout->addWidget(languageLabel, 0, 0);
  paneLayout->addWidget(languageListView, 1, 0);
  paneLayout->addWidget(componentLabel, 0, 1);
  paneLayout->addWidget(componentListView, 1, 1);

  proceedButton = new QPushButton(tr("Proceed"), this);
  connect(proceedButton, SIGNAL(clicked()),
          this, SLOT(handleProceed()));

  QHBoxLayout* buttonLayout = new QHBoxLayout;
  buttonLayout->setAlignment(Qt::AlignRight);
  buttonLayout->addWidget(proceedButton);

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addLayout(paneLayout);
  layout->addLayout(buttonLayout);
  setLayout(layout);

  emit getLanguageList(tp2);
}

EnqueueModWindow::~EnqueueModWindow()
{

}

void EnqueueModWindow::languageList(const QStringList& list)
{
  if (!list.isEmpty()) {
    languageListModel->setStringList(list);
  } else {
    /* If the mod does not include any languages, 0 is default */
    emit getComponentList(tp2, 0);
  }
}

void EnqueueModWindow::handleLanguageSelection(const QItemSelection& selected,
                                               const QItemSelection&)
{
  if (!selected.isEmpty()) {
    const QModelIndex i = languageListView->selectionModel()->currentIndex();
    emit getComponentList(tp2, i.row());
  }
}

void EnqueueModWindow::handleProceed() {
  const QModelIndex i = languageListView->selectionModel()->currentIndex();
  emit enqueueComponents(tp2, i.row());
  this->close();
}
