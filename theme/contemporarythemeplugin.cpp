#include "contemporarythemeplugin.h"

#include "contemporary2/contemporary.h"

ContemporaryThemePlugin::ContemporaryThemePlugin(QObject* parent) :
    QStylePlugin(parent) {
}

QStyle* ContemporaryThemePlugin::create(const QString& key) {
    if (key.toLower() == "contemporary") {
        return new Contemporary;
    } else if (key.toLower() == "contemporarylegacy") {
        return new Style;
    }
    return nullptr;
}
