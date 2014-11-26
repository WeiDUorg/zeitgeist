#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class DataManager;
class QLineEdit;

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  SettingsWindow(DataManager* dataManager, QWidget* parent = 0);

private slots:
  void browseForWeidu();
  void initialWeiduValidation(const QString& path);

private:
  DataManager* dataManager;
  QLineEdit* weiduTextField;
};

#endif // SETTINGSWINDOW_H
