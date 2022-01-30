#include "radiobuttonanimation.h"

#include <tvariantanimation.h>

struct RadioButtonAnimationPrivate {
    tVariantAnimation* anim;
    QRectF currentHoverRect;
    QRectF targetHoverRect;
};

RadioButtonAnimation::RadioButtonAnimation(QWidget* animating, QObject* parent)
    : Animation(animating, parent) {
    d = new RadioButtonAnimationPrivate();

    d->anim = new tVariantAnimation(this);
    connect(d->anim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentHoverRect = value.toRect();
    });
    connect(d->anim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    d->anim->setDuration(250);
}

RadioButtonAnimation::~RadioButtonAnimation() {
    delete d;
}

void RadioButtonAnimation::setIndicatorRect(QRectF rect) {
    if (d->targetHoverRect != rect) {
        d->targetHoverRect = rect;
        d->anim->setStartValue(d->currentHoverRect);
        d->anim->setEndValue(d->targetHoverRect);
        d->anim->setCurrentTime(0);
        d->anim->start();
    }
}

QRectF RadioButtonAnimation::currentIndicatorRect() {
    return d->anim->currentValue().toRectF();
}
