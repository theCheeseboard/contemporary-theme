#ifndef CONTEMPORARYLEGACY_H
#define CONTEMPORARYLEGACY_H

#include "contemporary_global.h"
#include <QCommonStyle>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTimer>
#include <QGraphicsColorizeEffect>
#include <QDesktopWidget>
#include <tvariantanimation.h>

//Include controls
#include <QPushButton>
#include <QMainWindow>
#include <QTextEdit>
#include <QCheckBox>
#include <QAbstractItemView>
#include <QComboBox>
#include <QToolButton>
#include <QRadioButton>
#include <QSettings>
#include <QCommandLinkButton>
#include <QAbstractItemView>
#include <QTabBar>
#include <QScrollBar>
#include <QLineEdit>

struct StylePrivate;
class CONTEMPORARYSHARED_EXPORT Style : public QCommonStyle {

    public:
        Style();
        ~Style();

        void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const override;
        void drawPrimitive(PrimitiveElement primitive, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

        QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const override;
        int pixelMetric(PixelMetric m, const QStyleOption* opt, const QWidget* widget) const override;
        QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* opt, const QWidget* widget) const override;
        void drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& palette, bool enabled, const QString& text, QPalette::ColorRole textRole) const override;
        QRect subElementRect(SubElement r, const QStyleOption* opt, const QWidget* widget) const override;

        void polish(QWidget* widget) override;
        void polish(QApplication* application) override;
        void unpolish(QApplication* application) override;

        int styleHint(StyleHint sh, const QStyleOption* opt, const QWidget* w, QStyleHintReturn* shret) const override;
        int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption* option, const QWidget* widget) const override;
        SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex* option, const QPoint& pt, const QWidget* w) const override;
        void tintImage(QImage& image, QColor tint) const;
    private:
        const QColor transparent = QColor::fromRgba64(0, 0, 0, 0);
        QColor col(int r, int g, int b) const;
        void scheduleRepaint(const QWidget* widget, int after = 1000 / 60) const;

        uint indetermiateProgressSection = 0;
        QTimer* indeterminateTimer = nullptr; //Increments the indeterminateProgressSection at regular intervals

        QVariant animation(QString id, QVariant retVal = "") const;
        void putAnimation(QString type, QString id, QVariant value) const;
        QString currentType(QString id) const;

        StylePrivate* d;
};

//const QWidget* Style::selectedMenu = nullptr;

#endif // CONTEMPORARYLEGACY_H
