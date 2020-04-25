/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2020 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#include "abstractitemviewhandler.h"

#include <QAbstractItemView>

struct AbstractItemViewHandlerPrivate {
    QMap<QWidget*, QMetaObject::Connection> connections;
};

AbstractItemViewHandler::AbstractItemViewHandler(QObject* parent) : AbstractWidgetHandler(parent) {
    d = new AbstractItemViewHandlerPrivate();
}

AbstractItemViewHandler::~AbstractItemViewHandler() {
    delete d;
}

void AbstractItemViewHandler::polish(QWidget* widget) {
    QAbstractItemView* view = qobject_cast<QAbstractItemView*>(widget);
    if (view) {
        view->viewport()->setAttribute(Qt::WA_Hover);

        QMetaObject::Connection c = connect(view->selectionModel(), &QItemSelectionModel::selectionChanged, this, [ = ] {
            QModelIndex index = view->currentIndex();
            if (index.isValid()) {
                view->setProperty("X-Contemporary-FocusDecorationGeometry", view->visualRect(index));
            }
        });
        d->connections.insert(widget, c);
    }
}

void AbstractItemViewHandler::unpolish(QWidget* widget) {
    QAbstractItemView* view = qobject_cast<QAbstractItemView*>(widget);
    if (view) {
        disconnect(d->connections.value(widget));
        d->connections.remove(widget);
    }
}
