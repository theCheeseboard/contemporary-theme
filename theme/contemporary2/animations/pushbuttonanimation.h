#ifndef PUSHBUTTONANIMATION_H
#define PUSHBUTTONANIMATION_H

#include "animation.h"

struct PushButtonAnimationPrivate;
class PushButtonAnimation : public Animation {
        Q_OBJECT
    public:
        explicit PushButtonAnimation(QWidget* animating, QObject* parent = nullptr);
        ~PushButtonAnimation();

        void setTargetColor(QColor col);
        QColor drawColor();

    signals:

    private:
        PushButtonAnimationPrivate* d;
};

#endif // PUSHBUTTONANIMATION_H
