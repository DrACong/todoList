/*************************************************
Copyright:bupt
Author:Wencong Su
Date:2022-06-16
Description: Generate QAnimationbutton, click to
display and hide the main Dialog
**************************************************/
#include "enterbuttondialog.h"
#include "ui_enterbuttondialog.h"
#include <QGraphicsOpacityEffect>
#define BTN_WIDTH  60
#define BTN_HEIGHT  45

EnterButtonDialog::EnterButtonDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterButtonDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->move(0,0);

    //create Enterkey button
    EnterKey  = new QAnimationButton(this);
    EnterKey->setGeometry(this->geometry().center().x() - BTN_WIDTH/2,
                             this->geometry().center().y() - BTN_HEIGHT/2,
                             BTN_WIDTH,
                             BTN_HEIGHT);;
    EnterKey->setStyleSheet("border:none;border-radius:15px;border-image:url(:/src/close.png)");

    //set icon to systemTray
    setTrayActionMenu();

    w = new TodoListDialog();
    connect(EnterKey, SIGNAL(clicked()),
            this, SLOT(show_mainDialog()));
    connect(w, SIGNAL(closeTodoList()),
            this, SLOT(show_mainDialog()));

}

EnterButtonDialog::~EnterButtonDialog()
{
    delete ui;
    if(EnterKey != nullptr)
        delete EnterKey;
    //delete w;don't need
}

void EnterButtonDialog::show_mainDialog()
{
    if(w->isVisible()) {
        EnterKey->setStyleSheet("border:none;border-radius:15px;border-image:url(:/src/close.png)");
        w->hide();
    }
    else//[case 1] history is visible, [case 2]history is non-visiable
    {
        if(w->history->isVisible())
        {
            w->history->hide();
            EnterKey->setStyleSheet("border:none;border-radius:15px;border-image:url(:/src/close.png)");
            return;
        }
        EnterKey->setStyleSheet("border:none;border-radius:15px;border-image:url(:/src/open.png)");
        w->show();

    }

}

void EnterButtonDialog::activatedSystemTrayIcon(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:

        break;
    case QSystemTrayIcon::DoubleClick:
        this->show();
        if(!w->isVisible())
            show_mainDialog();
        break;
    default:
        break;
    }
}

void EnterButtonDialog::setTrayActionMenu()
{
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(this);
    pMenu = new QMenu(this);
    trayIcon->setIcon(QIcon(":/src/book.ico"));
    trayIcon->setToolTip("待办事项");
    pShowMain = new QAction("显示主界面",this);
    pShowIcon = new QAction("显示桌面图标",this);
    pHideIcon = new QAction("隐藏桌面图标",this);
    pQuit = new QAction("退出",this);

    connect(pShowMain, &QAction::triggered, this, &EnterButtonDialog::show_mainDialog);
    connect(pShowIcon, &QAction::triggered, this, &EnterButtonDialog::show);
    connect(pHideIcon, &QAction::triggered, this, &EnterButtonDialog::hide);
    connect(pQuit, &QAction::triggered, this, qApp->quit);
    connect(trayIcon, &QSystemTrayIcon::activated, this, &EnterButtonDialog::activatedSystemTrayIcon);

    pMenu->addAction(pShowMain);
    pMenu->addAction(pShowIcon);
    pMenu->addAction(pHideIcon);
    pMenu->addSeparator();
    pMenu->addAction(pQuit);
    trayIcon->setContextMenu(pMenu);
    trayIcon->show();
}
