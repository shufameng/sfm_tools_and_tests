#include "tatwindow.h"
#include "ui_tatwindow.h"

TATWindow::TATWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TATWindow)
{
    ui->setupUi(this);

    enum Tool {
        ConvToQtImg
    };

    QStringList tools;
    tools << tr("Convert to qt image");
    tools << tr("WarCraft3 editor tool");

    ui->listWidget_contents->addItems(tools);

    connect(ui->listWidget_contents, &QListWidget::currentRowChanged, this, [this](int row) {
        if (row >= 0) {
            ui->stackedWidget->setCurrentIndex(row);
        }
    });

    ui->listWidget_contents->setCurrentRow(0);
}

TATWindow::~TATWindow()
{
    delete ui;
}

