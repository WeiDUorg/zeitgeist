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
  MainCentralWidget(QWidget* parent, const DataManager* dataManager);

private slots:
  void handleInstalledSelection(const QItemSelection& selected, const QItemSelection& deselected);

private:
  const DataManager* dataManager;
  QListView* availableModsView;
  QTreeView* queueView;
  QTreeView* installedModsView;
};

#endif // MAINCENTRALWIDGET_H
