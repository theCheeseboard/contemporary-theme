#include "contemporary.h"

Style::Style()
{
    settings = new QSettings("theSuite", "contemporary_widget", this);
    tsSettings = new QSettings("theSuite", "theShell");

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
    touchMode = tsSettings->value("input/touch", false).toBool();
}

Style::~Style() {
    theLibsGlobal::instance()->disconnect();
}

void Style::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {
    if (option == NULL) return;
    if (painter == NULL) return;

    QPalette pal = option->palette;
    QRect rect = option->rect;
    if (widget != NULL) {
        painter->setFont(widget->font());
    }
    painter->setPen(transparent);

    switch (element) {
    case QStyle::CE_PushButton:
    {
        const QStyleOptionButton* button = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (button == NULL) return;
        QBrush brush;
        QPen textPen;

        if (widget == NULL) {
            //Sorry... :(
            goto drawNormalButton;
        }

        if (widget->property("type") == "positive") {
            if (button->state & QStyle::State_Enabled) {
                brush = QBrush(col(0, 200, 0));
            } else {
                brush = QBrush(col(67, 67, 67));
            }

            if (button->state & QStyle::State_HasFocus) {
                brush = QBrush(col(0, 225, 0));
            }

            if (button->state & QStyle::State_MouseOver) {
                brush = QBrush(col(0, 250, 0));
            }

            if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                brush = QBrush(col(0, 150, 0));
            }
            textPen = QColor(0, 0, 0);
        } else if (widget->property("type") == "destructive") {
            if (button->state & QStyle::State_Enabled) {
                brush = QBrush(col(200, 0, 0));
            } else {
                brush = QBrush(col(67, 67, 67));
            }

            if (button->state & QStyle::State_HasFocus) {
                brush = QBrush(col(225, 0, 0));
            }

            if (button->state & QStyle::State_MouseOver) {
                brush = QBrush(col(250, 0, 0));
            }

            if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                brush = QBrush(col(150, 0, 0));
            }
            textPen = col(255, 255, 255);
        } else {
            drawNormalButton:
            if (button->features & QStyleOptionButton::Flat || (widget != NULL && widget->property("flatContemporary").toBool())) {
                brush = QBrush(pal.color(QPalette::Window));

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(pal.color(QPalette::Window).lighter());
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(pal.color(QPalette::Window).darker(150));
                }
                textPen = pal.color(QPalette::WindowText);
            } else {
                if (button->state & QStyle::State_Enabled) {
                    brush = QBrush(pal.color(QPalette::Button));
                } else {
                    brush = QBrush(pal.color(QPalette::Disabled, QPalette::Button));
                }

                if (button->state & QStyle::State_HasFocus) {
                    brush = QBrush(pal.color(QPalette::Button).lighter(125));
                }

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(pal.color(QPalette::Button).lighter());
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(pal.color(QPalette::Button).darker(150));
                }
                textPen = pal.color(QPalette::ButtonText);
            }

        }

        painter->setBrush(brush);
        painter->drawRect(rect);

        QString text = button->text;

        QRect textRect, iconRect;
        textRect.setLeft(rect.left() + (rect.width() / 2) - (button->fontMetrics.width(text) / 2));
        textRect.setWidth(button->fontMetrics.width(text));
        textRect.setTop(rect.top() + (rect.height() / 2) - (button->fontMetrics.height() / 2));
        textRect.setHeight(button->fontMetrics.height());

        if (!button->icon.isNull()) {
            int fullWidth = textRect.width() + button->iconSize.width();
            int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

            iconRect.setLeft(iconLeft);
            iconRect.setTop(rect.top() + (rect.height() / 2) - (button->iconSize.height() / 2));
            iconRect.setSize(button->iconSize);

            //textRect.adjust(button->iconSize.width() / 2, 0, 0, 0);
            textRect.moveLeft(iconRect.right());

            QIcon icon = button->icon;
            QImage image = icon.pixmap(button->iconSize).toImage();
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            /*if (image.colorCount() == 0) {
                QPainter colorizer(&image);
                colorizer.setCompositionMode(QPainter::CompositionMode_SourceIn);
                colorizer.fillRect(image.rect(), textPen.brush());
                colorizer.end();
            }*/
            tintImage(image, textPen.color());

            painter->drawImage(iconRect, image);
        }

        //Draw text
        painter->setPen(textPen);
        painter->drawText(textRect, Qt::AlignCenter, text.remove("&"));

        break;
    }
    case QStyle::CE_CheckBox:
    {
        const QStyleOptionButton* button = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (button == NULL) return;
        QRect checkArea(0, rect.height() / 2 - 6 * getDPIScaling(), 12 * getDPIScaling(), 12 * getDPIScaling());

        painter->setPen(pal.color(QPalette::WindowText));

        QBrush brush = QBrush(transparent);
        if (button->state & QStyle::State_MouseOver) {
            brush = QBrush(pal.color(QPalette::Window).lighter());
        }

        if (button->state & QStyle::State_Sunken) {
            brush = QBrush(pal.color(QPalette::Window).darker(150));
        }

        painter->setBrush(brush);
        painter->drawRect(checkArea);

        if (button->state & QStyle::State_On) {
            painter->setPen(transparent);
            painter->setBrush(pal.color(QPalette::WindowText));
            //painter->drawRect(checkArea.adjusted(2, 2, -1, -1));

            /*QRect selectionArea = checkArea.adjusted(2, 2, -1, -1);

            QPolygon triangle;
            triangle.append(selectionArea.topLeft());
            triangle.append(selectionArea.bottomLeft());
            triangle.append(QPoint(selectionArea.left() + (selectionArea.height() / 2), selectionArea.top() + (selectionArea.height() / 2)));
            painter->drawPolygon(triangle);*/
            painter->drawRect(checkArea);

            QRect tickArea = checkArea;
            tickArea.adjust(3 * getDPIScaling(), 3 * getDPIScaling(), -1 * getDPIScaling(), -1 * getDPIScaling());

            /*QPen pen = pal.color(QPalette::Window);
            pen.setWidth(1);
            painter->setPen(pen);

            QPoint p1, p2, p3;
            p1 = QPoint(tickArea.left(), tickArea.bottom() - (tickArea.height() / 2));
            p2 = QPoint(tickArea.left() + (tickArea.width() / 4) + (tickArea.width() / 6), tickArea.bottom() - (tickArea.height() / 4));
            //p2 = QPoint(tickArea.left() + (tickArea.width() / 4), tickArea.bottom());
            p3 = QPoint(tickArea.right(), tickArea.top() + (tickArea.height() / 4));

            painter->drawLine(p1, p2);
            painter->drawLine(p2, p3);*/
        } else if (button->state & QStyle::State_NoChange) {
            QPolygon triangle;
            triangle.append(checkArea.topLeft());
            triangle.append(checkArea.bottomLeft());
            triangle.append(checkArea.topRight());

            painter->setPen(transparent);
            painter->setBrush(pal.color(QPalette::WindowText));
            painter->drawPolygon(triangle);
        }


        //Draw text
        QString text = button->text;
        QRect textArea = rect;

        //Draw to the right of the checkbox
        textArea.setLeft(checkArea.right() + 4 * getDPIScaling());
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(textArea, Qt::AlignVCenter | Qt::AlignLeft, text.remove("&"));
    }
    case QStyle::CE_Header:
    {
        const QStyleOptionHeader* header = qstyleoption_cast<const QStyleOptionHeader*>(option);

        if (header == NULL) return;
        QString text = header->text;
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(rect, Qt::AlignCenter, text.remove("&"));

    }
    case QStyle::CE_RadioButton:
    {
        const QStyleOptionButton* button = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (button == NULL) return;

        QRect checkArea(0, rect.height() / 2 - 6 * getDPIScaling(), 12 * getDPIScaling(), 12 * getDPIScaling());

        painter->setPen(pal.color(QPalette::WindowText));
        QBrush brush = QBrush(transparent);
        if (button->state & QStyle::State_MouseOver) {
            brush = QBrush(pal.color(QPalette::Window).lighter());
        }

        if (button->state & QStyle::State_Sunken) {
            brush = QBrush(pal.color(QPalette::Window).darker(150));
        }
        painter->setBrush(brush);
        painter->drawEllipse(checkArea);

        if (button->state & QStyle::State_On) {
            painter->setPen(transparent);
            painter->setBrush(pal.color(QPalette::WindowText));
            painter->drawEllipse(checkArea.adjusted(2 * getDPIScaling(), 2 * getDPIScaling(), -1 * getDPIScaling(), -1 * getDPIScaling()));
        }

        //Draw text
        QString text = button->text;
        QRect textArea = rect;

        //Draw to the right of the checkbox
        textArea.setLeft(checkArea.right() + 4);
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(textArea, Qt::AlignVCenter | Qt::AlignLeft, text.remove("&"));
        break;
    }
    case QStyle::CE_ProgressBar:
    {
        const QStyleOptionProgressBar* bar = qstyleoption_cast<const QStyleOptionProgressBar*>(option);
        if (bar == NULL) return;

        QRect barArea = rect;
        barArea.adjust(1 * getDPIScaling(), 1 * getDPIScaling(), -2 * getDPIScaling(), -1 * getDPIScaling());

        painter->setPen(transparent);
        if (bar->maximum == 0 && bar->minimum == 0) {
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
                painter->setBrush(pal.brush(QPalette::Window));
                painter->drawRect(barArea);
                painter->setBrush(pal.brush(QPalette::Highlight));
                painter->drawRect(barArea.left(), barArea.top(), stageProgress, barArea.height());
                break;
            }
            case 1:
            {
                painter->setBrush(pal.brush(QPalette::Highlight));
                painter->drawRect(barArea);
                painter->setBrush(pal.brush(QPalette::Window));
                painter->drawRect(stageProgress, barArea.top(), stageProgress * 2, barArea.height());
                break;
            }
            case 2:
            {
                painter->setBrush(pal.brush(QPalette::Highlight));
                painter->drawRect(barArea);
                painter->setBrush(pal.brush(QPalette::Window));
                painter->drawRect(barArea.left(), barArea.top(), stageProgress, barArea.height());
                break;
            }
            case 3:
            {
                painter->setBrush(pal.brush(QPalette::Window));
                painter->drawRect(barArea);
                painter->setBrush(pal.brush(QPalette::Highlight));
                painter->drawRect(stageProgress, barArea.top(), stageProgress * 2, barArea.height());
            }
            }

            scheduleRepaint(widget);
        } else {
            //Determinate Progress bar
            painter->setBrush(pal.brush(QPalette::Highlight));
            int width = (((float) bar->progress - (float) bar->minimum) / ((float) bar->maximum - (float) bar->minimum)) * barArea.width();
            painter->drawRect(barArea.left(), barArea.top(), width, barArea.bottom());
        }

        painter->setPen(pal.color(QPalette::WindowText));
        painter->setBrush(QBrush(transparent));
        painter->drawRect(rect.adjusted(0, 0, -1 * getDPIScaling(), -1 * getDPIScaling()));
        break;
    }
    case QStyle::CE_MenuBarItem:
    {
        const QStyleOptionMenuItem* item = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
        if (item == NULL) return;

        static int menuPaintOutHeight;
        static int menuPaintInHeight;
        static QString selectedInMenu = "";
        static QString selectedOutMenu = "";

        if (item->state & QStyle::State_Selected || item->state & QStyle::State_Sunken) {
            painter->setPen(transparent);
            if (selectedInMenu == "") {
                selectedInMenu = item->text;

                tVariantAnimation* anim = new tVariantAnimation;
                anim->setStartValue(0);
                anim->setEndValue(rect.height());
                anim->setDuration(100);
                anim->setEasingCurve(QEasingCurve::OutCubic);
                connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
                    menuPaintInHeight = value.toInt();
                });
                connect(anim, SIGNAL(valueChanged(QVariant)), widget, SLOT(repaint()));
                connect(anim, SIGNAL(finished()), anim, SLOT(deleteLater()));
                anim->start();
            } else if (selectedInMenu == item->text) {
                painter->setBrush(pal.brush(QPalette::Highlight));
                painter->drawRect(rect.left(), rect.top(), rect.width(), menuPaintInHeight);
            }

            painter->setPen(pal.color(QPalette::HighlightedText));
        } else {
            if (selectedOutMenu != item->text) {
                if (selectedInMenu == item->text) {
                    selectedInMenu = "";
                    selectedOutMenu = item->text;

                    tVariantAnimation* anim = new tVariantAnimation;
                    anim->setStartValue(menuPaintInHeight);
                    anim->setEndValue(0);
                    anim->setDuration(100);
                    anim->setEasingCurve(QEasingCurve::OutCubic);
                    connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
                        menuPaintOutHeight = value.toInt();
                    });
                    connect(anim, SIGNAL(valueChanged(QVariant)), widget, SLOT(repaint()));
                    connect(anim, SIGNAL(finished()), anim, SLOT(deleteLater()));
                    connect(anim, &tVariantAnimation::finished, [=]() {
                        selectedOutMenu = "";
                    });
                    anim->start();
                }
            } else {
               painter->setBrush(pal.brush(QPalette::Highlight));
               painter->drawRect(rect.left(), rect.top(), rect.width(), menuPaintOutHeight);
            }
            painter->setPen(pal.color(QPalette::WindowText));
        }

        QString text = item->text;
        text.remove("&");
        painter->drawText(item->rect, Qt::AlignCenter, text);
        break;
    }
    case QStyle::CE_MenuItem:
    {
        const QStyleOptionMenuItem* item = qstyleoption_cast<const QStyleOptionMenuItem*>(option);
        if (item == NULL) return;

        QString text = item->text;
        text.remove("&");

        switch (item->menuItemType) {
        case QStyleOptionMenuItem::Normal:
        case QStyleOptionMenuItem::SubMenu:
        {
            bool selected;

            /*static int menuPaintOutWidth;
            static int menuPaintInWidth;
            static QString selectedInMenu = "";
            static QString selectedOutMenu = "";*/

            int halfHeight = rect.height() / 2;

            if (item->menuItemType == QStyleOptionMenuItem::SubMenu) {
                painter->setPen(transparent);
                QLinearGradient gradient;
                gradient.setStart(rect.right(), 0);
                gradient.setFinalStop(rect.right() - halfHeight, 0);
                gradient.setColorAt(0, pal.color(QPalette::WindowText));
                gradient.setColorAt(1, transparent);
                painter->setBrush(QBrush(gradient));

                painter->drawRect(item->rect.right() - halfHeight, item->rect.top(), halfHeight, item->rect.height());
            }

            QPen textPen;
            if (item->state & QStyle::State_Selected || item->state & QStyle::State_Sunken) {
                selected = true;
                if (animation(text, 0).toInt() == 0) {
                    putAnimation(text, "menuIn", 0);
                    tVariantAnimation* anim = new tVariantAnimation;
                    anim->setStartValue(0);
                    anim->setEndValue(rect.width());
                    anim->setDuration(100);
                    anim->setEasingCurve(QEasingCurve::OutCubic);
                    connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
                        if (currentType(text) == "menuIn") {
                            putAnimation(text, "menuIn", value.toInt());
                        } else {
                            anim->stop();
                            anim->deleteLater();
                        }
                    });
                    connect(anim, SIGNAL(valueChanged(QVariant)), widget, SLOT(repaint()));
                    connect(anim, SIGNAL(finished()), anim, SLOT(deleteLater()));
                    anim->start();
                }
                textPen = pal.color(QPalette::HighlightedText);
            } else {
                selected = false;
                if (animation(text, 0).toInt() != 0) {
                    if (currentType(text) != "menuOut") {
                        putAnimation(text, "menuOut", animation(text, 0));
                        tVariantAnimation* anim = new tVariantAnimation;
                        anim->setStartValue(animation(text, 0).toInt());
                        anim->setEndValue(0);
                        anim->setDuration((animation(text, 0).toFloat() / (float) rect.width()) * (float) 100);
                        anim->setEasingCurve(QEasingCurve::OutCubic);
                        connect(anim, &tVariantAnimation::valueChanged, [=](QVariant value) {
                            if (currentType(text) == "menuOut") {
                                putAnimation(text, "menuOut", value.toInt());
                            } else {
                                anim->stop();
                                anim->deleteLater();
                            }
                        });
                        connect(anim, SIGNAL(valueChanged(QVariant)), widget, SLOT(repaint()));
                        connect(anim, SIGNAL(finished()), anim, SLOT(deleteLater()));
                        anim->start();
                    }
                }
                textPen = pal.color(QPalette::WindowText);
            }

            painter->setPen(transparent);
            painter->setBrush(pal.brush(QPalette::Highlight));
            int selectionWidth = animation(text, 0).toInt();
            painter->drawRect(rect.left(), rect.top(), selectionWidth, rect.height());

            painter->setPen(textPen);

            QRect textRect = rect;
            //if (item->menuHasCheckableItems) {
            //    textRect.setLeft(textRect.height() + 4);
            //}

            textRect.setLeft(rect.left() + 24 * getDPIScaling());
            textRect.setRight(rect.right() - 4 * getDPIScaling());

            if (text.contains("\t")) {
                QString keyText = text.mid(text.indexOf("\t"));
                if (settings->value("keyboard/isApple", false).toBool()) {
                    keyText.replace("Ctrl+", "^");
                    keyText.replace("Alt+", "⌥");
                    keyText.replace("Meta+", "⌘");
                } else {
                    keyText.replace("Ctrl+", "⎈");
                    keyText.replace("Alt+", "⎇");
                    keyText.replace("Meta+", "❖");
                }
                keyText.replace("Shift+", "⇧");
                keyText.replace("Esc", "⎋");
                keyText.replace("Tab", "⇄");
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text.left(text.indexOf("\t")));
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignRight, keyText);
            } else {
                painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, text);
            }

            if (item->menuItemType == QStyleOptionMenuItem::SubMenu) {
                painter->setPen(transparent);
                painter->setBrush(pal.brush(QPalette::Window));

                QPolygon triangle1;
                triangle1.append(QPoint(rect.right(), rect.top()));
                triangle1.append(QPoint(rect.right(), rect.top() + halfHeight));
                triangle1.append(QPoint(rect.right() - halfHeight, rect.top()));
                painter->drawPolygon(triangle1);

                QPolygon triangle2;
                triangle2.append(QPoint(rect.right(), rect.bottom() + 1 * getDPIScaling()));
                triangle2.append(QPoint(rect.right(), rect.bottom() + 1 * getDPIScaling() - halfHeight));
                triangle2.append(QPoint(rect.right() - halfHeight, rect.bottom() + 1 * getDPIScaling()));
                painter->drawPolygon(triangle2);
            }

            if (item->checkType == QStyleOptionMenuItem::NotCheckable) {
                if (!item->icon.isNull()) {
                    QRect iconRect;
                    iconRect.setLeft(4 * getDPIScaling());
                    iconRect.setTop(rect.top() + (rect.height() / 2) - 8 * getDPIScaling());
                    iconRect.setSize(QSize(16 * getDPIScaling(), 16 * getDPIScaling()));

                    QIcon icon = item->icon;
                    QImage image = icon.pixmap(16 * getDPIScaling(), 16 * getDPIScaling()).toImage();
                    image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
                    tintImage(image, textPen.color());

                    painter->drawImage(iconRect, image);
                }
            } else {
                painter->setPen(pal.color(QPalette::WindowText));
                painter->setBrush(pal.brush(QPalette::Window));

                QPolygon triangle;
                triangle.append(rect.topLeft());
                triangle.append(rect.bottomLeft());
                triangle.append(QPoint(rect.left() + halfHeight, rect.top() + halfHeight));
                painter->drawPolygon(triangle);
            }

            if (item->checked) {
                painter->setPen(transparent);
                painter->setBrush(pal.brush(QPalette::WindowText));

                QPolygon triangle;
                triangle.append(rect.topLeft());
                triangle.append(rect.bottomLeft());
                triangle.append(QPoint(rect.left() + halfHeight, rect.top() + halfHeight));
                painter->drawPolygon(triangle);
            }

            break;
        }
        case QStyleOptionMenuItem::Separator:
        {
            if (text == "") {
                painter->setPen(pal.color(QPalette::WindowText));
                painter->drawLine(item->rect.topLeft(), item->rect.topRight());
            } else {
                painter->setBrush(pal.color(QPalette::WindowText));
                painter->drawRect(rect);
                painter->setPen(pal.color(QPalette::Window));
                painter->drawText(rect, Qt::AlignCenter, text);
            }
            break;
        }
        case QStyleOptionMenuItem::Margin:
        {
            painter->setBrush(pal.brush(QPalette::Window));
            painter->drawRect(rect);
        }
        }
        break;
    }
    case QStyle::CE_RubberBand:
    {
        QColor selection = pal.color(QPalette::Highlight);
        painter->setPen(selection);
        selection.setAlpha(127);
        painter->setBrush(selection);
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        break;
    }
    case QStyle::CE_ScrollBarSlider:
    {
        painter->setBrush(pal.brush(QPalette::Highlight));
        painter->drawRect(rect);
        break;
    }
    case QStyle::CE_TabBarTab:
    case QStyle::CE_TabBarTabShape:
    {
        const QStyleOptionTab* item = qstyleoption_cast<const QStyleOptionTab*>(option);
        if (item == NULL) return;

        QRect shapeRect = rect;
        shapeRect.adjust(0, 0, 0, -1);

        painter->setPen(pal.color(QPalette::WindowText));
        if (item->state & QStyle::State_Selected) {
            if (item->shape == QTabBar::RoundedNorth ||
                    item->shape == QTabBar::RoundedEast ||
                    item->shape == QTabBar::RoundedWest) {
                painter->drawLine(shapeRect.topLeft(), shapeRect.topRight());
            }

            if (item->shape == QTabBar::RoundedSouth ||
                    item->shape == QTabBar::RoundedEast ||
                    item->shape == QTabBar::RoundedWest) {
                painter->drawLine(shapeRect.bottomLeft(), shapeRect.bottomRight());
            }

            if (item->shape == QTabBar::RoundedSouth ||
                    item->shape == QTabBar::RoundedNorth ||
                    item->shape == QTabBar::RoundedWest) {
                painter->drawLine(shapeRect.topLeft(), shapeRect.bottomLeft());
            }

            if (item->shape == QTabBar::RoundedSouth ||
                    item->shape == QTabBar::RoundedEast ||
                    item->shape == QTabBar::RoundedNorth) {
                painter->drawLine(shapeRect.topRight(), shapeRect.bottomRight());
            }
        } else {
            painter->setBackground(pal.brush(QPalette::Window));
            //painter->drawRect(shapeRect);
            painter->drawLine(shapeRect.topLeft(), shapeRect.topRight());
            painter->drawLine(shapeRect.topRight(), shapeRect.bottomRight());
            painter->drawLine(shapeRect.bottomRight(), shapeRect.bottomLeft());
            painter->drawLine(shapeRect.bottomLeft(), shapeRect.topLeft());
        }

        if (element == QStyle::CE_TabBarTabShape) {
            break;
        }

        //Fall through
    }
    case QStyle::CE_TabBarTabLabel:
    {
        const QStyleOptionTab* item = qstyleoption_cast<const QStyleOptionTab*>(option);
        if (item == NULL) return;

        QString text = item->text;
        text.remove("&");
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(rect, Qt::AlignCenter, text);

        if (!item->icon.isNull()) {
            QRect iconRect = rect;
            iconRect.adjust(4 * getDPIScaling(), 0, 4 * getDPIScaling(), 0);
            iconRect.adjust(item->leftButtonSize.width() + 8 * getDPIScaling(), 0, item->leftButtonSize.width() + 8 * getDPIScaling(), 0);

            iconRect.setSize(item->iconSize);
            iconRect.moveTop((rect.height() / 2) - (item->iconSize.height() / 2));

            painter->drawImage(iconRect, item->icon.pixmap(item->iconSize).toImage());
        }

        break;
    }
    case QStyle::CE_ShapedFrame:
    {
        const QStyleOptionFrame* item = qstyleoption_cast<const QStyleOptionFrame*>(option);
        if (item == NULL) return;

        switch (item->frameShape) {
        case QFrame::StyledPanel:
        {
            painter->setBrush(pal.color(QPalette::Window));
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawRect(rect.adjusted(0, 0, -1, -1));
            break;
        }
        case QFrame::HLine:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topLeft(), rect.topRight());
            break;
        }
        case QFrame::VLine:
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
        }

        break;
    }
    case QStyle::CE_ToolBar:
    {
        painter->setBrush(pal.color(QPalette::Window));
        painter->drawRect(rect);
    }
    case QStyle::CE_SizeGrip:
    {
        QPolygon triangle;
        triangle.append(rect.bottomRight());
        triangle.append(QPoint(rect.bottom() - 8 * getDPIScaling(), rect.right()));
        triangle.append(QPoint(rect.bottom(), rect.right() - 8 * getDPIScaling()));

        painter->setBrush(pal.brush(QPalette::WindowText));
        painter->drawPolygon(triangle);
    }
    case QStyle::CE_ItemViewItem:
    {
        const QStyleOptionViewItem* item = qstyleoption_cast<const QStyleOptionViewItem*>(option);
        if (item == NULL) return;

        painter->setPen(transparent);

        QPen textPen;
        if (option->state & QStyle::State_Selected) {
            painter->setBrush(pal.brush(QPalette::Highlight));
            textPen = pal.color(QPalette::HighlightedText);
            //painter->setPen(pal.color(QPalette::HighlightedText));
        } else if (option->state & QStyle::State_MouseOver) {
            QColor col = pal.color(QPalette::Highlight);
            col.setAlpha(127);
            painter->setBrush(col);
            textPen = pal.color(QPalette::HighlightedText);
        } else {
            painter->setBrush(pal.brush(QPalette::Window));
            textPen = pal.color(QPalette::WindowText);
        }
        painter->drawRect(rect);

        painter->setBrush(item->backgroundBrush);
        painter->drawRect(rect);

        QRect iconRect = rect, textRect = rect;

        textRect.setHeight(rect.height() - 2);
        if (!item->icon.isNull()) {
            QSize iconSize = ((QAbstractItemView*) widget)->iconSize();
            //QSize iconSize(16, 16);
            if (!iconSize.isValid()) {
                iconSize = QSize(16 * getDPIScaling(), 16 * getDPIScaling());
            }
            iconRect.setSize(iconSize);
            QIcon icon = item->icon;
            QImage iconImage = icon.pixmap(iconSize).toImage();
            iconRect.moveLeft(rect.left() + 2);
            iconRect.moveTop(rect.top() + (rect.height() / 2) - (iconRect.height() / 2));
            painter->drawImage(iconRect, iconImage);
            textRect.setLeft(iconRect.right() + 6);
        } else {
            textRect.setLeft(rect.left() + 6);
        }

        painter->setPen(textPen);
        painter->setFont(item->font);
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, item->text);
        break;
    }
    case QStyle::CE_DockWidgetTitle:
    {
        const QStyleOptionDockWidget* title = qstyleoption_cast<const QStyleOptionDockWidget*>(option);
        if (title == NULL) return;

        QString text = title->title;
        text.remove("&");

        QRect textRect = rect;
        textRect.adjust(4 * getDPIScaling(), 0, 0, 0);

        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
        break;
    }
    case QStyle::CE_MenuBarEmptyArea:
    case QStyle::CE_RadioButtonLabel:
    case QStyle::CE_CheckBoxLabel:
    case QStyle::CE_PushButtonLabel:
        break;
    default:
        QCommonStyle::drawControl(element, option, painter, widget);
        break;
    }
}

