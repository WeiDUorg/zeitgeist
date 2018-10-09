/*
 *  Zeitgeist - Graphical WeiDU frontend
 *  Copyright (C) 2018  Fredrik Lindgren

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

#include "archivemodel.h"
#include "coordinator.h"
#include "datamanager.h"
#include "moddistwindow.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringListModel>
#include <QTreeView>
#include <QVBoxLayout>

ModDistWindow::ModDistWindow(QWidget* parent,
                             const Coordinator* coordinator) :
  QWidget(parent),targetName(QString()),
  currentDir(QDir::homePath())
{
  setWindowFlags(Qt::Window);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(tr("Create Mod Distribution"));
  resize(640, 480);

  ArchiveModel* filesModel = coordinator->dataManager->archiveModel;
  connect(this, SIGNAL(clearModel()),
          filesModel, SLOT(clear()));
  connect(this, SIGNAL(addFiles(const QStringList&)),
          filesModel, SLOT(addFiles(const QStringList&)));
  connect(this, SIGNAL(addDirs(const QStringList&)),
          filesModel, SLOT(addDirs(const QStringList&)));
  connect(filesModel, SIGNAL(isEmpty(bool)),
          this, SLOT(hasData(bool)));
  connect(filesModel, SIGNAL(workingDir(const QString&)),
          this, SLOT(workingDir(const QString&)));

  QLabel* filesLabel = new QLabel(tr("Files"), this);
  //QLabel* presetsLabel = new QLabel(tr("Presets"), this);

  createButton = new QPushButton(tr("Create"), this);
  createButton->setEnabled(false);
  createButton->setToolTip(createButtonDisabled);
  connect(createButton, SIGNAL(clicked()),
          this, SLOT(selectTargetName()));
  connect(this, SIGNAL(createDist(const QString&)),
          coordinator, SIGNAL(createModDistArchive(const QString&)));
  connect(coordinator->dataManager, SIGNAL(createModDistArchiveSuccess(bool)),
          this, SLOT(modDistArchiveSuccess(bool)));

  filesView = new QListView(this);
  filesView->setSelectionMode(QAbstractItemView::SingleSelection);
  filesView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  filesView->setModel(filesModel);

  /* There does not seem to be a way to select files and
     directories from a single dialog without dropping the
     native widgets
  */
  filesButton = new QPushButton(tr("Add Files"), this);
  dirButton = new QPushButton(tr("Add Directory"), this);
  connect(filesButton, SIGNAL(clicked()),
          this, SLOT(openFilesDialog()));
  connect(dirButton, SIGNAL(clicked()),
          this, SLOT(openDirDialog()));

  /*  presetsView = new QListView(this);
  presetsView->setSelectionMode(QAbstractItemView::SingleSelection);
  presetsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  */

  QHBoxLayout* filesHeaderLayout = new QHBoxLayout;
  filesHeaderLayout->addWidget(filesLabel);
  filesHeaderLayout->addStretch();
  filesHeaderLayout->addWidget(filesButton);
  filesHeaderLayout->addWidget(dirButton);

  QVBoxLayout* filesLayout = new QVBoxLayout;
  filesLayout->addLayout(filesHeaderLayout);
  filesLayout->addWidget(filesView);

  /*  QVBoxLayout* presetsLayout = new QVBoxLayout;
  presetsLayout->addWidget(presetsLabel);
  presetsLayout->addWidget(presetsView);
  */

  QHBoxLayout* listsLayout = new QHBoxLayout;
  listsLayout->addLayout(filesLayout);
  //listsLayout->addLayout(presetsLayout);

  QHBoxLayout* archiveLayout = new QHBoxLayout;
  archiveLayout->addWidget(createButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(listsLayout);
  mainLayout->addLayout(archiveLayout);

  setLayout(mainLayout);

  // List of presets
  // If you click a preset, the saved files and other info is loaded
  // If you click away from a non-empty state, the state is saved?; handled by dialog
  // When you create an archive, the state is saved as a preset; handled by dialog
  // Some way to delete presets
  // Some way to clear current state

  // Metadata:
  // Name of mod
  // Short description
  //
}

ModDistWindow::~ModDistWindow()
{
  emit clearModel();
}

void ModDistWindow::openFilesDialog()
{
  QStringList files =
    QFileDialog::getOpenFileNames(this,tr("Select files to add"),
                                  currentDir,QString(),nullptr,
                                  QFileDialog::ReadOnly);
  if (!files.isEmpty()) {
    emit addFiles(files);
  }
}

void ModDistWindow::openDirDialog()
{
  QString dir =
    QFileDialog::getExistingDirectory(this,tr("Select directory to add"),
                                      currentDir,QFileDialog::ShowDirsOnly |
                                      QFileDialog::ReadOnly);
  if (!dir.isEmpty()) {
    QStringList dirs;
    dirs << dir;
    emit addDirs(dirs);
  }
}

void ModDistWindow::selectTargetName()
{
  targetName =
    QFileDialog::getSaveFileName(this,tr("Select output file"),
                                 QDir::homePath(),"IE Mod (*.iemod)");
  if (!targetName.endsWith(".iemod", Qt::CaseInsensitive)) {
    targetName = targetName + ".iemod";
    // Check if the new targetName exists?
  }
  qDebug() << "Saving target as" << targetName;
  emit createDist(targetName);
}

void ModDistWindow::hasData(bool empty)
{
  if (!empty) {
    createButton->setEnabled(true);
    createButton->setToolTip(createButtonEnabled);
  } else {
    createButton->setEnabled(false);
    createButton->setToolTip(createButtonDisabled);
  }
}

void ModDistWindow::workingDir(const QString& dir)
{
  currentDir = dir;
}

void ModDistWindow::modDistArchiveSuccess(bool success)
{
  if (success) {
    QString title = tr("Success");
    QString body =
      tr("The distributable archive was successfully created at: ");
    body += targetName;
    QMessageBox::information(this, title, body, QMessageBox::Ok,
                             QMessageBox::Ok);
    this->close();
  } else {
    QString title = tr("Something went wrong");
    QString body =
      tr("Something went wrong during creation of the archive");
    QMessageBox::information(this, title, body, QMessageBox::Ok);
  }
}
