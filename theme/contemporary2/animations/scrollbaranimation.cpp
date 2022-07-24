/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2022 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "scrollbaranimation.h"

#include <QTimer>
#include <tvariantanimation.h>

struct ScrollBarAnimationPrivate {
        tVariantAnimation* opacity;
        tVariantAnimation* widthScaling;
        QTimer* defocusTimer;

        double targetOpacity = 0.3;
        double currentOpacity = 0.3;

        double targetWidthScaling = 1;
        double currentWidthScaling = 1;

        int currentValue = 0;
        bool hasMouseFocus = false;
};

ScrollBarAnimation::ScrollBarAnimation(QWidget* animating, QObject* parent) :
    Animation{animating, parent} {
    d = new ScrollBarAnimationPrivate();

    d->opacity = new tVariantAnimation(this);
    d->opacity->setStartValue(1.0);
    d->opacity->setEndValue(1.0);
    connect(d->opacity, &tVariantAnimation::valueChanged, this, [=](QVariant value) {
        d->currentOpacity = value.toDouble();
        if (animatingWidget) animatingWidget->update();
    });
    d->opacity->setDuration(100);

    d->widthScaling = new tVariantAnimation(this);
    d->widthScaling->setStartValue(1.0);
    d->widthScaling->setEndValue(1.0);
    connect(d->widthScaling, &tVariantAnimation::valueChanged, this, [=](QVariant value) {
        d->currentWidthScaling = value.toDouble();
        if (animatingWidget) animatingWidget->update();
    });
    d->widthScaling->setDuration(100);

    d->defocusTimer = new QTimer(this);
    d->defocusTimer->setInterval(1000);
    d->defocusTimer->setSingleShot(true);
    connect(d->defocusTimer, &QTimer::timeout, this, &ScrollBarAnimation::defocus);

    defocusLater();
}

ScrollBarAnimation::~ScrollBarAnimation() {
    delete d;
}

void ScrollBarAnimation::focus() {
    d->defocusTimer->stop();
    setTargetOpacity(1);
    setTargetWidthScaling(1);
}

void ScrollBarAnimation::defocus() {
    setTargetOpacity(0.3);
    setTargetWidthScaling(0.5);
    d->defocusTimer->stop();
}

void ScrollBarAnimation::defocusLater() {
    d->defocusTimer->start();
}

void ScrollBarAnimation::setTargetOpacity(double opacity) {
    if (d->targetOpacity != opacity) {
        d->targetOpacity = opacity;
        d->opacity->setStartValue(d->currentOpacity);
        d->opacity->setEndValue(d->targetOpacity);
        d->opacity->setCurrentTime(0);
        d->opacity->start();
    }
}

double ScrollBarAnimation::opacity() {
    return d->opacity->currentValue().toDouble();
}

void ScrollBarAnimation::setTargetWidthScaling(double widthScaling) {
    if (d->targetWidthScaling != widthScaling) {
        d->targetWidthScaling = widthScaling;
        d->widthScaling->setStartValue(d->currentWidthScaling);
        d->widthScaling->setEndValue(d->targetWidthScaling);
        d->widthScaling->setCurrentTime(0);
        d->widthScaling->start();
    }
}

double ScrollBarAnimation::widthScaling() {
    return d->widthScaling->currentValue().toDouble();
}

void ScrollBarAnimation::setCurrentValue(int value) {
    if (d->currentValue != value) {
        d->currentValue = value;
        focus();
        if (!d->hasMouseFocus) defocusLater();
    }
}

void ScrollBarAnimation::setHasMouseFocus(bool hasMouseFocus) {
    if (d->hasMouseFocus != hasMouseFocus) {
        d->hasMouseFocus = hasMouseFocus;
        if (hasMouseFocus) {
            focus();
        } else {
            defocusLater();
        }
    }
}
