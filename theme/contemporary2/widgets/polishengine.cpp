#include "polishengine.h"

#include <QAbstractButton>>
#include <QEvent>

PolishEngine::PolishEngine(QObject* parent) :
    QObject{parent} {
}

void PolishEngine::registerWidget(QWidget* widget) {
    if (qobject_cast<QAbstractButton*>(widget)) {
        widget->installEventFilter(this);
    }
}

void PolishEngine::deregisterWidget(QWidget* widget) {
    if (qobject_cast<QAbstractButton*>(widget)) {
        widget->removeEventFilter(this);
    }
}

bool PolishEngine::eventFilter(QObject* watched, QEvent* event) {
    if (auto btn = qobject_cast<QAbstractButton*>(watched)) {
        if (event->type() == QEvent::DynamicPropertyChange) {
            auto e = static_cast<QDynamicPropertyChangeEvent*>(event);
            if (e->propertyName() == "supplementaryText") {
                // Force the button to update its size
                // Update the icon so we don't send spurious a11y events
                auto icon = btn->icon();
                btn->setIcon(QIcon());
                btn->setIcon(icon);
            }
        }
    }
    return false;
}
