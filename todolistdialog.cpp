/*************************************************
Copyright:bupt
Author:Wencong Su
Date:2022-06-16
Description: a main Dialog of TodoList, it have
a tableview to record title, content and due date.
...
**************************************************/
#include "todolistdialog.h"
#include "ui_todolistdialog.h"
#include "WrapTextDelegate.h"

TodoListDialog::TodoListDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TodoListDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);
    btn_quit_flag = false;

    //set gif icon
    setIconGif();
    ui->btn_del->setEnabled(false);

    //check or create DataBase
    createDB();
    createTable();

    //create historyDialog
    history = new historyDialog(this,&db);

    //set StandardItemModel && title[main table]
    setStandardItemModelAndTitle();

    //create a calendar for global used
    cal = new QCalendarWidget(this);
    cal->setGeometry(0,0,296,188);
    cal->setGridVisible(true);
    cal->setHidden(true);
    connect(cal, SIGNAL(clicked(QDate)),
            this, SLOT(show_calendarEnable(QDate)));

    //update item from todolist.db
    updateListData();
    createHistoryModel();
}

TodoListDialog::~TodoListDialog()
{
    db.close();
    if(history)
        delete history;
    if(model)
        delete model;
    if(click_btn != nullptr)
        delete click_btn;
    delete cal;
    delete movie;
    delete ui;

}

/****************************************
* @projectName  TodoList:createDB
* @breif        create/open DB for save data
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::createDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\ACong_data\\Qt project\\build-TodoList-Desktop_Qt_6_3_0_MinGW_64_bit-Release\\todolist.db");
    if(db.open() == true)
        qDebug()<<"创建/打开数据库成功";

    else
        qDebug()<<"创建/打开数据库失败";
}

/****************************************
* @projectName  TodoList:createTable
* @breif        create 2 table(todolist & history)
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::createTable()
{
    QSqlQuery query;
    if(!db.tables().contains("todolist"))
    {
        QString str1 = QString("CREATE TABLE todolist (title TEXT,content TEXT,date TEXT)");
        if(query.exec(str1) == false)
            qDebug()<<query.lastError().text();
    }
    if(!db.tables().contains("history"))
    {
        QString str2 = QString("CREATE TABLE history (title TEXT,content TEXT,date TEXT)");
        if(query.exec(str2) == false)
            qDebug()<<query.lastError().text();
    }
}

/****************************************
* @projectName  TodoList:updateListData
* @breif        update data when open this app firstly
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::updateListData()
{
    QSqlQuery query;
    QString str = QString("SELECT * FROM todolist");
    if(query.exec(str))
    {
        //int rowCount = 0;
        while(query.next())
        {
            on_btn_add_clicked();

            model->setData(model->index(index-1,0),query.value(0).toString());
            model->setData(model->index(index-1,1),query.value(1).toString());
            //btn[rowCount++]->setText(query.value(2).toString());
            ((QPushButton*)(ui->todoview->indexWidget(model->index(index-1,2))))->setText(query.value(2).toString());
            if(isOverDate(query.value(2).toString()))
            {
                click_btn->setStyleSheet(btnOverDueDateBG);
                model->item(index-1,0)->setToolTip("超时！！\n请优先处理或调整时间");
            }
        }
    }
    else
    {
        qDebug()<<query.lastError().text();
    }
}

/****************************************
* @projectName  TodoList:createHistoryModel
* @breif        save the history data for use looking
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::createHistoryModel()
{
    QSqlTableModel *sqlHistoryModel = new QSqlTableModel(this,db);//sqlHistoryModel should release when history was destroy
    sqlHistoryModel->setTable("history");//指定数据表
    sqlHistoryModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    sqlHistoryModel->select();
    history->setHistoryViewData(sqlHistoryModel);
}

/****************************************
* @projectName  TodoList:setIconGif
* @breif        set right-down icon for UI
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::setIconGif()
{
    movie = new QMovie(":/src/logo.gif");
    QSize gifSize(ui->icon_gif->width(),ui->icon_gif->height());
    movie->setScaledSize(gifSize);
    ui->icon_gif->setMovie(movie);
    movie->start();
}

/****************************************
* @projectName  TodoList:setStandardItemModelAndTitle
* @breif        Init UI
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::setStandardItemModelAndTitle()
{
    model = new QStandardItemModel(this);
    index = 0;//init index

    ui->todoview->setModel(model);
    //set Column titles
    model->setColumnCount(3);
    model->setHeaderData(0, Qt::Horizontal, "Title");
    model->setHeaderData(1, Qt::Horizontal, "Content");
    model->setHeaderData(2, Qt::Horizontal, "DueDate");

    //set each title sizemode
    ui->todoview->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->todoview->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->todoview->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->todoview->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //
    ui->todoview->setAlternatingRowColors(true);
    //hide serial number
    ui->todoview->verticalHeader()->hide();

    //set Vertical ScrollBar always on
    ui->todoview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}

bool TodoListDialog::isOverDate(QString date)
{
    return date < QDate::currentDate().toString("yyyy-MM-dd");
}


/****************************************
* @projectName  TodoList:on_btn_add_clicked
* @breif        slot function, will add new item.
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::on_btn_add_clicked()
{
    if(index > -1 && index < MAX_COUNT)
    {
        model->setItem(index,new QStandardItem(" "));
        QString dueDay = QDate::currentDate().toString("yyyy-MM-dd");
        QPushButton* button = new QPushButton(dueDay);
        ui->todoview->setIndexWidget(model->index(index,2), button);
        button->setStyleSheet(btnNormalBG);
        click_btn = button;
        //set text delegate
        //parent is this, for delete when quit todolist
        auto delegate = new WrapTextDelegate(this);
        ui->todoview->setItemDelegateForColumn(1,delegate);
        connect(button, SIGNAL(clicked()),
                this,SLOT(btn_calendar()));
        index++;
        ui->btn_del->setEnabled(true);
        ui->todoview->scrollToBottom();
    }
}

/****************************************
* @projectName  TodoList:on_btn_save_clicked
* @breif        slot function, save data to DB
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::on_btn_save_clicked()
{
    QSqlQuery query;
    //读取list现有每项item的值
    if(!query.exec("delete from todolist"))
        qDebug()<<query.lastError().text();
    for(int i=0;i<index;i++)
    {
        QString str = QString("INSERT INTO todolist VALUES('%1','%2','%3')")
                        .arg(model->data(model->index(i,0)).toString()
                        ,(model->data(model->index(i,1)).toString())
                        ,(((QPushButton*)(ui->todoview->indexWidget(model->index(i,2))))->text()));
        if(query.exec(str) == false)
            qDebug()<<query.lastError().text();
    }
    //遍历model里面每一项item，保存到todolist 中

}

/****************************************
* @projectName  TodoList:on_btn_del_clicked
* @breif        slot function, will delete selected item
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::on_btn_del_clicked()
{
    QItemSelectionModel *isSelectMode = ui->todoview->selectionModel();

    if(isSelectMode->hasSelection())
    {
        //btn.erase(btn.begin()+isSelectMode->currentIndex().row());
        model->removeRow(isSelectMode->currentIndex().row());
        index--;
        if(!index)
            ui->btn_del->setEnabled(false);
    }

}

/****************************************
* @projectName  TodoList:btn_calendar
* @breif        slot function, will show calendar when cilck button
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::btn_calendar()
{
    //get sender button, for set it's text
    if(cal->isVisible())
        cal->setHidden(true);
    else {
        click_btn = qobject_cast<QPushButton *>(sender());
        cal->move(geometry().width()/4, height()/4);
        cal->setHidden(false);
    }
}

/****************************************
* @projectName  TodoList:show_calendarEnable
* @breif        slot function, will set selected item's date when chosse a date
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::show_calendarEnable(QDate date)
{
    cal->setHidden(true);
    click_btn->setText(date.toString("yyyy-MM-dd"));
    click_btn->setFocus();
    int row = ui->todoview->selectionModel()->currentIndex().row();
    if(isOverDate(date.toString("yyyy-MM-dd")))
    {
        click_btn->setStyleSheet(btnOverDueDateBG);
        //model->item(row,2)->setBackground(QColor(OVER_DUEDATE_COLOR));
        model->item(row,0)->setToolTip("超时！！\n请优先处理或调整时间");

    }
    else
    {
        click_btn->setStyleSheet(btnNormalBG);
        model->item(row,0)->setToolTip("");
    }
    //qDebug()<<ui->todoview->selectionModel()->currentIndex().row();
}

/****************************************
* @projectName  TodoList:on_btn_history_clicked
* @breif        slot function, to see history list
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::on_btn_history_clicked()
{
    this->hide();
    history->show();
}

/****************************************
* @projectName  TodoList
* @breif
* @param
* @return
* @author       Wencong
* @date         2022-07-02
*****************************************/
void TodoListDialog::closeEvent(QCloseEvent*)
{
    if(btn_quit_flag)
        qApp->quit();
    else
    {
        emit closeTodoList();
    }
}

