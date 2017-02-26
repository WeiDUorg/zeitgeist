#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>

class Coordinator;

class QComboBox;
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
  void weiduPath(const QString& path);
  void handleEeLang(const QString& lang) const;
  void eeLangRequest() const;

signals:
  void weiduPassOff(const QString& weiduPath);
  void doesWeiduExist();
  void eeLang(const QString& langDir) const;
  void installerAvailable(const bool& available);

private:
  QString eeLangToDir(const QString& lang) const;

  const Coordinator* coordinator;
  QLineEdit* weiduTextField;
  const QString locateWeidu = "Locate WeiDU";
  QLabel* weiduLabel;
  QComboBox* eeLangField;
};

#endif // SETTINGSWINDOW_H
