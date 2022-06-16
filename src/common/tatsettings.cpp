#include "tatsettings.h"

static const QString group_settings = "Settings";
static const QString key_convToQtImg_sconv_srcPath = "SConvToQtImgSrcPath";
static const QString key_convToQtImg_sconv_saveMethod = "SConvToQtImgSaveMethod";
static const QString key_convToQtImg_bconv_srcPath = "BConvToQtImgSrcPath";
static const QString key_convToQtImg_bconv_suffixs = "BConvToQtImgSuffixs";
static const QString key_httpSvrForTimeoutTest_ip = "HttpServerForTimeoutTestIP";
static const QString key_httpSvrForTimeoutTest_Port = "HttpServerForTimeoutTestPort";
static const QString key_httpClientForTimeoutTest_url = "HttpClientForTimeoutTestURL";

TATSettings::TATSettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent)
    , convToQtImg_sconv_saveMethod(OverrideSrcFile)
    , httpSvrForTimeoutTest_port(5678)
{

}

void TATSettings::createDefaultSettings()
{

}

bool TATSettings::saveToFile()
{
    beginGroup(group_settings);
    setValue(key_convToQtImg_sconv_srcPath, convToQtImg_sconv_srcPath);
    setValue(key_convToQtImg_sconv_saveMethod, convToQtImg_sconv_saveMethod);
    setValue(key_convToQtImg_bconv_srcPath, convToQtImg_bconv_srcPath);
    setValue(key_convToQtImg_bconv_suffixs, convToQtImg_bconv_suffixes);
    setValue(key_httpSvrForTimeoutTest_ip, httpSvrForTimeoutTest_ip);
    setValue(key_httpSvrForTimeoutTest_Port, httpSvrForTimeoutTest_port);
    setValue(key_httpClientForTimeoutTest_url, httpClientForTimeoutTest_url);
    endGroup();
    return true;
}

bool TATSettings::loadFromFile()
{
    QVariant v;
    beginGroup(group_settings);

    v = value(key_convToQtImg_sconv_srcPath);
    if (v.isValid())
            convToQtImg_sconv_srcPath = v.toString();

    v = value(key_convToQtImg_sconv_saveMethod);
    if (v.isValid())
            convToQtImg_sconv_saveMethod = v.toInt();

    v = value(key_convToQtImg_bconv_srcPath);
    if (v.isValid())
        convToQtImg_bconv_srcPath = v.toString();

    v = value(key_convToQtImg_bconv_suffixs);
    if (v.isValid())
        convToQtImg_bconv_suffixes = v.toString();

    v = value(key_httpSvrForTimeoutTest_ip);
    if (v.isValid())
        httpSvrForTimeoutTest_ip = v.toString();

    v = value(key_httpSvrForTimeoutTest_Port);
    if (v.isValid())
        httpSvrForTimeoutTest_port = v.toUInt();

    v = value(key_httpClientForTimeoutTest_url);
    if (v.isValid())
        httpClientForTimeoutTest_url = v.toString();

    endGroup();
    return true;
}