/****************************************
* @projectName  TodoList:on_btn_quit_clicked
* @breif        Yes to quit whole APP
* @param        None
* @return       None
* @author       Wencong
* @date         2022-07-05
*****************************************/
void TodoListDialog::on_btn_quit_clicked()
{
    QMessageBox::StandardButton mb = QMessageBox::question(this, "Quit",
                                                               " Quit TodoList ?\nMake sure you have been saved data",
                                                               QMessageBox::StandardButton::Yes|
                                                               QMessageBox::StandardButton::Cancel);
    if(QMessageBox::StandardButton::Yes == mb)
    {
        btn_quit_flag = true;
        this->close();
    }
}

/****************************************
* @projectName  TodoList::on_btn_ok_clicked
* @breif        remove selected item, update to history.db
* @param
* @return
* @author       Wencong
* @date         2022-07-16
*****************************************/

void TodoListDialog::on_btn_ok_clicked()
{
    QItemSelectionModel *isSelectMode = ui->todoview->selectionModel();

    if(isSelectMode->hasSelection())
    {
        tableData data;
        data.title = model->data(model->index(isSelectMode->currentIndex().row(),0)).toString();
        data.content = model->data(model->index(isSelectMode->currentIndex().row(),1)).toString();
        data.date = ((QPushButton*)(ui->todoview->indexWidget(model->index(isSelectMode->currentIndex().row(),2))))->text();
        if(history->updateOkItem(data))
        {
            //delete item from todolist
            QSqlQuery query;
            QString str = QString("select * from todolist where title='%1' and content='%2'").arg(data.title,
                                                                                                data.content);
            if(query.exec(str))//如果有数据，需要进行删除
            {
                str = QString("delete from todolist where title='%1' and content='%2'").arg(data.title,
                                                                                            data.content);
                query.exec(str);

            }
            //delete from todolist's view
            model->removeRow(isSelectMode->currentIndex().row());
            index--;
        }
    }
}

