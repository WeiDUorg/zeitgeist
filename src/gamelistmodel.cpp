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

#include "gamelistmodel.h"

#include <algorithm>
#include <QDebug>
#include <QDirIterator>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QStandardItem>
#include <QString>
#include <QStringList>
#include <QVariant>

GameListModel::GameListModel(QObject* parent) :
  QStandardItemModel(parent)
{
  QStringList headerLabels;
  headerLabels << tr("Game Name") << tr("Game Location") << tr("Game Language");
  setHorizontalHeaderLabels(headerLabels);
  connect(this, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex)),
          this, SLOT(handleDataChanged(const QModelIndex&, const QModelIndex)));
}

void GameListModel::addRow(const QString& gameName, const QString& gamePath,
                           const GameType& gameType, const QString& eeLang)
{
  QList<QStandardItem*> row;
  QStandardItem* pathItem = new QStandardItem(gamePath);
  pathItem->setData(QVariant(stringOfType(gameType)), GAME_TYPE);
  QStringList dirs = langDirs(gamePath);
  QStringList langs;
  foreach (QString dir, dirs) {
    langs << eeDirToLang(dir);
  }
  QStandardItem* langItem = new QStandardItem();
  if (!langs.isEmpty()) {
    langItem->setData(QVariant(langs), LANG_LIST);
    if (langs.contains(eeLang)) {
      langItem->setData(QVariant(eeLang), Qt::EditRole);
    }
  }
  row << new QStandardItem(gameName) << pathItem << langItem;
  appendRow(row);
}

void GameListModel::addGame(const QString& path)
{
  QString gamePath = findKeyFileDirectory(path);
  if (gamePath.isEmpty()) {
    emit notAGameDirectory(path);
  } else if (!duplicate(gamePath)) {
    GameType gameType = fingerprintGameDirectory(gamePath);
    QString gameName = prettyPrintGameType(gameType);
    addRow(gameName, gamePath, gameType, eeDirToLang("en_us"));
  }
}

void GameListModel::removeGame(const QModelIndex& index)
{
  int row = index.row();
  if (index.isValid() && row < rowCount()) {
    emit gameRemoved(pathOfIndex(index));
    removeRows(row, COLUMN_PATH);
  }
}

QList<GameListDataEntry> GameListModel::exportData() const
{
  int rows = rowCount();
  QList<GameListDataEntry> result;
  result.reserve(rows);
  for (int i = 0; i < rows; ++i) {
    GameListDataEntry row;
    row.name = item(i, COLUMN_NAME)->text();
    row.path = item(i, COLUMN_PATH)->text();
    QString lang;
    if (eeGame(row.path)) {
      lang = eeLangToDir(item(i, COLUMN_LANG)->data(Qt::EditRole).toString());
    }
    row.lang = lang;
    result.append(row);
  }
  return result;
}

void GameListModel::importData(const QList<GameListDataEntry>& dataList)
{
  int rows = dataList.length();
  for (int i = 0; i < rows; ++i) {
    GameListDataEntry row = dataList.at(i);
    QDir dir(row.path);
    if (validGame(row.path)) {
      GameType type = fingerprintGameDirectory(row.path);
      QString lang = eeDirToLang(row.lang);
      addRow(row.name, row.path, type, lang);
    }
  }
}

QString GameListModel::pathOfIndex(const QModelIndex& index) const
{
  int row = index.row();
  if (index.isValid() && row < rowCount()) {
    return item(row, COLUMN_PATH)->text();
  }
  return QString();
}

QString GameListModel::identifierOfPath(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, COLUMN_PATH)->text()) == 0) {
      return item(i, COLUMN_NAME)->text();
    }
  }
  qDebug() << "Could not find an identity for path" << path;
  return QString();
}

/*
 * Return the directory which contains the file chitin.key
 * and (dialog.tlk or lang/en_us/dialog.tlk). The matching
 * against dialog.tlk (or equivalent) is necessary to avoid
 * false positives on chitin.key files located in backup
 * directories.
 */
QString GameListModel::findKeyFileDirectory(const QString& path) const
{
  QString chitinkey("chitin.key");
  QString dialogtlk("dialog.tlk");
  QString dialogtlkee("lang/en_us/" + dialogtlk);
  QStringList nameFilter(chitinkey);
  QDirIterator iterator(path, nameFilter, QDir::Files,
                        QDirIterator::Subdirectories);
  while (iterator.hasNext()) {
    iterator.next();
    QFileInfo info = iterator.fileInfo();
    QDir dir = info.dir();
    if (dir.exists(dialogtlk) ||
        dir.exists(dialogtlkee)) {
      return dir.absolutePath();
    }
  }
  return QString();
}

