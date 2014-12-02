#ifndef LOGFILE_H
#define LOFFILE_H

#include <QObject>
#include <QList>

class QString;

struct LogFileComponent
{
  QString name;
  int language;
  int number;
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
