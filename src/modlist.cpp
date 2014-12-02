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

#include "modlist.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QListView>

ModList::ModList(const QString& title, QWidget* parent) : QWidget(parent)
{
  view = new QListView(this);
  QLabel* label = new QLabel(title, this);
  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(view);
  setLayout(layout);
}
