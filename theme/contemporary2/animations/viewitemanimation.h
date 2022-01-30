#ifndef VIEWITEMANIMATION_H
#define VIEWITEMANIMATION_H

#include "animation.h"

struct ViewItemAnimationPrivate;
class ViewItemAnimation : public Animation {
        Q_OBJECT
    public:
        explicit ViewItemAnimation(QWidget* animating, QObject* parent = nullptr);
        ~ViewItemAnimation();

        void setHoverRect(QRect rect);
        void clearHoverRect();
        QRect hoverRect();
        QRect lastSetRect();

    signals:

    private:
        ViewItemAnimationPrivate* d;

        QRect clearRect(QRect rect);
};

#endif // VIEWITEMANIMATION_H
