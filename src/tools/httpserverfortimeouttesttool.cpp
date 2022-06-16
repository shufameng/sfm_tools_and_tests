#include "httpserverfortimeouttesttool.h"
#include "ui_httpserverfortimeouttesttool.h"
#include "tatapp.h"
#include <QInputDialog>

HttpServerForTimeoutTestTool::HttpServerForTimeoutTestTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HttpServerForTimeoutTestTool)
{
    ui->setupUi(this);
    ui->pushButton_stop->setDisabled(true);

    // Loading settings
    ui->lineEdit_ip->setText(theSettings->httpSvrForTimeoutTest_ip);
    ui->lineEdit_port->setText(QString::number(theSettings->httpSvrForTimeoutTest_port));

    connect(ui->pushButton_start, &QPushButton::clicked,
            this, &HttpServerForTimeoutTestTool::onButtonStart);
    connect(ui->pushButton_stop, &QPushButton::clicked,
            this, &HttpServerForTimeoutTestTool::onButtonStop);

    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::acceptError,
            this, &HttpServerForTimeoutTestTool::onServerAcceptError);
    connect(m_server, &QTcpServer::newConnection,
            this, &HttpServerForTimeoutTestTool::onServerNewConnection);

    // Update settings
    connect(ui->lineEdit_ip, &QLineEdit::textChanged, [](const QString &text) {
        theSettings->httpSvrForTimeoutTest_ip = text;
    });
    connect(ui->lineEdit_port, &QLineEdit::textChanged, [](const QString &text) {
        theSettings->httpSvrForTimeoutTest_port = text.toUInt();
    });

    connect(ui->listWidget_conns, &QListWidget::customContextMenuRequested,
            this, &HttpServerForTimeoutTestTool::onListWidgetCustomMenuRequested);

    connect(ui->listWidget_conns, &QListWidget::currentTextChanged, this, [this](const QString &text) {
        ui->label_selectedConn->setText(QString("Selected Connection: %1")
                                        .arg(text));
    });

    connect(ui->pushButton_sendData, &QPushButton::clicked,
            this, &HttpServerForTimeoutTestTool::onButtonSendData);
    connect(ui->pushButton_genMetadata, &QPushButton::clicked,
            this, &HttpServerForTimeoutTestTool::onButtonGenMetaData);
    connect(ui->pushButton_genDataOfLen, &QPushButton::clicked,
            this, &HttpServerForTimeoutTestTool::onButtonGenDataOfLen);

    connect(ui->pushButton_cleanLog, &QPushButton::clicked, this, [this]() {
        ui->textEdit_log->clear();
    });
}

HttpServerForTimeoutTestTool::~HttpServerForTimeoutTestTool()
{
    delete ui;
}

void HttpServerForTimeoutTestTool::addLog(const QString &log)
{
    ui->textEdit_log->append(
                QString("[%1] %2")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(log)
                );
}

QString HttpServerForTimeoutTestTool::getClientName(const QTcpSocket *s)
{
    if (!s)
        return QString();

    return QString("%1:%2")
            .arg(s->peerAddress().toString())
            .arg(s->peerPort());
}

void HttpServerForTimeoutTestTool::onButtonStart()
{
    if (m_server->isListening())
        return;

    bool ok;
    ok = m_server->listen(QHostAddress::Any, ui->lineEdit_port->text().toUShort());
    if (!ok) {
        addLog(QString("Start server failed, %1").arg(m_server->errorString()));
        return;
    }

    addLog("Server started.");
    setStatus(Started);
}

void HttpServerForTimeoutTestTool::onButtonStop()
{
    m_server->close();
    addLog("Server stopped.");
    setStatus(Stopped);
}

void HttpServerForTimeoutTestTool::onServerAcceptError(QAbstractSocket::SocketError error)
{
    addLog(QString("Accept error, code=%1").arg(error));
}

void HttpServerForTimeoutTestTool::onServerNewConnection()
{
    QTcpSocket *s = m_server->nextPendingConnection();
    if (!s)
        return;

    addLog(QString("New connection, %1").arg(getClientName(s)));

    addClientToList(s);

    connect(s, &QTcpSocket::connected, this, &HttpServerForTimeoutTestTool::onClientConnected);
    connect(s, &QTcpSocket::disconnected, this, &HttpServerForTimeoutTestTool::onClientDisconnected);
    connect(s, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &HttpServerForTimeoutTestTool::onClientError);
    connect(s, &QTcpSocket::readyRead, this, &HttpServerForTimeoutTestTool::onClientReadyRead);
}

