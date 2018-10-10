#ifndef PUSHBUTTONANIMATION_H
#define PUSHBUTTONANIMATION_H

#include <QObject>
#include "animation.h"
#include "tvariantanimation.h"

class PushButtonAnimation : public Animation
{
        Q_OBJECT
    public:
        explicit PushButtonAnimation(QWidget* animating, QObject *parent = nullptr);
        ~PushButtonAnimation();

        QColor drawColor();
    signals:

    public slots:
        void setTargetColor(QColor col);

    private:
        tVariantAnimation* anim;
        QColor targetColor;
        QColor currentColor;
};

#endif // PUSHBUTTONANIMATION_H
