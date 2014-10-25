QT += core gui widgets

TARGET = zeitgeist
TEMPLATE = app

artifacts_dir = tmp
MOC_DIR = $$artifacts_dir
OBJECTS_DIR = $$artifacts_dir

include(src.pri)
