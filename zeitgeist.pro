TARGET = zeitgeist
TEMPLATE = app

# QMake's debug/release build mechanics are complete BS. So, in order
# to build a release build, you need to remove the debug option and
# maybe add the release option (depending on the defaults of your Qt
# distribution).

CONFIG += c++11 debug

CONFIG(debug, debug|release) {
message ("debug build")
BUILD_DIR = build/debug
} else {
message ("release build")
DEFINES += QT_NO_DEBUG_OUTPUT
BUILD_DIR = build/release
}

QT += core gui widgets

RESOURCES += resources/resources.qrc

QMAKE_CXXFLAGS += \
    -Wextra \
    -Werror \
    -pedantic \
    -Wold-style-cast \
    -Wfloat-equal \
    -Wcast-qual \
    -Wcast-align \
    -Wuninitialized \
    -Winit-self \
    -Woverloaded-virtual \

DESTDIR = $$BUILD_DIR
MOC_DIR = $$BUILD_DIR
OBJECTS_DIR = $$BUILD_DIR
RCC_DIR = $$BUILD_DIR

include(src.pri)

OUTPUT_FILE = $$quote($$DESTDIR/$$TARGET$$TARGET_EXT)
win32:OUTPUT_FILE ~= s,/,\\,g

QMAKE_POST_LINK += $$QMAKE_COPY $$OUTPUT_FILE "."
