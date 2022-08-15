#ifndef TODOLISTDIALOG_H
#define TODOLISTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QDate>
#include <QCalendarWidget>
#include <QKeyEvent>
#include <QMovie>
#include <QMessageBox>

#include <QSqlDatabase>
#include <QSqlQuery>

#include <QSqlTableModel>
#include <QSqlError>

#include "historydialog.h"
#define MAX_COUNT 50
const QString btnOverDueDateBG = "QPushButton{background-color: red;border-style: none;}"
                                 "QPushButton:hover{ background-color:rgba(14 , 135 , 228,0.5); }"
                                 "QPushButton:pressed{ background-color:rgba(143 , 145 , 255,0.5); padding-left:3px; padding-top:3px;} ";
const QString btnNormalBG = "QPushButton{background-color:rgb(245,245,245);color: black;border-style: none;}"
                            "QPushButton:hover{ background-color:rgba(14 , 135 , 228,0.5); }"
                            "QPushButton:pressed{ background-color:rgba(143 , 145 , 255,0.5); padding-left:3px; padding-top:3px;} ";
QT_BEGIN_NAMESPACE
namespace Ui { class TodoListDialog; }
QT_END_NAMESPACE

class TodoListDialog : public QDialog
{
    Q_OBJECT

public:
    TodoListDialog(QWidget *parent = nullptr);
    ~TodoListDialog();

    //void keyPressEvent(QKeyEvent *);
private:
    void createDB();

    void createTable();

    void createHistoryModel();//use for history dialog

    void updateListData();

    void setIconGif();

    void setStandardItemModelAndTitle();

    bool isOverDate(QString);

signals:
    void closeTodoList();

private slots:
    void on_btn_add_clicked();

    void on_btn_del_clicked();

    void btn_calendar();

    void show_calendarEnable(QDate date);

    void on_btn_history_clicked();

    void on_btn_save_clicked();

    void closeEvent(QCloseEvent*);

    void on_btn_quit_clicked();

    void on_btn_ok_clicked();

private:
    Ui::TodoListDialog *ui;
    QStandardItemModel *model;
    //QVector<QPushButton*> btn;
    QCalendarWidget* cal;
    QMovie *movie;
    QPushButton* click_btn;//save button when calendar was opened
    int index;
    bool btn_quit_flag;

    QSqlDatabase db;//create Qt-sql connection

    int overDate = 0;
public:
    historyDialog *history;//use for history dialog


};
#endif // TODOLISTDIALOG_H
