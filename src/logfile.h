#ifndef LOGFILE_H
#define LOFFILE_H

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
  LogFile(const QString& gamePath, QObject* parent = 0);

private:
  QList<LogFileComponent> parse(const QString& logFilePath) const;
  bool validLine(const QString& line) const;
  LogFileComponent parseLine(const QString& line) const;
  QList<LogFileComponent> data;
};

#endif // LOGFILE_H
