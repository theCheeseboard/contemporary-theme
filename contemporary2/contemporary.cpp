#include "contemporary.h"

Contemporary::Contemporary() {
    oldStyle = new Style();
    anim = new AnimationEngine;

    indeterminateTimer = new QTimer(this);
    if (theLibsGlobal::instance()->powerStretchEnabled()) {
        indeterminateTimer->setInterval(1000 / 5);
    } else {
        indeterminateTimer->setInterval(1000 / 60);
    }
    connect(indeterminateTimer, &QTimer::timeout, [=]() {
        //Yes, this can overflow - and that's good. :)
        if (theLibsGlobal::instance()->powerStretchEnabled()) {
            indetermiateProgressSection += 120;
        } else {
            indetermiateProgressSection += 10;
        }
    });

    connect(theLibsGlobal::instance(), &theLibsGlobal::powerStretchChanged, [=](bool isOn) {
        if (isOn) {
            indeterminateTimer->setInterval(1000 / 5);
        } else {
            indeterminateTimer->setInterval(1000 / 60);
        }
    });

    animations = new QMap<WId, tVariantAnimation*>();
}

Contemporary::~Contemporary() {
    delete oldStyle;
    delete anim;
    delete animations;
}

void Contemporary::scheduleRepaint(const QWidget *widget, int after) const {
    QTimer* updateTimer = new QTimer;
    updateTimer->setInterval(after);
    updateTimer->setSingleShot(true);
    connect(widget, SIGNAL(destroyed(QObject*)), updateTimer, SLOT(stop()));
    connect(widget, SIGNAL(destroyed(QObject*)), updateTimer, SLOT(deleteLater()));
    connect(updateTimer, SIGNAL(timeout()), widget, SLOT(repaint()));
    connect(updateTimer, SIGNAL(timeout()), updateTimer, SLOT(stop()));
    connect(updateTimer, SIGNAL(timeout()), updateTimer, SLOT(deleteLater()));
    updateTimer->start();
}

void Contemporary::tintImage(QImage &image, QColor tint) const {
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

float Contemporary::getDPIScaling() const {
    float currentDPI = QApplication::desktop()->logicalDpiX();
    return currentDPI / (float) 96;
}

void Contemporary::drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget *widget) const {
    switch (element) {
        case CE_ProgressBarContents: drawControlProgressBarContents(option, painter, widget); break;
        case CE_ProgressBarGroove: drawControlProgressBarGroove(option, painter, widget); break;
        case CE_ProgressBar: drawControlProgressBar(option, painter, widget); break;
        case CE_HeaderSection: drawControlHeaderSection(option, painter, widget); break;
        case CE_HeaderEmptyArea: drawControlHeaderEmptyArea(option, painter, widget); break;
        case CE_MenuBarEmptyArea: drawMenubarEmptyArea(option, painter, widget); break;
        case CE_MenuItem: drawControlMenuItem(option, painter, widget); break;
        case CE_CheckBoxLabel: drawControlCheckboxLabel(option, painter, widget); break;
        default: QCommonStyle::drawControl(element, option, painter, widget);
    }
}

void Contemporary::drawComplexControl(ComplexControl control, const QStyleOptionComplex* option, QPainter* painter, const QWidget *widget) const {
    switch (control) {
        case CC_ComboBox: drawComplexComboBox(option, painter, widget);
        default: QCommonStyle::drawComplexControl(control, option, painter, widget);
    }
}

