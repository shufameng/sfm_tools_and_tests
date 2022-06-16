#include "tatwindow.h"
#include "ui_tatwindow.h"

#include "tools/convtoqtimgtool.h"
#include "tools/war3editortool.h"
#include "tools/httpserverfortimeouttesttool.h"
#include "tools/httpclientfortimeouttesttool.h"

TATWindow::TATWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TATWindow)
{
    ui->setupUi(this);

    //
    QStringList tools;
    tools << tr("Convert to qt image");
    tools << tr("WarCraft3 editor tool");
    tools << tr("Http server for timeout test");
    tools << tr("Http client for timeout test");
    ui->listWidget_contents->addItems(tools);

    //
    int i = 0;
    ui->stackedWidget->insertWidget(i++, new ConvToQtImgTool());
    ui->stackedWidget->insertWidget(i++, new War3EditorTool());
    ui->stackedWidget->insertWidget(i++, new HttpServerForTimeoutTestTool());
    ui->stackedWidget->insertWidget(i++, new HttpClientForTimeoutTestTool());

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