void Style::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const {
    if (option == NULL) return;
    if (painter == NULL) return;

    QPalette pal = option->palette;
    QRect rect = option->rect;
    if (widget != NULL) {
        painter->setFont(widget->font());
    }
    painter->setPen(transparent);

    switch (control) {
    case QStyle::CC_ComboBox:
    {
        const QStyleOptionComboBox* item = qstyleoption_cast<const QStyleOptionComboBox*>(option);
        if (item == NULL) return;

        painter->setPen(pal.color(QPalette::WindowText));
        QBrush brush;

        if (item->state & QStyle::State_Item) {
            brush = QBrush(pal.color(QPalette::Highlight).darker(150));
        } else {
            if (item->state & QStyle::State_Enabled) {
                brush = QBrush(pal.color(QPalette::Window));
            } else {
                brush = QBrush(pal.color(QPalette::Disabled, QPalette::Highlight));
            }

            if (item->state & QStyle::State_HasFocus) {
                brush = QBrush(pal.color(QPalette::Highlight).lighter(125));
            }

            if (item->state & QStyle::State_MouseOver) {
                brush = QBrush(pal.color(QPalette::Highlight).lighter());
            }

            if (item->state & QStyle::State_On) {
                brush = QBrush(pal.color(QPalette::Highlight).darker(150));
            }
        }
        painter->setBrush(brush);
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        if (item->currentIcon.isNull()) {
            painter->drawText(rect.adjusted(4 * getDPIScaling(), 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, item->currentText);
        } else {
            painter->drawText(rect.adjusted(8 * getDPIScaling() + item->iconSize.width(), 0, 0, 0), Qt::AlignVCenter | Qt::AlignLeft, item->currentText);
        }

        //if (item->state & QStyle::State_) {
            painter->setBrush(pal.color(QPalette::WindowText));
            painter->setPen(transparent);

            int halfHeight = rect.height() / 2;
            rect.adjust(1 * getDPIScaling(), 1 * getDPIScaling(), -2 * getDPIScaling(), -2 * getDPIScaling());

            QPolygon triangle;
            triangle.append(QPoint(rect.right(), rect.bottom()));
            triangle.append(QPoint(rect.right(), rect.bottom() - halfHeight));
            triangle.append(QPoint(rect.right() - halfHeight, rect.bottom()));
            painter->drawPolygon(triangle);
        //}
    }
    case QStyle::CC_ToolButton:
    {
        const QStyleOptionToolButton* button = qstyleoption_cast<const QStyleOptionToolButton*>(option);
        if (button == NULL) return;

        QBrush brush;
        QPen textPen;

        if (widget->property("type") == "positive") {
            if (button->state & QStyle::State_Enabled) {
                brush = QBrush(col(0, 200, 0));
            } else {
                brush = QBrush(col(67, 67, 67));
            }

            if (button->state & QStyle::State_HasFocus) {
                brush = QBrush(col(0, 225, 0));
            }

            if (button->state & QStyle::State_MouseOver) {
                brush = QBrush(col(0, 250, 0));
            }

            if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                brush = QBrush(col(0, 150, 0));
            }
            textPen = QColor(0, 0, 0);
        } else if (widget->property("type") == "destructive") {
            if (button->state & QStyle::State_Enabled) {
                brush = QBrush(col(200, 0, 0));
            } else {
                brush = QBrush(col(67, 67, 67));
            }

            if (button->state & QStyle::State_HasFocus) {
                brush = QBrush(col(225, 0, 0));
            }

            if (button->state & QStyle::State_MouseOver) {
                brush = QBrush(col(250, 0, 0));
            }

            if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                brush = QBrush(col(150, 0, 0));
            }
            textPen = col(255, 255, 255);
        } else {
            if (button->state & QStyle::State_Enabled) {
                brush = QBrush(pal.color(QPalette::Window));

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(pal.color(QPalette::Window).lighter());
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(pal.color(QPalette::Window).darker(150));
                }
            }
            textPen = pal.color(QPalette::WindowText);
        }

        painter->setBrush(brush);
        painter->drawRect(rect);

        QString text = button->text;

        QRect textRect, iconRect;
        bool showText, showIcon;
        switch (button->toolButtonStyle) {
        case Qt::ToolButtonIconOnly:
        case Qt::ToolButtonFollowStyle:
        {
            showText = false;
            showIcon = true;

            int fullWidth = textRect.width() + button->iconSize.width();
            int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

            iconRect.setLeft(iconLeft);
            iconRect.setTop(rect.top() + (rect.height() / 2) - (button->iconSize.height() / 2));
            iconRect.setSize(button->iconSize);
            break;
        }
        case Qt::ToolButtonTextBesideIcon:
        {
            showText = true;
            showIcon = true;
            textRect.setLeft(rect.left() + (rect.width() / 2) - (button->fontMetrics.width(text) / 2));
            textRect.setWidth(button->fontMetrics.width(text));
            textRect.setTop(rect.top() + (rect.height() / 2) - (button->fontMetrics.height() / 2));
            textRect.setHeight(button->fontMetrics.height());

            if (!button->icon.isNull()) {
                int fullWidth = textRect.width() + button->iconSize.width();
                int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

                iconRect.setLeft(iconLeft);
                iconRect.setTop(rect.top() + (rect.height() / 2) - (button->iconSize.height() / 2));
                iconRect.setSize(button->iconSize);

                //textRect.adjust(button->iconSize.width() / 2, 0, 0, 0);
                textRect.moveLeft(iconRect.right());
            }
            break;
        }
        case Qt::ToolButtonTextUnderIcon:
        {
            showText = true;
            showIcon = true;

            if (!button->icon.isNull()) {
                int fullWidth = textRect.width() + button->iconSize.width();
                int iconLeft = rect.left() + (rect.width() / 2) - (fullWidth / 2);

                iconRect.setLeft(iconLeft);
                iconRect.setTop(rect.top() + (rect.height() / 2) - (button->iconSize.height() / 2));
                iconRect.setSize(button->iconSize);

                //textRect.adjust(button->iconSize.width() / 2, 0, 0, 0);
                //textRect.moveLeft(iconRect.right());
            }

            textRect.setLeft(rect.left() + (rect.width() / 2) - (button->fontMetrics.width(text) / 2));
            textRect.setWidth(button->fontMetrics.width(text));
            //textRect.setTop(rect.top() + (rect.height() / 2) - (button->fontMetrics.height() / 2));
            textRect.setTop(iconRect.bottom());
            textRect.setHeight(button->fontMetrics.height());

            //Move each rectangle up a little
            iconRect.moveTop(iconRect.top() - (button->fontMetrics.height() / 2));
            textRect.moveTop(textRect.top() - (button->fontMetrics.height() / 2));
            break;
        }
        case Qt::ToolButtonTextOnly:
        {
            showText = true;
            showIcon = false;

            textRect.setLeft(rect.left() + (rect.width() / 2) - (button->fontMetrics.width(text) / 2));
            textRect.setWidth(button->fontMetrics.width(text));
            textRect.setTop(rect.top() + (rect.height() / 2) - (button->fontMetrics.height() / 2));
            textRect.setHeight(button->fontMetrics.height());
        }
        }

        if (showText) {
            //Draw text
            painter->setPen(textPen);
            painter->drawText(textRect, Qt::AlignCenter, text.remove("&"));
        }

        if (showIcon) {
            QIcon icon;
            if (button->arrowType == Qt::UpArrow) {
                icon = QIcon::fromTheme("go-up");
            } else if (button->arrowType == Qt::DownArrow) {
                icon = QIcon::fromTheme("go-down");
            } else if (button->arrowType == Qt::LeftArrow) {
                icon = QIcon::fromTheme("go-previous");
            } else if (button->arrowType == Qt::RightArrow) {
                icon = QIcon::fromTheme("go-next");
            } else {
                icon = button->icon;
            }
            QImage image = icon.pixmap(button->iconSize).toImage();
            image = image.convertToFormat(QImage::Format_ARGB32_Premultiplied);
            /*if (image.colorCount() == 0) {
                QPainter colorizer(&image);
                colorizer.setCompositionMode(QPainter::CompositionMode_SourceIn);
                colorizer.fillRect(image.rect(), textPen.brush());
                colorizer.end();
            }*/
            tintImage(image, textPen.color());

            painter->drawImage(iconRect, image);
        }

        break;
    }
    case QStyle::CC_Slider:
    {
        const QStyleOptionSlider* slider = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (slider == NULL) return;

        //Adjust rectangle to be only 10px tall if it is larger.
        if (rect.height() > 16 * getDPIScaling()) {
            int reduction = (rect.height() - 16 * getDPIScaling()) / 2;
            rect.adjust(0, reduction, 0, -reduction);
        }

        //Draw border
        painter->setPen(pal.color(QPalette::WindowText));
        painter->setBrush(pal.color(QPalette::Window));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));

        //Draw selected portion
        QRect selection = rect;
        selection.adjust(1, 1, -2 * getDPIScaling(), -2 * getDPIScaling());
        selection.setWidth((((float) slider->sliderPosition - (float) slider->minimum) / ((float) slider->maximum - (float) slider->minimum)) * (float) selection.width());
        painter->setPen(transparent);
        painter->setBrush(pal.brush(QPalette::Highlight));
        painter->drawRect(selection);

        //Draw Thumb
        QRect thumb = rect;
        thumb.setSize(QSize(rect.height() - 1 * getDPIScaling(), rect.height() - 1 * getDPIScaling()));
        thumb.moveLeft(selection.right() - (thumb.width() / 2));

        if (slider->state & State_On || slider->state & State_Sunken) {
            QColor highlightColor = pal.color(QPalette::Highlight).darker(150);
            painter->setPen(pal.color(QPalette::HighlightedText));
            painter->setBrush(highlightColor);
        } else if (slider->activeSubControls & QStyle::SC_SliderHandle) {
            painter->setPen(pal.color(QPalette::HighlightedText));
            painter->setBrush(pal.brush(QPalette::Highlight));
        } else {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->setBrush(pal.brush(QPalette::Window));
        }
        painter->drawRect(thumb);
        break;
    }
    case QStyle::CC_ScrollBar:
    {
        //Draw background
        painter->setBrush(pal.brush(QPalette::Window));
        painter->drawRect(rect);
        QCommonStyle::drawComplexControl(control, option, painter, widget);
        break;
    }
    default:
        QCommonStyle::drawComplexControl(control, option, painter, widget);
    }

}