void HttpServerForTimeoutTestTool::onClientConnected()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    addLog(QString("Client %1 connected").arg(getClientName(s)));
}

void HttpServerForTimeoutTestTool::onClientDisconnected()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    addLog(QString("Client %1 disconnected").arg(getClientName(s)));
    removeClientFromList(s);
}

void HttpServerForTimeoutTestTool::onClientError(QAbstractSocket::SocketError error)
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    addLog(QString("Client %1 error, code=%2, msg=%3")
           .arg(getClientName(s))
           .arg(error)
           .arg(s->errorString()));
}

void HttpServerForTimeoutTestTool::onClientReadyRead()
{
    QTcpSocket *s = qobject_cast<QTcpSocket*>(sender());
    addLog(QString("Data received from client %1: %2")
           .arg(getClientName(s))
           .arg(QString::fromUtf8(s->readAll())));
}

void HttpServerForTimeoutTestTool::addClientToList(QTcpSocket *s)
{
    QListWidgetItem *item = new QListWidgetItem(ui->listWidget_conns);
    item->setData(Qt::UserRole + 1, QVariant((quint32)s));
    item->setText(getClientName(s));
    ui->label_connCount->setText(QString("Connections: %1").arg(ui->listWidget_conns->count()));
}

void HttpServerForTimeoutTestTool::removeClientFromList(QTcpSocket *s)
{
    int count = ui->listWidget_conns->count();
    for (int i = 0; i < count; ++i) {
        QListWidgetItem *item = ui->listWidget_conns->item(i);
        if (item->data(Qt::UserRole + 1).toUInt() == (quint32)s) {
            ui->listWidget_conns->takeItem(i);
            delete item;
            s->deleteLater();
            break;
        }
    }
    ui->label_connCount->setText(QString("Connections: %1").arg(ui->listWidget_conns->count()));
}

void HttpServerForTimeoutTestTool::setStatus(HttpServerForTimeoutTestTool::Status s)
{
    if (m_status != s) {
        m_status = s;

        switch (s) {
        case Started:
            ui->pushButton_start->setEnabled(false);
            ui->pushButton_stop->setEnabled(true);
            ui->label_status->setText("Status: started");
            ui->lineEdit_ip->setEnabled(false);
            ui->lineEdit_port->setEnabled(false);
            break;
        case Stopped:
            ui->pushButton_start->setEnabled(true);
            ui->pushButton_stop->setEnabled(false);
            ui->label_status->setText("Status: stopped");
            ui->lineEdit_ip->setEnabled(true);
            ui->lineEdit_port->setEnabled(true);
            break;
        default:
            break;
        }
    }
}

void HttpServerForTimeoutTestTool::onListWidgetCustomMenuRequested(const QPoint &p)
{
    if (!ui->listWidget_conns->itemAt(p))
        return;

    QMenu m;
    m.addAction(tr("Disconnect"), this, [this]() {
        QTcpSocket *s = selectedConnSocket();
        if (s) {
            s->close();
        }
    });
    m.exec(QCursor::pos());
}

void HttpServerForTimeoutTestTool::onButtonSendData()
{
    QString data = ui->textEdit_data->toPlainText();

    if (ui->checkBox_replaceBreackLine->isChecked())
        data.replace("\n", "\r\n");

    QTcpSocket *s = selectedConnSocket();
    if (s)
        s->write(data.toUtf8());
    else
        QMessageBox::warning(window(), tr("Warning"),
                             tr("No connection was selected!\nSelect a connection in the list view!"));
}

void HttpServerForTimeoutTestTool::onButtonGenMetaData()
{
    ui->textEdit_data->setText(
                "HTTP/1.1 200 OK\n"
                "Content-Length: 1024\n\n"
                );
}

void HttpServerForTimeoutTestTool::onButtonGenDataOfLen()
{
    QInputDialog dlg(window());
    dlg.setInputMode(QInputDialog::IntInput);
    dlg.setIntRange(1, 1024*1024);
    dlg.setIntValue(1024);
    dlg.setWindowTitle(tr("Specify data length"));
    dlg.setLabelText(tr("Lenght Of Data:"));
    if (QDialog::Rejected == dlg.exec())
        return;

    int len = dlg.intValue();
    ui->textEdit_data->setPlainText(QString(len, '1'));
}

QTcpSocket *HttpServerForTimeoutTestTool::selectedConnSocket()
{
    QListWidgetItem *item = ui->listWidget_conns->currentItem();
    if (!item)
        return nullptr;

    return (QTcpSocket*)item->data(Qt::UserRole + 1).toUInt();
}

