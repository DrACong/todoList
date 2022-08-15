/*************************************************
Copyright:bupt
Author:Wencong Su
Date:2022-06-16
Description: set a Delegate for the todoList to
record the Contents.
**************************************************/
#include "WrapTextDelegate.h"

WrapTextDelegate::WrapTextDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

WrapTextDelegate::~WrapTextDelegate()
{
    //qDebug()<<"WrapTextDelegate delete";
}

QWidget* WrapTextDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem  &/*option*/,
                                        const QModelIndex &/*index*/) const
{
    QTextEdit *editor = new QTextEdit(parent);
    editor->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::SubWindow);
    editor->setStyleSheet("background-color: rgb(185,185,138)");
    //editor->setAttribute(Qt::WA_TranslucentBackground);
    return editor;
}
//set line text
void WrapTextDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString text = index.model()->data(index, Qt::EditRole).toString();
    QTextEdit *lineEdit = static_cast<QTextEdit*>(editor);
    lineEdit->setText(text);
    //lineEdit->setFixedHeight(lineEdit->document()->size().height()+2);
    lineEdit->moveCursor(QTextCursor::End);
}
//set to model
void WrapTextDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
{
    QTextEdit *lineEdit = static_cast<QTextEdit*>(editor);
    QString text = lineEdit->toPlainText();//toPlainText：返回多行文本框的文本内容
    //qDebug()<<text;
    model->setData(index, text, Qt::EditRole);//绘制到tableview,会自动再执行setEditorData更新内容
}

void WrapTextDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                          const QModelIndex &/*index*/) const
{
    //((QAbstractItemModel*)index.model())->setData(index, "\n\n\n", Qt::DisplayRole);//model->setdata会自动触发setEditorData,即edit内容有变化
    editor->setGeometry(option.rect.x(),option.rect.y()-20,option.rect.width(),option.rect.height() + 40);
}
