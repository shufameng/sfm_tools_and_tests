#ifndef WAR3EDITORTOOL_H
#define WAR3EDITORTOOL_H

#include <QWidget>

namespace Ui {
class War3EditorTool;
}

class War3EditorTool : public QWidget
{
    Q_OBJECT

public:
    explicit War3EditorTool(QWidget *parent = nullptr);
    ~War3EditorTool();

    void updateResult();
    void updatePreview();

private:
    Ui::War3EditorTool *ui;
};

#endif // WAR3EDITORTOOL_H
