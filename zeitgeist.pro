QT += core gui widgets

TARGET = zeitgeist
TEMPLATE = app

CONFIG += c++11

artifacts_dir = tmp
MOC_DIR = $$artifacts_dir
OBJECTS_DIR = $$artifacts_dir

include(src.pri)
