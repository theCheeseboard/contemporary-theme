#ifndef CONTEMPORARY_H
#define CONTEMPORARY_H

#include "contemporarylegacy.h"
#include <tvariantanimation.h>

#define OPT_CAST(type) const type* opt = qstyleoption_cast<const type*>(option);
#define OPT_VARS bool reverse = opt->direction == Qt::RightToLeft;

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

class CONTEMPORARYSHARED_EXPORT Contemporary : public QCommonStyle
{
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

        void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget *widget) const override;
        void drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget *widget) const override;
        void drawPrimitive(PrimitiveElement primitive, const QStyleOption *option, QPainter *painter, const QWidget *widget) const override;

        QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const override;
        int pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const override;
        QIcon standardIcon(StandardPixmap standardIcon, const QStyleOption *opt, const QWidget *widget) const override;
        void drawItemText(QPainter* painter, const QRect &rect, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole) const override;
        QRect subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget) const override;

        void polish(QWidget* widget) override;

        int styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const override;
        int layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget) const override;
        SubControl hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *option, const QPoint &pt, const QWidget *w) const override;
        void tintImage(QImage &image, QColor tint) const;

    signals:

    public slots:

    private:
        Style* oldStyle;
        float getDPIScaling() const;
        void scheduleRepaint(const QWidget* widget, int after = 1000 / 60) const;

        uint indetermiateProgressSection = 0;
        QTimer* indeterminateTimer = nullptr; //Increments the indeterminateProgressSection at regular intervals
        QMap<WId, tVariantAnimation*>* animations;

        void drawPrimitiveIndicatorCheckBox(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitiveIndicatorRadioButton(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitiveFrame(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitivePanelButtonCommand(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitivePanelLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitivePanelItemViewItem(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitiveIndicatorArrow(Direction dir, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitiveIndicatorBranch(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawPrimitiveFrameFocusRect(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

        void drawControlProgressBarContents(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawControlProgressBarGroove(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawControlProgressBar(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawControlHeaderSection(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawControlHeaderEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
        void drawMenubarEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

        void drawComplexComboBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;


};

#endif // CONTEMPORARY_H
