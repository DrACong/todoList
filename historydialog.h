#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class historyDialog;
}

struct tableData
{
    QString title;
    QString content;
    QString date;
};

class historyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit historyDialog(QWidget *parent = nullptr, QSqlDatabase *db = nullptr);
    ~historyDialog();

public:
    void setHistoryViewData(QSqlTableModel *);

    bool updateOkItem(tableData);

private:
    bool inputConvertToFilter(QString &);
signals:
    void back();

    //void closeHistoryList();

private slots:
    void on_backButton_clicked();

    void closeEvent(QCloseEvent*);

    void on_btn_search_clicked();

    void resizeEvent(QResizeEvent*);

    void on_btn_cancel_clicked();

private:
    Ui::historyDialog *ui;
    QWidget *todolist;//no need delete, parent has it's own destructor

    QSqlTableModel *sqlHistoryModel;//for save todolist's sqlHistoryModel,should delete when quit

    const QSqlDatabase db;//create Qt-sql connection
};

#endif // HISTORYDIALOG_H
