#ifndef ENQUEUEMODWINDOW_H
#define ENQUEUEMODWINDOW_H

#include <QTreeView>
#include <QWidget>

class Coordinator;
class EnqueueModModel;
class WeiduLog;

class QCheckBox;
class QItemSelection;
class QKeyEvent;
class QListView;
class QMouseEvent;
class QPushButton;
class QString;
class QStringListModel;
class QTreeView;

class ComponentListView : public QTreeView
{
  Q_OBJECT

public:
  ComponentListView(QWidget* parent,
                    EnqueueModModel* model);

protected:
  void mouseReleaseEvent(QMouseEvent* event);
  void keyReleaseEvent(QKeyEvent* event);

signals:
  void radioToggled(const QModelIndex& index);
};

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
  void handleSelectAll(int state);
  void componentsAvailable(bool available);
  void stateChanged(Qt::CheckState state);

signals:
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, int index);
  void enqueueComponents(const QString& modName, int lang);
  void resetModel();
  void select(bool select);

private:
  const Coordinator* coordinator;
  const QString windowTitle = tr("Enqueue mod");
  const QString tp2;

  QListView* languageListView;
  QStringListModel* languageListModel;
  ComponentListView* componentListView;
  QPushButton* proceedButton;
  QCheckBox* selectAll;
};

#endif // ENQUEUEMODWINDOW_H
