//
// Created by giacomo on 12/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_QTDROPDOWNDELEGATE_H
#define FUZZYSTRINGMATCHING2_QTDROPDOWNDELEGATE_H


#include <QStyledItemDelegate>

//! [0]
class QTDropdownDelegate : public QStyledItemDelegate
{
Q_OBJECT

public:
    QTDropdownDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
};


#endif //FUZZYSTRINGMATCHING2_QTDROPDOWNDELEGATE_H
