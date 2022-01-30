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
#include "focusdecoration.h"

#include <QEvent>
#include <QPainter>
#include <tvariantanimation.h>

struct FocusDecorationPrivate {
    QWidget* focusWidget = nullptr;
    bool allowVisible = false;
    bool shouldAnimate = true;

    tVariantAnimation* colourPulse;
    tVariantAnimation* geometryAnim;

    QMetaObject::Connection oldWindowDestroyEvent;
};

FocusDecoration::FocusDecoration(QWidget* parent) : QWidget(parent) {
    this->setAttribute(Qt::WA_TransparentForMouseEvents);

    d = new FocusDecorationPrivate();

    d->colourPulse = new tVariantAnimation(this);
    d->colourPulse->setForceAnimation(true);
    d->colourPulse->setStartValue(QColor(0, 100, 255));
    d->colourPulse->setEndValue(QColor(0, 200, 255));
    d->colourPulse->setDuration(500);
    connect(d->colourPulse, &tVariantAnimation::valueChanged, this, [ = ] {
        this->update();
    });
    connect(d->colourPulse, &tVariantAnimation::finished, this, [ = ] {
        if (d->colourPulse->direction() == tVariantAnimation::Forward) {
            d->colourPulse->setDirection(tVariantAnimation::Backward);
        } else {
            d->colourPulse->setDirection(tVariantAnimation::Forward);
        }
        d->colourPulse->start();
    });
    d->colourPulse->start();

    d->geometryAnim = new tVariantAnimation(this);
    d->geometryAnim->setDuration(100);
    d->geometryAnim->setEasingCurve(QEasingCurve::OutCubic);
    connect(d->geometryAnim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        this->setGeometry(value.toRect());
    });
}

FocusDecoration::~FocusDecoration() {
    delete d;
}

void FocusDecoration::setWidget(QWidget* widget) {
    if (d->focusWidget) {
        d->focusWidget->removeEventFilter(this);
    }

    d->focusWidget = widget;

    if (widget) {
        d->focusWidget->installEventFilter(this);

        if (d->allowVisible) this->setVisible(true);

        this->updateParent();
    } else {
        this->setVisible(false);
    }
}

void FocusDecoration::setAllowVisible(bool allowVisible) {
    d->allowVisible = allowVisible;
    if (!allowVisible && this->isVisible()) this->setVisible(false);
}

void FocusDecoration::updatePosition() {
    if (!d->focusWidget) return;

    this->raise();

    QRect decorationGeometry;
    if (d->focusWidget->dynamicPropertyNames().contains("X-Contemporary-FocusDecorationGeometry")) {
        decorationGeometry = d->focusWidget->property("X-Contemporary-FocusDecorationGeometry").toRect();
    } else {
        decorationGeometry = d->focusWidget->geometry();
        decorationGeometry.moveTo(0, 0);
    }

    decorationGeometry.moveTo(d->focusWidget->mapToGlobal(decorationGeometry.topLeft()));
    decorationGeometry.moveTo(d->focusWidget->window()->mapFromGlobal(decorationGeometry.topLeft()));

    decorationGeometry.adjust(SC_DPI(-5), SC_DPI(-5), SC_DPI(5), SC_DPI(5));

    d->geometryAnim->stop();
    d->geometryAnim->setStartValue(this->geometry());
    d->geometryAnim->setEndValue(decorationGeometry);

    if (d->shouldAnimate) {
        d->geometryAnim->start();
    } else {
        d->shouldAnimate = true;
        d->geometryAnim->setCurrentTime(d->geometryAnim->duration());
    }
}

void FocusDecoration::updateParent() {
    if (!d->focusWidget) return;

    if (this->parent() != d->focusWidget->window()) d->shouldAnimate = false;
    this->setParent(d->focusWidget->window());

    disconnect(d->oldWindowDestroyEvent);
    d->oldWindowDestroyEvent = connect(d->focusWidget->window(), &QWidget::destroyed, this, [ = ] {
        disconnect(d->oldWindowDestroyEvent);
        this->setParent(nullptr);
    });

    this->updatePosition();
}

bool FocusDecoration::eventFilter(QObject* watched, QEvent* event) {
    switch (event->type()) {
        case QEvent::Resize:
        case QEvent::Move:
            this->updatePosition();
            break;
        case QEvent::ParentChange:
            this->updateParent();
            break;
        case QEvent::DynamicPropertyChange: {
            QDynamicPropertyChangeEvent* e = static_cast<QDynamicPropertyChangeEvent*>(event);
            if (e->propertyName() == "X-Contemporary-FocusDecorationGeometry") this->updatePosition();
        }
    }

    return false;
}

void FocusDecoration::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    painter.setBrush(d->colourPulse->currentValue().value<QColor>());
    painter.setPen(Qt::transparent);
    painter.drawRect(0, 0, this->width(), SC_DPI(5));
    painter.drawRect(0, 0, SC_DPI(5), this->height());
    painter.drawRect(0, this->height() - SC_DPI(5), this->width(), SC_DPI(5));
    painter.drawRect(this->width() - SC_DPI(5), 0, SC_DPI(5), this->height());
}
