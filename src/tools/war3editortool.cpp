#include "war3editortool.h"
#include "ui_war3editortool.h"
#include <QClipboard>
#include "tatapp.h"
#include <QDebug>

War3EditorTool::War3EditorTool(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::War3EditorTool)
{
    ui->setupUi(this);

    connect(ui->lineEdit_abilId, &QLineEdit::textChanged, this, [this](const QString &) {
        updateResult();
    });
    connect(ui->spinBox_maxLv, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int) {
        updateResult();
    });
    connect(ui->textEdit_mainDesc, &QTextEdit::textChanged, this, &War3EditorTool::updateResult);
    connect(ui->checkBox_area, &QCheckBox::clicked, this, &War3EditorTool::updateResult);
    connect(ui->checkBox_cost, &QCheckBox::clicked, this, &War3EditorTool::updateResult);
    connect(ui->checkBox_range, &QCheckBox::clicked, this, &War3EditorTool::updateResult);
    connect(ui->checkBox_cooldown, &QCheckBox::clicked, this, &War3EditorTool::updateResult);
    connect(ui->checkBox_duration, &QCheckBox::clicked, this, &War3EditorTool::updateResult);

    connect(ui->pushButton_copyToClipboard, &QPushButton::clicked, this, [this]() {
        QString text = ui->textEdit_res->toPlainText();
        text.replace("\n", "|n");
        theApp->clipboard()->setText(text);
    });

    ui->textEdit_preview->setStyleSheet("QTextEdit {"
                                        "border:none;"
                                        "background:#454545;"
                                        "}");

    connect(ui->textEdit_res, &QTextEdit::textChanged, this, &War3EditorTool::updatePreview);
}

War3EditorTool::~War3EditorTool()
{
    delete ui;
}

void War3EditorTool::updateResult()
{
    int lv = ui->spinBox_maxLv->value();
    QString id = ui->lineEdit_abilId->text();

    QString res;
    res += ui->textEdit_mainDesc->toPlainText();
    res += "\n\n";

    //QString temp;
    if (ui->checkBox_cost->isChecked()) {
        res += "魔法消耗: ";
        for (int i = 1; i <= lv; ++i) {
            res += QString("<%1,Cost%2>").arg(id).arg(i);
            if (i + 1 < lv) {
                res += "/";
            }
        }
        res += "\n";
    }
    if (ui->checkBox_cooldown->isChecked()) {
        res += "冷却时间: ";
        for (int i = 1; i <= lv; ++i) {
            res += QString("<%1,Cool%2>").arg(id).arg(i);
            if (i + 1 < lv) {
                res += "/";
            }
        }
        res += "\n";
    }
    if (ui->checkBox_duration->isChecked()) {
        res += "持续时间: ";
        for (int i = 1; i <= lv; ++i) {
            res += QString("<%1,Dur%2>").arg(id).arg(i);
            if (i + 1 < lv) {
                res += "/";
            }
        }
        res += "\n";
    }
    if (ui->checkBox_range->isChecked()) {
        res += "施法距离: ";
        for (int i = 1; i <= lv; ++i) {
            res += QString("<%1,Rng%2>").arg(id).arg(i);
            if (i + 1 < lv) {
                res += "/";
            }
        }
        res += "\n";
    }
    if (ui->checkBox_area->isChecked()) {
        res += "影响范围: ";
        for (int i = 1; i <= lv; ++i) {
            res += QString("<%1,Area%2>").arg(id).arg(i);
            if (i + 1 < lv) {
                res += "/";
            }
        }
        res += "\n";
    }

    ui->textEdit_res->setText(res);

}

void War3EditorTool::updatePreview()
{
    QString text = ui->textEdit_res->toPlainText();
    QString html;

    QString frag;
    QColor cl(Qt::white);
    for (auto iter = text.begin(); iter != text.end(); ++iter) {
        if ('\n' == (*iter)) {
            if (!frag.isEmpty()) {
                html += QString("<font color=\"%1\">%2</font>")
                        .arg(cl.name(QColor::HexRgb))
                        .arg(frag);
            }
            html += "<br>";

            frag.clear();
            cl = Qt::white;
        }
        else if ('|' == (*iter)) {

        }
        else {
            frag.append(*iter);
        }
    }

    qDebug() << html;

    ui->textEdit_preview->setHtml(html);
}
