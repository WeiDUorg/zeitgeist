#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <QWidget>

class Coordinator;

class QLineEdit;
class QPlainTextEdit;
class QString;

class TerminalWindow : public QWidget
{
  Q_OBJECT

public:
  TerminalWindow(QWidget* parent, const Coordinator* coordinator);
  ~TerminalWindow();

private slots:
  void processOutput(const QString& text);
  void generateInput();

signals:
  void processInput(const QString& text);

private:
  const Coordinator* coordinator;
  QString windowTitle = "Terminal";
  QPlainTextEdit* outputPane;
  QLineEdit* inputLine;
};

#endif // TERMINALWINDOW_H
