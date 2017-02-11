#ifndef LOGFILE_H
#define LOGFILE_H

#include <QObject>
#include <QList>

class QString;

struct LogFileComponent
{
  QString modName;
  int language;
  int number;
  QString comment;
};

class LogFile : public QObject
{
  Q_OBJECT

public:
  LogFile(QObject* parent, const QString& gamePath);
  QList<LogFileComponent> data;

private:
  QList<LogFileComponent> parse(const QString& logFilePath) const;
  bool validLine(const QString& line) const;
  LogFileComponent parseLine(const QString& line) const;
};

#endif // LOGFILE_H