void Style::drawPrimitive(PrimitiveElement primitive, const QStyleOption *option, QPainter *painter, const QWidget *widget) const {

    QPalette pal = option->palette;
    QRect rect = option->rect;

    if (widget != NULL) {
        painter->setFont(widget->font());
    }
    painter->setPen(transparent);

    int halfHeight = rect.height() / 2;
    int halfWidth = rect.width() / 2;

    switch (primitive) {
        case QStyle::PE_FrameGroupBox:
        case QStyle::PE_FrameDockWidget:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawRect(rect.adjusted(0, 0, -1 * getDPIScaling(), -1 * getDPIScaling()));
            break;
        }
        case QStyle::PE_FrameTabWidget:
        case QStyle::PE_FrameTabBarBase:
        {
            const QStyleOptionTabWidgetFrame* item = qstyleoption_cast<const QStyleOptionTabWidgetFrame*>(option);
            if (item == NULL) return;

            rect.adjust(0, 0, -1 * getDPIScaling(), -1 * getDPIScaling());
            int tabBarWidth = item->tabBarRect.width();
            int tabBarHeight = item->tabBarRect.height();

            painter->setPen(pal.color(QPalette::WindowText));
            if (item->shape == QTabBar::RoundedNorth) {
                painter->drawLine(rect.left() + tabBarWidth, rect.top(), rect.right(), rect.top());
            } else {
                painter->drawLine(rect.topLeft(), rect.topRight());
            }

            if (item->shape == QTabBar::RoundedEast) {
                painter->drawLine(rect.right(), rect.top() + tabBarHeight, rect.right(), rect.bottom());
            } else {
                painter->drawLine(rect.topRight(), rect.bottomRight());
            }

            if (item->shape == QTabBar::RoundedSouth) {
                painter->drawLine(rect.left() + tabBarWidth, rect.bottom(), rect.right(), rect.bottom());
            } else {
                painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            }

            if (item->shape == QTabBar::RoundedWest) {
                painter->drawLine(rect.left(), rect.top() + tabBarHeight, rect.left(), rect.bottom());
            } else {
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
            }

            break;
        }
        /*case QStyle::PE_FrameTabWidget:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            painter->drawLine(rect.bottomRight(), rect.topRight());
            break;
        }*/
        case QStyle::PE_PanelLineEdit:
        {
            painter->setBrush(pal.brush(QPalette::Window));
            painter->drawRect(rect);
            if (option->state & QStyle::State_Enabled) {
                painter->setPen(pal.color(QPalette::WindowText));
            } else {
                painter->setPen(pal.color(QPalette::Disabled, QPalette::WindowText));
            }

            painter->drawLine(rect.topLeft(), rect.bottomLeft());
            painter->drawLine(rect.bottomLeft(), rect.bottomRight());
            break;
        }
        case QStyle::PE_PanelMenu:
        {
            painter->setBrush(pal.brush(QPalette::Window));
            painter->setPen(transparent);
            painter->drawRect(option->rect);

            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(option->rect.topLeft(), option->rect.bottomLeft());
            painter->drawLine(option->rect.bottomLeft(), option->rect.bottomRight());
            painter->drawLine(option->rect.bottomRight(), option->rect.topRight());
            painter->drawLine(option->rect.topRight(), option->rect.topLeft());
            break;
        }
        case QStyle::PE_IndicatorTabClose:
        {
            QBrush brush;
            if (option->state & QStyle::State_Enabled) {
                brush = QBrush(col(200, 0, 0));
            } else {
                brush = QBrush(col(67, 67, 67));
            }

            if (option->state & QStyle::State_MouseOver) {
                brush = QBrush(col(250, 0, 0));
            }

            if (option->state & QStyle::State_Sunken) {
                brush = QBrush(col(150, 0, 0));
            }

            QRect closeButtonRect = rect;
            //closeButtonRect.adjust(3, 3, -3, -3);

            painter->setBrush(brush);
            painter->drawEllipse(closeButtonRect);
            break;
        }
        case PE_IndicatorTabTearLeft:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topLeft(), rect.bottomLeft());
            break;
        }
        case PE_IndicatorTabTearRight:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topRight(), rect.bottomRight());
            break;
        }
        case QStyle::PE_IndicatorArrowRight:
        case QStyle::PE_IndicatorArrowLeft:
        case QStyle::PE_IndicatorArrowDown:
        case QStyle::PE_IndicatorArrowUp:
        case QStyle::PE_IndicatorSpinDown:
        case QStyle::PE_IndicatorSpinUp:
        {
            painter->setBrush(pal.color(QPalette::WindowText));

            QImage image;
            if (primitive == QStyle::PE_IndicatorArrowRight) {
                image = QIcon::fromTheme("go-next").pixmap(16 * getDPIScaling(), 16 * getDPIScaling()).toImage();
            } else if (primitive == QStyle::PE_IndicatorArrowLeft) {
                image = QIcon::fromTheme("go-previous").pixmap(16 * getDPIScaling(), 16 * getDPIScaling()).toImage();
            } else if (primitive == QStyle::PE_IndicatorArrowDown || primitive == QStyle::PE_IndicatorSpinDown) {
                image = QIcon::fromTheme("go-down").pixmap(16 * getDPIScaling(), 16 * getDPIScaling()).toImage();
            } else if (primitive == QStyle::PE_IndicatorArrowUp || primitive == QStyle::PE_IndicatorSpinUp) {
                image = QIcon::fromTheme("go-up").pixmap(16 * getDPIScaling(), 16 * getDPIScaling()).toImage();
            }
            tintImage(image, pal.color(QPalette::WindowText));

            QRect imageRect;
            imageRect.setTop(rect.top() + (rect.height() / 2) - 8 * getDPIScaling());
            imageRect.setLeft(rect.left() + (rect.width() / 2) - 8 * getDPIScaling());
            imageRect.setSize(QSize(16 * getDPIScaling(), 16 * getDPIScaling()));
            painter->drawImage(imageRect, image);
            break;
        }
        case QStyle::PE_IndicatorBranch:
        {

            if (option->state & QStyle::State_Item) {
                //Draw horizontal branch
                painter->setPen(pal.color(QPalette::WindowText));
                painter->drawLine(rect.left() + rect.width() / 2, rect.top() + rect.height() / 2, rect.right(), rect.top() + rect.height() / 2);

                //Draw top part of vertical branch
                painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.top() + rect.height() / 2);
            }


            if (option->state & QStyle::State_Sibling){
                //Show vertical branch
                painter->setPen(pal.color(QPalette::WindowText));
                painter->drawLine(rect.left() + rect.width() / 2, rect.top(), rect.left() + rect.width() / 2, rect.bottom());
            }

            if (option->state & QStyle::State_Children) {
                //Draw disclosure triangle (closed)
                QRect triangleRect = rect;
                triangleRect.setTop(rect.top() + (rect.height()) / 2 - 4 * getDPIScaling());
                triangleRect.setLeft(rect.left() + (rect.width()) / 2 - 4 * getDPIScaling());
                triangleRect.setSize(QSize(8 * getDPIScaling(), 8 * getDPIScaling()));
                painter->setBrush(pal.color(QPalette::WindowText));
                painter->drawRect(triangleRect);
            }

            if (option->state & QStyle::State_Open) {
                //Show disclosure triangle (open)
                QRect triangleRect = rect;
                triangleRect.setTop(rect.top() + (rect.height()) / 2 - 4 * getDPIScaling());
                triangleRect.setLeft(rect.left() + (rect.width()) / 2 - 4 * getDPIScaling());
                triangleRect.setSize(QSize(8 * getDPIScaling(), 8 * getDPIScaling()));
                painter->setBrush(pal.color(QPalette::Window));
                painter->setPen(pal.color(QPalette::WindowText));
                painter->drawRect(triangleRect);
            }
            break;
        }
        case QStyle::PE_PanelToolBar:
        {
            painter->setBrush(pal.color(QPalette::Window));
            painter->drawRect(rect);
            break;
        }
        case QStyle::PE_PanelItemViewItem:
        case QStyle::PE_PanelItemViewRow:
        {
            //this->drawControl(CE_ItemViewItem, option, painter, widget);

            const QStyleOptionViewItem* item = qstyleoption_cast<const QStyleOptionViewItem*>(option);
            if (item == NULL) return;

            painter->setPen(transparent);
            QPen textPen;
            if (option->state & QStyle::State_Selected) {
                painter->setBrush(pal.brush(QPalette::Highlight));
                textPen = pal.color(QPalette::HighlightedText);
                //painter->setPen(pal.color(QPalette::HighlightedText));
            } else if (option->state & QStyle::State_MouseOver) {
                QColor col = pal.color(QPalette::Highlight);
                col.setAlpha(127);
                painter->setBrush(col);
                textPen = pal.color(QPalette::HighlightedText);
            } else {
                painter->setBrush(pal.brush(QPalette::Window));
                textPen = pal.color(QPalette::WindowText);

                //painter->setPen(pal.color(QPalette::WindowText));
            }
            painter->drawRect(rect);

            painter->setBrush(item->backgroundBrush);
            painter->drawRect(rect);

            QRect iconRect, textRect = rect;

            textRect.setHeight(rect.height() - 2 * getDPIScaling());
            if (!item->icon.isNull()) {
                iconRect.setSize(((QAbstractItemView*) widget)->iconSize());
                QIcon icon = item->icon;
                QImage iconImage = icon.pixmap(iconRect.size()).toImage();
                iconRect.moveLeft(rect.left() + 2 * getDPIScaling());
                iconRect.moveTop(rect.top() + (rect.height() / 2) - (iconRect.height() / 2));
                painter->drawImage(iconRect, iconImage);
                textRect.setLeft(iconRect.right() + 6 * getDPIScaling());
            } else {
                textRect.setLeft(rect.left() + 6 * getDPIScaling());
            }


            painter->setPen(textPen);
            painter->drawText(rect, "");
            break;
        }
        case QStyle::PE_IndicatorMenuCheckMark:
        {
            painter->setBrush(pal.brush(QPalette::Window));

            QPolygon triangle1;
            triangle1.append(QPoint(rect.right(), rect.top()));
            triangle1.append(QPoint(rect.right(), rect.top() + halfHeight));
            triangle1.append(QPoint(rect.right() - halfHeight, rect.top()));
            painter->drawPolygon(triangle1);

            QPolygon triangle2;
            triangle2.append(QPoint(rect.right(), rect.bottom()));
            triangle2.append(QPoint(rect.right(), rect.bottom() - halfHeight));
            triangle2.append(QPoint(rect.right() - halfHeight, rect.bottom()));
            painter->drawPolygon(triangle2);
            break;
        }
        case QStyle::PE_PanelTipLabel:
        {
            painter->setBrush(pal.brush(QPalette::Window));
            painter->drawRect(rect);
        }
        case QStyle::PE_PanelStatusBar:
        {
            painter->setPen(pal.color(QPalette::WindowText));
            painter->drawLine(rect.topLeft(), rect.topRight());
            break;
        }
        case QStyle::PE_PanelButtonCommand:
        {
            const QStyleOptionButton* button = qstyleoption_cast<const QStyleOptionButton*>(option);
            if (button == NULL) return;
            QBrush brush;
            QPen textPen;

            if (widget == NULL) {
                //Sorry... :(
                goto drawNormalButton;
            }

            if (widget->property("type") == "positive") {
                if (button->state & QStyle::State_Enabled) {
                    brush = QBrush(col(0, 200, 0));
                } else {
                    brush = QBrush(col(67, 67, 67));
                }

                if (button->state & QStyle::State_HasFocus) {
                    brush = QBrush(col(0, 225, 0));
                }

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(col(0, 250, 0));
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(col(0, 150, 0));
                }
                textPen = QColor(0, 0, 0);
            } else if (widget->property("type") == "destructive") {
                if (button->state & QStyle::State_Enabled) {
                    brush = QBrush(col(200, 0, 0));
                } else {
                    brush = QBrush(col(67, 67, 67));
                }

                if (button->state & QStyle::State_HasFocus) {
                    brush = QBrush(col(225, 0, 0));
                }

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(col(250, 0, 0));
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(col(150, 0, 0));
                }
                textPen = col(255, 255, 255);
            } else {
                drawNormalButton:
                brush = QBrush(pal.color(QPalette::Window));

                if (button->state & QStyle::State_MouseOver) {
                    brush = QBrush(pal.color(QPalette::Window).lighter());
                }

                if (button->state & QStyle::State_Sunken || button->state & QStyle::State_On) {
                    brush = QBrush(pal.color(QPalette::Window).darker(150));
                }
                textPen = pal.color(QPalette::WindowText);
            }

            painter->setBrush(brush);
            painter->drawRect(rect);
            break;
        }
        /*case QStyle::PE_FrameButtonTool:
        case QStyle::PE_PanelButtonTool:
        {
            const QStyleOptionToolBox* button = qstyleoption_cast<const QStyleOptionToolBox*>(option);
            if (button == NULL) return;

            QBrush brush = QBrush(pal.color(QPalette::Window));
            QPen textPen;

            if (button->state & QStyle::State_MouseOver) {
                brush = QBrush(pal.color(QPalette::Window).lighter());
            }

            if (button->state & QStyle::State_Sunken) {
                brush = QBrush(pal.color(QPalette::Window).darker(150));
            }

            if (button->state & QStyle::State_On) {
                brush = QBrush(pal.color(QPalette::Highlight));
            }

            textPen = pal.color(QPalette::WindowText);

            painter->setBrush(brush);
            painter->drawRect(rect);

            //painter->drawText(rect, Qt::AlignCenter, button->text);
            break;
        }*/
        case QStyle::PE_IndicatorToolBarHandle:
        {
            const QStyleOptionToolBar* bar = qstyleoption_cast<const QStyleOptionToolBar*>(option);
            if (bar == NULL) return;

            painter->setPen(pal.color(QPalette::Foreground));
            if (bar->direction == Qt::LeftToRight) {
                painter->drawLine(rect.topLeft(), rect.bottomLeft());
                painter->drawLine(rect.left() + 2 * getDPIScaling(), rect.top(), rect.left() + 2 * getDPIScaling(), rect.bottom());
            } else if (bar->direction == Qt::RightToLeft) {
                painter->drawLine(rect.bottomLeft(), rect.bottomRight());
                painter->drawLine(rect.left(), rect.bottom() + 2 * getDPIScaling(), rect.right(), rect.bottom() + 2 * getDPIScaling());
            }
            break;
        }
        case QStyle::PE_IndicatorButtonDropDown:
        {
            painter->setBrush(pal.brush(QPalette::Foreground));

            QPolygon pol;
            pol.append(rect.bottomRight());
            pol.append(QPoint(rect.right(), rect.bottom() - 4 * getDPIScaling()));
            pol.append(QPoint(rect.right() - 4 * getDPIScaling(), rect.bottom()));

            painter->drawPolygon(pol);

            break;
        }
        case QStyle::PE_PanelButtonBevel:
        case QStyle::PE_PanelMenuBar:
        case QStyle::PE_FrameMenu:
        case PE_FrameFocusRect:
        case QStyle::PE_FrameStatusBar:
        {
            //Don't do anything.
            break;
        }
        default:
            //qDebug() << "Style not handled:" << primitive;
            QCommonStyle::drawPrimitive(primitive, option, painter, widget);
    }
}

