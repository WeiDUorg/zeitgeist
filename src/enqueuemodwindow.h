#ifndef ENQUEUEMODWINDOW_H
#define ENQUEUEMODWINDOW_H

#include <QWidget>

class Coordinator;

class QListWidget;
class QListView;
class QString;

class EnqueueModWindow : public QWidget
{
  Q_OBJECT

public:
  EnqueueModWindow(QWidget* parent, const Coordinator* coordinator, const QString& tp2);
  ~EnqueueModWindow();

private:
  const Coordinator* coordinator;
  const QString windowTitle = tr("Enqueue mod");
  const QString tp2;

  QListView* languageListView;
  QListWidget* componentListView;
};

#endif // ENQUEUEMODWINDOW_H
