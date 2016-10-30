#include "plugin.h"

Plugin::Plugin(QObject *parent) : QStylePlugin(parent)
{

}

QStyle* Plugin::create(const QString &key) {
    if (key.toLower() == "contemporary") {
        return new Style;
    }
    return NULL;
}
