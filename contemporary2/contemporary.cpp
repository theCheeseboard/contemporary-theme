#include "contemporary.h"

Contemporary::Contemporary() {
    oldStyle = new Style();

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

        case PE_FrameFocusRect: drawPrimitiveFrameFocusRect(option, painter, widget);

        case PE_FrameMenu:
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
    oldStyle->drawItemText(painter, rect, alignment, palette, enabled, text, textRole);
}

QRect Contemporary::subElementRect(SubElement r, const QStyleOption *opt, const QWidget *widget) const {
    return oldStyle->subElementRect(r, opt, widget);
}

void Contemporary::polish(QWidget* widget) {
    oldStyle->polish(widget);
}

int Contemporary::styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const {
    switch (sh) {
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
            background = WINDOW_PRESS_COLOR;
        } else if (opt->state & State_MouseOver) {
            background = WINDOW_HOVER_COLOR;
        } else {
            background = WINDOW_COLOR;
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
    painter->setPen(Qt::transparent);
    painter->setBrush(background);
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

    animations->insert(widget->winId(), new tVariantAnimation());

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

    QRect r = option->rect;
    r.setHeight(2);
    r.moveBottom(0);
    painter->setBrush(ACCENT_HOVER_COLOR);
    painter->drawRect(r);
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
    drawItemText(painter, textRect, Qt::AlignLeft | Qt::AlignVCenter, opt->palette, opt->state & State_Enabled, opt->currentText, QPalette::WindowText);

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
