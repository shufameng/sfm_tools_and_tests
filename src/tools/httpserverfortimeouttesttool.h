#ifndef HTTPSERVERFORTIMEOUTTESTTOOL_H
#define HTTPSERVERFORTIMEOUTTESTTOOL_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDateTime>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>

namespace Ui {
class HttpServerForTimeoutTestTool;
}

class HttpServerForTimeoutTestTool : public QWidget
{
    Q_OBJECT

public:
    enum Status {
        Started,
        Stopped
    };
    explicit HttpServerForTimeoutTestTool(QWidget *parent = nullptr);
    ~HttpServerForTimeoutTestTool();
    void addLog(const QString &log);
    static QString getClientName(const QTcpSocket *s);
protected:
    void onButtonStart();
    void onButtonStop();
    void onServerAcceptError(QAbstractSocket::SocketError error);
    void onServerNewConnection();
    void onClientConnected();
    void onClientDisconnected();
    void onClientError(QAbstractSocket::SocketError error);
    void onClientReadyRead();
    void addClientToList(QTcpSocket *s);
    void removeClientFromList(QTcpSocket *s);
    void setStatus(Status s);
    void onListWidgetCustomMenuRequested(const QPoint &pt);
    void onButtonSendData();
    void onButtonGenMetaData();
    void onButtonGenDataOfLen();
    QTcpSocket *selectedConnSocket();
private:
    Ui::HttpServerForTimeoutTestTool *ui;

    QTcpServer *m_server;
    Status m_status;
};

#endif // HTTPSERVERFORTIMEOUTTESTTOOL_H
