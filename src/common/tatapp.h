#ifndef TATAPP_H
#define TATAPP_H

#include <QApplication>
#include <QDir>
#include <QFile>

#include "tatlogger.h"
#include "tatsettings.h"

#if defined(qApp)
#undef qApp
#endif
#define theApp (static_cast<TATApp *>(QCoreApplication::instance()))
#define theSettings (static_cast<TATApp *>(QCoreApplication::instance())->settings())
#define theLogger (static_cast<TATApp *>(QCoreApplication::instance())->logger())

class TATApp : public QApplication
{
    Q_OBJECT
public:
    TATApp(int &argc, char **argv);
    ~TATApp();

    static QString userDataDirPath();
    static QString settingsFilePath();
    static QString logDirPath();

    TATLogger *logger() {
        return m_logger;
    }
    TATSettings *settings() {
        return m_settings;
    }

protected:
    void onAboutToQuit();

private:
    TATLogger *m_logger;
    TATSettings *m_settings;
};

#endif // TATAPP_H