QStyle::SubControl Style::hitTestComplexControl(ComplexControl cc, const QStyleOptionComplex *option, const QPoint &pt, const QWidget *w) const {
    if (option == NULL) return QStyle::SC_None;

    QRect rect = option->rect;

    switch (cc) {
    case QStyle::CC_Slider:
    {
        const QStyleOptionSlider* slider = qstyleoption_cast<const QStyleOptionSlider*>(option);
        if (slider == NULL) return QStyle::SC_None;

        QRect selection = rect;
        selection.adjust(1 * getDPIScaling(), 1 * getDPIScaling(), -2 * getDPIScaling(), -2 * getDPIScaling());
        selection.setWidth((((float) slider->sliderPosition - (float) slider->minimum) / ((float) slider->maximum - (float) slider->minimum)) * (float) selection.width());

        QRect thumb = rect;
        thumb.setSize(QSize(rect.height() - 1, rect.height() - 1));
        thumb.moveLeft(selection.right() - (thumb.width() / 2));

        if (thumb.contains(pt)) {
            return QStyle::SC_SliderHandle;
        } else {
            return QStyle::SC_SliderGroove;
        }
    }
    }
    return QCommonStyle::hitTestComplexControl(cc, option, pt, w);
}

void Style::polish(QWidget *widget) {
    if (widget == NULL) return;
    if (qobject_cast<QCommandLinkButton*>(widget)) {
        widget->setAttribute(Qt::WA_Hover);
        widget->setAttribute(Qt::WA_StyledBackground);
    } else if (qobject_cast<QPushButton*>(widget) ||
            qobject_cast<QCheckBox*>(widget) ||
            qobject_cast<QComboBox*>(widget) ||
            qobject_cast<QToolButton*>(widget) ||
            qobject_cast<QRadioButton*>(widget) ||
            qobject_cast<QSlider*>(widget)) {
        widget->setAttribute(Qt::WA_Hover);
    } else if (qobject_cast<QAbstractItemView*>(widget)) {
        qobject_cast<QAbstractItemView*>(widget)->viewport()->setAttribute(Qt::WA_Hover);
    }
}

