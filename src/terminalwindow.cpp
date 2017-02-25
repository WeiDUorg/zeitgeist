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

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

TerminalWindow::TerminalWindow(QWidget* parent) :
  QWidget(parent)
{
  resize (640, 480);
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::WindowModal);
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(windowTitle);

  QPlainTextEdit* outputPane = new QPlainTextEdit(this);
  outputPane->setReadOnly(true);

  QLineEdit* inputLine = new QLineEdit(this);

  QPushButton* enterButton = new QPushButton(tr("Enter"), this);

  QHBoxLayout* inputLayout = new QHBoxLayout;
  inputLayout->addWidget(inputLine);
  inputLayout->addWidget(enterButton);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(outputPane);
  mainLayout->addLayout(inputLayout);

  setLayout(mainLayout);
}

TerminalWindow::~TerminalWindow()
{

}
