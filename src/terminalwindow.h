#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QWidget>

class QString;

class TerminalWindow : public QWidget
{
  Q_OBJECT

public:
  TerminalWindow(QWidget* parent);
  ~TerminalWindow();

private:
  QString windowTitle = "Terminal";
};

#endif // TERMINALWINDOW_H