void Contemporary::drawPrimitive(PrimitiveElement primitive, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    switch (primitive) {
        case PE_IndicatorItemViewItemCheck:
        case PE_IndicatorCheckBox:
            drawPrimitiveIndicatorCheckBox(option, painter, widget); break;

        case PE_IndicatorRadioButton: drawPrimitiveIndicatorRadioButton(option, painter, widget); break;

        case PE_PanelButtonCommand: drawPrimitivePanelButtonCommand(option, painter, widget); break;

        case PE_PanelLineEdit: drawPrimitivePanelLineEdit(option, painter, widget); break;

        case PE_PanelItemViewItem: drawPrimitivePanelItemViewItem(option, painter, widget); break;

        case PE_IndicatorSpinUp: drawPrimitiveIndicatorArrow(Up, option, painter, widget); break;
        case PE_IndicatorSpinDown: drawPrimitiveIndicatorArrow(Down, option, painter, widget); break;
        case PE_IndicatorArrowLeft: drawPrimitiveIndicatorArrow(Left, option, painter, widget); break;
        case PE_IndicatorArrowRight: drawPrimitiveIndicatorArrow(Right, option, painter, widget); break;
        case PE_IndicatorArrowUp: drawPrimitiveIndicatorArrow(Up, option, painter, widget); break;
        case PE_IndicatorArrowDown: drawPrimitiveIndicatorArrow(Down, option, painter, widget); break;

        case PE_IndicatorBranch: drawPrimitiveIndicatorBranch(option, painter, widget); break;

        case PE_FrameFocusRect: drawPrimitiveFrameFocusRect(option, painter, widget); break;

        case PE_FrameMenu:
        case PE_PanelMenu:
        case PE_FrameGroupBox:
        case PE_Frame:
            drawPrimitiveFrame(option, painter, widget); break;
        //default: oldStyle->drawPrimitive(primitive, option, painter, widget);
    }
}

QSize Contemporary::sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const {
    return oldStyle->sizeFromContents(ct, opt, contentsSize, widget);
}

int Contemporary::pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const {
    return oldStyle->pixelMetric(m, opt, widget);
}

QIcon Contemporary::standardIcon(StandardPixmap standardIcon, const QStyleOption *opt, const QWidget *widget) const {
    return oldStyle->standardIcon(standardIcon, opt, widget);
}

void Contemporary::drawItemText(QPainter* painter, const QRect &rect, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole) const {
    QTextOption opt;
    opt.setAlignment((Qt::Alignment) alignment);

    QString t = text;
    t.remove("&");
    painter->setPen(palette.color(enabled ? QPalette::Normal : QPalette::Disabled, textRole));
    painter->drawText(rect, t, opt);
}

QRect Contemporary::subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget) const {
    switch (r) {
        case SE_CheckBoxIndicator:
        case SE_RadioButtonIndicator:
            return subElementComboBoxIndicator(opt, widget);

        case SE_CheckBoxContents:
        case SE_RadioButtonContents:
            return subElementComboBoxContents(opt, widget);

        case SE_CheckBoxFocusRect:
        case SE_RadioButtonFocusRect:
            return subElementComboBoxFocusRect(opt, widget);

        case SE_PushButtonFocusRect:
            return subElementPushButtonFocusRect(opt, widget);

        case SE_PushButtonContents:
            return subElementPushButtonContents(opt, widget);

        case SE_ItemViewItemFocusRect:
            return QRect();

        default: return oldStyle->subElementRect(r, opt, widget);
    }

}

void Contemporary::polish(QWidget* widget) {
    anim->registerWidget(widget);

    oldStyle->polish(widget);
}

void Contemporary::unpolish(QWidget *widget) {
    anim->deregisterWidget(widget);
}

int Contemporary::styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const {
    switch (sh) {
        case SH_Menu_FlashTriggeredItem:
            return true;

        default: return oldStyle->styleHint(sh, opt, w, shret);
    }
}

int Contemporary::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget) const {
    return oldStyle->layoutSpacing(control1, control2, orientation, option, widget);
}

Contemporary::SubControl Contemporary::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *option, const QPoint &pt, const QWidget *w) const {
    return oldStyle->hitTestComplexControl(cc, option, pt, w);
}

void Contemporary::drawPrimitiveIndicatorCheckBox(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton)
    if (opt == nullptr) return;

    painter->setPen(WINDOW_TEXT_COLOR);
    if (opt->state & State_Sunken) {
    painter->setBrush(WINDOW_PRESS_COLOR);
    } else if (opt->state & State_MouseOver) {
        painter->setBrush(WINDOW_HOVER_COLOR);
    } else {
        painter->setBrush(WINDOW_COLOR);
    }
    painter->drawRect(opt->rect);

    if (opt->state & State_NoChange) {
        QPolygon fill;
        fill.append(opt->rect.topLeft());
        fill.append(opt->rect.bottomLeft());
        fill.append(opt->rect.bottomRight());
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawPolygon(fill);
    } else if (opt->state & State_On) {
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawRect(opt->rect);
    }
}

