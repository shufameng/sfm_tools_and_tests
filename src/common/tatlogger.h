#ifndef TATLOGGER_H
#define TATLOGGER_H

#include <QFile>
#include <QDateTime>

class TATLogger
{
public:
    TATLogger();
    ~TATLogger();

    QString filePath() const;
    void setFilePath(const QString &path);

    bool open();
    void close();
    void add(const QString &log);
    bool isOpen() const;
    QString errorString() const;

private:
    QFile m_file;
};

#endif // TATLOGGER_H
