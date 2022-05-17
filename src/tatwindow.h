#ifndef TATWINDOW_H
#define TATWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class TATWindow; }
QT_END_NAMESPACE

class TATWindow : public QMainWindow
{
    Q_OBJECT

public:
    TATWindow(QWidget *parent = nullptr);
    ~TATWindow();


private:
    Ui::TATWindow *ui;
};
#endif // TATWINDOW_H
