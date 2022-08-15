#include "historydialog.h"
#include "qsqlquery.h"
#include "ui_historydialog.h"
#include <QRegExp>

historyDialog::historyDialog(QWidget *parent, QSqlDatabase *db) :
    QDialog(parent),
    ui(new Ui::historyDialog),
    db(*db)
{
    ui->setupUi(this);
    ui->historyView->verticalHeader()->hide();
    ui->historyView->setAlternatingRowColors(true);
    ui->historyView->setEditTriggers(QAbstractItemView::NoEditTriggers);//can not edit
    todolist = parent;
    ui->backButton->adjustSize();
    ui->searchEdit->setPlaceholderText("tips[title:example date:2022-05-06]");
    ui->searchEdit->setToolTip("各条件之间请用空格区分");
}

historyDialog::~historyDialog()
{
    delete sqlHistoryModel;
    delete ui;
}

void historyDialog::setHistoryViewData(QSqlTableModel *model)
{
    sqlHistoryModel = model;
    ui->historyView->setModel(sqlHistoryModel);//setmodel 之后才能设置头属性
    ui->historyView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->historyView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->historyView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->historyView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->historyView->setAlternatingRowColors(true);
    //ui->historyView->resizeRowsToContents();
}

/****************************************
* @projectName  historyDialog::updateOkItem
* @breif        update item when ok_button was clicked in todolist
* @param        tableData
* @return       true for success
* @author       Wencong
* @date         2022-07-17
*****************************************/
bool historyDialog::updateOkItem(tableData data)
{
    QSqlQuery query;
    QString str = QString("INSERT INTO history VALUES('%1','%2','%3')").arg(data.title,
                                                                             data.content,
                                                                             data.date);
    if(query.exec(str))
    {
        //update table view
        sqlHistoryModel->select();
        return true;
    }
    return false;
}

void historyDialog::on_backButton_clicked()
{
    this->hide();
    todolist->show();
}

void historyDialog::closeEvent(QCloseEvent*)
{
   on_backButton_clicked();
}

/****************************************
* @projectName  historyDialog::on_btn_search_clicked
* @breif        get input and search from db
* @param
* @return
* @author       Wencong
* @date         2022-07-09
*****************************************/
void historyDialog::on_btn_search_clicked()
{
    QString text = ui->searchEdit->text();
    inputConvertToFilter(text);
    //qDebug()<<text;
    sqlHistoryModel->setFilter(text);
    sqlHistoryModel->select();
    //ui->historyView->setModel(sqlHistoryModel);
}

/****************************************
* @projectName  historyDialog:inputConvertToFilter
* @breif        use for db's filter
* @param
* @return
* @author       Wencong
* @date         2022-07-10
*****************************************/
bool historyDialog::inputConvertToFilter(QString &input)
{
    //title:.*?\s qt中\\代表\,无 ?
    QString res;
    QRegExp title_rx("^title:.*\\s|title:.*$");
    QRegExp contents_rx("content:.*\\s|content:.*$");
    QRegExp date_rx("date:.*\\s|date:.*$");
    title_rx.setMinimal(true);
    contents_rx.setMinimal(true);
    date_rx.setMinimal(true);

    if(title_rx.indexIn(input) > -1)//find index
    {
        res += tr("%1'").arg(title_rx.cap(0).simplified().replace(":","='"));
    }
    if(contents_rx.indexIn(input) > -1)//find index
    {
        if(res.isEmpty())
            res += tr("%1'").arg(contents_rx.cap(0).simplified().replace(":","='"));
        else
            res += tr(" or %1'").arg(contents_rx.cap(0).simplified().replace(":","='"));
    }
    if(date_rx.indexIn(input) > -1)//find index
    {
        if(res.isEmpty())
            res += tr("%1'").arg(date_rx.cap(0).simplified().replace(":","='"));
        else
            res += tr(" or %1'").arg(date_rx.cap(0).simplified().replace(":","='"));
    }
    input = res;
    return res.isEmpty();
}

void historyDialog::on_btn_cancel_clicked()
{
    sqlHistoryModel->setTable("history");
    sqlHistoryModel->select();
    //clear search bar
    ui->searchEdit->clear();
    ui->historyView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->historyView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->historyView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    ui->historyView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void historyDialog::resizeEvent(QResizeEvent*)
{
    //ui->historyView->resizeRowsToContents();
}



