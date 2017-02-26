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
#include "controller.h"
#include "platform.h"

#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QErrorMessage>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

SettingsWindow::SettingsWindow(QWidget* parent, const Coordinator* coordinator) :
  QWidget(parent), coordinator(coordinator)
{
  setWindowFlags(Qt::Window);
  setWindowTitle(tr("Edit Program Settings"));

  weiduLabel = new QLabel(locateWeidu, this);
  weiduLabel->setMinimumWidth(150); // Could probably be less fragile
  connect(this, SIGNAL(installerAvailable(const bool&)),
          parent, SLOT(installerAvailable(const bool&)));
  connect(coordinator->controller, SIGNAL(weiduVersionSignal(const int&)),
          this, SLOT(weiduVersion(const int&)));
  connect(coordinator->controller, SIGNAL(confirmedWeiduPath(const QString&)),
          this, SLOT(weiduPath(const QString&)));
  connect(this, SIGNAL(doesWeiduExist()),
          coordinator->controller, SLOT(weiduCheck()));
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

  eeLangField = new QComboBox(this);
  eeLangField->setEditable(false);
  // What the hell kind of language does not have a list literal?
  eeLangField->addItem("Czech");
  eeLangField->addItem("German");
  eeLangField->addItem("English");
  eeLangField->addItem("French");
  eeLangField->addItem("Italian");
  eeLangField->addItem("Japanese");
  eeLangField->addItem("Korean");
  eeLangField->addItem("Polish");
  eeLangField->addItem("Brazilian Portuguese");
  eeLangField->addItem("Russian");
  eeLangField->addItem("Simplified Chinese");
  eeLangField->addItem("Spanish");
  eeLangField->addItem("Turkish");
  eeLangField->addItem("Ukrainian");
  eeLangField->setCurrentIndex(2); // English
  connect(eeLangField, SIGNAL(currentIndexChanged(const QString&)),
          this, SLOT(handleEeLang(const QString&)));
  connect(this, SIGNAL(eeLang(const QString&)),
          coordinator, SIGNAL(eeLang(const QString&)));
  connect(coordinator, SIGNAL(eeLangRequest()),
          this, SLOT(eeLangRequest()));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(weiduLayout);
  mainLayout->addWidget(eeLangField);

  setLayout(mainLayout);

  emit doesWeiduExist();
}

void SettingsWindow::browseForWeidu()
{
  QString filter = WEIDU_EXECUTABLE;
  QString* selectedFilter = nullptr;
  QString path = QFileDialog::getOpenFileName(this, locateWeidu,
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
    emit installerAvailable(false);
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
  weiduLabel->setText(locateWeidu);
}

void SettingsWindow::weiduVersion(const int& version)
{
  weiduLabel->setText(tr("WeiDU version: ") + QString::number(version));
}

void SettingsWindow::weiduPath(const QString& path)
{
  weiduTextField->blockSignals(true);
  weiduTextField->setText(path);
  weiduTextField->blockSignals(false);
}

QString SettingsWindow::eeLangToDir(const QString& lang) const
{
  if (lang == "Czech") {
    return "cs_cz";
  } else if (lang == "German") {
    return "de_de";
  } else if (lang == "English") {
    return "en_us";
  } else if (lang == "French") {
    return "fr_fr";
  } else if (lang == "Italian") {
    return "it_it";
  } else if (lang == "Japanese") {
    return "ja_jp";
  } else if (lang == "Korean") {
    return "ko_kr";
  } else if (lang == "Polish") {
    return "pl_pl";
  } else if (lang == "Brazilian Portuguese") {
    return "pt_br";
  } else if (lang == "Russian") {
    return "ru_ru";
  } else if (lang == "Simplified Chinese") {
    return "zh_cn";
  } else if (lang == "Spanish") {
    return "es_es";
  } else if (lang == "Turkish") {
    return "tr_tr";
  } else if (lang == "Ukrainian") {
    return "uk_ua";
  } else {
    return "en_us";
  }
}

void SettingsWindow::handleEeLang(const QString& lang) const
{
  emit eeLang(eeLangToDir(lang));
}

void SettingsWindow::eeLangRequest() const
{
  handleEeLang(eeLangField->currentText());
}
