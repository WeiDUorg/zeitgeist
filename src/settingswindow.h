#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class Coordinator;

class QLabel;
class QLineEdit;

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  SettingsWindow(QWidget* parent, const Coordinator* coordinator);

private slots:
  void browseForWeidu();
  void initialWeiduValidation(const QString& path);
  void weiduFailedValidation(const QString& weiduPath);
  void weiduVersion(const int& version);

signals:
  void weiduPassOff(const QString& weiduPath);

private:
  const Coordinator* coordinator;
  QLineEdit* weiduTextField;
  const QString locateWeidu = "Locate WeiDU";
  QLabel* weiduLabel;
};

#endif // SETTINGSWINDOW_H
