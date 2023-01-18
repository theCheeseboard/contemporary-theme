#include "contemporary.h"

#include <QPainter>
#include <QTimer>
#include <tpaintcalculator.h>

#include "animations/checkboxanimation.h"
#include "animations/menuitemanimation.h"
#include "animations/pushbuttonanimation.h"
#include "animations/radiobuttonanimation.h"
#include "animations/scrollbaranimation.h"
#include "animations/viewitemanimation.h"

#include "widgets/polishengine.h"

#include <QAbstractItemView>
#include <QLineEdit>
#include <QStyleOptionButton>

#include "focusDecorations/focusdecorationcontroller.h"

struct ContemporaryPrivate {
        uint indetermiateProgressSection = 0;
        QTimer* indeterminateTimer = nullptr; // Increments the indeterminateProgressSection at regular intervals
        QMap<WId, tVariantAnimation*>* animations;

        Style* oldStyle;
        AnimationEngine* anim;
        FocusDecorationController* focusDecorations;
        PolishEngine* polish;
};

Contemporary::Contemporary() {
    d = new ContemporaryPrivate();
    d->oldStyle = new Style();
    d->anim = new AnimationEngine;
    d->focusDecorations = new FocusDecorationController();
    d->polish = new PolishEngine();

    d->indeterminateTimer = new QTimer(this);
    if (libContemporaryCommon::instance()->powerStretchEnabled()) {
        d->indeterminateTimer->setInterval(1000 / 5);
    } else {
        d->indeterminateTimer->setInterval(1000 / 60);
    }
    connect(d->indeterminateTimer, &QTimer::timeout, [=]() {
        // Yes, this can overflow - and that's good. :)
        if (libContemporaryCommon::instance()->powerStretchEnabled()) {
            d->indetermiateProgressSection += 120;
        } else {
            d->indetermiateProgressSection += 10;
        }
    });

    connect(libContemporaryCommon::instance(), &libContemporaryCommon::powerStretchChanged, [=](bool isOn) {
        if (isOn) {
            d->indeterminateTimer->setInterval(1000 / 5);
        } else {
            d->indeterminateTimer->setInterval(1000 / 60);
        }
    });

    d->animations = new QMap<WId, tVariantAnimation*>();
}

Contemporary::~Contemporary() {
    delete d->polish;
    delete d->oldStyle;
    delete d->anim;
    delete d->animations;
    delete d->focusDecorations;
    delete d;
}

void Contemporary::scheduleRepaint(const QWidget* widget, int after) const {
    QTimer* updateTimer = new QTimer;
    updateTimer->setInterval(after);
    updateTimer->setSingleShot(true);
    connect(widget, &QObject::destroyed, updateTimer, &QTimer::stop);
    connect(widget, &QObject::destroyed, updateTimer, &QObject::deleteLater);
    connect(updateTimer, SIGNAL(timeout()), widget, SLOT(repaint()));
    connect(updateTimer, &QTimer::timeout, updateTimer, &QTimer::stop);
    connect(updateTimer, &QTimer::timeout, updateTimer, &QObject::deleteLater);
    updateTimer->start();
}

void Contemporary::tintImage(QImage& image, QColor tint) const {
    bool doPaint = true;
    for (int y = 0; y < image.height(); y++) {
        for (int x = 0; x < image.width(); x++) {
            QColor pixelCol = image.pixelColor(x, y);
            if ((pixelCol.blue() > pixelCol.green() - 10 && pixelCol.blue() < pixelCol.green() + 10) &&
                (pixelCol.green() > pixelCol.red() - 10 && pixelCol.green() < pixelCol.red() + 10)) {
            } else {
                doPaint = false;
            }
        }
    }

    if (doPaint) {
        QPainter painter(&image);
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.fillRect(0, 0, image.width(), image.height(), tint);
        painter.end();
    }
}

void Contemporary::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    switch (element) {
        case CE_ProgressBarContents:
            drawControlProgressBarContents(option, painter, widget);
            break;
        case CE_ProgressBarGroove:
            drawControlProgressBarGroove(option, painter, widget);
            break;
        case CE_ProgressBar:
            drawControlProgressBar(option, painter, widget);
            break;
        case CE_HeaderSection:
            drawControlHeaderSection(option, painter, widget);
            break;
        case CE_HeaderEmptyArea:
            drawControlHeaderEmptyArea(option, painter, widget);
            break;
        case CE_MenuBarEmptyArea:
            drawControlMenubarEmptyArea(option, painter, widget);
            break;
        case CE_MenuBarItem:
            drawControlMenubarItem(option, painter, widget);
            break;
        case CE_MenuItem:
            drawControlMenuItem(option, painter, widget);
            break;
        case CE_CheckBoxLabel:
            drawControlCheckboxLabel(option, painter, widget);
            break;
        case CE_PushButtonLabel:
            drawControlPushButtonLabel(option, painter, widget);
            break;
        case CE_ComboBoxLabel:
            drawControlComboBoxLabel(option, painter, widget);
            break;
        case CE_ToolBar:
            drawControlToolbar(option, painter, widget);
            break;
        case CE_PushButton:
            drawControlPushButton(option, painter, widget);
            break;
        case CE_TabBarTabShape:
            drawControlTabBarTabShape(option, painter, widget);
            break;
        case CE_ShapedFrame:
            drawControlShapedFrame(option, painter, widget);
            break;
        default:
            QCommonStyle::drawControl(element, option, painter, widget);
    }
}

void Contemporary::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    switch (control) {
        case CC_ComboBox:
            drawComplexComboBox(option, painter, widget);
            break;
        case CC_ToolButton:
            drawComplexToolButton(option, painter, widget);
            break;
        case CC_Slider:
            drawComplexSlider(option, painter, widget);
            break;
        case CC_ScrollBar:
            drawComplexScrollBar(option, painter, widget);
            break;
        default:
            QCommonStyle::drawComplexControl(control, option, painter, widget);
    }
}

void Contemporary::drawPrimitive(PrimitiveElement primitive, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    switch (primitive) {
        case PE_IndicatorItemViewItemCheck:
        case PE_IndicatorCheckBox:
            drawPrimitiveIndicatorCheckBox(option, painter, widget);
            break;

        case PE_IndicatorRadioButton:
            drawPrimitiveIndicatorRadioButton(option, painter, widget);
            break;

        case PE_PanelButtonCommand:
        case PE_PanelButtonTool:
            drawPrimitivePanelButtonCommand(option, painter, widget);
            break;

        case PE_PanelLineEdit:
            drawPrimitivePanelLineEdit(option, painter, widget);
            break;

        case PE_PanelItemViewItem:
            drawPrimitivePanelItemViewItem(option, painter, widget);
            break;

        case PE_IndicatorSpinUp:
            drawPrimitiveIndicatorArrow(Up, option, painter, widget);
            break;
        case PE_IndicatorSpinDown:
            drawPrimitiveIndicatorArrow(Down, option, painter, widget);
            break;
        case PE_IndicatorArrowLeft:
            drawPrimitiveIndicatorArrow(Left, option, painter, widget);
            break;
        case PE_IndicatorArrowRight:
            drawPrimitiveIndicatorArrow(Right, option, painter, widget);
            break;
        case PE_IndicatorArrowUp:
            drawPrimitiveIndicatorArrow(Up, option, painter, widget);
            break;
        case PE_IndicatorArrowDown:
            drawPrimitiveIndicatorArrow(Down, option, painter, widget);
            break;

        case PE_IndicatorBranch:
            drawPrimitiveIndicatorBranch(option, painter, widget);
            break;

        case PE_FrameFocusRect:
            drawPrimitiveFrameFocusRect(option, painter, widget);
            break;

        case PE_FrameMenu:
        case PE_PanelMenu:
        case PE_FrameGroupBox:
        case PE_Frame:
            drawPrimitiveFrame(option, painter, widget);
            break;
        case PE_IndicatorTabClose:
            drawPrimitiveIndicatorTabClose(option, painter, widget);
            break;
            // default: oldStyle->drawPrimitive(primitive, option, painter, widget);
    }
}

