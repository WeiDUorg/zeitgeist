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
#include "coordinator.h"
#include "controller.h"

#include <QAbstractItemView>
#include <QDebug>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QListView>
#include <QLabel>
#include <QStringListModel>

EnqueueModWindow::EnqueueModWindow(QWidget* parent,
                                   const Coordinator* coordinator,
                                   const QString& tp2) :
  QWidget(parent), coordinator(coordinator), tp2(tp2)
{
  resize (640, 480); // Should ideally assume a size depending on parent's size
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(windowTitle + ": " + tp2);

  QLabel* languageLabel = new QLabel(tr("Languages"), this);
  languageListModel = new QStringListModel(this);
  languageListView = new QListView(this);
  languageListView->setSelectionMode(QAbstractItemView::SingleSelection);
  languageListView->setModel(languageListModel);
  connect(this, SIGNAL(getLanguageList(const QString&)),
          coordinator->controller, SLOT(getLanguageList(const QString&)));
  connect(coordinator->controller, SIGNAL(languageList(const QStringList&)),
          this, SLOT(languageList(const QStringList&)));

  QLabel* componentLabel = new QLabel(tr("Components"), this);
  componentListView = new QListWidget(this);

  QGridLayout* layout = new QGridLayout(this);
  layout->addWidget(languageLabel, 0, 0);
  layout->addWidget(languageListView, 1, 0);
  layout->addWidget(componentLabel, 0, 1);
  layout->addWidget(componentListView, 1, 1);
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
  } // else proceed with language-index 0
}
