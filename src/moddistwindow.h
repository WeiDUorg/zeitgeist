#ifndef MODDISTWINDOW_H
#define MODDISTWINDOW_H

#include <QWidget>

class ArchiveModel;
class Coordinator;

class QLineEdit;
class QListView;
class QPushButton;
class QString;
class QStringList;
class QStringListModel;

class ModDistWindow : public QWidget
{
  Q_OBJECT

public:
  ModDistWindow(QWidget* parent, const Coordinator* corrdinator);
  ~ModDistWindow();

private slots:
  void openFilesDialog();
  void openDirDialog();
  void selectTargetName();
  void hasData(bool empty);
  void workingDir(const QString& dir);
  void modDistArchiveSuccess(bool success);

signals:
  void createDist(const QString& targetName);
  void clearModel();
  void addFiles(const QStringList& files);
  void addDirs(const QStringList& dirs);

private:
  QPushButton* createButton;
  QString targetName;

  QListView* filesView;
  QPushButton* filesButton;
  QPushButton* dirButton;
  QString currentDir;

  // QListView* presetsView;

  const QString createButtonEnabled =
    tr("Create distributable archive");
  const QString createButtonDisabled =
    tr("Add some files or directories first");

};

#endif // MODDISTWINDOW_H