QSize Contemporary::sizeFromContents(ContentsType ct, const QStyleOption* opt, const QSize& contentsSize, const QWidget* widget) const {
    switch (ct) {
        case CT_MenuItem:
            {
                QStyleOptionMenuItem item(*static_cast<const QStyleOptionMenuItem*>(opt));
                item.rect = QRect(0, 0, 1, pixelMetric(PM_SmallIconSize, &item, widget) + SC_DPI_W(8, widget));

                tPaintCalculator calculator = paintCalculatorMenuItem(&item, nullptr, widget);
                calculator.setBoundsCalculationExcludeList({"background"});
                return calculator.sizeWithMargins().toSize();
            }
        case CT_PushButton:
            {
                QStyleOptionButton item(*static_cast<const QStyleOptionButton*>(opt));
                item.rect = QRect(0, 0, 1, 1);

                tPaintCalculator calculator = paintCalculatorPushButton(&item, nullptr, widget);
                calculator.setBoundsCalculationExcludeList({"background"});
                QPointF offset = calculator.rectNames().contains("supplementaryText") ? QPointF(9, 6) : QPointF(9, 9);
                auto size = calculator.sizeWithMargins(calculator.visualBoundingRect().topLeft() - SC_DPI_WT(offset, QPointF, widget)).toSize();
                if (size.height() < SC_DPI_W(16, widget)) size.setHeight(SC_DPI_W(16, widget));
                return size;
            }
        case CT_ScrollBar:
            return QCommonStyle::sizeFromContents(ct, opt, contentsSize, widget);
        default:
            return d->oldStyle->sizeFromContents(ct, opt, contentsSize, widget);
    }
}

int Contemporary::pixelMetric(PixelMetric m, const QStyleOption* opt, const QWidget* widget) const {
    switch (m) {
        case PM_MessageBoxIconSize:
            return SC_DPI_W(64, widget);
        case PM_SubMenuOverlap:
            return 0;
        case PM_MenuPanelWidth:
        case PM_MenuBarItemSpacing:
        case PM_MenuBarPanelWidth:

        case PM_TabBarTabShiftHorizontal:
        case PM_TabBarTabShiftVertical:

        case PM_ToolBarItemMargin:
            return 0;
        case PM_MenuHMargin:
        case PM_MenuVMargin:
            return SC_DPI_W(1, widget);
        case PM_CheckBoxLabelSpacing:
        case PM_RadioButtonLabelSpacing:
            return SC_DPI_W(4, widget);
        case PM_ToolBarIconSize:
        case PM_SliderControlThickness:
            return SC_DPI_W(16, widget);
        case PM_ScrollBarExtent:
        case PM_ScrollView_ScrollBarOverlap:
            return SC_DPI_W(10, widget);
        case PM_IndicatorWidth:
        case PM_IndicatorHeight:
        case PM_ExclusiveIndicatorHeight:
        case PM_ExclusiveIndicatorWidth:
            return SC_DPI_W(16, widget);

        case PM_ButtonShiftHorizontal:
        case PM_ButtonShiftVertical:
            return 0;

        default:
            return QCommonStyle::pixelMetric(m, opt, widget);
    }
}

QIcon Contemporary::standardIcon(StandardPixmap standardIcon, const QStyleOption* opt, const QWidget* widget) const {
    return d->oldStyle->standardIcon(standardIcon, opt, widget);
}

void Contemporary::drawItemText(QPainter* painter, const QRect& rect, int alignment, const QPalette& palette, bool enabled, const QString& text, QPalette::ColorRole textRole) const {
    QTextOption opt;
    opt.setAlignment((Qt::Alignment) alignment);

    QString t = text;
    t.remove("&");
    painter->setPen(palette.color(enabled ? QPalette::Normal : QPalette::Disabled, textRole));
    painter->drawText(rect, t, opt);
}

QRect Contemporary::subElementRect(SubElement r, const QStyleOption* opt, const QWidget* widget) const {
    switch (r) {
        case SE_CheckBoxIndicator:
        case SE_RadioButtonIndicator:
            return subElementCheckBoxIndicator(opt, widget);

        case SE_CheckBoxContents:
        case SE_RadioButtonContents:
            return subElementCheckBoxContents(opt, widget);

        case SE_CheckBoxFocusRect:
        case SE_RadioButtonFocusRect:
            return subElementCheckBoxFocusRect(opt, widget);

        case SE_CheckBoxClickRect:
            return subElementCheckBoxClickRect(opt, widget);

        case SE_PushButtonFocusRect:
            return subElementPushButtonFocusRect(opt, widget);

        case SE_PushButtonContents:
            return subElementPushButtonContents(opt, widget);

        case SE_ItemViewItemFocusRect:
            return QRect();

        default:
            return d->oldStyle->subElementRect(r, opt, widget);
    }
}

void Contemporary::polish(QWidget* widget) {
    d->polish->registerWidget(widget);
    d->anim->registerWidget(widget);

    d->oldStyle->polish(widget);
}

void Contemporary::unpolish(QWidget* widget) {
    d->anim->deregisterWidget(widget);
    d->polish->deregisterWidget(widget);
}

int Contemporary::styleHint(StyleHint sh, const QStyleOption* opt, const QWidget* w, QStyleHintReturn* shret) const {
    switch (sh) {
        case SH_Menu_FlashTriggeredItem:
        case SH_Menu_SupportsSections:
        case SH_ComboBox_Popup:
        case SH_Menu_MouseTracking:
        case SH_MenuBar_MouseTracking:
            return true;

        default:
            return d->oldStyle->styleHint(sh, opt, w, shret);
    }
}

int Contemporary::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption* option, const QWidget* widget) const {
    return d->oldStyle->layoutSpacing(control1, control2, orientation, option, widget);
}

Contemporary::SubControl Contemporary::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex* option, const QPoint& pt, const QWidget* w) const {
    switch (cc) {
        case CC_Slider:
            return hitTestSlider(option, pt, w);
        case CC_ScrollBar:
            return hitTestScrollBar(option, pt, w);
        default:
            return d->oldStyle->hitTestComplexControl(cc, option, pt, w);
    }
}

void Contemporary::drawPrimitiveIndicatorCheckBox(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorCheckBox(option, painter, widget, false).performPaint("indicator");
}

void Contemporary::drawPrimitiveIndicatorRadioButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorCheckBox(option, painter, widget, true).performPaint("indicator");
}

void Contemporary::drawPrimitiveFrame(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;

    painter->setPen(WINDOW_TEXT_COLOR);
    painter->setBrush(Qt::transparent);
    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
}

void Contemporary::drawPrimitivePanelButtonCommand(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorPushButton(option, painter, widget).performPaint("background");
}

