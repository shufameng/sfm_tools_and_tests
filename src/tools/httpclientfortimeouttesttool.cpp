#include "httpclientfortimeouttesttool.h"
#include "ui_httpclientfortimeouttesttool.h"
#include "tatapp.h"



HttpClientForTimeoutTestTool::HttpClientForTimeoutTestTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HttpClientForTimeoutTestTool),
    m_manager(nullptr)
{
    ui->setupUi(this);

    // Load and update settings.
    ui->lineEdit_url->setText(theSettings->httpClientForTimeoutTest_url);
    connect(ui->lineEdit_url, &QLineEdit::textChanged, [](const QString &text) {
        theSettings->httpClientForTimeoutTest_url = text;
    });

    connect(ui->pushButton_sendRequest, &QPushButton::clicked,
            this, &HttpClientForTimeoutTestTool::onButtonSendRequest);
}

HttpClientForTimeoutTestTool::~HttpClientForTimeoutTestTool()
{
    delete ui;
}

void HttpClientForTimeoutTestTool::onButtonSendRequest()
{
    if (!m_manager) {
        m_manager = new QNetworkAccessManager(this);
    }

    QNetworkRequest req(QUrl(ui->lineEdit_url->text()));
    QNetworkReply *pRep = m_manager->get(req);
    connect(pRep, &QNetworkReply::readyRead, this, [this]() {
        QNetworkReply *pRep = qobject_cast<QNetworkReply*>(sender());
        QByteArray data = pRep->readAll();
        addLog(QString("%1 bytes data received").arg(data.length()));
    });
    connect(pRep, &QNetworkReply::finished, this, [this]() {
        QNetworkReply *pRep = qobject_cast<QNetworkReply*>(sender());
        addLog(QString("Reply finished, code=%1, msg=%2")
               .arg(pRep->error())
               .arg(pRep->errorString()));
        pRep->deleteLater();
    });
    connect(pRep, &QNetworkReply::metaDataChanged, this, [this]() {
        QNetworkReply *pRep = qobject_cast<QNetworkReply*>(sender());
        uint httpCode = pRep->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        uint contentLen = pRep->header(QNetworkRequest::ContentLengthHeader).toUInt();
        addLog(QString("metaDataChanged, httpStatusCode=%1, contentLength=%2")
               .arg(httpCode)
               .arg(contentLen));
    });
}

void HttpClientForTimeoutTestTool::addLog(const QString &log)
{
    ui->plainTextEdit_log->appendPlainText(
                QString("[%1] %2")
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                .arg(log)
                );
}
