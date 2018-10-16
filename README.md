# Zeitgeist
A graphical frontend for WeiDU

## Build
These instructions are provided in the hopes they will be useful, but
I'm afraid I cannot account for all the variability you may
encounter. You may need to improvise or act on your own initiative.

To build Zeitgeist you need the following:

* A C++ compiler that supports C++11. Additionally, you'll need `make`
and the other components of a typical tool chain.

* Qt 5: I'm fairly certain it still builds on 5.8 and perhaps
something earlier still.
    - GNU/Linux: Hit up your package manager; you want the development
      package.
    - Windows: There is an installer available here:
      <https://www.qt.io/download> You can save yourself time and disk
      space by only choosing to install the files for the compiler you
      intend to use. I use MinGW and have had better success compiling
      with Creator than with `qmake` on the command line. Your mileage
      may vary.
    - macOS: You might want to use [Homebrew](https://brew.sh/). You want
      the development package.

* The [QuaZip](https://stachenov.github.io/quazip/) library
    - GNU/Linux: You want the development package compiled against Qt 5.
    - Windows: I think you'll have to compile the library
      yourself. You'll need zlib; I'd suggest getting something
      already compiled from
      [GnuWin32](http://gnuwin32.sourceforge.net/packages/zlib.htm). Refer
      to the build instructions for QuaZip itself.
    - macOS: [Homebrew](https://brew.sh/) seems to have this packaged, as
      well. But I don't know if it's the development package or not.

Before you can compile, you'll need to open the file zeitgeist.pro and
edit the library paths for QuaZip. The `INCLUDEPATH` path should point
to the directory containing the QuaZip header files and the `LIBS`
path should point to the QuaZip .SO/.DLL file.

At this point, you should, with a little luck, be able to build Zeitgeist using
either `qmake` to create a Makefile, followed by `make`, or by using
Qt Creator.

## Licence
GNU GPL, version 3 or any later version.
