#include "animation.h"

Animation::Animation(QWidget* animating, QObject *parent) : QObject(parent)
{
    this->animatingWidget = animating;
}

