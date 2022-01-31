#include "viewitemanimation.h"

#include "tvariantanimation.h"

#include <QAbstractItemView>

struct ViewItemAnimationPrivate {
    tVariantAnimation* anim;
    QRect currentHoverRect;
    QRect targetHoverRect;
    bool isClear = true;
};

ViewItemAnimation::ViewItemAnimation(QWidget* animating, QObject* parent)
    : Animation(animating, parent) {
    d = new ViewItemAnimationPrivate();

    d->anim = new tVariantAnimation(this);
    connect(d->anim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentHoverRect = value.toRect();
    });
    connect(d->anim, SIGNAL(valueChanged(QVariant)), qobject_cast<QAbstractItemView*>(animatingWidget)->viewport(), SLOT(update()));
    connect(d->anim, &tVariantAnimation::finished, this, [ = ] {
        if (d->targetHoverRect.width() == 0) d->isClear = true;
    });
    d->anim->setDuration(50);
}

ViewItemAnimation::~ViewItemAnimation() {
    delete d;
}

void ViewItemAnimation::setHoverRect(QRect rect) {
    if (d->targetHoverRect != rect) {
        d->targetHoverRect = rect;
        d->anim->setStartValue(d->isClear ? clearRect(d->targetHoverRect) : d->currentHoverRect);
        d->anim->setEndValue(d->targetHoverRect);
        d->anim->setCurrentTime(0);
        d->anim->start();
        d->isClear = false;
    }
}

void ViewItemAnimation::clearHoverRect() {
    setHoverRect(clearRect(d->targetHoverRect));
}

QRect ViewItemAnimation::hoverRect() {
    return d->anim->currentValue().toRect();
}

QRect ViewItemAnimation::lastSetRect() {
    return d->targetHoverRect;
}

QRect ViewItemAnimation::clearRect(QRect rect) {
    QRect newRect = rect;
    newRect.setWidth(0);
    if (animatingWidget->layoutDirection() == Qt::RightToLeft) newRect.moveRight(animatingWidget->geometry().width());
    return newRect;
}
