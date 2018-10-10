#include "pushbuttonanimation.h"

PushButtonAnimation::PushButtonAnimation(QWidget* animating, QObject *parent) : Animation(animating, parent)
{
    anim = new tVariantAnimation;
    connect(anim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
        currentColor = value.value<QColor>();
        animatingWidget->update();
    });
    anim->setDuration(100);

    currentColor = animatingWidget->palette().color(QPalette::Button);
}

PushButtonAnimation::~PushButtonAnimation() {
    anim->stop();
    anim->deleteLater();
}

void PushButtonAnimation::setTargetColor(QColor col) {
    if (targetColor != col) {
        targetColor = col;
        anim->setStartValue(currentColor);
        anim->setEndValue(targetColor);
        anim->setCurrentTime(0);
        anim->start();
    }
}

QColor PushButtonAnimation::drawColor() {
    return anim->currentValue().value<QColor>();
}
