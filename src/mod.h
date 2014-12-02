#ifndef MOD_H
#define MOD_H

#include <QObject>

class QString;

class Mod : public QObject
{
  Q_OBJECT

public:
  Mod(const QString& tp2Path, QObject* parent = 0);
  QString getName() const;

private:
  const QString tp2Path;
  QString name;
};

#endif // MOD_H
