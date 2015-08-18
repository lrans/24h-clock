/*
  Copyright (C) 2014 Jolla Ltd.
  Contact: Slava Monich <slava.monich@jolla.com>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "QuickClock.h"
#include "ClockSettings.h"
#include "SystemState.h"
#include "ClockDebug.h"

#include <QtGui>
#include <QtQuick>
#include <sailfishapp.h>

void registerClockTypes(const char* uri, int v1 = 1, int v2 = 0)
{
    qmlRegisterType<QuickClock>(uri, v1, v2, "Clock");
    qmlRegisterType<ClockSettings>(uri, v1, v2, "ClockSettings");
    qmlRegisterType<SystemState>(uri, v1, v2, "SystemState");
}

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    int result = 0;
    QGuiApplication* app = SailfishApp::application(argc, argv);

    QLocale locale;
    QTranslator* translator = new QTranslator(app);
    QString transDir = SailfishApp::pathTo("translations").toLocalFile();
    QString transFile("24h-clock");
    if (translator->load(locale, transFile, "-", transDir) ||
        translator->load(transFile, transDir)) {
        app->installTranslator(translator);
    } else {
        QDEBUG("Failed to load translator for" << locale);
        delete translator;
    }

    registerClockTypes("twentyfour.clock", 1, 0);

    QQuickView *view = SailfishApp::createView();
    view->setSource(SailfishApp::pathTo(QString("qml/main.qml")));
    view->show();

    result = app->exec();

    delete view;
    delete app;
    return result;
}
