/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2020 Victor Tran
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
#include "focusdecorationcontroller.h"

#include <QApplication>
#include "focusdecoration.h"

struct FocusDecorationControllerPrivate {
    QApplication* app;
    FocusDecoration* decorationWidget;

    bool lastInputWasKeyboard = false;
};

FocusDecorationController::FocusDecorationController(QObject* parent) : QObject(parent) {
    d = new FocusDecorationControllerPrivate();
    d->decorationWidget = new FocusDecoration();
}

FocusDecorationController::~FocusDecorationController() {
    d->decorationWidget->deleteLater();
    delete d;
}

void FocusDecorationController::setApplication(QApplication* application) {
    if (d->app != nullptr) clearApplication();
    d->app = application;

    d->app->installEventFilter(this);
    connect(d->app, &QApplication::focusChanged, this, &FocusDecorationController::focusChanged);
}

QApplication* FocusDecorationController::application() {
    return d->app;
}

void FocusDecorationController::clearApplication() {
    d->app->removeEventFilter(this);
    disconnect(d->app, &QApplication::focusChanged, this, &FocusDecorationController::focusChanged);
    d->app = nullptr;
}

void FocusDecorationController::focusChanged(QWidget* oldWidget, QWidget* newWidget) {
    d->decorationWidget->setWidget(newWidget);
}

bool FocusDecorationController::eventFilter(QObject* watched, QEvent* event) {
    switch (event->type()) {
        case QEvent::KeyPress:
        case QEvent::KeyRelease:
            d->lastInputWasKeyboard = true;
            this->updateAllowVisible();
            break;
        case QEvent::MouseMove:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::TouchBegin:
        case QEvent::TouchUpdate:
        case QEvent::TouchEnd:
            d->lastInputWasKeyboard = false;
            this->updateAllowVisible();
            break;
        case QEvent::DynamicPropertyChange:
            if (watched == d->app) {
                this->updateAllowVisible();
            }
            break;
        default:
            break;
    }

    return false;
}

void FocusDecorationController::updateAllowVisible() {
    bool allowVisible = true;
    if (!d->lastInputWasKeyboard) allowVisible = false;
    if (d->app->property("X-Contemporary-FocusDecorationOff").toBool()) allowVisible = false;
    d->decorationWidget->setAllowVisible(allowVisible);
}