void Contemporary::drawPrimitiveIndicatorRadioButton(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton)
    if (opt == nullptr) return;

    painter->setPen(WINDOW_TEXT_COLOR);
    if (opt->state & State_Sunken) {
    painter->setBrush(WINDOW_PRESS_COLOR);
    } else if (opt->state & State_MouseOver) {
        painter->setBrush(WINDOW_HOVER_COLOR);
    } else {
        painter->setBrush(WINDOW_COLOR);
    }
    painter->drawEllipse(opt->rect);

    if (opt->state & State_NoChange) {
        QPolygon fill;
        fill.append(opt->rect.topLeft());
        fill.append(opt->rect.bottomLeft());
        fill.append(opt->rect.bottomRight());
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawPolygon(fill);
    } else if (opt->state & State_On) {
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawEllipse(opt->rect);
    }
}

void Contemporary::drawPrimitiveFrame(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;

    painter->setPen(WINDOW_TEXT_COLOR);
    painter->setBrush(Qt::transparent);
    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
}

void Contemporary::drawPrimitivePanelButtonCommand(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton)
    if (opt == nullptr) return;

    //Get animation
    PushButtonAnimation* anim = qobject_cast<PushButtonAnimation*>(this->anim->a(widget));

    QColor background;
    if (widget && widget->property("type") == "destructive") {
        if (opt->state & State_On || opt->state & State_Sunken) {
            background = QColor(100, 0, 0);
        } else if (opt->state & State_MouseOver) {
            background = QColor(200, 0, 0);
        } else {
            background = QColor(150, 0, 0);
        }
    } else if (opt->features & QStyleOptionButton::Flat) {
        if (opt->state & State_On || opt->state & State_Sunken) {
            background = QColor(0, 0, 0, 100);
        } else if (opt->state & State_MouseOver) {
            background = QColor(255, 255, 255, 100);
        } else {
            background = QColor(255, 255, 255, 0);
        }
    } else {
        if (opt->state & State_On || opt->state & State_Sunken) {
            background = BUTTON_PRESS_COLOR;
        } else if (opt->state & State_MouseOver) {
            background = BUTTON_HOVER_COLOR;
        } else {
            background = BUTTON_COLOR;
        }
    }

    QColor backgroundColor;
    if (anim == nullptr) {
        background = background;
    } else {
        anim->setTargetColor(background);
        backgroundColor = anim->drawColor();
    }

    painter->setPen(Qt::transparent);
    painter->setBrush(backgroundColor);
    painter->drawRect(opt->rect);

    if (widget && widget->property("type") == "positive") {
        //Draw border around button
        QPen pen;
        pen.setColor(QColor(0, 255, 0));
        pen.setWidth(3 * getDPIScaling());
        painter->setPen(pen);

        painter->setBrush(Qt::transparent);
        painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
    }
}

void Contemporary::drawPrimitivePanelLineEdit(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;
    OPT_VARS

    painter->setPen(WINDOW_TEXT_COLOR);
    if (reverse) {
        painter->drawLine(opt->rect.topRight(), opt->rect.bottomRight());
    } else {
        painter->drawLine(opt->rect.topLeft(), opt->rect.bottomLeft());
    }
    painter->drawLine(opt->rect.bottomRight(), opt->rect.bottomLeft());
}

void Contemporary::drawPrimitivePanelItemViewItem(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionViewItemV4)
    if (opt == nullptr) return;

    const QAbstractItemView* itemView = qobject_cast<const QAbstractItemView*>(widget);

    QColor background;
    if (opt->state & State_Selected) {
        background = ACCENT_COLOR;
    } else if (opt->state & State_MouseOver && (!itemView || itemView->selectionMode() != QAbstractItemView::NoSelection)) {
        background = ACCENT_COLOR;
        background.setAlpha(127);
    } else {
        background = WINDOW_COLOR;
    }
    painter->setPen(Qt::transparent);
    painter->setBrush(background);
    painter->drawRect(opt->rect);
}