void Contemporary::drawPrimitivePanelLineEdit(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;
    OPT_VARS

    if (QLineEdit* lineEdit = qobject_cast<QLineEdit*>(option->styleObject)) {
        if (!lineEdit->hasFrame()) return;
    }

    painter->setPen(WINDOW_TEXT_COLOR);
    if (reverse) {
        painter->drawLine(opt->rect.topRight(), opt->rect.bottomRight());
    } else {
        painter->drawLine(opt->rect.topLeft(), opt->rect.bottomLeft());
    }
    painter->drawLine(opt->rect.bottomRight(), opt->rect.bottomLeft());
}

void Contemporary::drawPrimitivePanelItemViewItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionViewItem)
    if (opt == nullptr) return;

    const QAbstractItemView* itemView = qobject_cast<const QAbstractItemView*>(widget);
    ViewItemAnimation* anim = qobject_cast<ViewItemAnimation*>(d->anim->a(widget));

    QColor background;
    bool isHover = opt->state & State_MouseOver && (!itemView || itemView->selectionMode() != QAbstractItemView::NoSelection);
    if (opt->state & State_Selected) {
        background = ACCENT_COLOR;
    } else if (isHover) {
        if (anim) {
            anim->setHoverRect(opt->rect);
            background = Qt::transparent;
        } else {
            background = ACCENT_COLOR;
            background.setAlpha(127);
        }
    } else {
        background = Qt::transparent;
    }

    painter->setPen(Qt::transparent);
    painter->setBrush(background);
    painter->drawRect(opt->rect);

    if (anim) {
        if (!isHover && opt->rect == anim->lastSetRect()) anim->clearHoverRect();

        if (anim->hoverRect().intersects(opt->rect)) {
            background = ACCENT_COLOR;
            background.setAlpha(127);
            painter->setBrush(background);
            painter->drawRect(anim->hoverRect());
        }
    }
}

void Contemporary::drawPrimitiveIndicatorArrow(Direction dir, const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;

    QSize size = opt->rect.size();

    QImage image;
    if (dir == Right) {
        image = QIcon::fromTheme("go-next").pixmap(size).toImage();
    } else if (dir == Left) {
        image = QIcon::fromTheme("go-previous").pixmap(size).toImage();
    } else if (dir == Down) {
        image = QIcon::fromTheme("go-down").pixmap(size).toImage();
    } else if (dir == Up) {
        image = QIcon::fromTheme("go-up").pixmap(size).toImage();
    }
    tintImage(image, WINDOW_TEXT_COLOR);

    painter->drawImage(opt->rect, image);
}

void Contemporary::drawPrimitiveIndicatorBranch(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;
    OPT_VARS

    QRect rect = opt->rect;
    if (opt->state & QStyle::State_Item) {
        // Draw horizontal branch
        painter->setPen(WINDOW_TEXT_COLOR);

        if (reverse) {
            painter->drawLine(rect.left(), rect.top() + rect.height() / 2, rect.left() + rect.width() / 2, rect.top() + rect.height() / 2);
        } else {
            painter->drawLine(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2, rect.right(), rect.top() + rect.height() / 2);
        }

        // Draw top part of vertical branch
        painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.top() + rect.height() / 2);
    }

    if (opt->state & QStyle::State_Sibling) {
        // Show vertical branch
        painter->setPen(WINDOW_TEXT_COLOR);
        painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.bottom());
    }

    if (opt->state & QStyle::State_Children) {
        // Draw disclosure triangle (closed)
        QRect triangleRect = rect;
        triangleRect.setTop(rect.top() + (rect.height()) / 2 - SC_DPI_W(4, widget));
        triangleRect.setLeft(rect.left() + (rect.width()) / 2 - SC_DPI_W(4, widget));
        triangleRect.setSize(QSize(SC_DPI_W(8, widget), SC_DPI_W(8, widget)));
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawRect(triangleRect);
    }

    if (opt->state & QStyle::State_Open) {
        // Show disclosure triangle (open)
        QRect triangleRect = rect;
        triangleRect.setTop(rect.top() + (rect.height()) / 2 - SC_DPI_W(4, widget));
        triangleRect.setLeft(rect.left() + (rect.width()) / 2 - SC_DPI_W(4, widget));
        triangleRect.setSize(QSize(SC_DPI_W(8, widget), SC_DPI_W(8, widget)));
        painter->setBrush(WINDOW_COLOR);
        painter->setPen(WINDOW_TEXT_COLOR);
        painter->drawRect(triangleRect);
    }
}

void Contemporary::drawPrimitiveFrameFocusRect(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionFocusRect);
    if (opt == nullptr) return;

    // No-op: focus rect is drawn by other means

    /*QRect r;
    r.setLeft(0);
    r.setBottom(opt->rect.bottom());
    r.setWidth(opt->rect.width());
    r.setTop(opt->rect.bottom() - 2);*/
    //    painter->setBrush(QColor(255, 255, 255, 100));
    //    painter->setPen(Qt::transparent);
    //    painter->drawRect(opt->rect);
}

void Contemporary::drawControlProgressBarContents(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorProgressBar(option, painter, widget).performPaint("contents");
}

void Contemporary::drawControlProgressBarGroove(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionProgressBar)
    if (opt == nullptr) return;

    paintCalculatorProgressBar(option, painter, widget).performPaint("groove");
}

void Contemporary::drawControlProgressBar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    drawControlProgressBarContents(option, painter, widget);
    drawControlProgressBarGroove(option, painter, widget);
}

void Contemporary::drawControlHeaderSection(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionHeader)
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    if (opt->state & State_Sunken) {
        painter->setBrush(ACCENT_PRESS_COLOR);
    } else {
        painter->setBrush(ACCENT_COLOR);
    }
    painter->drawRect(opt->rect);

    painter->setPen(WINDOW_COLOR);
    painter->drawLine(opt->rect.topRight(), opt->rect.bottomRight());

    if (opt->state & State_UpArrow) {
    }
}

void Contemporary::drawControlHeaderEmptyArea(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionProgressBar)
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    painter->setBrush(ACCENT_COLOR);
    painter->drawRect(opt->rect);
}

void Contemporary::drawControlMenubarEmptyArea(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOption);
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    painter->setBrush(WINDOW_COLOR);
    painter->drawRect(opt->rect);
}

void Contemporary::drawControlMenubarItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorMenuBarItem(option, painter, widget).performPaint();
}

void Contemporary::drawControlToolbar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionToolBar);
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    painter->setPen(WINDOW_COLOR);
    painter->drawRect(opt->rect);
}

void Contemporary::drawControlPushButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return;

    if (opt->features & QStyleOptionButton::CommandLinkButton) {
        drawPrimitive(PE_PanelButtonCommand, option, painter, widget);
    } else {
        QCommonStyle::drawControl(CE_PushButton, option, painter, widget);
    }
}

void Contemporary::drawControlMenuItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorMenuItem(option, painter, widget).performPaint();
}

void Contemporary::drawControlPushButtonLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    tPaintCalculator paintCalculator = paintCalculatorPushButton(option, painter, widget);
    paintCalculator.performPaint("icon");
    paintCalculator.performPaint("text");
    paintCalculator.performPaint("supplementaryText");
}

void Contemporary::drawControlComboBoxLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    tPaintCalculator paintCalculator = paintCalculatorComboBox(option, painter, widget);
    paintCalculator.performPaint("text");
}

