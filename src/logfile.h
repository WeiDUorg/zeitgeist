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
  LogFile(QString gamePath, QObject* parent = 0);

private:
  QList<LogFileComponent> parse(QString logFilePath);
  bool validLine(QString line);
  LogFileComponent parseLine(QString line);
  QList<LogFileComponent> data;
};

#endif // LOGFILE_H
