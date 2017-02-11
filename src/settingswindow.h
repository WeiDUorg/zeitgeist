#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class Coordinator;
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

signals:
  void weiduPassOff(const QString& weiduPath);

private:
  const Coordinator* coordinator;
  QLineEdit* weiduTextField;
};

#endif // SETTINGSWINDOW_H