void Contemporary::drawPrimitiveIndicatorArrow(Direction dir, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
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

void Contemporary::drawPrimitiveIndicatorBranch(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    const QStyleOption* opt = option;
    if (opt == nullptr) return;
    OPT_VARS

    QRect rect = opt->rect;
    if (opt->state & QStyle::State_Item) {
        //Draw horizontal branch
        painter->setPen(WINDOW_TEXT_COLOR);

        if (reverse) {
            painter->drawLine(rect.left(), rect.top() + rect.height() / 2, rect.left() + rect.width() / 2, rect.top() + rect.height() / 2);
        } else {
            painter->drawLine(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2, rect.right(), rect.top() + rect.height() / 2);
        }

        //Draw top part of vertical branch
        painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.top() + rect.height() / 2);
    }

    if (opt->state & QStyle::State_Sibling) {
        //Show vertical branch
        painter->setPen(WINDOW_TEXT_COLOR);
        painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.bottom());
    }

    if (opt->state & QStyle::State_Children) {
        //Draw disclosure triangle (closed)
        QRect triangleRect = rect;
        triangleRect.setTop(rect.top() + (rect.height()) / 2 - 4 * getDPIScaling());
        triangleRect.setLeft(rect.left() + (rect.width()) / 2 - 4 * getDPIScaling());
        triangleRect.setSize(QSize(8 * getDPIScaling(), 8 * getDPIScaling()));
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->drawRect(triangleRect);
    }

    if (opt->state & QStyle::State_Open) {
        //Show disclosure triangle (open)
        QRect triangleRect = rect;
        triangleRect.setTop(rect.top() + (rect.height()) / 2 - 4 * getDPIScaling());
        triangleRect.setLeft(rect.left() + (rect.width()) / 2 - 4 * getDPIScaling());
        triangleRect.setSize(QSize(8 * getDPIScaling(), 8 * getDPIScaling()));
        painter->setBrush(WINDOW_COLOR);
        painter->setPen(WINDOW_TEXT_COLOR);
        painter->drawRect(triangleRect);
    }
}

void Contemporary::drawPrimitiveFrameFocusRect(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionFocusRect);
    if (opt == nullptr) return;

    /*QRect r;
    r.setLeft(0);
    r.setBottom(opt->rect.bottom());
    r.setWidth(opt->rect.width());
    r.setTop(opt->rect.bottom() - 2);*/
    painter->setBrush(QColor(255, 255, 255, 100));
    painter->setPen(Qt::transparent);
    painter->drawRect(opt->rect);
}

void Contemporary::drawControlProgressBarContents(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionProgressBar)
    if (opt == nullptr) return;

    QRect barArea = opt->rect;//.adjusted(0, 0, -1, -1);
    //barArea.adjust(1 * getDPIScaling(), 1 * getDPIScaling(), -2 * getDPIScaling(), -1 * getDPIScaling());

    painter->setPen(Qt::transparent);
    if (opt->maximum == 0 && opt->minimum == 0) {
        if (!indeterminateTimer->isActive()) {
            //Start the timer here to save CPU time if the timer is not needed.
            indeterminateTimer->start();
        }

        int fullWidth = barArea.width() * 4;
        int stageProgress = indetermiateProgressSection % fullWidth;
        int stage = stageProgress / barArea.width();
        stageProgress = stageProgress % barArea.width();

        switch (stage) {
            case 0:
            {
                painter->setBrush(WINDOW_COLOR);
                painter->drawRect(barArea);
                painter->setBrush(ACCENT_COLOR);
                painter->drawRect(barArea.left(), barArea.top(), stageProgress, barArea.height());
                break;
            }
            case 1:
            {
                painter->setBrush(ACCENT_COLOR);
                painter->drawRect(barArea);
                painter->setBrush(WINDOW_COLOR);
                painter->drawRect(stageProgress, barArea.top(), stageProgress * 2, barArea.height());
                break;
            }
            case 2:
            {
                painter->setBrush(ACCENT_COLOR);
                painter->drawRect(barArea);
                painter->setBrush(WINDOW_COLOR);
                painter->drawRect(barArea.left(), barArea.top(), stageProgress, barArea.height());
                break;
            }
            case 3:
            {
                painter->setBrush(WINDOW_COLOR);
                painter->drawRect(barArea);
                painter->setBrush(ACCENT_COLOR);
                painter->drawRect(stageProgress, barArea.top(), stageProgress * 2, barArea.height());
            }
        }

        scheduleRepaint(widget);
    } else {
        //Determinate Progress bar
        painter->setBrush(ACCENT_COLOR);
        int width = (((float) opt->progress - (float) opt->minimum) / ((float) opt->maximum - (float) opt->minimum)) * barArea.width();
        painter->drawRect(barArea.left(), barArea.top(), width, barArea.bottom());
    }
}

