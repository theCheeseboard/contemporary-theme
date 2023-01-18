#ifndef POLISHENGINE_H
#define POLISHENGINE_H

#include <QObject>

class PolishEngine : public QObject {
        Q_OBJECT
    public:
        explicit PolishEngine(QObject* parent = nullptr);

        void registerWidget(QWidget* widget);
        void deregisterWidget(QWidget* widget);

    signals:

        // QObject interface
    public:
        bool eventFilter(QObject* watched, QEvent* event);
};

#endif // POLISHENGINE_H
