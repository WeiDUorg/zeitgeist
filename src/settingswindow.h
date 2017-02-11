#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class Coordinator;
class QLineEdit;

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  SettingsWindow(const Coordinator* coordinator, QWidget* parent = 0);

private slots:
  void browseForWeidu();
  void initialWeiduValidation(const QString& path);
  void weiduFailedValidation(const QString& weiduPath);

signals:
  void weiduPassOff(const QString& weiduPath);

private:
  const Coordinator* coordinator;
  QLineEdit* weiduTextField;
};

#endif // SETTINGSWINDOW_H
