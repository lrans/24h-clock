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

#include "ClockRenderer.h"
#include "sunriset.h"
#include "ClockDebug.h"

#include <QPainterPath>

class TwentyFour : public ClockRenderer
{
public:
    TwentyFour() : ClockRenderer("24H") {}
    virtual void paintDialPlate(QPainter* aPainter, const QSize& aSize,
        ClockTheme* aTheme, bool aDrawBackground);
    virtual void paintHourMinHands(QPainter* aPainter, const QSize& aSize,
        const QTime& aTime, ClockTheme* aTheme);
    virtual void paintSecHand(QPainter* aPainter, const QSize& aSize,
        const QTime& aTime, ClockTheme* aTheme);
    static void paintHand(QPainter* aPainter, const QRectF& aRect,
        qreal aAngle, const QBrush& aBrush,
        qreal aX = 0.0, qreal aY = 0.0);
};

ClockRenderer*
ClockRenderer::new24H()
{
    return new TwentyFour;
}

void
TwentyFour::paintDialPlate(
    QPainter* aPainter,
    const QSize& aSize,
    ClockTheme* aTheme,
    bool aDrawBackground)
{
    const qreal w = aSize.width();
    const qreal h = aSize.height();
    const qreal d = qMin(w, h);
    const qreal r1 = d / 2;
    const qreal r2 = r1 - qMax(qreal(1), qreal(d / 98));
    const qreal r3 = d / 16.25;

    aPainter->save();
    aPainter->translate(w/2, h/2);

    // Draw the dial plate
    QPointF center(0,0);
    if (aDrawBackground) {
        aPainter->setPen(Qt::NoPen);
        aPainter->setBrush(QBrush(aTheme->iBackgroundColor2));
        aPainter->drawEllipse(center, r1, r1);
        aPainter->setBrush(QBrush(aTheme->iBackgroundColor));
        aPainter->drawEllipse(center, r2, r2);
        aPainter->setBrush(QBrush(aTheme->iBackgroundColor1));
        aPainter->drawEllipse(center, r3, r3);
    }

    // Draw the sunrise/set
    //qreal sunrise = 30;
    //qreal sunset = 120;

    //sun_rise_set(year,month,day,lon,lat,rise,set)
    // versailles : lat=48.807N, lon=2.137E
    double rise, set;
    QDate today = QDate::currentDate();

    int sunriset = sun_rise_set(today.year(), today.month(), today.day(), 2.137, 48.807, &rise, &set);

    QTRACE("- sunriset : " << sunriset << " rise=" << rise << " set=" << set);

    int SECS_IN_A_DAY = 3600 * 24;

    qreal sunrise = 270 - ((SECS_IN_A_DAY - rise * 3600) / SECS_IN_A_DAY) * 360;
    qreal sunset = 270 - ((SECS_IN_A_DAY - set * 3600) / SECS_IN_A_DAY) * 360;

    qreal sunrisetRatio = .7;
    aPainter->setPen(Qt::NoPen);

    // draw day
    aPainter->setBrush(QBrush(aTheme->iDayColor));
    aPainter->drawPie(-w*sunrisetRatio/2, -h*sunrisetRatio/2, w*sunrisetRatio, h*sunrisetRatio, sunrise * 16, (sunset - sunrise) * 16);

    // draw night
    aPainter->setBrush(QBrush(aTheme->iNightColor));
    aPainter->drawPie(-w*sunrisetRatio/2, -h*sunrisetRatio/2, w*sunrisetRatio, h*sunrisetRatio, sunset * 16, (360 - (sunset - sunrise)) * 16);

    const qreal y = d / 84;
    const qreal y1 = qMax(qreal(1), qreal(d / 195));
    const qreal x1 = d / 2.71;
    const qreal x2 = d / 2.25;
    const qreal x3 = d / 2.1;
    QRectF hourMarkRect(x1, -y, (x3-x1), 2*y);
    QRectF minMarkRect(x2, -y1, (x3-x2), 2*y1);
    QBrush markBrush(aTheme->iHourMinHandColor);

    aPainter->setPen(aTheme->iHourMinHandColor);
    /*for (int i=0; i<60; i++) {
        if (i % 5) {
            aPainter->fillRect(minMarkRect, markBrush);
        } else {
            aPainter->fillRect(hourMarkRect, markBrush);
        }
        aPainter->rotate(6.0);
    }*/
    for (int i=0; i<24; i++) {
        if (i % 6) {
            aPainter->fillRect(minMarkRect, markBrush);
        } else {
            aPainter->fillRect(hourMarkRect, markBrush);
        }
        aPainter->rotate(360/24);
    }
    aPainter->restore();
}

