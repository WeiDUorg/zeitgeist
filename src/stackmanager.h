#ifndef STACKMANAGER_H
#define STACKMANAGER_H

#include <QList>
#include <QObject>
#include <QStack>

class WeiduLog;
struct WeiduLogComponent;

class StackManager : public QObject
{
  Q_OBJECT

public:
  /* Returns a sorted WeiduLog of mods to be uninstalled and modifies
   * the QStack to hold, in stack-order, a list of mods to be
   * reinstalled */
  static WeiduLog* uninstall(WeiduLog* toUninstall,
                             const WeiduLog* logFile,
                             QStack<QList<WeiduLogComponent>>& reinstall);

private:
  /* sort in reverse installation order */
  static WeiduLog* sortForUninstall(const WeiduLog* modList);

  static QStack<QStack<WeiduLogComponent>> toStack(const WeiduLog* logfile);
};

#endif // STACKMANAGER_H
