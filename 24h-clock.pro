TARGET = 24h-clock
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
    translations/*.ts \
    24h-clock.desktop \
    rpm/24h-clock.spec \
    rpm/24h-clock.changes

CONFIG += sailfishapp_i18n
TRANSLATIONS += \
    translations/24h-clock.ts \
    translations/24h-clock-ru.ts
