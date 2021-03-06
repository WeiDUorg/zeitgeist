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

#include "mainwindow.h"
#include "coordinator.h"
#include "platform.h"

#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QIODevice>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationDisplayName("Zeitgeist");

    QFile stylesheet(":/stylesheet.qss");
    if (stylesheet.open(QIODevice::ReadOnly | QIODevice::Text)) {
      app.setStyleSheet(stylesheet.readAll());
      stylesheet.close();
    }

    Coordinator* coordinator = new Coordinator();

    MainWindow w(coordinator);

    coordinator->restoreState();
    coordinator->initWeidu(QCoreApplication::applicationDirPath() +
                           "/" + WEIDU_EXECUTABLE);

    w.show();

    return app.exec();
}
