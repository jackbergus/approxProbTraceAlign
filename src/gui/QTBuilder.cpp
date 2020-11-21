/*
 * QTBuilder.cpp
 * This file is part of ProbabilisticTraceAlignment
 *
 * Copyright (C) 2020 - Giacomo Bergami
 *
 * ProbabilisticTraceAlignment is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * ProbabilisticTraceAlignment is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ProbabilisticTraceAlignment. If not, see <http://www.gnu.org/licenses/>.
 */

 
//
// Created by giacomo on 12/11/20.
//


#include "gui/QTBuilder.h"

QLineEdit *addTextField(QGridLayout *echoGrid, size_t &rowid, const std::string &format, const std::string &defaultText,
                        int maxLength) {
    QLabel *echoLabel = new QLabel(QString::fromStdString(format));
    QLineEdit* tb = new QLineEdit();
    if (!defaultText.empty()) tb->setText(QString::fromStdString(defaultText));
    if (maxLength > 0)        tb->setMaxLength(maxLength);
    echoGrid->addWidget(echoLabel, rowid, 0);
    echoGrid->addWidget(tb, rowid, 1);

    rowid++;
    return tb;
}

QCheckBox *addCheckBox(QGridLayout *echoGrid, size_t &rowid, const std::string &format, bool checked) {
    QCheckBox* tb = new QCheckBox(QString::fromStdString(format));
    tb->setChecked(checked);
    echoGrid->addWidget(tb, rowid, 0, 1, 2);
    rowid++;
    return tb;
}

QSpinBox *addNumericBox(QGridLayout *echoGrid, size_t &rowid, const std::string &format, size_t defaultValue) {
    QLabel *echoLabel = new QLabel(QString::fromStdString(format));
    QSpinBox *tb = new QSpinBox();
    tb->setValue(defaultValue);
    tb->setRange(0, std::numeric_limits<int>::max());
    tb->setMaximum(std::numeric_limits<int>::max());
    echoGrid->addWidget(echoLabel, rowid, 0);
    echoGrid->addWidget(tb, rowid, 1);
    rowid++;
    return tb;
}

QDoubleSpinBox *
addNumericBox(QGridLayout *echoGrid, size_t &rowid, const std::string &format, double defaultValue, double min,
              double max) {
    QLabel *echoLabel = new QLabel(QString::fromStdString(format));
    QDoubleSpinBox *tb = new QDoubleSpinBox();
    tb->setValue(defaultValue);
    tb->setRange(min, max);
    tb->setMinimum(min);
    tb->setMaximum(max);
    tb->setSingleStep(0.001);
    tb->setDecimals(10);
    echoGrid->addWidget(echoLabel, rowid, 0);
    echoGrid->addWidget(tb, rowid, 1);
    rowid++;
    return tb;
}

QStandardItem *generateCheckboxCell(const std::string &itemFieldName, bool isChecked) {
    QStandardItem* item = new QStandardItem(QString::fromStdString(itemFieldName));
    item->setCheckable(true);
    item->setEditable(false);
    item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
    return item;
}