void Contemporary::drawComplexComboBox(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    tPaintCalculator paintCalculator = paintCalculatorComboBox(option, painter, widget);
    paintCalculator.performPaint("background");
    paintCalculator.performPaint("arrow");
}

void Contemporary::drawComplexToolButton(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorToolButton(option, painter, widget).performPaint();
}

void Contemporary::drawComplexSlider(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorSlider(option, painter, widget).performPaint();
}

void Contemporary::drawComplexScrollBar(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorScrollBar(option, painter, widget).performPaint();
}

void Contemporary::drawControlCheckboxLabel(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    paintCalculatorCheckBox(option, painter, widget, false).performPaint("contents");
}

QRect Contemporary::subElementCheckBoxIndicator(const QStyleOption* option, const QWidget* widget) const {
    tPaintCalculator paintCalculator = paintCalculatorCheckBox(option, nullptr, widget, false);
    return paintCalculator.boundsOf("indicator").toRect();
}

QRect Contemporary::subElementCheckBoxContents(const QStyleOption* option, const QWidget* widget) const {
    tPaintCalculator paintCalculator = paintCalculatorCheckBox(option, nullptr, widget, false);
    return paintCalculator.boundsOf("contents").toRect();
}

QRect Contemporary::subElementPushButtonContents(const QStyleOption* option, const QWidget* widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    return opt->rect;
}

QRect Contemporary::subElementPushButtonFocusRect(const QStyleOption* option, const QWidget* widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    return opt->rect;
}

QStyle::SubControl Contemporary::hitTestSlider(const QStyleOptionComplex* option, const QPoint& point, const QWidget* widget) const {
    QStringList areas = paintCalculatorSlider(option, nullptr, widget).hitTest(point);
    if (areas.contains("thumb")) return QStyle::SC_SliderHandle;
    return QStyle::SC_SliderGroove;
}

QStyle::SubControl Contemporary::hitTestScrollBar(const QStyleOptionComplex* option, const QPoint& point, const QWidget* widget) const {
    QStringList areas = paintCalculatorScrollBar(option, nullptr, widget).hitTest(point);
    if (areas.contains("slider")) return QStyle::SC_ScrollBarSlider;
    return QStyle::SC_ScrollBarGroove;
}