/*
 * Determine the generic game type of a game directory by
 * looking for characteristic files.
 */
GameType GameListModel::fingerprintGameDirectory(const QString& path) const
{
  QDir dir(path);
  if (dir.exists("bgmain2.exe")) {
    return GameType::BG;
  } else if (dir.exists("baldur.exe") && dir.exists("bgconfig.exe")) {
    return GameType::BG2;
  } else if (dir.exists("idmain.exe")) {
    return GameType::IWD;
  } else if (dir.exists("iwd2.exe")) {
    return GameType::IWD2;
  } else if (dir.exists("torment.exe")) {
    return GameType::PST;
  } else if (dir.exists("movies/bgenter.wbm")) {
    return GameType::BGEE;
  } else if (dir.exists("movies/melissan.wbm")) {
    return GameType::BG2EE;
  } else if (dir.exists("movies/howseer.wbm")) {
    return GameType::IWDEE;
  }
  return GameType::UNKNOWN;
}

QString GameListModel::prettyPrintGameType(const GameType& type) const
{
  switch (type) {
  case GameType::BG:
    return QString("Baldur's Gate");
  case GameType::BG2:
    return QString("Baldur's Gate II");
  case GameType::IWD:
    return QString("Icewind Dale");
  case GameType::IWD2:
    return QString("Icewind Dale II");
  case GameType::PST:
    return QString("Planescape: Torment");
  case GameType::BGEE:
    return QString("Baldur's Gate: Enhanced Edition");
  case GameType::BG2EE:
    return QString("Baldur's Gate II: Enhanced Edition");
  case GameType::IWDEE:
    return QString("Icewind Dale: Enhanced Edition");
  default: // GameType::UNKNOWN
    return QString("IE game");
  }
}

bool GameListModel::duplicate(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, COLUMN_PATH)->text()) == 0) {
      return true;
    }
  }
  return false;
}

//QMetaEnum (4.8) seems to lack a typeOfString equivalent so I ain't bothering
QString GameListModel::stringOfType(const GameType& type) const
{
  switch (type) {
  case GameType::BG:
    return "BG";
  case GameType::BG2:
    return "BG2";
  case GameType::IWD:
    return "IWD";
  case GameType::IWD2:
    return "IWD2";
  case GameType::PST:
    return "PST";
  case GameType::BGEE:
    return "BGEE";
  case GameType::BG2EE:
    return "BG2EE";
  case GameType::IWDEE:
    return "IWDEE";
  default: // GameType::UNKNOWN
    return "UNKNOWN";
  }
}

GameType GameListModel::typeOfString(const QString& name) const
{
  if (name == "BG") {
    return GameType::BG;
  } else if (name == "BG2") {
    return GameType::BG2;
  } else if (name == "IWD") {
    return GameType::IWD;
  } else if (name == "IWD2") {
    return GameType::IWD2;
  } else if (name == "PST") {
    return GameType::PST;
  } else if (name == "BGEE") {
    return GameType::BGEE;
  } else if (name == "BG2EE") {
    return GameType::BG2EE;
  } else if (name == "IWDEE") {
    return GameType::IWDEE;
  } else {
    return GameType::UNKNOWN;
  }
}

bool GameListModel::eeGame(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, COLUMN_PATH)->text()) == 0) {
      QStandardItem* pathItem = item(i, COLUMN_PATH);
      GameType type = typeOfString(pathItem->data(GAME_TYPE).toString());
      switch (type) {
      case GameType::BGEE:
      case GameType::BG2EE:
      case GameType::IWDEE:
        return true;
      default:
        return false;
      }
    }
  }
  return false;
}

QString GameListModel::eeLang(const QString& path) const
{
  for (int i = 0; i < rowCount(); ++i) {
    if (path.compare(item(i, COLUMN_PATH)->text()) == 0) {
      return eeLangToDir(item(i, COLUMN_LANG)->data(Qt::EditRole).toString());
    }
  }
  return QString();
}

bool GameListModel::validGame(const QString& path) const
{
  return QDir(path).exists() && !findKeyFileDirectory(path).isEmpty();
}

