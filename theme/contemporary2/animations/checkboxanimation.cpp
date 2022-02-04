#include "checkboxanimation.h"

#include <tvariantanimation.h>

struct CheckboxAnimationPrivate {
    tVariantAnimation* backgroundAnim;
    QRectF currentHoverRect;
    QRectF targetHoverRect;

    tVariantAnimation* checkAnim;
    qreal currentCheckProgress = -1;
    qreal targetCheckProgress = -1;

    QRectF indicatorRect;

    Qt::CheckState currentCheckState = Qt::Unchecked;

    std::function<void()> afterAnim = nullptr;

    bool isBackgroundSetUp = false;
    bool isCheckSetUp = false;
    bool drawIndeterminatePoly = false;
};

CheckboxAnimation::CheckboxAnimation(QWidget* animating, QObject* parent)
    : Animation(animating) {
    d = new CheckboxAnimationPrivate();

    d->backgroundAnim = new tVariantAnimation(this);
    connect(d->backgroundAnim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentHoverRect = value.toRect();
    });
    connect(d->backgroundAnim, &tVariantAnimation::finished, this, [ = ] {
        if (d->afterAnim) {
            d->afterAnim();
            d->afterAnim = nullptr;
        }
    });
    if (animatingWidget) connect(d->backgroundAnim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    d->backgroundAnim->setEasingCurve(QEasingCurve::OutCubic);
    d->backgroundAnim->setDuration(100);

    d->checkAnim = new tVariantAnimation(this);
    connect(d->checkAnim, &tVariantAnimation::valueChanged, this, [ = ](QVariant value) {
        d->currentCheckProgress = value.toReal();
    });
    connect(d->checkAnim, &tVariantAnimation::finished, this, [ = ] {
        if (d->afterAnim) {
            d->afterAnim();
            d->afterAnim = nullptr;
        }
    });
    if (animatingWidget) connect(d->checkAnim, SIGNAL(valueChanged(QVariant)), animatingWidget, SLOT(update()));
    d->checkAnim->setEasingCurve(QEasingCurve::OutCubic);
    d->checkAnim->setDuration(100);
}

CheckboxAnimation::~CheckboxAnimation() {
    delete d;
}

void CheckboxAnimation::setIndicatorRect(QRectF indicator) {
    d->indicatorRect = indicator;
}

void CheckboxAnimation::setCheckState(Qt::CheckState checkState) {
    if (d->currentCheckState == checkState && d->isCheckSetUp) return;

    switch (d->currentCheckState) {
        case Qt::Checked:
            switch (checkState) {
                case Qt::PartiallyChecked:
                    setCheckTarget(0);
                    d->afterAnim = [ = ] {
                        d->drawIndeterminatePoly = true;
                        setCheckTarget(1);
                    };
                    break;
                case Qt::Unchecked:
                    setCheckTarget(0);
                    d->afterAnim = std::bind(&CheckboxAnimation::setIndicatorOff, this);
                    break;
                case Qt::Checked:
                    setIndicatorOn();
                    setCheckTarget(1);
                    break;
            }
            break;
        case Qt::PartiallyChecked:
            switch (checkState) {
                case Qt::Checked:
                    setCheckTarget(0);
                    d->afterAnim = [ = ] {
                        d->drawIndeterminatePoly = false;
                        setCheckTarget(1);
                    };
                    break;
                case Qt::Unchecked:
                    setCheckTarget(0);
                    d->afterAnim = std::bind(&CheckboxAnimation::setIndicatorOff, this);
                    break;
                case Qt::PartiallyChecked:
                    setIndicatorOn();
                    setCheckTarget(1);
                    d->drawIndeterminatePoly = true;
                    break;
            }
            break;
        case Qt::Unchecked:
            switch (checkState) {
                case Qt::PartiallyChecked:
                    setIndicatorOn();
                    d->afterAnim = [ = ] {
                        d->drawIndeterminatePoly = true;
                        setCheckTarget(1);
                    };
                    break;
                case Qt::Checked:
                    d->drawIndeterminatePoly = false;
                    setIndicatorOn();
                    d->afterAnim = std::bind(&CheckboxAnimation::setCheckTarget, this, 1);
                    break;
                case Qt::Unchecked:
                    setIndicatorOff();
                    d->afterAnim = std::bind(&CheckboxAnimation::setCheckTarget, this, 0);
                    break;
            }
            break;
    }
    d->currentCheckState = checkState;
}

