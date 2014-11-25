#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class DataManager;

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  SettingsWindow(DataManager* dataManager, QWidget* parent = 0);

private:
  DataManager* dataManager;
};

#endif // SETTINGSWINDOW_H
