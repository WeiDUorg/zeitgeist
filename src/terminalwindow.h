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

protected:
  void closeEvent(QCloseEvent* event);

private slots:
  void processOutput(const QString& text);
  void generateInput();
  void installTaskEnded();

signals:
  void processInput(const QString& text);

private:
  const Coordinator* coordinator;
  QString windowTitle = "Terminal";
  bool safeExit;
  QPlainTextEdit* outputPane;
  QLineEdit* inputLine;
};

#endif // TERMINALWINDOW_H
