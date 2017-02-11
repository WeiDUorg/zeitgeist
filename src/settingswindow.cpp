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

#include "settingswindow.h"
#include "coordinator.h"
#include "platform.h"

#include <QDebug>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QString>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QErrorMessage>

SettingsWindow::SettingsWindow(const Coordinator* coordinator, QWidget* parent) :
  QWidget(parent), coordinator(coordinator)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Edit Program Settings"));

  const QString weidu = "WeiDU";
  QLabel* weiduLabel = new QLabel(weidu, this);
  weiduTextField = new QLineEdit(this);
  weiduTextField->setMinimumWidth(100);
  connect(weiduTextField, SIGNAL(textChanged(const QString&)),
          this, SLOT(initialWeiduValidation(const QString&)));
  connect(this, SIGNAL(weiduPassOff(const QString&)),
          coordinator, SLOT(weiduPath(const QString&)));
  connect(coordinator, SIGNAL(weiduFailedValidationSignal(const QString&)),
          this, SLOT(weiduFailedValidation(const QString&)));
  QPushButton* weiduBrowse = new QPushButton(tr("Browse"), this);
  weiduBrowse->setFocus();
  connect(weiduBrowse, SIGNAL(clicked()),
          this, SLOT(browseForWeidu()));
  QHBoxLayout* weiduLayout = new QHBoxLayout;
  weiduLayout->addWidget(weiduLabel);
  weiduLayout->addWidget(weiduTextField);
  weiduLayout->addWidget(weiduBrowse);

  setLayout(weiduLayout);
}

void SettingsWindow::browseForWeidu()
{
  QString caption = tr("Locate WeiDU");
  QString filter = WEIDU_EXECUTABLE;
  QString* selectedFilter = 0;
  QString path = QFileDialog::getOpenFileName(this, caption,
                                              QDir::homePath(),
                                              filter, selectedFilter,
                                              QFileDialog::ReadOnly);
  if (!path.isEmpty()) {
    weiduTextField->setText(path);
  }
}

/* This function verifies that the selected file is superficially a valid
   WeiDU executable
*/
void SettingsWindow::initialWeiduValidation(const QString& path)
{
  QFileInfo info(path);
  if (info.isFile() && info.fileName().compare(WEIDU_EXECUTABLE,
                                               Qt::CaseInsensitive) == 0) {
    // There ought to be some kind of visual indication, so the user knows
    // it's ok to close the settings window and proceed
    qDebug() << "WeiDU at" << path << "passes initial inspection";
    emit weiduPassOff(path);
  }
}

/* This function is called if WeiduManager got a WeiDU that is not a runnable
   WeiDU executable
*/
void SettingsWindow::weiduFailedValidation(const QString& weiduPath)
{
  QErrorMessage* error = new QErrorMessage(this);
  error->showMessage(tr("The selected file is not an executable WeiDU binary: ") + weiduPath);
  weiduTextField->clear();
}