QRectF CheckboxAnimation::currentIndicatorRect() {
    return d->backgroundAnim->currentValue().toRectF();
}

QPolygonF CheckboxAnimation::currentCheckPolygon() {
    if (d->drawIndeterminatePoly) {
        double halfHeight = d->indicatorRect.center().y();
        QLineF line(QPointF(d->indicatorRect.left() + d->indicatorRect.width() * 0.25, halfHeight),
            QPointF(d->indicatorRect.left() + d->indicatorRect.width() * 0.75, halfHeight));

        QPolygonF poly;
        poly.append(line.p1());
        poly.append(line.pointAt(d->checkAnim->currentValue().toReal()));
        return poly;
    } else {
        QLineF firstLinePart(QPointF(d->indicatorRect.left() + d->indicatorRect.width() * 0.23, d->indicatorRect.top() + d->indicatorRect.height() * 0.6),
            QPointF(d->indicatorRect.left() + d->indicatorRect.width() * 0.53, d->indicatorRect.top() + d->indicatorRect.height() * 0.8));
        QLineF secondLinePart(firstLinePart.p2(),
            QPointF(d->indicatorRect.left() + d->indicatorRect.width() * 0.83, d->indicatorRect.top() + d->indicatorRect.height() * 0.3));

        double animationProgress = d->checkAnim->currentValue().toReal();
        firstLinePart.translate(-d->indicatorRect.width() * 0.05 * (1 - animationProgress), 0);
        secondLinePart.translate(-d->indicatorRect.width() * 0.05 * (1 - animationProgress), 0);

        QPolygonF poly;
        poly.append(firstLinePart.p1());
        poly.append(firstLinePart.pointAt(qBound(0.0, animationProgress * 2, 1.0)));
        if (animationProgress > 0.5) poly.append(secondLinePart.pointAt(qBound(0.0, (animationProgress - 0.5) * 2, 1.0)));
        return poly;
    }
}

void CheckboxAnimation::setIndicatorInnerRect(QRectF indicator) {
    if (d->targetHoverRect != indicator) {
        d->targetHoverRect = indicator;
        d->backgroundAnim->setStartValue(d->isBackgroundSetUp ? d->currentHoverRect : d->targetHoverRect);
        d->backgroundAnim->setEndValue(d->targetHoverRect);
        d->backgroundAnim->setCurrentTime(0);
        d->backgroundAnim->start();
        d->currentHoverRect = d->backgroundAnim->currentValue().toRectF();
        d->isBackgroundSetUp = true;
    }
}

void CheckboxAnimation::setIndicatorOn() {
    QRectF onRect = d->indicatorRect;
//    onRect.setSize(onRect.size() *= 0);
//    onRect.moveCenter(QRectF(d->indicatorRect).center());
    onRect.setWidth(0);
    onRect.moveLeft(d->indicatorRect.right());

    setIndicatorInnerRect(onRect);
}

void CheckboxAnimation::setIndicatorOff() {
    setIndicatorInnerRect(d->indicatorRect);
}

void CheckboxAnimation::setCheckTarget(qreal target) {
    if (d->targetCheckProgress != target) {
        d->targetCheckProgress = target;
        d->checkAnim->setStartValue(d->isCheckSetUp ? d->currentCheckProgress : d->targetCheckProgress);
        d->checkAnim->setEndValue(d->targetCheckProgress);
        d->checkAnim->setCurrentTime(0);
        d->checkAnim->start();
        d->currentCheckProgress = d->checkAnim->currentValue().toReal();
        d->isCheckSetUp = true;
    }
}
