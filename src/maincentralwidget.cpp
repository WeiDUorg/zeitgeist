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

#include <QHBoxLayout>

#include "maincentralwidget.h"
#include "modlist.h"
#include "modtree.h"

MainCentralWidget::MainCentralWidget()
{
  availableMods = new ModList(tr("Available Mods"));
  queuedMods = new ModTree(tr("Queue"));
  installedMods = new ModTree(tr("Installed Mods"));

  QHBoxLayout* layout = new QHBoxLayout;
  layout->addWidget(availableMods);
  layout->addWidget(queuedMods);
  layout->addWidget(installedMods);

  setLayout(layout);
}
