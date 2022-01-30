#include "pushbuttonanimation.h"

#include <QAbstractButton>
#include "tvariantanimation.h"

struct PushButtonAnimationPrivate {
    tVariantAnimation* anim;
    QColor targetColor;
    QColor currentColor;
};

PushButtonAnimation::PushButtonAnimation(QWidget* animating, QObject* parent) : Animation(animating, parent) {
    d = new PushButtonAnimationPrivate();

    d->anim = new tVariantAnimation(this);
    connect(d->anim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentColor = value.value<QColor>();
    });
    connect(d->anim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    d->anim->setDuration(100);

    d->currentColor = animatingWidget->palette().color(QPalette::Button);
}

PushButtonAnimation::~PushButtonAnimation() {
    d->anim->stop();
    d->anim->deleteLater();

    delete d;
}

void PushButtonAnimation::setTargetColor(QColor col) {
    if (d->targetColor != col) {
        d->targetColor = col;
        d->anim->setStartValue(d->currentColor);
        d->anim->setEndValue(d->targetColor);
        d->anim->setCurrentTime(0);
        d->anim->start();
    }
}

QColor PushButtonAnimation::drawColor() {
    return d->anim->currentValue().value<QColor>();
}
