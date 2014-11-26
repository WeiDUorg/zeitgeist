#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef Q_OS_LINUX
#define WEIDU_EXECUTABLE "weidu"
#elif Q_OS_OSX
#define WEIDU_EXECUTABLE "weidu"
#elif Q_OS_WIN
#define WEIDU_EXECUTABLE "weidu.exe"
#endif

#endif // PLATFORM_H
