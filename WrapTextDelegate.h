#ifndef WRAPTEXTDELEGATE_H
#define WRAPTEXTDELEGATE_H
#pragma once
#include <QStyledItemDelegate>
#include <QTextEdit>

class WrapTextDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit WrapTextDelegate(QObject *parent = nullptr);
    ~WrapTextDelegate();

public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};

#endif // WRAPTEXTDELEGATE_H
