#include "tatsettings.h"

static const QString group_settings = "Settings";
static const QString key_convToQtImg_sconv_srcPath = "SConvToQtImgSrcPath";
static const QString key_convToQtImg_sconv_saveMethod = "SConvToQtImgSaveMethod";
static const QString key_convToQtImg_bconv_srcPath = "BConvToQtImgSrcPath";
static const QString key_convToQtImg_bconv_suffixs = "BConvToQtImgSuffixs";

TATSettings::TATSettings(const QString &fileName, QSettings::Format format, QObject *parent)
    : QSettings(fileName, format, parent)
    , convToQtImg_sconv_saveMethod(OverrideSrcFile)
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
    endGroup();
    return true;
}

bool TATSettings::loadFromFile()
{
    beginGroup(group_settings);
    convToQtImg_sconv_srcPath = value(key_convToQtImg_sconv_srcPath).toString();
    convToQtImg_sconv_saveMethod = value(key_convToQtImg_sconv_saveMethod).toInt();
    convToQtImg_bconv_srcPath = value(key_convToQtImg_bconv_srcPath).toString();
    convToQtImg_bconv_suffixes = value(key_convToQtImg_bconv_suffixs).toString();
    endGroup();
    return true;
}
