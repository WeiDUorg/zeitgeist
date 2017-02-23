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

#include "enqueuemodwindow.h"
#include "datamanager.h"
#include "coordinator.h"
#include "controller.h"
#include "installedmodsmodel.h"
#include "queuedmodsmodel.h"
#include "weidulog.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QItemSelectionModel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QListView>
#include <QLabel>
#include <QModelIndex>
#include <QModelIndexList>
#include <QPushButton>
#include <QStringListModel>
#include <QVBoxLayout>

EnqueueModWindow::EnqueueModWindow(QWidget* parent,
                                   const Coordinator* coordinator,
                                   const QString& tp2) :
  QWidget(parent), coordinator(coordinator), tp2(tp2), currentComponentList(0)
{
  resize (640, 520); // Should ideally assume a size depending on parent's size
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(windowTitle + ": " + tp2);
  connect(this, SIGNAL(enqueueComponents(WeiduLog*)),
          coordinator->dataManager, SLOT(enqueueComponents(WeiduLog*)));

  QLabel* languageLabel = new QLabel(tr("Languages"), this);
  languageListModel = new QStringListModel(this);
  languageListView = new QListView(this);
  languageListView->setSelectionMode(QAbstractItemView::SingleSelection);
  languageListView->setModel(languageListModel);
  connect(this, SIGNAL(getLanguageList(const QString&)),
          coordinator->controller, SLOT(getLanguageList(const QString&)));
  connect(coordinator->controller, SIGNAL(languageList(const QStringList&)),
          this, SLOT(languageList(const QStringList&)));
  connect(languageListView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&,
                                                                      const QItemSelection&)),
          this, SLOT(handleLanguageSelection(const QItemSelection&, const QItemSelection)));
  connect(this, SIGNAL(getComponentList(const QString&, const int&)),
          coordinator->controller, SLOT(getComponentList(const QString&, const int&)));
  connect(coordinator->controller, SIGNAL(componentList(WeiduLog*)),
          this, SLOT(componentList(WeiduLog*)));

  QLabel* componentLabel = new QLabel(tr("Components"), this);
  componentListView = new QListWidget(this);

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
  if (currentComponentList) {
    delete currentComponentList;
    currentComponentList = 0;
  }
}

void EnqueueModWindow::languageList(const QStringList& list)
{
  if (!list.isEmpty()) {
    languageListModel->setStringList(list);
  } else {
    emit getComponentList(tp2, 0); // If the mod does not include any languages, 0 is default
  }
}

void EnqueueModWindow::handleLanguageSelection(const QItemSelection& selected, const QItemSelection&)
{
  if (!selected.isEmpty()) {
    if (currentComponentList) {
      delete currentComponentList;
      currentComponentList = 0;
    }
    emit getComponentList(tp2, languageListView->selectionModel()->currentIndex().row());
  }
}

void EnqueueModWindow::componentList(WeiduLog* list)
{
  currentComponentList = list;
  componentListView->clear();
  QList<int> installedComponents =
    coordinator->dataManager->installedModsModel->installedComponents(tp2);
  QList<int> queuedComponents =
    coordinator->dataManager->inQueuedModsModel->queuedComponents(tp2);
  int count = 0;
  foreach (QList<WeiduLogComponent> compList, list->data) {
    foreach (WeiduLogComponent comp, compList) {
      // Can't preserve index because setHidden() does nothing
      if (!installedComponents.contains(comp.number) &&
          !queuedComponents.contains(comp.number)) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setText(comp.comment);
        item->setCheckState(Qt::Unchecked);
        componentListView->insertItem(count, item);
        count++;
      }
    }
  }
}

void EnqueueModWindow::handleProceed() {
  // QListWidget preclude doing this by index (vide supra)
  QList<WeiduLogComponent> acc;
  for (int i = 0; i < componentListView->count(); i++) {
    QListWidgetItem* item = componentListView->item(i);
    if (item->checkState()) {
      foreach (QList<WeiduLogComponent> list, currentComponentList->data) {
        foreach (WeiduLogComponent comp, list) {
          if (comp.comment.compare(item->text()) == 0) {
            acc.append(comp);
          }
        }
      }
    }
  }
  QList<QList<WeiduLogComponent>> result;
  result << acc;
  emit enqueueComponents(new WeiduLog(0, result));
  this->close();
}
