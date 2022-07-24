#ifndef ANIMATION_H
#define ANIMATION_H

#include <QPointer>
#include <QWidget>

class Animation : public QObject {
        Q_OBJECT
    public:
        explicit Animation(QWidget* animating, QObject* parent = nullptr);

    signals:

    public slots:

    protected:
        QPointer<QWidget> animatingWidget;
};

#endif // ANIMATION_H
