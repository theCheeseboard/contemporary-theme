#ifndef CONTEMPORARYTHEMEPLUGIN_H
#define CONTEMPORARYTHEMEPLUGIN_H

#include <QStylePlugin>
//#include "contemporary2/contemporary.h"
#include "contemporarylegacy.h"

class ContemporaryThemePlugin : public QStylePlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QStyleFactoryInterface" FILE "plugin.json")
public:
    explicit ContemporaryThemePlugin(QObject *parent = 0);

    QStyle* create(const QString &key);
};

#endif // CONTEMPORARYTHEMEPLUGIN_H
