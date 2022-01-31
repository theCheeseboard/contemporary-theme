#include "menuitemanimation.h"

#include <tvariantanimation.h>

struct MenuItemAnimationPrivate {
    tVariantAnimation* anim;
    QRect currentHoverRect;
    QRect targetHoverRect;
    bool isClear = true;
};

MenuItemAnimation::MenuItemAnimation(QWidget* animating, QObject* parent)
    : Animation(animating, parent) {
    d = new MenuItemAnimationPrivate();

    d->anim = new tVariantAnimation(this);
    connect(d->anim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentHoverRect = value.toRect();
    });
    connect(d->anim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    connect(d->anim, &tVariantAnimation::finished, this, [ = ] {
        if (d->targetHoverRect.width() == 0) d->isClear = true;
    });
    d->anim->setDuration(50);
}

MenuItemAnimation::~MenuItemAnimation() {
    delete d;
}

void MenuItemAnimation::setHoverRect(QRect rect) {
    if (d->targetHoverRect != rect) {
        d->targetHoverRect = rect;
        d->anim->setStartValue(d->isClear ? clearRect(d->targetHoverRect) : d->currentHoverRect);
        d->anim->setEndValue(d->targetHoverRect);
        d->anim->setCurrentTime(0);
        d->anim->start();
        d->isClear = false;
    }
}

void MenuItemAnimation::clearHoverRect() {
    setHoverRect(clearRect(d->targetHoverRect));
}

QRect MenuItemAnimation::hoverRect() {
    return d->anim->currentValue().toRect();
}

QRect MenuItemAnimation::lastSetRect() {
    return d->targetHoverRect;
}

QRect MenuItemAnimation::clearRect(QRect rect) {
    QRect newRect = rect;
    newRect.setWidth(0);
    if (animatingWidget->layoutDirection() == Qt::RightToLeft) newRect.moveRight(animatingWidget->geometry().width());
    return newRect;
}
