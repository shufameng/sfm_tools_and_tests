#include "tatapp.h"


TATApp::TATApp(int &argc, char **argv) :
    QApplication(argc, argv)
{
    m_logger = new TATLogger();
    m_logger->setFilePath(QDir(logDirPath()).filePath(
                              QDateTime::currentDateTime().toString("yyyy-MM-dd.log")));
    m_logger->open();

    m_settings = new TATSettings(settingsFilePath(), QSettings::IniFormat, this);
    m_settings->loadFromFile();

    connect(this, &TATApp::aboutToQuit, this, &TATApp::onAboutToQuit);
}

TATApp::~TATApp()
{
    delete m_logger;
}

QString TATApp::userDataDirPath()
{
    const QString dirName = "userdata";
    QDir dir(applicationDirPath());
    if (!dir.cd(dirName)) {
        dir.mkdir(dirName);
        dir.cd(dirName);
    }
    return dir.path();
}

QString TATApp::settingsFilePath()
{
    const QString dirName = "settings";
    const QString fileName = "general_settings.ini";
    QDir dir(userDataDirPath());
    if (!dir.cd(dirName)) {
        dir.mkdir(dirName);
        dir.cd(dirName);
    }
    return dir.filePath(fileName);
}

QString TATApp::logDirPath()
{
    const QString dirName = "log";
    QDir dir(userDataDirPath());
    if (!dir.cd(dirName)) {
        dir.mkdir(dirName);
        dir.cd(dirName);
    }
    return dir.path();
}

void TATApp::onAboutToQuit()
{
    settings()->saveToFile();
}
