#ifndef WEIDUMANAGER_H
#define WEIDUMANAGER_H

#include <QObject>

class QByteArray;
class QProcess;
class QString;
class QStringList;

class WeiduManager : public QObject
{
  Q_OBJECT

public:
  WeiduManager(const QString& weiduPath);
  bool executable() const;

private slots:
  void terminateManager();
  void quack();
  void newGamePath(const QString& path);

  void version();
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, const int& index);

signals:
  void quacks(const bool& quacks);
  /* Emitted when a game path is needed but this has none */
  void requestGamePath();

  void versionSignal(const int& version);
  void languageList(const QStringList& languageList);
  void componentList();

private:
  QByteArray run(const QStringList& arguments);

  const QString weiduPath;
  QString gamePath;
  QProcess* process;
};

#endif // WEIDUMANAGER_H
