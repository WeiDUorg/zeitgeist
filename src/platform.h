#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef Q_OS_LINUX
#define WEIDU_EXECUTABLE "weidu"
#elif defined(Q_OS_MACOS)
#define WEIDU_EXECUTABLE "weidu"
#elif defined(Q_OS_WIN)
#define WEIDU_EXECUTABLE "weidu.exe"
#endif

#endif // PLATFORM_H