void
TwentyFour::paintHand(
    QPainter* aPainter,
    const QRectF& aRect,
    qreal aAngle,
    const QBrush& aBrush,
    qreal aX,
    qreal aY)
{
    const qreal x = aRect.right() - aRect.height() / 3.0;

    QPainterPath path;
    path.moveTo(aRect.left(), aRect.bottom());
    path.lineTo(aRect.left(), aRect.top());
    path.lineTo(x, aRect.top());
    path.lineTo(aRect.right(), (aRect.top() + aRect.bottom())/2.0);
    path.lineTo(x, aRect.bottom());
    path.closeSubpath();

    aPainter->save();
    aPainter->translate(aX,aY);
    aPainter->rotate(aAngle);
    aPainter->setPen(Qt::NoPen);
    aPainter->setBrush(aBrush);
    aPainter->drawPath(path);
    aPainter->restore();
}

void
TwentyFour::paintHourMinHands(
    QPainter* aPainter,
    const QSize& aSize,
    const QTime& aTime,
    ClockTheme* aTheme)
{
    // Draw hands
    const qreal d = qMin(aSize.width(),aSize.height());
    const qreal x = d / 2 - qMax(qreal(1), qreal(d / 98));
    const qreal yh = d / 40;
    const qreal ym = d / 60;
    const qreal xh1 = -(d / 8.6);
    const qreal xh2 = d / 3.4;
    const qreal xm1 = xh1;
    const qreal xm2 = qMin(qreal(d / 2.03), x - 2);
    QRectF hourHandRect(xh1, -yh, (xh2-xh1), 2*yh);
    QRectF minHandRect(xm1, -ym, (xm2-xm1), 2*ym);
    QBrush handBrush(aTheme->iHourMinHandColor);
    QBrush shadowBrush1(aTheme->iHandShadowColor1);
    QBrush shadowBrush2(aTheme->iHandShadowColor2);

    qreal hourAngle = 15.0 * (aTime.hour() +
        (aTime.minute() + aTime.second()/60.0)/60.0) + 90;
    qreal minAngle= 6.0 * (aTime.minute() +
        (aTime.second() + aTime.msec()/1000.0)/60.0) - 90;

    QRectF rect(QPoint(0,0), aSize);
    aPainter->save();
    aPainter->translate(rect.center());
    aPainter->setPen(aTheme->iHourMinHandColor);
    paintHand(aPainter, hourHandRect, hourAngle, handBrush);
    paintHand(aPainter, minHandRect, minAngle, shadowBrush2, -1, -1);
    paintHand(aPainter, minHandRect, minAngle, shadowBrush2, 2, 2);
    paintHand(aPainter, minHandRect, minAngle, shadowBrush1, 1, 1);
    paintHand(aPainter, minHandRect, minAngle, handBrush);
    aPainter->restore();
}

void
TwentyFour::paintSecHand(
    QPainter* aPainter,
    const QSize& aSize,
    const QTime& aTime,
    ClockTheme* aTheme)
{
    const qreal w = aSize.width();
    const qreal h = aSize.height();
    const qreal d = qMin(w, h);

    const qreal y = qMax(qreal(1), qreal(d / 195));
    const qreal x1 = -(d / 6.97);
    const qreal x2 = d / 2;
    const qreal r = d / 23;

    QPainterPath path;
    path.moveTo(x1, 0);
    path.lineTo(x1+y, -y);
    path.lineTo(x2-y, -y);
    path.lineTo(x2, 0);
    path.lineTo(x2-y, y);
    path.lineTo(x1+y, y);
    path.closeSubpath();

    QBrush secBrush(aTheme->iSecondHandColor);
    QPointF center(0,0);

    aPainter->save();
    aPainter->setPen(Qt::NoPen);
    aPainter->setBrush(secBrush);
    aPainter->translate(w/2, h/2);
    aPainter->rotate(6.0 * (aTime.second() + aTime.msec()/1000.0) - 90);
    aPainter->drawPath(path);
    aPainter->drawEllipse(center, r, r);
    aPainter->setBrush(QBrush(Qt::white));
    aPainter->drawEllipse(center, 2, 2);
    aPainter->setBrush(QBrush(Qt::black));
    aPainter->drawEllipse(center, 1, 1);
    aPainter->restore();
}