QColor Style::col(int r, int g, int b) const {
    return QColor::fromRgb(r, g, b);
}

QSize Style::sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget) const {
    if (widget == NULL) return QSize();
    QSize size = contentsSize;

    switch (ct) {
        case CT_LineEdit:
        {
            size.setHeight(size.height() + 5 * getDPIScaling());
            return size;
        }
        case CT_MenuBarItem:
        {
            if (touchMode) {
                size.setHeight(size.height() + 20 * getDPIScaling());
            } else {
                size.setHeight(size.height() + 10 * getDPIScaling());
            }
            size.setWidth(size.width() + 20 * getDPIScaling());
            return size;
        }
        case CT_ScrollBar:
        {
            int width = 5;
            if (opt->state & QStyle::State_MouseOver) {
                width = 10;
            }

            if (size.width() > size.height()) { //Horizontal
                size.setHeight(width * getDPIScaling());
            } else { //Vertical
                size.setWidth(width * getDPIScaling());
            }
            return size;
        }
        case CT_ItemViewItem:
        {
            const QStyleOptionViewItem* item = qstyleoption_cast<const QStyleOptionViewItem*>(opt);
            if (item == NULL) return size;

            QString text = item->text;

            if (item->icon.isNull()) {
                QSize iconSize = ((QAbstractItemView*) widget)->iconSize();
                if (iconSize.isNull()) {
                    iconSize = QSize(16 * getDPIScaling(), 16 * getDPIScaling());
                }
                size.setHeight(opt->fontMetrics.height() + 6 * getDPIScaling());
                size.setWidth(opt->fontMetrics.width(text) + iconSize.width() + 32 * getDPIScaling());
            } else {
                QAbstractItemView* wid = (QAbstractItemView*) widget;
                QSize iconSize = item->icon.actualSize(wid->iconSize());
                size.setHeight(opt->fontMetrics.height() + iconSize.height() + 6 * getDPIScaling());
                size.setWidth(opt->fontMetrics.width(text) + iconSize.width() + 16 * getDPIScaling());
            }

            if (touchMode) {
                size.setHeight(size.height() + 20 * getDPIScaling());
            }
            return size;
        }
        case CT_PushButton:
        case CT_ToolButton:
        {
            if (touchMode) {
                size.setHeight(contentsSize.height() + 30 * getDPIScaling());
            } else {
                size.setHeight(contentsSize.height() + 20 * getDPIScaling());
            }
            size.setWidth(contentsSize.width() + 20 * getDPIScaling());
            return size;
        }
        case CT_TabBarTab:
        {
            size.setHeight(contentsSize.height() + 10 * getDPIScaling());
            size.setWidth(contentsSize.width() + 10 * getDPIScaling());
            return size;
        }
        case CT_ProgressBar:
        {
            size.setHeight(10 * getDPIScaling());
            return size;
        }
        case CT_MenuItem:
        {
            const QStyleOptionMenuItem* item = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
            if (item == NULL) return size;

            if (item->menuItemType == QStyleOptionMenuItem::Normal ||
                    (item->menuItemType == QStyleOptionMenuItem::Separator && item->text != "")) {
                size.setHeight(item->fontMetrics.height() + 6 * getDPIScaling());
                if (touchMode) {
                    size.setHeight(size.height() + 10 * getDPIScaling());
                }
                size.setWidth(item->fontMetrics.width(item->text) + 28 * getDPIScaling());
            } else {
                size = QCommonStyle::sizeFromContents(ct, opt, size, widget);
            }

            return size;
        }
        default:
            return QCommonStyle::sizeFromContents(ct, opt, size, widget);
    }
}

