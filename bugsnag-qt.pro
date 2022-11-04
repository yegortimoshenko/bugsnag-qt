QT += network

TARGET = bugsnag-qt
TEMPLATE = lib

DEFINES += BUGSNAGQT_LIBRARY
SOURCES += bugsnag.cpp
HEADERS += bugsnag.h bugsnag-qt_global.h

headers.files = $$HEADERS

unix {
  headers.path = $$PREFIX/include
  target.path = $$PREFIX/lib

  INSTALLS += headers target

  QMAKE_PKGCONFIG_NAME = $$TARGET
  QMAKE_PKGCONFIG_DESCRIPTION = Bugsnag client library for Qt 5
  QMAKE_PKGCONFIG_PREFIX = $$PREFIX
  QMAKE_PKGCONFIG_LIBDIR = $$target.path
  QMAKE_PKGCONFIG_INCDIR = $$headers.path
  QMAKE_PKGCONFIG_VERSION = 20180522.005732
  QMAKE_PKGCONFIG_DESTDIR = pkgconfig

  CONFIG += create_pc create_prl
}
