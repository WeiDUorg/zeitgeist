#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QList>
#include <QWidget>

class QItemSelection;
class QItemSelectionModel;
class QListView;
class QModelIndex;
class QTreeView;

class Coordinator;
class AvailableModsModel;
class InstalledModsModel;

class MainCentralWidget : public QWidget
{
  Q_OBJECT

public:
  MainCentralWidget(QWidget* parent, const Coordinator* coordinator);

private slots:
  void handleInstalledSelection(const QItemSelection& selected, const QItemSelection& deselected);

private:
  const Coordinator* coordinator;
  QListView* availableModsView;
  QTreeView* queueView;
  QTreeView* installedModsView;
};

#endif // MAINCENTRALWIDGET_H
