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
#include "zip.h"

#include <QDebug>
#include <QFileInfo>
#include <QIODevice>
#include <QString>
#include <QStringList>

#include <JlCompress.h>
#include <quazip.h>
#include <quazipfile.h>

bool Zip::write(const ArchiveModel* data, const QString& outFile)
{
  QuaZip zip(outFile);
  zip.setFileNameCodec("UTF-8");

  if (!zip.open(QuaZip::mdCreate)) return false;
  qDebug() << "ZIP archive opened";

  QStringList archiveNames = data->archiveNames();
  QStringList filePaths = data->filePaths();

  QFile inFile;
  QFileInfo info;
  QuaZipFile zipFile(&zip);

  for (int i = 0; i < filePaths.count(); i++) {
    QString filePath = filePaths.at(i);
    QString archiveName = archiveNames.at(i);

    info.setFile(filePath);
    if (!info.isFile()) continue;

    qDebug() << "Compressing" << filePath << "as" << archiveName;

    inFile.setFileName(filePath);
    if (!inFile.open(QIODevice::ReadOnly)) return false;

    if (!zipFile.open(QIODevice::WriteOnly,
                      QuaZipNewInfo(archiveName, filePath)))
      return false;

    if (!copyData(inFile, zipFile) || zipFile.getZipError() != UNZ_OK)
      return false;

    inFile.close();
    zipFile.close();
    if (zipFile.getZipError() != UNZ_OK) return false;
  }

  zip.close();
  if (zip.getZipError() != UNZ_OK) return false;
  qDebug() << "ZIP archive closed";

  return true;
}

bool Zip::extract(const QString& inFile, const QString& outDir)
{
  //QuaZip zip(inFile);
  //zip.setFileNameCodec("UTF-8");

  // should probably test if files exist first

  return !JlCompress::extractDir(inFile, outDir).isEmpty();
}

bool Zip::copyData(QIODevice& inFile, QIODevice& outFile)
{
  const qint64 maxLen = 4096;
  while (!inFile.atEnd()) {
    char buff[maxLen];
    qint64 len = inFile.read(buff, maxLen);
    if (len <= 0)
      return false;
    if (outFile.write(buff, len) != len)
      return false;
  }
  return true;
}
