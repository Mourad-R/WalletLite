# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

QT += core network xml widgets gui
QT += sql
QT += charts
QT += webenginewidgets

TARGET = "Achain Wallet Lite"
TEMPLATE = app

DEFINES += QT_WIDGETS_LIB QT_XML_LIB QT_NETWORK_LIB QT_NO_FOREACH

LIBS += $$PWD/lib/Mac/libqrencode.a
ICON = achain.icns

UI_DIR += ./GeneratedFiles
include(WalletLite.pri)
include(BlockChain.pri)

QMAKE_LFLAGS += -framework CoreGraphics