#ifndef CONTEMPORARY_H
#define CONTEMPORARY_H

#include "contemporarylegacy.h"
#include "animations/animationengine.h"
#include <tvariantanimation.h>
#include <tpaintcalculator.h>
#include <QStyleOptionButton>

#define OPT_CAST(type) const type* opt = qstyleoption_cast<const type*>(option);
#define OPT_VARS bool reverse = opt->direction == Qt::RightToLeft; Qt::AlignmentFlag textHorizontalAlignment = reverse ? Qt::AlignRight : Qt::AlignLeft; Q_UNUSED(textHorizontalAlignment)

#define WINDOW_TEXT_COLOR opt->palette.color(QPalette::WindowText)
#define WINDOW_COLOR opt->palette.color(QPalette::Window)
#define WINDOW_HOVER_COLOR WINDOW_COLOR.lighter(150)
#define WINDOW_PRESS_COLOR WINDOW_COLOR.darker(150)
#define BUTTON_COLOR opt->palette.color(QPalette::Button)
#define BUTTON_HOVER_COLOR BUTTON_COLOR.lighter(150)
#define BUTTON_PRESS_COLOR BUTTON_COLOR.darker(150)
#define BUTTON_TEXT_COLOR opt->palette.color(QPalette::ButtonText)
#define ACCENT_COLOR opt->palette.color(QPalette::Highlight)
#define ACCENT_HOVER_COLOR ACCENT_COLOR.lighter(150)
#define ACCENT_PRESS_COLOR ACCENT_COLOR.darker(150)
#define ACCENT_TEXT_COLOR opt->palette.color(QPalette::HighlightedText)

struct ContemporaryPrivate;
class CONTEMPORARYSHARED_EXPORT Contemporary : public QCommonStyle {
        Q_OBJECT

        enum Direction {
            Up,
            Right,
            Down,
            Left
        };

    public:
        Contemporary();
        ~Contemporary();

        void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const override;
        void drawPrimitive(PrimitiveElement primitive, const QStyleOption* option, QPainter* painter, const QWidget* widget) const override;

        QSize sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const override;
        int pixelMetric(PixelMetric m, const QStyleOption* opt, const QWidget* widget) const override;
        QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption* opt, const QWidget* widget) const override;
        void drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& palette, bool enabled, const QString& text, QPalette::ColorRole textRole) const override;
        QRect subElementRect(SubElement r, const QStyleOption* opt, const QWidget* widget) const override;

        void polish(QWidget* widget) override;
        void unpolish(QWidget* widget) override;

        int styleHint(StyleHint sh, const QStyleOption* opt, const QWidget* w, QStyleHintReturn* shret) const override;
        int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption* option, const QWidget* widget) const override;
        SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex* option, const QPoint& pt, const QWidget* w) const override;
        void tintImage(QImage& image, QColor tint) const;

        void aaa(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;

    signals:

    public slots:

    private:
        ContemporaryPrivate* d;

        float getDPIScaling() const;
        void scheduleRepaint(const QWidget* widget, int after = 1000 / 60) const;

        void drawPrimitiveIndicatorCheckBox(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitiveIndicatorRadioButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitiveFrame(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitivePanelButtonCommand(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitivePanelLineEdit(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitivePanelItemViewItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitiveIndicatorArrow(Direction dir, const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitiveIndicatorBranch(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawPrimitiveFrameFocusRect(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;

        void drawControlProgressBarContents(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlProgressBarGroove(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlProgressBar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlHeaderSection(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlHeaderEmptyArea(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlCheckboxLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlMenuItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlPushButtonLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlComboBoxLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlMenubarEmptyArea(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlMenubarItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        void drawControlToolbar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;

        void drawComplexComboBox(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;
        void drawComplexToolButton(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;
        void drawComplexSlider(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;

        QRect subElementCheckBoxIndicator(const QStyleOption* option, const QWidget* widget) const;
        QRect subElementCheckBoxContents(const QStyleOption* option, const QWidget* widget) const;
        QRect subElementCheckBoxFocusRect(const QStyleOption* option, const QWidget* widget) const;
        QRect subElementCheckBoxClickRect(const QStyleOption* option, const QWidget* widget) const;
        QRect subElementPushButtonContents(const QStyleOption* option, const QWidget* widget) const;
        QRect subElementPushButtonFocusRect(const QStyleOption* option, const QWidget* widget) const;

        SubControl hitTestSlider(const QStyleOptionComplex* option, const QPoint& point, const QWidget* widget) const;

        tPaintCalculator paintCalculatorCheckBox(const QStyleOption* option, QPainter* painter, const QWidget* widget, bool isRadioButton) const;
        tPaintCalculator paintCalculatorPushButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorProgressBar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorToolButton(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorSlider(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorComboBox(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorMenuBarItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;
        tPaintCalculator paintCalculatorMenuItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const;

        QPixmap shortcutPixmap(const QFont font, const QPalette pal, const QString text) const;

        QColor buttonBackground(const QStyleOptionButton* opt, const QWidget* widget) const;

        // QStyle interface
    public:
        void polish(QApplication* application) override;
        void unpolish(QApplication* application) override;
};

#endif // CONTEMPORARY_H
