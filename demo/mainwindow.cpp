#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <tcsdtools.h>
#include <QActionGroup>

struct MainWindowPrivate {
    tCsdTools csd;
    QActionGroup* exclusiveGroup;
};

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    d = new MainWindowPrivate();

    d->csd.installMoveAction(ui->topWidget);
    d->csd.installResizeAction(this);

    if (tCsdGlobal::windowControlsEdge() == tCsdGlobal::Left) {
        ui->leftCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    } else {
        ui->rightCsdLayout->addWidget(d->csd.csdBoxForWidget(this));
    }

    ui->checkBox_4->setCheckState(Qt::PartiallyChecked);

    ui->menuBar->setParent(ui->menuFrame);
    ui->menuLayout->addWidget(ui->menuBar);

    d->exclusiveGroup = new QActionGroup(this);
    d->exclusiveGroup->addAction(ui->actionMutually_Exclusive_1);
    d->exclusiveGroup->addAction(ui->actionMutually_Exclusive_2);
    d->exclusiveGroup->addAction(ui->actionMutually_Exclusive_3);
    d->exclusiveGroup->setExclusive(true);
}

MainWindow::~MainWindow() {
    delete ui;
    delete d;
}