int Style::styleHint(StyleHint sh, const QStyleOption *opt, const QWidget *w, QStyleHintReturn *shret) const {
    switch (sh) {
    case QStyle::SH_Menu_MouseTracking:
    case QStyle::SH_MenuBar_MouseTracking:
    case QStyle::SH_Menu_SloppySubMenus:
    case QStyle::SH_RubberBand_Mask:
    case QStyle::SH_ComboBox_ListMouseTracking:
    case QStyle::SH_Menu_SupportsSections:
    case QStyle::SH_Slider_StopMouseOverSlider:
    case QStyle::SH_Widget_Animate:
    case QStyle::SH_Menu_KeyboardSearch:
    case QStyle::SH_DialogButtonBox_ButtonsHaveIcons:
    case QStyle::SH_ScrollBar_Transient:
        return true;
    case QStyle::SH_MessageBox_CenterButtons:
    case QStyle::SH_Slider_SnapToValue:
        return false;
    case QStyle::SH_ItemView_ScrollMode:
        return QAbstractItemView::ScrollPerPixel;
    case QStyle::SH_LineEdit_PasswordMaskDelay:
    case QStyle::SH_Menu_SubMenuPopupDelay:
        return 0;
    default:
        return QCommonStyle::styleHint(sh, opt, w, shret);
    }
}

