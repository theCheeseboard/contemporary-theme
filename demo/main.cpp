#include "mainwindow.h"

#include <QApplication>
#include "contemporary2/contemporary.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    a.setStyle(new Contemporary());
//    a.setStyle("breeze");

    bool isLight = false;
    QPalette pal;

    //Get the accent colour
    QColor accentCol;
#ifdef Q_OS_WIN
    QSettings accentDetection("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM", QSettings::NativeFormat);
    if (accentDetection.contains("ColorizationColor")) {
        accentCol = QColor::fromRgb(QRgb(accentDetection.value("ColorizationColor").toInt() & 0x00FFFFFF));
        if (d->currentStyle == ContemporaryLight) {
            pal.setColor(QPalette::Button, accentCol.lighter(150));
        } else {
            pal.setColor(QPalette::Button, accentCol);
        }
    } else {
#endif
        accentCol = QColor::fromRgb((isLight ? 0xC400C8FF : 0xC4003296) & 0x00FFFFFF);
        pal.setColor(QPalette::Button, accentCol);
#ifdef Q_OS_WIN
    }
#endif

    if (isLight) {
        pal.setColor(QPalette::ButtonText, QColor(0, 0, 0));
        pal.setColor(QPalette::Highlight, accentCol.lighter(125));
        pal.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
        pal.setColor(QPalette::Disabled, QPalette::Button, accentCol.darker(200));
        pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0, 0, 0));

        pal.setColor(QPalette::Window, QColor(210, 210, 210));
        pal.setColor(QPalette::Base, QColor(210, 210, 210));
        pal.setColor(QPalette::AlternateBase, QColor(210, 210, 210));
        pal.setColor(QPalette::WindowText, QColor(0, 0, 0));
        pal.setColor(QPalette::Text, QColor(0, 0, 0));
        pal.setColor(QPalette::ToolTipText, QColor(0, 0, 0));

        pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(100, 100, 100));
    } else {
        pal.setColor(QPalette::ButtonText, QColor(255, 255, 255));
        pal.setColor(QPalette::Highlight, accentCol.lighter(125));
        pal.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
        pal.setColor(QPalette::Disabled, QPalette::Button, accentCol.darker(200));
        pal.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(150, 150, 150));

        pal.setColor(QPalette::Window, QColor(40, 40, 40));
        pal.setColor(QPalette::Base, QColor(40, 40, 40));
        pal.setColor(QPalette::AlternateBase, QColor(60, 60, 60));
        pal.setColor(QPalette::WindowText, QColor(255, 255, 255));
        pal.setColor(QPalette::Text, QColor(255, 255, 255));
        pal.setColor(QPalette::ToolTipText, QColor(255, 255, 255));

        pal.setColor(QPalette::Disabled, QPalette::WindowText, QColor(150, 150, 150));
    }

    QApplication::setPalette(pal);
    QApplication::setPalette(pal, "QDockWidget");
    QApplication::setPalette(pal, "QToolBar");

    QIcon::setThemeName("contemporary");

    QFont fnt("Contemporary", 10);
    a.setFont(fnt);

    MainWindow w;
    w.show();
    return a.exec();
}
