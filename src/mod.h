#ifndef MOD_H
#define MOD_H

#include <QObject>

class QString;

class Mod : public QObject
{
  Q_OBJECT

public:
  Mod(QObject* parent, const QString& tp2Path);
  QString getName() const;

  const QString tp2Path;

private:
  QString name;
};

#endif // MOD_H
