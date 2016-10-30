#ifndef PLUGIN_H
#define PLUGIN_H

#include <QStylePlugin>
#include "contemporary.h"

class Plugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "plugin.json")
public:
    explicit Plugin(QObject *parent = 0);

    QStyle* create(const QString &key);
};

#endif // PLUGIN_H
