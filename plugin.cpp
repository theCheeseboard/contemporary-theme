#include "plugin.h"

Plugin::Plugin(QObject *parent) : QStylePlugin(parent)
{

}

QStyle* Plugin::create(const QString &key) {
    if (key.toLower() == "contemporary") {
        //return new Contemporary;
        return new Style;
    } else if (key.toLower() == "contemporarylegacy") {
        return new Style;
    }
    return nullptr;
}
