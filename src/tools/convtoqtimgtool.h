#ifndef CONVTOQTIMGTOOL_H
#define CONVTOQTIMGTOOL_H

#include <QWidget>
#include <QThread>
#include <QQueue>

namespace Ui {
class ConvToQtImgTool;
}

//
class BConvToQtImgThread : public QThread
{
    Q_OBJECT
public:
    explicit BConvToQtImgThread(QObject *parent = nullptr);
    static qint64 countFileInDir(const QString &dirPath);
    void setDirPath(const QString &path);
    void setSuffixes(const QStringList &suffixes);
    const QStringList &failedListCRef() const { return m_failedList; }
protected:
    void run() override;
signals:
    void fileTotalChanged(qint64 count);
    void fileFailedChanged(qint64 count);
    void convProgress(qint64 finished, qint64 total);
    void convFinished(bool succeed, const QString &msg);
private:
    QString m_dirPath;
    QStringList m_suffixes;
    QStringList m_failedList;
};

//
class ConvToQtImgTool : public QWidget
{
    Q_OBJECT

public:
    explicit ConvToQtImgTool(QWidget *parent = nullptr);
    ~ConvToQtImgTool();

    static bool convToQtImgOverrideSrc(const QString &path);

private:
    Ui::ConvToQtImgTool *ui;
};

#endif // CONVTOQTIMGTOOL_H
