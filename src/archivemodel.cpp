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

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QFileInfo>
#include <QString>
#include <QStringList>

ArchiveModel::ArchiveModel(QObject* parent) :
  QStringListModel(parent)
{

}

QStringList ArchiveModel::archiveNames() const
{
  return names;
}

QStringList ArchiveModel::filePaths() const
{
  return paths;
}

void ArchiveModel::clear()
{
  beginResetModel();
  removeRows(0, rowCount());
  endResetModel();
  names.clear();
  paths.clear();
  unexpanded.clear();
  emit isEmpty(true);
}

void ArchiveModel::add(const QStringList& files, bool dir)
{
  QStringList tmp = stringList();
  foreach (QString file, files) {
    if (QFileInfo::exists(file)) {
      QFileInfo info(file);
      QString path;
      if (dir) {
        path = info.canonicalPath();
      } else {
        path = info.canonicalFilePath();
      }
      QString name = info.fileName();
      if (dir) {
        name += "/";
      }
      qDebug() << "Adding file" << name <<
        "in path" << path;
      names << name;
      tmp << name;
      paths << path;
      emit workingDir(path);
      if (dir) {
        qDebug() << "Adding" << file << "to unexpanded";
        unexpanded << file;
      }
    }
  }
  setStringList(tmp);
  emit isEmpty(tmp.isEmpty());
}

void ArchiveModel::addFiles(const QStringList& files)
{
  add(files, false);
}

void ArchiveModel::addDirs(const QStringList& dirs)
{
  add(dirs, true);
}

void ArchiveModel::finalise()
{
  expand();
  emit ready();
}

void ArchiveModel::expand()
{
  foreach (QString dir, unexpanded) {
    qDebug() << "Expanding" << dir << "into:";
    QString root = QFileInfo(dir).fileName();
    QDir::Filters f = QDir::AllEntries | QDir::NoDotAndDotDot;
    QDirIterator i(dir, f, QDirIterator::Subdirectories);
    while (i.hasNext()) {
      QFileInfo info(i.next());
      QString expanded = root + "/" +
        info.filePath().remove(0, dir.length() + 1);
      qDebug() << expanded;
      names << expanded;
      paths << info.filePath();
    }
  }
  qDebug() << "Expansion complete";
}