void Contemporary::drawControlProgressBarGroove(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionProgressBar)
    if (opt == nullptr) return;

    painter->setPen(WINDOW_TEXT_COLOR);
    painter->setBrush(QBrush(Qt::transparent));
    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
}

void Contemporary::drawControlProgressBar(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    drawControlProgressBarContents(option, painter, widget);
    drawControlProgressBarGroove(option, painter, widget);
}

void Contemporary::drawControlHeaderSection(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
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

void Contemporary::drawControlHeaderEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionProgressBar)
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    painter->setBrush(ACCENT_COLOR);
    painter->drawRect(opt->rect);
}

void Contemporary::drawMenubarEmptyArea(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOption);
    if (opt == nullptr) return;

    painter->setPen(Qt::transparent);
    painter->setBrush(WINDOW_COLOR);
    painter->drawRect(opt->rect);
}

void Contemporary::drawControlMenuItem(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionMenuItem);
    if (opt == nullptr) return;
    OPT_VARS;

    switch (opt->menuItemType) {
        case QStyleOptionMenuItem::Normal:
        case QStyleOptionMenuItem::SubMenu: {
            QColor backgroundColor;
            QPalette::ColorRole textColor;
            if (opt->state & QStyle::State_Selected || opt->state & QStyle::State_Sunken) {
                backgroundColor = opt->palette.color(QPalette::Highlight);
                textColor = QPalette::HighlightedText;
            } else {
                backgroundColor = opt->palette.color(QPalette::Window);
                textColor = QPalette::WindowText;
            }

            painter->setPen(Qt::transparent);
            painter->setBrush(backgroundColor);
            painter->drawRect(opt->rect);

            if (!opt->icon.isNull()) {
                QRect iconRect;
                iconRect.setLeft(opt->rect.left() + 2 * getDPIScaling());
                iconRect.setWidth(pixelMetric(PM_SmallIconSize, opt, widget));
                iconRect.setHeight(pixelMetric(PM_SmallIconSize, opt, widget));
                iconRect.moveTop(opt->rect.top() + opt->rect.height() / 2 - pixelMetric(PM_SmallIconSize, opt, widget) / 2);

                drawItemPixmap(painter, iconRect, Qt::AlignCenter, opt->icon.pixmap(pixelMetric(PM_SmallIconSize, opt, widget)));
            }

            QRect textRect;

            textRect.setLeft(opt->rect.left() + 4 * getDPIScaling() + pixelMetric(PM_SmallIconSize, opt, widget));
            textRect.setHeight(opt->fontMetrics.height());
            textRect.moveTop(opt->rect.top() + opt->rect.height() / 2 - opt->fontMetrics.height() / 2);
            textRect.setRight(opt->rect.right());

            drawItemText(painter, textRect, textHorizontalAlignment, opt->palette, opt->state & QStyle::State_Enabled, opt->text, textColor);
            break;
        }
        case QStyleOptionMenuItem::Separator: {
            if (opt->text == "") {
                painter->setPen(opt->palette.color(QPalette::WindowText));
                painter->drawLine(opt->rect.topLeft(), opt->rect.topRight());
            } else {
                painter->setBrush(opt->palette.color(QPalette::WindowText));
                painter->drawRect(opt->rect);
                drawItemText(painter, opt->rect, Qt::AlignCenter, opt->palette, opt->state & QStyle::State_Enabled, opt->text, QPalette::Window);
            }
        }
    }
}

