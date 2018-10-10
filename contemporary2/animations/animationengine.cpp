#include "animationengine.h"

#include <QPushButton>

AnimationPair::AnimationPair() {

}

AnimationEngine::AnimationEngine(QObject *parent) : QObject(parent)
{

}

void AnimationEngine::registerWidget(QWidget *widget) {
    if (qobject_cast<QPushButton*>(widget)) {
        AnimationPair pair;
        pair.widget = widget;
        pair.anim = new PushButtonAnimation(widget);

        connect(widget, &QWidget::destroyed, [=] {
            this->deregisterWidget(widget);
        });

        animations.append(pair);
    }
}

void AnimationEngine::deregisterWidget(QWidget *widget) {
    for (AnimationPair a : animations) {
        if (a.widget == widget) {
            a.anim->deleteLater();
            animations.removeOne(a);
            return;
        }
    }
}

Animation* AnimationEngine::a(const QWidget *widget) {
    for (AnimationPair a : animations) {
        if (a.widget == widget) return a.anim;
    }
    return nullptr;

    /*
    if (animations.contains(widget)) {
        return animations.value(widget);
    } else {
        return nullptr;
    }*/
}