int Style::layoutSpacing(QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption *option, const QWidget *widget) const {
    return 9 * getDPIScaling();
}

void Style::scheduleRepaint(const QWidget *widget, int after) const {
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

void Style::tintImage(QImage &image, QColor tint) const {
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

QVariant Style::animation(QString id, QVariant retVal) const {
    if (animationIds.contains(id)) {
        return animations.at(animationIds.indexOf(id));
    } else {
        return retVal;
    }
}

void Style::putAnimation(QString id, QString type, QVariant value) const {
    if (animationIds.contains(id)) {
        animations.replace(animationIds.indexOf(id), value);
        animationTypes.replace(animationIds.indexOf(id), type);
    } else {
        animationIds.append(id);
        animations.append(value);
        animationTypes.append(type);
    }
}

QString Style::currentType(QString id) const {
    if (animationIds.contains(id)) {
        return animationTypes.at(animationIds.indexOf(id));
    } else {
        return "";
    }
}

int Style::pixelMetric(PixelMetric m, const QStyleOption *opt, const QWidget *widget) const {
    switch (m) {
    case PM_MessageBoxIconSize:
        return 64 * getDPIScaling();
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
        return 1 * getDPIScaling();
    case PM_CheckBoxLabelSpacing:
    case PM_RadioButtonLabelSpacing:
        return 4 * getDPIScaling();
    case PM_ToolBarIconSize:
    case PM_SliderControlThickness:
        return 16 * getDPIScaling();
    case PM_ScrollView_ScrollBarOverlap:
        return 5 * getDPIScaling();
    default:
        return QCommonStyle::pixelMetric(m, opt, widget);
    }
}

QIcon Style::standardIcon(StandardPixmap standardIcon, const QStyleOption *opt, const QWidget *widget) const {
    switch (standardIcon) {
    case QStyle::SP_DialogNoButton:
        return QIcon::fromTheme("dialog-cancel");
    case QStyle::SP_TitleBarCloseButton:
        return QIcon::fromTheme("window-close");
    default:
        return QCommonStyle::standardIcon(standardIcon, opt, widget);
    }
}

void Style::drawItemText(QPainter *painter, const QRect &rect, int alignment, const QPalette &palette, bool enabled, const QString &text, QPalette::ColorRole textRole) const {
    //painter->setPen(palette.color(enabled ? QPalette::Normal : QPalette::Disabled, textRole));
    //painter->drawText(rect, alignment, text);
    QCommonStyle::drawItemText(painter, rect, alignment, palette, enabled, text, textRole);
}


float Style::getDPIScaling() const {
    float currentDPI = QApplication::desktop()->logicalDpiX();
    return currentDPI / (float) 96;
}