void Contemporary::drawComplexComboBox(const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionComboBox);
    if (opt == nullptr) return;
    OPT_VARS;

    painter->setPen(WINDOW_TEXT_COLOR);
    QBrush brush = QBrush(WINDOW_COLOR);

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
    painter->setBrush(brush);
    painter->drawRect(opt->rect.adjusted(0, 0, -1, -1));
    
    QRect textRect;
    if (reverse) {
        if (opt->currentIcon.isNull()) {
            textRect = opt->rect.adjusted(0, 0, -4 * getDPIScaling(), 0);
        } else {
            textRect = opt->rect.adjusted(0, 0, -8 * getDPIScaling() + opt->iconSize.width(), 0);
        }
    } else {
        if (opt->currentIcon.isNull()) {
            textRect = opt->rect.adjusted(4 * getDPIScaling(), 0, 0, 0);
        } else {
            textRect = opt->rect.adjusted(8 * getDPIScaling() + opt->iconSize.width(), 0, 0, 0);
        }
    }
    drawItemText(painter, textRect, textHorizontalAlignment | Qt::AlignVCenter, opt->palette, opt->state & State_Enabled, opt->currentText, QPalette::WindowText);

    if (opt->subControls & SC_ComboBoxFrame) {
        if (opt->editable) {
            drawPrimitive(PE_FrameLineEdit, option, painter, widget);
        } else {
            drawPrimitive(PE_Frame, option, painter, widget);
        }
    }

    if (opt->subControls & SC_ComboBoxArrow) {
        painter->setBrush(WINDOW_TEXT_COLOR);
        painter->setPen(Qt::transparent);

        QPolygon triangle;
        int halfHeight = opt->rect.height() / 2;
        if (reverse) {
            QRect rect = opt->rect.adjusted(3 * getDPIScaling(), 1 * getDPIScaling(), -1 * getDPIScaling(), -2 * getDPIScaling());
            triangle.append(QPoint(rect.left(), rect.bottom()));
            triangle.append(QPoint(rect.left(), rect.bottom() - halfHeight));
            triangle.append(QPoint(rect.left() + halfHeight, rect.bottom()));
        } else {
            QRect rect = opt->rect.adjusted(1 * getDPIScaling(), 1 * getDPIScaling(), -2 * getDPIScaling(), -2 * getDPIScaling());
            triangle.append(QPoint(rect.right(), rect.bottom()));
            triangle.append(QPoint(rect.right(), rect.bottom() - halfHeight));
            triangle.append(QPoint(rect.right() - halfHeight, rect.bottom()));
        }
        painter->drawPolygon(triangle);
    }
}

void Contemporary::drawControlCheckboxLabel(const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return;
    OPT_VARS;

    drawItemText(painter, opt->rect, textHorizontalAlignment | Qt::AlignVCenter, opt->palette, opt->state & State_Enabled, opt->text, QPalette::WindowText);
}

QRect Contemporary::subElementComboBoxIndicator(const QStyleOption *option, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    QRect indicatorRect;
    indicatorRect.setLeft(2 * getDPIScaling());
    indicatorRect.setWidth(11 * getDPIScaling()); //11 to take the pen width into account
    indicatorRect.setTop(opt->rect.height() / 2 - 12 * getDPIScaling() / 2);
    indicatorRect.setHeight(11 * getDPIScaling());
    return indicatorRect;
}

QRect Contemporary::subElementComboBoxContents(const QStyleOption *option, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    int center = opt->rect.height() / 2;

    QRect textRect;
    textRect.setLeft(18 * getDPIScaling());
    textRect.setWidth(opt->fontMetrics.width(opt->text));
    textRect.setTop(center - opt->fontMetrics.height() / 2);
    textRect.setHeight(opt->fontMetrics.height());

    return textRect;
}

QRect Contemporary::subElementPushButtonContents(const QStyleOption *option, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    return opt->rect;
}

QRect Contemporary::subElementPushButtonFocusRect(const QStyleOption *option, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    QRect rect = option->rect;
    if (opt->features & QStyleOptionButton::CommandLinkButton) {
        //Put focus indicator on the leading side
        rect.setWidth(2 * getDPIScaling());
        if (reverse) {
            rect.moveRight(option->rect.width() - 1);
        }
    } else {
        //Underline the button
        rect.setHeight(2 * getDPIScaling());
        rect.moveBottom(option->rect.height() - 1);
    }

    return rect;
}

QRect Contemporary::subElementComboBoxFocusRect(const QStyleOption *option, const QWidget *widget) const {
    OPT_CAST(QStyleOptionButton);
    if (opt == nullptr) return QRect();
    OPT_VARS;

    QRect indicatorRect;
    indicatorRect.setLeft(2 * getDPIScaling());
    indicatorRect.setWidth(12 * getDPIScaling());
    indicatorRect.setTop(opt->rect.height() / 2 - 12 * getDPIScaling() / 2);
    indicatorRect.setHeight(12 * getDPIScaling());

    QRect rect;
    rect.setLeft(0);
    rect.setWidth(16 * getDPIScaling());
    rect.setTop(opt->rect.height() / 2 - 16 * getDPIScaling() / 2);
    rect.setHeight(16 * getDPIScaling());
    return rect;
}

