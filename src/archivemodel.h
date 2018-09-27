#ifndef ARCHIVEMODEL_H
#define ARCHIVEMODEL_H

#include <QStringListModel>

class QObject;

class ArchiveModel : public QStringListModel
{
  Q_OBJECT

public:
  ArchiveModel(QObject* parent);

public slots:
  QStringList archiveNames() const;
  QStringList filePaths() const;
  void finalise();

private slots:
  void clear();
  void addFiles(const QStringList& files);
  void addDirs(const QStringList& dirs);

signals:
  void isEmpty(bool empty);
  void workingDir(const QString& dir);
  void ready();

private:
  void add(const QStringList& files, bool dir);
  void expand();

  QStringList names;
  QStringList paths;
  QStringList unexpanded;
};

#endif // ARCHIVEMODEL_H
