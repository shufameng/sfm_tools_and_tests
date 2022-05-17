#ifndef TATSETTINGS_H
#define TATSETTINGS_H

#include <QSettings>

class TATSettings : public QSettings
{
    Q_OBJECT
public:

    enum ConvToQtImgSaveMethod {
        OverrideSrcFile,
        SaveToNewFile
    };

    TATSettings(const QString &fileName, Format format, QObject *parent = nullptr);

    void createDefaultSettings();
    bool saveToFile();
    bool loadFromFile();

    QString convToQtImg_sconv_srcPath;
    int     convToQtImg_sconv_saveMethod;
    QString convToQtImg_bconv_srcPath;
    QString convToQtImg_bconv_suffixes;
};

#endif // TATSETTINGS_H