QStringList GameListModel::langDirs(const QString& gamePath) const
{
  QStringList result;
  if (QDir(gamePath).exists("lang")) {
    QDirIterator iterator(gamePath + "/lang", QDir::AllDirs);
    QRegExp langPattern("^[a-zA-Z]{2,2}_[a-zA-Z]{2,2}$");
    while (iterator.hasNext()) {
      iterator.next();
      QString dirName = iterator.fileName();
      if (langPattern.exactMatch(dirName)) {
        result << dirName;
      }
    }
  }
  std::sort(result.begin(), result.end(),
            [](const QString& l, const QString& r) {
              return l < r;
            });
  return result;
}

QVariant GameListModel::data(const QModelIndex& index, int role) const
{
  if (index.column() == COLUMN_LANG && role == Qt::DisplayRole) {
    return item(index.row(), COLUMN_LANG)->data(LANG_LIST);
  } else {
    return item(index.row(), index.column())->data(role);
  }
}

void GameListModel::handleDataChanged(const QModelIndex& topLeft,
                                      const QModelIndex& bottomRight)
{
  if (topLeft.column() == COLUMN_LANG && bottomRight.column() == COLUMN_LANG
   && topLeft.row() == bottomRight.row()) {
    QStandardItem* langItem = item(topLeft.row(), COLUMN_LANG);
    QString lang = eeLangToDir(langItem->data(Qt::EditRole).toString());
    qDebug() << "Emitting EE lang" << lang;
    emit eeLangSignal(lang);
  }
}

QString GameListModel::eeLangToDir(const QString& lang) const
{
  if (lang.compare("Czech", Qt::CaseInsensitive) == 0) {
    return "cs_cz";
  } else if (lang.compare("German", Qt::CaseInsensitive) == 0) {
    return "de_de";
  } else if (lang.compare("English", Qt::CaseInsensitive) == 0) {
    return "en_us";
  } else if (lang.compare("French", Qt::CaseInsensitive) == 0) {
    return "fr_fr";
  } else if (lang.compare("Italian", Qt::CaseInsensitive) == 0) {
    return "it_it";
  } else if (lang.compare("Japanese", Qt::CaseInsensitive) == 0) {
    return "ja_jp";
  } else if (lang.compare("Korean", Qt::CaseInsensitive) == 0) {
    return "ko_kr";
  } else if (lang.compare("Polish", Qt::CaseInsensitive) == 0) {
    return "pl_pl";
  } else if (lang.compare("Brazilian Portuguese", Qt::CaseInsensitive) == 0) {
    return "pt_br";
  } else if (lang.compare("Russian", Qt::CaseInsensitive) == 0) {
    return "ru_ru";
  } else if (lang.compare("Simplified Chinese", Qt::CaseInsensitive) == 0) {
    return "zh_cn";
  } else if (lang.compare("Spanish", Qt::CaseInsensitive) == 0) {
    return "es_es";
  } else if (lang.compare("Turkish", Qt::CaseInsensitive) == 0) {
    return "tr_tr";
  } else if (lang.compare("Ukrainian", Qt::CaseInsensitive) == 0) {
    return "uk_ua";
  } else {
    return "en_us";
  }
}

QString GameListModel::eeDirToLang(const QString& dir) const
{
  if (dir.compare("cs_cz", Qt::CaseInsensitive) == 0) {
    return "Czech";
  } else if (dir.compare("de_de", Qt::CaseInsensitive) == 0) {
    return "German";
  } else if (dir.compare("en_us", Qt::CaseInsensitive) == 0) {
    return "English";
  } else if (dir.compare("fr_fr", Qt::CaseInsensitive) == 0) {
    return "French";
  } else if (dir.compare("it_it", Qt::CaseInsensitive) == 0) {
    return "Italian";
  } else if (dir.compare("ja_jp", Qt::CaseInsensitive) == 0) {
    return "Japanese";
  } else if (dir.compare("ko_kr", Qt::CaseInsensitive) == 0) {
    return "Korean";
  } else if (dir.compare("pl_pl", Qt::CaseInsensitive) == 0) {
    return "Polish";
  } else if (dir.compare("pt_br", Qt::CaseInsensitive) == 0) {
    return "Brazilian Portuguese";
  } else if (dir.compare("ru_ru", Qt::CaseInsensitive) == 0) {
    return "Russian";
  } else if (dir.compare("zh_cn", Qt::CaseInsensitive) == 0) {
    return "Simplified Chinese";
  } else if (dir.compare("es_es", Qt::CaseInsensitive) == 0) {
    return "Spanish";
  } else if (dir.compare("tr_tr", Qt::CaseInsensitive) == 0) {
    return "Turkish";
  } else if (dir.compare("uk_ua", Qt::CaseInsensitive) == 0) {
    return "Ukrainian";
  } else {
    return "English";
  }
}
