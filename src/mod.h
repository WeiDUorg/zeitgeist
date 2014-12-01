#ifndef MOD_H
#define MOD_H

#include <QObject>

class QString;

class Mod : public QObject
{
  Q_OBJECT

public:
  Mod(QString tp2Path, QObject* parent = 0);

private:
  const QString tp2Path;
  QString modName;
};

#endif // MOD_H
