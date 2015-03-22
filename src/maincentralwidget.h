#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QList>
#include <QWidget>

class QItemSelection;
class QItemSelectionModel;
class QListView;
class QModelIndex;
class QTreeView;

class DataManager;
class AvailableModsModel;
class InstalledModsModel;

class MainCentralWidget : public QWidget
{
  Q_OBJECT

public:
  MainCentralWidget(const DataManager* dataManager, QWidget* parent = 0);

private slots:
  void handleInstalledSelection(const QItemSelection& selected, const QItemSelection& deselected);

private:
  const DataManager* dataManager;
  QListView* availableModsView;
  QTreeView* queueView;
  QTreeView* installedModsView;
  AvailableModsModel* availableModsModel;
  InstalledModsModel* installedModsModel;
};

#endif // MAINCENTRALWIDGET_H