tPaintCalculator Contemporary::paintCalculatorCheckBox(const QStyleOption* option, QPainter* painter, const QWidget* widget, bool isRadioButton) const {
    OPT_CAST(QStyleOptionButton);
    //    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(option->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    QRect indicatorRect;
    indicatorRect.setWidth(SC_DPI(11)); // 11 to take the pen width into account
    indicatorRect.setHeight(SC_DPI(11));
    indicatorRect.moveCenter(QPoint(SC_DPI(13), option->rect.center().y()));
    calculator.addRect("indicator", indicatorRect, [=](QRectF bounds) {
        painter->save();
        painter->setPen(WINDOW_TEXT_COLOR);

        QColor innerColor;
        if (option->state & State_Sunken) {
            innerColor = WINDOW_PRESS_COLOR;
        } else if (option->state & State_MouseOver) {
            innerColor = WINDOW_HOVER_COLOR;
        } else {
            innerColor = WINDOW_COLOR;
        }

        if (isRadioButton) {
            painter->setRenderHint(QPainter::Antialiasing);
            painter->setBrush(WINDOW_TEXT_COLOR);
            painter->drawEllipse(option->rect);

            painter->setBrush(innerColor);

            QRectF onRect = option->rect;
            onRect.setSize(onRect.size() *= 0.6);
            onRect.moveCenter(QRectF(option->rect).center());

            RadioButtonAnimation* anim = qobject_cast<RadioButtonAnimation*>(d->anim->a(widget));
            if (!anim) {
                anim = new RadioButtonAnimation(nullptr);
                anim->deleteLater();
            }

            if (option->state & State_On) {
                anim->setIndicatorRect(onRect);
            } else {
                anim->setIndicatorRect(option->rect);
            }

            painter->drawEllipse(anim->currentIndicatorRect());
        } else {
            Qt::CheckState checkState = Qt::Unchecked;
            if (option->state & State_NoChange) checkState = Qt::PartiallyChecked;
            if (option->state & State_On) checkState = Qt::Checked;

            bool needsFastForward = false;
            CheckboxAnimation* anim = qobject_cast<CheckboxAnimation*>(d->anim->a(widget));
            if (!anim) {
                anim = new CheckboxAnimation(nullptr);
                anim->deleteLater();
                needsFastForward = true;
            }

            anim->setIndicatorRect(option->rect);
            anim->setCheckState(checkState);
            anim->fastForward();

            painter->setBrush(WINDOW_TEXT_COLOR);
            painter->drawRect(option->rect);

            painter->setBrush(innerColor);
            painter->drawRect(anim->currentIndicatorRect());

            painter->setRenderHint(QPainter::Antialiasing);
            painter->setPen(QPen(innerColor, SC_DPI_T(1.5, qreal), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawPolyline(anim->currentCheckPolygon());
        }
        painter->restore();
    });

    if (opt) {
        QRect textRect;
        textRect.setLeft(indicatorRect.right() + pixelMetric(QStyle::PM_CheckBoxLabelSpacing, option, widget));
        textRect.setWidth(opt->fontMetrics.horizontalAdvance(opt->text) + 1);
        textRect.setTop(opt->rect.height() / 2 - opt->fontMetrics.height() / 2);
        textRect.setHeight(opt->fontMetrics.height());
        calculator.addRect("contents", textRect, [=](QRectF bounds) {
            drawItemText(painter, opt->rect, textHorizontalAlignment | Qt::AlignVCenter, opt->palette, opt->state & State_Enabled, opt->text, QPalette::WindowText);
        });
    }

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorPushButton(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    calculator.addRect("background", opt->rect, [=](QRectF bounds) {
        // Get animation
        PushButtonAnimation* anim = qobject_cast<PushButtonAnimation*>(d->anim->a(widget));

        QColor backgroundColor;
        if (anim == nullptr) {
            backgroundColor = buttonBackground(opt, widget);
        } else {
            anim->setTargetColor(buttonBackground(opt, widget));
            backgroundColor = anim->drawColor();
        }

        painter->setPen(Qt::transparent);
        painter->setBrush(backgroundColor);
        painter->drawRect(opt->rect);

        if (widget && widget->property("type") == "positive") {
            // Draw border around button
            QPen pen;
            pen.setColor(QColor(0, 255, 0));
            pen.setWidth(SC_DPI_W(3, widget));
            painter->setPen(pen);

            painter->setBrush(Qt::transparent);
            painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
        }
    });

    QString text = opt->text;

    QRect textRect, iconRect;
    textRect.setLeft(opt->rect.left() + (opt->rect.width() / 2) - (opt->fontMetrics.horizontalAdvance(text) / 2));
    textRect.setWidth(opt->fontMetrics.horizontalAdvance(text) + 1);
    textRect.setTop(opt->rect.top() + (opt->rect.height() / 2) - (opt->fontMetrics.height() / 2));
    textRect.setHeight(opt->fontMetrics.height());

    auto supplementaryText = widget->property("supplementaryText").toString();
    QRect supplementaryTextRect = textRect;
    QFont supplementaryTextFont = widget->font();
    supplementaryTextFont.setPointSizeF(supplementaryTextFont.pointSizeF() * 0.7);

    if (widget && !supplementaryText.isEmpty()) {
        QFontMetrics supplementaryTextFontMetrics(supplementaryTextFont);

        supplementaryTextRect.setWidth(supplementaryTextFontMetrics.horizontalAdvance(supplementaryText) + 1);
        supplementaryTextRect.setHeight(supplementaryTextFontMetrics.height());

        textRect.moveTop(opt->rect.top() + (opt->rect.height() / 2) - ((opt->fontMetrics.height() + supplementaryTextRect.height()) / 2) - SC_DPI_W(1, widget));
        supplementaryTextRect.moveTop(textRect.bottom() + SC_DPI_W(2, widget));

        if (supplementaryTextRect.width() > textRect.width()) {
            // Center the text based on the supplementary text rect
            supplementaryTextRect.moveLeft(opt->rect.left() + (opt->rect.width() / 2) - (supplementaryTextRect.width() / 2));
            textRect.moveLeft(supplementaryTextRect.left());
        }
    }

    if (!opt->icon.isNull()) {
        int fullWidth = textRect.width() + opt->iconSize.width();
        int iconLeft = opt->rect.left() + (opt->rect.width() / 2) - (fullWidth / 2);

        iconRect.setLeft(iconLeft);
        iconRect.setTop(opt->rect.top() + (opt->rect.height() / 2) - (opt->iconSize.height() / 2));
        iconRect.setSize(opt->iconSize);

        textRect.moveLeft(iconRect.right() + SC_DPI(4));
        supplementaryTextRect.moveLeft(iconRect.right() + SC_DPI(4));

        QIcon icon = opt->icon;
        QImage image = icon.pixmap(opt->iconSize).toImage();
        image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
        libContemporaryCommon::tintImage(image, WINDOW_TEXT_COLOR);

        calculator.addRect("icon", iconRect, [=](QRectF paintBounds) {
            painter->drawImage(paintBounds, image);
        });
    }

    // Draw text
    if (widget && !supplementaryText.isEmpty()) {
        calculator.addRect("supplementaryText", supplementaryTextRect, [painter, supplementaryTextFont, supplementaryText](QRectF paintBounds) {
            painter->save();
            painter->setFont(supplementaryTextFont);
            painter->drawText(paintBounds, Qt::AlignLeft, supplementaryText);
            painter->restore();
        });
    }

    calculator.addRect("text", textRect, [=](QRectF paintBounds) {
        drawItemText(painter, paintBounds.toRect(), Qt::AlignCenter, opt->palette, opt->state & QStyle::State_Enabled, opt->text, QPalette::ButtonText);
    });

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorProgressBar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionProgressBar);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    calculator.addRect("contents", opt->rect, [=](QRectF paintBounds) {
        tPaintCalculator paintCalculator;
        paintCalculator.setLayoutDirection(opt->direction);
        paintCalculator.setPainter(painter);
        paintCalculator.setDrawBounds(opt->rect);

        if (opt->maximum == 0 && opt->minimum == 0) {
            // Indeterminate progress bar
            if (!d->indeterminateTimer->isActive()) {
                // Start the timer here to save CPU time if the timer is not needed.
                d->indeterminateTimer->start();
            }

            int fullWidth = opt->rect.width() * 4;
            int stageProgress = d->indetermiateProgressSection % fullWidth;
            int stage = stageProgress / opt->rect.width();
            stageProgress = stageProgress % opt->rect.width();

            switch (stage) {
                case 0:
                    {
                        paintCalculator.addRect(QRect(opt->rect.left(), opt->rect.top(), stageProgress, opt->rect.height()), [=](QRectF paintBounds) {
                            painter->setPen(Qt::transparent);
                            painter->setBrush(WINDOW_COLOR);
                            painter->drawRect(opt->rect);
                            painter->setBrush(ACCENT_COLOR);
                            painter->drawRect(paintBounds);
                        });
                        break;
                    }
                case 1:
                    {
                        paintCalculator.addRect(QRect(stageProgress, opt->rect.top(), stageProgress * 2, opt->rect.height()), [=](QRectF paintBounds) {
                            painter->setPen(Qt::transparent);
                            painter->setBrush(ACCENT_COLOR);
                            painter->drawRect(opt->rect);
                            painter->setBrush(WINDOW_COLOR);
                            painter->drawRect(paintBounds);
                        });
                        break;
                    }
                case 2:
                    {
                        paintCalculator.addRect(QRect(opt->rect.left(), opt->rect.top(), stageProgress, opt->rect.height()), [=](QRectF paintBounds) {
                            painter->setPen(Qt::transparent);
                            painter->setBrush(ACCENT_COLOR);
                            painter->drawRect(opt->rect);
                            painter->setBrush(WINDOW_COLOR);
                            painter->drawRect(paintBounds);
                        });
                        break;
                    }
                case 3:
                    {
                        paintCalculator.addRect(QRect(stageProgress, opt->rect.top(), stageProgress * 2, opt->rect.height()), [=](QRectF paintBounds) {
                            painter->setPen(Qt::transparent);
                            painter->setBrush(WINDOW_COLOR);
                            painter->drawRect(opt->rect);
                            painter->setBrush(ACCENT_COLOR);
                            painter->drawRect(paintBounds);
                        });
                    }
            }

            scheduleRepaint(widget);
        } else {
            int width = (((float) opt->progress - (float) opt->minimum) / ((float) opt->maximum - (float) opt->minimum)) * opt->rect.width();
            paintCalculator.addRect(QRect(opt->rect.left(), opt->rect.top(), width, opt->rect.bottom()), [=](QRectF paintBounds) {
                painter->setPen(Qt::transparent);
                painter->setBrush(WINDOW_COLOR);
                painter->drawRect(opt->rect);
                painter->setBrush(ACCENT_COLOR);
                painter->drawRect(paintBounds);
            });
        }

        paintCalculator.performPaint();
    });

    calculator.addRect("groove", opt->rect, [=](QRectF paintBounds) {
        painter->setPen(WINDOW_TEXT_COLOR);
        painter->setBrush(Qt::transparent);
        painter->drawRect(paintBounds.adjusted(1, 1, -1, -1));
    });

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorToolButton(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionToolButton);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    calculator.addRect("background", opt->rect, [=](QRectF bounds) {
        // Get animation
        PushButtonAnimation* anim = qobject_cast<PushButtonAnimation*>(d->anim->a(widget));

        QStyleOptionButton buttonStyle;
        buttonStyle.direction = opt->direction;
        buttonStyle.state = opt->state;
        buttonStyle.features = QStyleOptionButton::Flat;
        buttonStyle.rect = opt->rect;

        QColor backgroundColor;
        if (anim == nullptr) {
            backgroundColor = buttonBackground(&buttonStyle, widget);
        } else {
            anim->setTargetColor(buttonBackground(&buttonStyle, widget));
            backgroundColor = anim->drawColor();
        }

        painter->setPen(Qt::transparent);
        painter->setBrush(backgroundColor);
        painter->drawRect(opt->rect);
    });

    Qt::ToolButtonStyle style = opt->toolButtonStyle;
    if (opt->icon.isNull()) {
        if (style == Qt::ToolButtonTextBesideIcon || style == Qt::ToolButtonTextUnderIcon) style = Qt::ToolButtonTextOnly;
    }

    bool drawText = false, drawIcon = false;
    QRect textRect, iconRect;
    iconRect.setSize(opt->iconSize);
    textRect.setHeight(opt->fontMetrics.height());
    textRect.setWidth(opt->fontMetrics.horizontalAdvance(opt->text));

    switch (style) {
        case Qt::ToolButtonIconOnly:
            {
                // Center the icon
                iconRect.moveCenter(opt->rect.center());
                drawIcon = true;
                break;
            }
        case Qt::ToolButtonTextOnly:
            {
                // Center the text
                textRect.moveCenter(opt->rect.center());
                drawText = true;
                break;
            }
        case Qt::ToolButtonTextBesideIcon:
            {
                // Adjust elements relative to each other
                iconRect.moveTopLeft(QPoint(0, 0));
                textRect.moveCenter(iconRect.center());
                textRect.moveLeft(iconRect.right() + SC_DPI(9));

                // Now center them within the parent rectangle
                QRect bounding = iconRect.united(textRect);
                bounding.moveCenter(opt->rect.center());
                iconRect.moveTopLeft(bounding.topLeft());
                textRect.moveTopLeft(textRect.topLeft() + bounding.topLeft());

                drawText = true;
                drawIcon = true;
                break;
            }
        case Qt::ToolButtonTextUnderIcon:
            {
                // Adjust elements relative to each other
                iconRect.moveTopLeft(QPoint(0, 0));
                textRect.moveCenter(iconRect.center());
                textRect.moveTop(iconRect.bottom() + SC_DPI(9));

                // Now center them within the parent rectangle
                QRect bounding = iconRect.united(textRect);
                bounding.moveCenter(opt->rect.center());
                iconRect.moveTopLeft(bounding.topLeft());
                textRect.moveTopLeft(textRect.topLeft() + bounding.topLeft());

                drawText = true;
                drawIcon = true;
                break;
            }
        case Qt::ToolButtonFollowStyle:
            break;
    }

    if (drawIcon) {
        calculator.addRect("icon", iconRect, [=](QRectF paintBounds) {
            QIcon icon = opt->icon;
            QImage image = icon.pixmap(opt->iconSize).toImage();
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            libContemporaryCommon::tintImage(image, WINDOW_TEXT_COLOR);

            painter->drawImage(paintBounds, image);
        });
    }

    if (drawText) {
        // Draw text
        calculator.addRect("text", textRect, [=](QRectF paintBounds) {
            drawItemText(painter, paintBounds.toRect(), Qt::AlignCenter, opt->palette, opt->state & QStyle::State_Enabled, opt->text, QPalette::ButtonText);
        });
    }

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorSlider(const QStyleOptionComplex* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionSlider);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    int thumbSize = qMin(option->rect.width(), option->rect.height());
    QRect thumbRect;
    thumbRect.setSize(QSize(thumbSize, thumbSize));
    thumbRect.moveCenter(opt->rect.center());

    QRect leftGroove = option->rect;
    QRect rightGroove = option->rect;

    int position = sliderPositionFromValue(opt->minimum, opt->maximum, opt->sliderValue, opt->rect.width() - thumbSize, opt->upsideDown);
    if (opt->orientation == Qt::Horizontal) {
        thumbRect.moveLeft(position);
        leftGroove.setWidth(position);
        rightGroove.setLeft(position);
    } else {
        thumbRect.moveTop(position);
        leftGroove.setHeight(position);
        rightGroove.setTop(position);
    }

    calculator.addRect("leftGroove", leftGroove, [=](QRectF paintBounds) {
        painter->setPen(Qt::transparent);
        painter->setBrush(opt->palette.color(QPalette::Highlight));
        painter->drawRect(paintBounds);
    });

    calculator.addRect("border", option->rect, [=](QRectF paintBounds) {
        painter->setPen(opt->palette.color(QPalette::WindowText));
        painter->setBrush(Qt::transparent);
        painter->drawRect(paintBounds.adjusted(1, 1, -1, -1));
    });

    calculator.addRect("thumb", thumbRect, [=](QRectF paintBounds) {
        painter->setPen(opt->palette.color(QPalette::WindowText));
        painter->setBrush(opt->palette.color(QPalette::Window));
        painter->drawRect(paintBounds.adjusted(1, 1, -1, -1));
    });

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorComboBox(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionComboBox);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    calculator.addRect("background", opt->rect, [=](QRectF drawBounds) {
        painter->setPen(WINDOW_TEXT_COLOR);
        QBrush brush = QBrush(WINDOW_COLOR);

        if (opt->state & QStyle::State_Enabled) {
            if (opt->state & QStyle::State_Item) {
                brush = QBrush(ACCENT_PRESS_COLOR);
            } else {
                if (opt->state & QStyle::State_MouseOver) {
                    brush = QBrush(ACCENT_HOVER_COLOR);
                }

                if (opt->state & QStyle::State_On) {
                    brush = QBrush(ACCENT_PRESS_COLOR);
                }
            }
        }
        painter->setBrush(brush);
        painter->drawRect(drawBounds.adjusted(0, 0, -1, -1));
    });

    QRect textRect;
    textRect.setWidth(opt->fontMetrics.horizontalAdvance(opt->currentText) + 1);
    textRect.setHeight(opt->fontMetrics.height());
    textRect.moveCenter(opt->rect.center());

    if (opt->currentIcon.isNull()) {
        textRect.moveLeft(opt->rect.left() + SC_DPI(4));
    } else {
        textRect.moveLeft(opt->rect.left() + SC_DPI(8) + opt->iconSize.width());
    }

    calculator.addRect("text", textRect, [=](QRectF drawBounds) {
        if (opt->editable) return; // Don't draw any text if it is editable
        drawItemText(painter, drawBounds.toRect(), textHorizontalAlignment | Qt::AlignVCenter, opt->palette, opt->state & State_Enabled, opt->currentText, QPalette::WindowText);
    });

    if (opt->subControls & SC_ComboBoxArrow) {
        QRect arrowBounds;
        arrowBounds.setWidth(opt->rect.height() / 2);
        arrowBounds.setHeight(opt->rect.height() / 2);
        arrowBounds.moveBottom(opt->rect.bottom() - SC_DPI(1));
        arrowBounds.moveRight(opt->rect.right() - SC_DPI(1));
        calculator.addRect("arrow", arrowBounds, [=](QRectF drawBounds) {
            painter->setBrush(WINDOW_TEXT_COLOR);
            painter->setPen(Qt::transparent);

            QPolygonF triangle;
            if (reverse) {
                triangle.append(drawBounds.topLeft());
                triangle.append(drawBounds.bottomRight());
                triangle.append(drawBounds.bottomLeft());
            } else {
                triangle.append(drawBounds.topRight());
                triangle.append(drawBounds.bottomRight());
                triangle.append(drawBounds.bottomLeft());
            }
            painter->drawPolygon(triangle);
        });
    }

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorMenuBarItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionMenuItem);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    if (opt->state & State_MouseOver | opt->state & State_Selected) {
        calculator.addRect("selection", opt->rect, [=](QRectF drawBounds) {
            painter->setPen(Qt::transparent);
            painter->setBrush(ACCENT_COLOR);
            painter->drawRect(drawBounds);
        });
    }

    calculator.addRect("text", opt->rect, [=](QRectF drawBounds) {
        drawItemText(painter, drawBounds.toRect(), Qt::AlignCenter, opt->palette, opt->state & State_Enabled, opt->text, QPalette::Text);
    });

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorMenuItem(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionMenuItem);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    MenuItemAnimation* anim = qobject_cast<MenuItemAnimation*>(d->anim->a(widget));

    switch (opt->menuItemType) {
        case QStyleOptionMenuItem::Normal:
        case QStyleOptionMenuItem::SubMenu:
            {
                QColor backgroundColor;
                QPalette::ColorRole textColor;

                bool selected = false;
                if (opt->state & QStyle::State_Selected || opt->state & QStyle::State_Sunken) {
                    backgroundColor = opt->palette.color(QPalette::Highlight);
                    textColor = QPalette::HighlightedText;
                    selected = true;
                    if (anim) anim->setHoverRect(opt->rect);
                } else {
                    backgroundColor = opt->palette.color(QPalette::Window);
                    textColor = QPalette::WindowText;
                }

                if (!selected && anim && anim->lastSetRect() == opt->rect) anim->clearHoverRect();

                calculator.addRect("background", opt->rect, [=](QRectF drawBounds) {
                    painter->setPen(Qt::transparent);
                    painter->setBrush(WINDOW_COLOR);
                    painter->drawRect(drawBounds);

                    if (anim) {
                        if (anim->hoverRect().intersects(opt->rect)) {
                            painter->setBrush(ACCENT_COLOR);
                            painter->drawRect(anim->hoverRect());
                        }
                    } else if (selected) {
                        painter->setBrush(ACCENT_COLOR);
                        painter->drawRect(opt->rect);
                    }
                });

                if (opt->checkType != QStyleOptionMenuItem::NotCheckable) {
                    QStyleOptionButton indicatorStyle;
                    indicatorStyle.rect = opt->rect;
                    QRect indicatorRect = subElementRect(SE_CheckBoxIndicator, &indicatorStyle, nullptr);

                    QRect checkRect;
                    checkRect.setSize(indicatorRect.size());
                    checkRect.moveLeft(opt->rect.left() + SC_DPI(2));
                    checkRect.moveTop(opt->rect.top() + opt->rect.height() / 2 - indicatorRect.height() / 2);

                    calculator.addRect("check", checkRect, [=](QRectF drawBounds) {
                        QStyleOptionButton indicatorStyle;
                        indicatorStyle.rect = drawBounds.toRect();
                        if (opt->checked) indicatorStyle.state |= State_On;

                        drawPrimitive(opt->checkType == QStyleOptionMenuItem::Exclusive ? PE_IndicatorRadioButton : PE_IndicatorCheckBox, &indicatorStyle, painter, nullptr);
                    });
                } else if (!opt->icon.isNull()) {
                    QRect iconRect;
                    iconRect.setWidth(pixelMetric(PM_SmallIconSize, opt, widget));
                    iconRect.setHeight(pixelMetric(PM_SmallIconSize, opt, widget));
                    iconRect.moveLeft(opt->rect.left() + SC_DPI(2));
                    iconRect.moveTop(opt->rect.top() + opt->rect.height() / 2 - pixelMetric(PM_SmallIconSize, opt, widget) / 2);

                    calculator.addRect("icon", iconRect, [=](QRectF drawBounds) {
                        QImage icon = opt->icon.pixmap(pixelMetric(PM_SmallIconSize, opt, widget)).toImage();
                        libContemporaryCommon::tintImage(icon, WINDOW_TEXT_COLOR);
                        painter->drawImage(drawBounds, icon);
                    });
                }

                QStringList textParts = opt->text.split("\t");
                QString menuText = textParts.at(0);

                QRect textRect;
                textRect.setHeight(opt->fontMetrics.height());
                textRect.setWidth(opt->fontMetrics.horizontalAdvance(menuText) + 1);
                textRect.moveTop(opt->rect.top() + opt->rect.height() / 2 - opt->fontMetrics.height() / 2);
                textRect.moveLeft(opt->rect.left() + SC_DPI(4) + pixelMetric(PM_SmallIconSize, opt, widget));

                calculator.addRect("text", textRect, [=](QRectF drawBounds) {
                    drawItemText(painter, drawBounds.toRect(), textHorizontalAlignment, opt->palette, opt->state & QStyle::State_Enabled, menuText, textColor);
                });

                if (textParts.count() == 2) {
                    QFont shortcutFont = opt->font;
                    shortcutFont.setPointSizeF(shortcutFont.pointSizeF() * 0.7);

                    QString shortcutText = textParts.at(1);
                    QPixmap shortcutPixmap = this->shortcutPixmap(shortcutFont, opt->palette, shortcutText);

                    QRect shortcutRect;
                    shortcutRect.setSize(shortcutPixmap.size());
                    shortcutRect.moveCenter(textRect.center());
                    shortcutRect.moveRight(opt->rect.right() - SC_DPI(4));
                    if (shortcutRect.left() < textRect.right() + SC_DPI(4)) shortcutRect.moveLeft(textRect.right() + SC_DPI(4));
                    calculator.addRect("shortcut", shortcutRect, [=](QRectF drawBounds) {
                        painter->drawPixmap(drawBounds.toRect(), shortcutPixmap);
                    });
                }

                if (opt->menuItemType == QStyleOptionMenuItem::SubMenu) {
                    QRect arrowRect;
                    arrowRect.setSize(SC_DPI_T(QSize(6, 8), QSize));
                    arrowRect.moveCenter(textRect.center());
                    arrowRect.moveRight(opt->rect.right() - SC_DPI(4));
                    if (arrowRect.left() < textRect.right() + SC_DPI(4)) arrowRect.moveLeft(textRect.right() + SC_DPI(4));

                    calculator.addRect("arrow", arrowRect, [=](QRectF drawBounds) {
                        QPolygonF arrow;
                        if (opt->direction == Qt::LeftToRight) {
                            arrow.append(drawBounds.topLeft());
                            arrow.append(drawBounds.bottomLeft());
                            arrow.append(QPointF(drawBounds.right(), drawBounds.center().y()));
                        } else {
                            arrow.append(drawBounds.topRight());
                            arrow.append(drawBounds.bottomRight());
                            arrow.append(QPointF(drawBounds.left(), drawBounds.center().y()));
                        }

                        painter->setPen(Qt::transparent);
                        painter->setBrush(opt->palette.color(textColor));
                        painter->drawPolygon(arrow);
                    });
                }

                break;
            }
        case QStyleOptionMenuItem::Separator:
            {
                if (opt->text == "") {
                    calculator.addRect(QRect(opt->rect.left(), opt->rect.top(), opt->rect.width(), 1), [=](QRectF drawBounds) {
                        painter->setPen(opt->palette.color(QPalette::WindowText));
                        painter->drawLine(drawBounds.topLeft(), drawBounds.topRight());
                    });
                } else {
                    QRect textRect;
                    textRect.setWidth(opt->fontMetrics.horizontalAdvance(opt->text) + 1);
                    textRect.setHeight(opt->fontMetrics.height());
                    textRect.moveCenter(opt->rect.center());
                    if (textRect.left() < opt->rect.left()) textRect.moveLeft(opt->rect.left());

                    calculator.addRect(textRect, [=](QRectF drawBounds) {
                        painter->setBrush(opt->palette.color(QPalette::WindowText));
                        painter->drawRect(opt->rect);
                        drawItemText(painter, drawBounds.toRect(), Qt::AlignCenter, opt->palette, opt->state & QStyle::State_Enabled, opt->text, QPalette::Window);
                    });
                }
            }
    }

    return calculator;
}

tPaintCalculator Contemporary::paintCalculatorScrollBar(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    OPT_CAST(QStyleOptionSlider);
    if (opt == nullptr) return tPaintCalculator();
    OPT_VARS;

    tPaintCalculator calculator;
    calculator.setLayoutDirection(opt->direction);
    calculator.setPainter(painter);
    calculator.setDrawBounds(option->rect);

    ScrollBarAnimation* anim = qobject_cast<ScrollBarAnimation*>(d->anim->a(widget));

    double widthScaling = option->state & State_MouseOver ? 1 : 0.5;
    if (anim) {
        anim->setHasMouseFocus(option->state & State_MouseOver);
        anim->setCurrentValue(opt->sliderValue);
        widthScaling = anim->widthScaling();
    }

    // Always ask for an LTR version: we don't want to double flip the slider
    QStyleOptionSlider subControlRect(*opt);
    subControlRect.direction = Qt::LeftToRight;
    QRectF sliderRect = this->subControlRect(QStyle::CC_ScrollBar, &subControlRect, SC_ScrollBarSlider, widget);

    if (opt->orientation == Qt::Vertical) {
        int right = sliderRect.right();
        sliderRect.setWidth(sliderRect.width() * widthScaling);
        sliderRect.moveRight(right);
    } else {
        int bottom = sliderRect.bottom();
        sliderRect.setHeight(sliderRect.height() * widthScaling);
        sliderRect.moveBottom(bottom);
    }

    calculator.addRect("slider", sliderRect, [=](QRectF drawBounds) {
        if (anim) painter->setOpacity(anim->opacity());
        painter->fillRect(drawBounds, ACCENT_COLOR);
    });

    return calculator;
}

QPixmap Contemporary::shortcutPixmap(const QFont font, const QPalette pal, const QString text) const {
    QStringList parts = text.split("+");
    QFontMetrics metrics(font);

    QPainter* painter;

    tPaintCalculator calculator;
    int cx = 0;
    for (QString part : parts) {
        QRect bounds;
        bounds.setWidth(metrics.horizontalAdvance(part) + SC_DPI(10));
        bounds.setHeight(metrics.height() + SC_DPI(4));
        bounds.moveTop(0);
        bounds.moveLeft(cx);

        calculator.addRect(bounds, [=, &painter](QRectF drawBounds) {
            painter->setOpacity(0.7);
            painter->setPen(Qt::transparent);
            painter->setBrush(pal.color(QPalette::WindowText));
            painter->drawRoundedRect(drawBounds, 2, 2);

            painter->setOpacity(1);
            painter->setPen(pal.color(QPalette::Window));
            painter->setBrush(Qt::transparent);
            painter->drawText(drawBounds, Qt::AlignCenter, part);
        });

        cx += bounds.width() + SC_DPI(2);
    }

    QPixmap pixmap(calculator.sizeWithMargins().toSize());
    pixmap.fill(Qt::transparent);
    painter = new QPainter(&pixmap);

    calculator.setDrawBounds(calculator.boundingRect());
    calculator.setPainter(painter);
    calculator.performPaint();

    painter->end();
    delete painter;

    return pixmap;
}

QColor Contemporary::buttonBackground(const QStyleOptionButton* option, const QWidget* widget) const {
    QColor background;
    if (widget && widget->property("type") == "destructive") {
        if (option->state & State_On || option->state & State_Sunken || option->state & State_Selected) {
            background = QColor(100, 0, 0);
        } else if (option->state & State_MouseOver) {
            background = QColor(200, 0, 0);
        } else {
            background = QColor(150, 0, 0);
        }
    } else if (option->features & QStyleOptionButton::Flat) {
        if (option->state & State_On || option->state & State_Sunken || option->state & State_Selected) {
            background = QColor(0, 0, 0, 100);
        } else if (option->state & State_MouseOver) {
            background = QColor(255, 255, 255, 100);
        } else {
            background = QColor(255, 255, 255, 0);
        }
    } else {
        if (option->state & State_On || option->state & State_Sunken || option->state & State_Selected) {
            background = BUTTON_PRESS_COLOR;
        } else if (option->state & State_MouseOver) {
            background = BUTTON_HOVER_COLOR;
        } else {
            background = BUTTON_COLOR;
        }
    }
    return background;
}

QRect Contemporary::subElementCheckBoxFocusRect(const QStyleOption* option, const QWidget* widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    QRect indicatorRect;
    indicatorRect.setLeft(SC_DPI_W(2, widget));
    indicatorRect.setWidth(SC_DPI_W(12, widget));
    indicatorRect.setTop(opt->rect.height() / 2 - SC_DPI_W(12, widget) / 2);
    indicatorRect.setHeight(SC_DPI_W(12, widget));

    QRect rect;
    rect.setLeft(0);
    rect.setWidth(SC_DPI_W(16, widget));
    rect.setTop(opt->rect.height() / 2 - SC_DPI_W(16, widget) / 2);
    rect.setHeight(SC_DPI_W(16, widget));
    return rect;
}

QRect Contemporary::subElementCheckBoxClickRect(const QStyleOption* option, const QWidget* widget) const {
    return paintCalculatorCheckBox(option, nullptr, widget, false).visualBoundingRect().toRect();
}

void Contemporary::polish(QApplication* application) {
    d->focusDecorations->setApplication(application);
}

void Contemporary::unpolish(QApplication* application) {
    if (d->focusDecorations->application() == application) d->focusDecorations->clearApplication();
}

void Contemporary::drawControlTabBarTabShape(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    //    painter->fillRect(option->rect, Qt::red);
    OPT_CAST(QStyleOptionTab);
    if (opt == nullptr) return;
    OPT_VARS;

    QStyleOptionButton buttonStyle;
    buttonStyle.state = opt->state;
    buttonStyle.rect = opt->rect;
    buttonStyle.direction = opt->direction;

    tPaintCalculator paintCalculator = paintCalculatorPushButton(&buttonStyle, painter, nullptr);
    paintCalculator.performPaint();
}

void Contemporary::drawPrimitiveIndicatorTabClose(const QStyleOption* option, QPainter* painter, const QWidget* widget) const {
    QColor color(127, 127, 127, 100);
    if (option->state & State_MouseOver) {
        color = Qt::red;
    }

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(color);
    painter->setPen(Qt::transparent);
    painter->drawEllipse(option->rect);
}
void Contemporary::drawControlShapedFrame(const QStyleOption* option,
    QPainter* painter,
    const QWidget* widget) const {
    OPT_CAST(QStyleOptionFrame);
    if (opt == nullptr) return;

    QPolygonF frame;
    switch (opt->frameShape) {
        case QFrame::NoFrame:
            break;
        case QFrame::Box:
        case QFrame::StyledPanel:
        case QFrame::Panel:
        case QFrame::WinPanel:
            frame.append(opt->rect.topLeft());
            frame.append(opt->rect.topRight());
            frame.append(opt->rect.bottomRight());
            frame.append(opt->rect.bottomLeft());
            frame.append(opt->rect.topLeft());
            break;
        case QFrame::HLine:
            frame.append(opt->rect.topLeft());
            frame.append(opt->rect.topRight());
            break;
        case QFrame::VLine:
            frame.append(opt->rect.topLeft());
            frame.append(opt->rect.bottomLeft());
            break;
    }

    painter->setPen(QPen(libContemporaryCommon::lineColor(opt->palette.color(QPalette::WindowText)), SC_DPI_W(1, widget)));
    painter->drawPolyline(frame);
}
