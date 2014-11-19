#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainCentralWidget;
class QLabel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();
  void createStatusBar();

 private:
  MainCentralWidget* centralWidget;
  QLabel* statusBarGameLabel;
  QLabel* statusBarCurrentGame;
};

#endif // MAINWINDOW_H
