#ifndef ENQUEUEMODWINDOW_H
#define ENQUEUEMODWINDOW_H

#include <QWidget>

class Coordinator;

class QItemSelection;
class QListWidget;
class QListView;
class QString;
class QStringListModel;

class EnqueueModWindow : public QWidget
{
  Q_OBJECT

public:
  EnqueueModWindow(QWidget* parent, const Coordinator* coordinator, const QString& tp2);
  ~EnqueueModWindow();

private slots:
  void languageList(const QStringList& list);
  void handleLanguageSelection(const QItemSelection& selected, const QItemSelection&);

signals:
  void getLanguageList(const QString& tp2);
  void getComponentList(const QString& tp2, const int& index);

private:
  const Coordinator* coordinator;
  const QString windowTitle = tr("Enqueue mod");
  const QString tp2;

  QListView* languageListView;
  QStringListModel* languageListModel;
  QListWidget* componentListView;
};

#endif // ENQUEUEMODWINDOW_H
