#ifndef ENTERBUTTONDIALOG_H
#define ENTERBUTTONDIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QMenu>
#include "AnimationButton.h"
#include "todolistdialog.h"

namespace Ui {
class EnterButtonDialog;
}

class EnterButtonDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnterButtonDialog(QWidget *parent = nullptr);
    ~EnterButtonDialog();

    TodoListDialog* w;

private slots:
    void show_mainDialog();
    void activatedSystemTrayIcon(QSystemTrayIcon::ActivationReason);
private:
    Ui::EnterButtonDialog *ui;
    QAnimationButton* EnterKey;

    //action
    QAction *pHideIcon;
    QAction *pShowIcon;
    QAction *pShowMain;
    QAction *pQuit;
    QMenu *pMenu;
public:
    void setTrayActionMenu();
};

#endif // ENTERBUTTONDIALOG_H
