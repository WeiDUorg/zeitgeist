#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainCentralWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

 private:
  MainCentralWidget* centralWidget;
};

#endif // MAINWINDOW_H
