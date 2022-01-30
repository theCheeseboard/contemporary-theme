#ifndef MENUITEMANIMATION_H
#define MENUITEMANIMATION_H

#include "animation.h"

struct MenuItemAnimationPrivate;
class MenuItemAnimation : public Animation {
        Q_OBJECT
    public:
        explicit MenuItemAnimation(QWidget* animating, QObject* parent = nullptr);
        ~MenuItemAnimation();

        QRect clearRect(QRect rect);

        void setHoverRect(QRect rect);
        void clearHoverRect();
        QRect hoverRect();
        QRect lastSetRect();

    signals:

    private:
        MenuItemAnimationPrivate* d;
};

#endif // MENUITEMANIMATION_H
