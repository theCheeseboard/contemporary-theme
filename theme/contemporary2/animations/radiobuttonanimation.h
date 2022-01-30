#ifndef RADIOBUTTONANIMATION_H
#define RADIOBUTTONANIMATION_H

#include "animation.h"

struct RadioButtonAnimationPrivate;
class RadioButtonAnimation : public Animation {
        Q_OBJECT
    public:
        explicit RadioButtonAnimation(QWidget* animating, QObject* parent = nullptr);
        ~RadioButtonAnimation();

        void setIndicatorRect(QRectF rect);
        QRectF currentIndicatorRect();

    signals:

    private:
        RadioButtonAnimationPrivate* d;
};

#endif // RADIOBUTTONANIMATION_H
