#include "convtoqtimgtool.h"
#include "ui_convtoqtimgtool.h"
#include "tatapp.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#include <shellapi.h> // Shell32.lib
#endif

#include <QFileDialog>
#include <QMessageBox>
#include <QUuid>
#include <QButtonGroup>
#include <QDebug>

ConvToQtImgTool::ConvToQtImgTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConvToQtImgTool)
{
    ui->setupUi(this);

    // Load settings
    ui->lineEdit_sconv_srcPath->setText(theSettings->convToQtImg_sconv_srcPath);
    ui->lineEdit_bconv_srcPath->setText(theSettings->convToQtImg_bconv_srcPath);
    ui->lineEdit_bconv_suffixs->setText(theSettings->convToQtImg_bconv_suffixes);
    if (TATSettings::OverrideSrcFile == theSettings->convToQtImg_sconv_saveMethod)
        ui->radioButton_sconv_overrideSrc->setChecked(true);
    else if (TATSettings::SaveToNewFile == theSettings->convToQtImg_sconv_saveMethod)
        ui->radioButton_sconv_saveToNewFile->setChecked(true);

    // SConv save method radio buttons.
    QButtonGroup *grp = new QButtonGroup(this);
    grp->addButton(ui->radioButton_sconv_overrideSrc);
    grp->addButton(ui->radioButton_sconv_saveToNewFile);
    connect(grp, static_cast<void (QButtonGroup::*)(QAbstractButton*)>(&QButtonGroup::buttonClicked),
            this, [this](QAbstractButton *button) {
        if (button == ui->radioButton_sconv_overrideSrc)
            theSettings->convToQtImg_sconv_saveMethod = TATSettings::OverrideSrcFile;
        else if (button == ui->radioButton_sconv_saveToNewFile)
            theSettings->convToQtImg_sconv_saveMethod = TATSettings::SaveToNewFile;
    });

    // SConv browse src path.
    connect(ui->pushButton_sconv_browseSrcPath, &QPushButton::clicked, this, [this]() {
        QString path =
        QFileDialog::getOpenFileName(topLevelWidget(),
                                     QString(),
                                     QString(),
                                     "Image File(*.jpg *.png *.bmp);;All Files(*.*)");
        if (path.isEmpty())
            return;

        ui->lineEdit_sconv_srcPath->setText(path);
        theApp->settings()->convToQtImg_sconv_srcPath = path;
    });

    // SConv open src dir.
    connect(ui->pushButton_sconv_openSrcDir, &QPushButton::clicked, this, [this]() {
        QFileInfo info(ui->lineEdit_sconv_srcPath->text());
        QString dirPath = info.dir().absolutePath();
#ifdef Q_OS_WINDOWS
        dirPath = QDir::toNativeSeparators(dirPath);
        ShellExecute(NULL, TEXT("open"), dirPath.toStdWString().c_str(), NULL, NULL, SW_SHOW);
#endif
    });

    // SConv start
    connect(ui->pushButton_sconv_start, &QPushButton::clicked, this, [this]() {
        QString srcFilePath = ui->lineEdit_sconv_srcPath->text();

        if (srcFilePath.trimmed().isEmpty()) {
            QMessageBox::warning(topLevelWidget(),
                                 tr("Warning"),
                                 tr("Source file path can not be empty!"));
            return;
        }

        if (!QFile::exists(srcFilePath)) {
            QMessageBox::warning(topLevelWidget(),
                                 tr("Warning"),
                                 tr("Source file does not exists!"));
            return;
        }

        QImage image(srcFilePath);
        if (image.isNull()) {
            QMessageBox::warning(topLevelWidget(),
                                 tr("Warning"),
                                 tr("Unsupported format!"));
            return;
        }

        if (ui->radioButton_sconv_overrideSrc->isChecked()) {
            bool ok = convToQtImgOverrideSrc(srcFilePath);
            if (!ok) {
                QMessageBox::warning(topLevelWidget(),
                                     tr("Warning"),
                                     tr("Convert failed!"));
                return;
            }
            QMessageBox::information(topLevelWidget(),
                                     tr("Information"),
                                     tr("Successfully done!"));
        }
        else if (ui->radioButton_sconv_saveToNewFile->isChecked()) {
            QFileInfo info(srcFilePath);
            QString saveName = QString("%1_%2.%3")
                    .arg(info.completeBaseName())
                    .arg(QUuid::createUuid().toString(QUuid::WithoutBraces))
                    .arg(info.suffix());
            bool ok = image.save(info.dir().filePath(saveName));
            if (!ok) {
                QMessageBox::warning(topLevelWidget(),
                                     tr("Warning"),
                                     tr("Save to %1 Failed!").arg(saveName));
                return;
            }
            QMessageBox::information(topLevelWidget(),
                                     tr("Information"),
                                     tr("Successfully done!"));
        }
    });

    // BConv browse src dir.
    connect(ui->pushButton_bconv_browseSrcPath, &QPushButton::clicked, this, [this]() {
        QString path = QFileDialog::getExistingDirectory(topLevelWidget());
        if (path.isEmpty())
            return;
        ui->lineEdit_bconv_srcPath->setText(path);
        theSettings->convToQtImg_bconv_srcPath = path;
    });

    // BConv open src dir.
    connect(ui->pushButton_bconv_openSrcDir, &QPushButton::clicked, this, [this]() {
        QString dirPath = ui->lineEdit_bconv_srcPath->text();
#ifdef Q_OS_WINDOWS
        dirPath = QDir::toNativeSeparators(dirPath);
        ShellExecute(NULL, TEXT("open"), dirPath.toStdWString().c_str(), NULL, NULL, SW_SHOW);
#endif
    });

    // BConv start.
    connect(ui->pushButton_bconv_start, &QPushButton::clicked, this, [this]() {
        ui->pushButton_bconv_start->setEnabled(false);
        QString srcDirPath = ui->lineEdit_bconv_srcPath->text();
        QString suffixesStr = ui->lineEdit_bconv_suffixs->text();
        QStringList suffixes = suffixesStr.split(",");

        theSettings->convToQtImg_bconv_suffixes = suffixesStr;

        if (srcDirPath.isEmpty()) {
            QMessageBox::warning(topLevelWidget(),
                                 tr("Warning"),
                                 tr("Source directory path can not be empty!"));
            return;
        }

        if (!QFile::exists(srcDirPath)) {
            QMessageBox::warning(topLevelWidget(),
                                 tr("Warning"),
                                 tr("Source directory does not exists!"));
            return;
        }

        BConvToQtImgThread *thread = new BConvToQtImgThread();
        thread->setDirPath(srcDirPath);
        thread->setSuffixes(suffixes);

        connect(thread, &BConvToQtImgThread::fileTotalChanged, this, [this](qint64 count) {
            ui->label_bconv_status->setText(tr("Counting files: %1").arg(count));
        });

        connect(thread, &BConvToQtImgThread::fileFailedChanged, this, [this](qint64 count) {

        });

        connect(thread, &BConvToQtImgThread::convProgress, this, [this](qint64 finished, qint64 total) {
            ui->label_bconv_status->setText(tr("Converting: %1/%2").arg(finished).arg(total));
        });

        connect(thread, &BConvToQtImgThread::convFinished, this, [this](bool succeed, const QString &msg) {
            QString realMsg = msg;
            BConvToQtImgThread *thread = qobject_cast<BConvToQtImgThread*>(sender());
            const QStringList &failedList = thread->failedListCRef();
            realMsg += tr(" %1 files convert failed.").arg(failedList.size());
            ui->label_bconv_status->setText(realMsg);
            if (!failedList.isEmpty()) {
                QMessageBox::warning(topLevelWidget(), tr("Warning"), tr("%1 files failed!").arg(failedList.size()));
            }
            thread->deleteLater();
            ui->pushButton_bconv_start->setEnabled(true);
        });

        thread->start();

    });
}

