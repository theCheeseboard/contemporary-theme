#ifndef CHECKBOXANIMATION_H
#define CHECKBOXANIMATION_H

#include "animation.h"

struct CheckboxAnimationPrivate;
class CheckboxAnimation : public Animation {
        Q_OBJECT
    public:
        explicit CheckboxAnimation(QWidget* animating, QObject* parent = nullptr);
        ~CheckboxAnimation();

        void fastForward();

        void setIndicatorRect(QRectF indicator);
        void setCheckState(Qt::CheckState checkState);

        QRectF currentIndicatorRect();
        QPolygonF currentCheckPolygon();

    signals:

    private:
        CheckboxAnimationPrivate* d;

        void setIndicatorInnerRect(QRectF indicator);
        void setIndicatorOn();
        void setIndicatorOff();

        void setCheckTarget(qreal target);
};

#endif // CHECKBOXANIMATION_H
