#ifndef ENQUEUEMODWINDOW_H
#define ENQUEUEMODWINDOW_H

#include <QWidget>

class Coordinator;
class WeiduLog;

class QItemSelection;
class QListView;
class QPushButton;
class QString;
class QStringListModel;
class QTreeView;

class EnqueueModWindow : public QWidget
{
  Q_OBJECT

public:
  EnqueueModWindow(QWidget* parent, const Coordinator* coordinator,
                   const QString& tp2);
  ~EnqueueModWindow();

private slots:
  void languageList(const QStringList& list);
  void handleLanguageSelection(const QItemSelection& selected,
                               const QItemSelection&);
  void handleProceed();

signals:
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, const int& index);
  void enqueueComponents(const QString& modName, int lang);
  void resetModel();

private:
  const Coordinator* coordinator;
  const QString windowTitle = tr("Enqueue mod");
  const QString tp2;

  QListView* languageListView;
  QStringListModel* languageListModel;
  QTreeView* componentListView;
  QPushButton* proceedButton;
};

#endif // ENQUEUEMODWINDOW_H
