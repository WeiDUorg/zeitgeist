#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QList>
#include <QWidget>

class AvailableModsModel;
class Coordinator;
class InstalledModsModel;
class WeiduLog;

class QItemSelection;
class QItemSelectionModel;
class QListView;
class QModelIndex;
class QTreeView;

class MainCentralWidget : public QWidget
{
  Q_OBJECT

public:
  MainCentralWidget(QWidget* parent, const Coordinator* coordinator);

private slots:
  void handleInstalledSelection(const QItemSelection& selected, const QItemSelection& deselected);
  void handleAvailableSelection(const QItemSelection& selected, const QItemSelection&);
  void getSelectedAvailableMod();
  void getSelectedInstalledMods();
  void clearInstalledSelection();

signals:
  void availableModSelected(const bool& selected);
  void selectedAvailableMod(const QString&);
  void installedModSelected(const bool& selected);
  void selectedInstalledMods(WeiduLog* selection);

private:
  const Coordinator* coordinator;
  QListView* availableModsView;
  QTreeView* installQueueView;
  QTreeView* uninstallQueueView;
  QTreeView* installedModsView;
};

#endif // MAINCENTRALWIDGET_H
