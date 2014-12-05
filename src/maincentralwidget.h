#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QWidget>

class QListView;
class QTreeView;

class DataManager;
class AvailableModsModel;

class MainCentralWidget : public QWidget
{
  Q_OBJECT

public:
  MainCentralWidget(const DataManager* dataManager, QWidget* parent = 0);

private:
  const DataManager* dataManager;
  QListView* availableModsView;
  QTreeView* queueView;
  QTreeView* installedModsView;
  AvailableModsModel* availableModsModel;
};

#endif // MAINCENTRALWIDGET_H