ConvToQtImgTool::~ConvToQtImgTool()
{
    delete ui;
}

bool ConvToQtImgTool::convToQtImgOverrideSrc(const QString &path)
{
    QImage image(path);

    if (image.isNull()) {
        return false;
    }

    QFileInfo info(path);
    QString backupPath = path + "." + QUuid::createUuid().toString(QUuid::WithoutBraces);

    if (!QFile::rename(path, backupPath)) {
        return false;
    }

    if (!image.save(path)) {
        QFile::rename(backupPath, path);
        return false;
    }

    QFile::remove(backupPath);
    return true;
}

BConvToQtImgThread::BConvToQtImgThread(QObject *parent)
    : QThread(parent)
{

}

qint64 BConvToQtImgThread::countFileInDir(const QString &dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists()) {
        return 0;
    }

    QQueue<QString> dirs;
    qint64 count = 0;

    dirs.enqueue(dirPath);
    while (!dirs.isEmpty()) {
        QFileInfoList entries = QDir(dirs.dequeue()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        foreach (const QFileInfo &entry, entries) {
            if (entry.isFile()) {
                ++count;
            }
            else if (entry.isDir()) {
                dirs.enqueue(entry.absoluteFilePath());
            }
            else {

            }
        }
    }
    return count;
}

void BConvToQtImgThread::setDirPath(const QString &path)
{
    m_dirPath = path;
}

void BConvToQtImgThread::setSuffixes(const QStringList &suffixes)
{
    m_suffixes = suffixes;
}

void BConvToQtImgThread::run()
{
    m_failedList.clear();
    qint64 total = 0;
    qint64 finished = 0;
    qint64 failed = 0;
    QStringList lowerSuffixes;

    //
    if (!QDir(m_dirPath).exists()) {
        emit convFinished(false, tr("Source dir does not exists."));
        return;
    }

    // Suffixes to low case.
    foreach (const QString &s, m_suffixes) {
        QString ts = s.trimmed().toLower();
        if (!ts.isEmpty()) {
            lowerSuffixes.append(ts);
        }
    }

    // Get all files match conditions.
    QQueue<QString> files;
    QQueue<QString> dirs;
    dirs.enqueue(m_dirPath);
    while (!dirs.isEmpty()) {
        QFileInfoList entries = QDir(dirs.dequeue()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        foreach (const QFileInfo &entry, entries) {
            if (entry.isFile()) {
                if (lowerSuffixes.isEmpty() || lowerSuffixes.contains(entry.suffix().toLower())) {
                    ++total;
                    files.enqueue(entry.absoluteFilePath());
                    emit fileTotalChanged(total);
                }
            }
            else if (entry.isDir()) {
                dirs.enqueue(entry.absoluteFilePath());
            }
            else {

            }
        }
    }

    // No file matching conditions.
    if (0 == total) {
        emit convFinished(false, tr("No file match conditions"));
        return;
    }

    while (!files.isEmpty()) {
        QString path = files.dequeue();
        bool ok = ConvToQtImgTool::convToQtImgOverrideSrc(path);
        if (ok) {
            ++finished;
        }
        else {
            ++failed;
            m_failedList.append(path);
            emit fileFailedChanged(failed);
        }
        emit convProgress(finished + failed, total);
    }

    emit convFinished(true, tr("Successfully done."));
}

