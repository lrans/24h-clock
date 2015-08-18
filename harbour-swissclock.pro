TARGET = harbour-swissclock
CONFIG += sailfishapp
CONFIG += link_pkgconfig
PKGCONFIG += mlite5 sailfishapp
QMAKE_CXXFLAGS += -Wno-unused-parameter
QT += dbus positioning

CONFIG(debug, debug|release) {
  DEFINES += CLOCK_DEBUG=1
}

SOURCES += \
    src/main.cpp \
    src/ClockRenderer.cpp \
    src/ClockRendererHelsinkiMetro.cpp \
    src/ClockRendererSwissRailroad.cpp \
    src/ClockRenderer24H.cpp \
    src/ClockSettings.cpp \
    src/ClockTheme.cpp \
    src/QuickClock.cpp \
    src/SystemState.cpp \
    src/sunriset.cpp

HEADERS += \
    src/ClockDebug.h \
    src/ClockRenderer.h \
    src/ClockSettings.h \
    src/ClockTheme.h \
    src/QuickClock.h \
    src/SystemState.h \
    src/sunriset.h

OTHER_FILES += \
    qml/*.qml \
    qml/*.png \
    harbour-swissclock.desktop \
    translations/*.ts \
    rpm/harbour-swissclock.changes \
    rpm/harbour-swissclock.spec

CONFIG += sailfishapp_i18n
TRANSLATIONS += \
    translations/harbour-swissclock.ts \
    translations/harbour-swissclock-ru.ts
