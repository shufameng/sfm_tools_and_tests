#include "tatlogger.h"

TATLogger::TATLogger()
{

}

TATLogger::~TATLogger()
{
    close();
}

QString TATLogger::filePath() const
{
    return m_file.fileName();
}

void TATLogger::setFilePath(const QString &path)
{
    m_file.setFileName(path);
}

bool TATLogger::open()
{
    if (m_file.isOpen())
        return true;

    return m_file.open(QIODevice::WriteOnly | QIODevice::Append);
}

void TATLogger::close()
{
    if (m_file.isOpen())
        m_file.close();
}

void TATLogger::add(const QString &log)
{
    if (!isOpen())
        return;

    m_file.write(QString("[%1] %2\n")
                 .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                 .arg(log).toUtf8());
    m_file.flush();
}

bool TATLogger::isOpen() const
{
    return m_file.isOpen();
}

QString TATLogger::errorString() const
{
    return m_file.errorString();
}
