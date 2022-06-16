#ifndef HTTPCLIENTFORTIMEOUTTESTTOOL_H
#define HTTPCLIENTFORTIMEOUTTESTTOOL_H

#include <QWidget>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDateTime>

namespace Ui {
class HttpClientForTimeoutTestTool;
}

class HttpClientForTimeoutTestTool : public QWidget
{
    Q_OBJECT

public:
    explicit HttpClientForTimeoutTestTool(QWidget *parent = nullptr);
    ~HttpClientForTimeoutTestTool();

protected:
    void onButtonSendRequest();
    void addLog(const QString &log);
private:
    Ui::HttpClientForTimeoutTestTool *ui;
    QNetworkAccessManager *m_manager;
};

#endif // HTTPCLIENTFORTIMEOUTTESTTOOL_H
