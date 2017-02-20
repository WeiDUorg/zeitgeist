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

/* Should maybe be split into a reader class and a data class; data should be const */

class LogFile : public QObject
{
  Q_OBJECT

public:
  LogFile(QObject* parent, const QString& gamePath);
  QList<LogFileComponent> getData() const;

private:
  QList<LogFileComponent> parse(const QString& logFilePath) const;
  bool validLine(const QString& line) const;
  LogFileComponent parseLine(const QString& line) const;
  QList<LogFileComponent> data;
};

#endif // LOGFILE_H
