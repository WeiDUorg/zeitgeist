#ifndef MAINCENTRALWIDGET_H
#define MAINCENTRALWIDGET_H

#include <QList>
#include <QModelIndex>
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
  void handleInstalledSelection(const QItemSelection& selected,
                                const QItemSelection& deselected);
  void handleAvailableSelection(const QItemSelection& selected,
                                const QItemSelection&);
  void handleInstallQueueSelection(const QItemSelection& selected,
                                   const QItemSelection& deselected);
  void handleUninstallQueueSelection(const QItemSelection& selected,
                                     const QItemSelection& deselected);
  void handleQueueAvailability(const QModelIndex&, int, int);
  void getSelectedAvailableMod();
  void clearAvailableSelection();
  void getSelectedInstalledMods();
  void clearInstalledSelection();
  void getSelectedQueuedMods();
  void clearQueuedSelection();

signals:
  void availableModSelected(bool selected);
  void selectedAvailableMod(const QString&);
  void installedModSelected(bool selected);
  void selectedInstalledMods(WeiduLog* selection);
  void queuedModSelected(bool selected);
  void selectedInstallQueuedMods(const QModelIndexList& indices);
  void selectedUninstallQueuedMods(const QModelIndexList& indices);
  void queuedModAvailable(bool available);

private:
  void handleTreeSelection(QItemSelectionModel* selectionModel,
                           const QItemSelection& selected,
                           const QItemSelection& deselected);
  const Coordinator* coordinator;
  QListView* availableModsView;
  QTreeView* installQueueView;
  QTreeView* uninstallQueueView;
  QTreeView* installedModsView;
};

#endif // MAINCENTRALWIDGET_H
