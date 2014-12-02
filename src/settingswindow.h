#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class DataManager;
class QLineEdit;

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  SettingsWindow(const DataManager* dataManager, QWidget* parent = 0);

private slots:
  void browseForWeidu();
  void initialWeiduValidation(const QString& path);

private:
  const DataManager* dataManager;
  QLineEdit* weiduTextField;
};

#endif // SETTINGSWINDOW_H
