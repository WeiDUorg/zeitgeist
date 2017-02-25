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

#include "terminalwindow.h"
#include "controller.h"
#include "coordinator.h"

#include <QByteArray>
#include <QDebug>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>

TerminalWindow::TerminalWindow(QWidget* parent,
                               const Coordinator* coordinator) :
  QWidget(parent), coordinator(coordinator)
{
  resize (640, 480);
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(windowTitle);

  outputPane = new QPlainTextEdit(this);
  outputPane->setReadOnly(true);

  inputLine = new QLineEdit(this);

  QPushButton* enterButton = new QPushButton(tr("Enter"), this);

  QHBoxLayout* inputLayout = new QHBoxLayout;
  inputLayout->addWidget(inputLine);
  inputLayout->addWidget(enterButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(outputPane);
  mainLayout->addLayout(inputLayout);

  setLayout(mainLayout);

  connect(coordinator->controller, SIGNAL(processOutput(const QString&)),
          this, SLOT(processOutput(const QString&)));
  connect(enterButton, SIGNAL(clicked()),
          this, SLOT(generateInput()));
  connect(this, SIGNAL(processInput(const QString&)),
          coordinator->controller, SIGNAL(processInput(const QString&)));
}

TerminalWindow::~TerminalWindow()
{

}

void TerminalWindow::processOutput(const QString& text)
{
  outputPane->appendPlainText(text);
}

void TerminalWindow::generateInput()
{
  const QString text = inputLine->text();
  if (!text.isEmpty()) {
    inputLine->clear();
    qDebug() << "Emitting process input" << text;
    emit processInput(text);
  }
}
