#ifndef ANIMATIONENGINE_H
#define ANIMATIONENGINE_H

#include <QObject>
#include <QMap>

#include "pushbuttonanimation.h"

struct AnimationPair {
    AnimationPair();

    const QWidget* widget = nullptr;
    Animation* anim = nullptr;

    operator const char*() const {
        return (const char*) widget;
    }
};

class AnimationEngine : public QObject
{
        Q_OBJECT
    public:
        explicit AnimationEngine(QObject *parent = nullptr);

        void registerWidget(QWidget* widget);
        void deregisterWidget(QWidget* widget);
        Animation* a(const QWidget* widget);

    signals:

    public slots:

    private:
        QVector<AnimationPair> animations;
        //QMap<QWidget*, Animation*> animations;
};

#endif // ANIMATIONENGINE_H
