/*
 * QTBuilder.h
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

#ifndef FUZZYSTRINGMATCHING2_QTBUILDER_H
#define FUZZYSTRINGMATCHING2_QTBUILDER_H

#include <gui/QTUtils.h>
#include <magic_enum.hpp>

template <typename enumType> QComboBox *getDropdownMenuFromEnum()  {
    QComboBox *echoComboBox = new QComboBox();
    constexpr std::size_t color_count = magic_enum::enum_count<enumType>();
    constexpr auto& color_names = magic_enum::enum_names<enumType>();
    for (size_t i = 0; i<color_count; i++) {
        echoComboBox->addItem(QString::fromStdString(color_names[i].data()));
    }
    return echoComboBox;
}

template <typename enumType>
QComboBox *addDropdownMenuFromEnum(QGridLayout *echoGrid, size_t &rowid, const std::string &format, enumType defaultValue) {
    QLabel *echoLabel = new QLabel(QString::fromStdString(format));
    QComboBox *echoComboBox = getDropdownMenuFromEnum<enumType>();
    int index = echoComboBox->findText(magic_enum::enum_name(defaultValue).data());
    assert ( index != -1 ); // -1 for not found
    echoComboBox->setCurrentIndex(index);
    echoGrid->addWidget(echoLabel, rowid, 0);
    echoGrid->addWidget(echoComboBox, rowid, 1);
    rowid++;
    return echoComboBox;
}

QLineEdit *addTextField(QGridLayout *echoGrid, size_t &rowid, const std::string &format, const std::string &defaultText,
                        int maxLength = -1);
QCheckBox* addCheckBox(QGridLayout *echoGrid, size_t& rowid, const std::string &format, bool checked);
QSpinBox* addNumericBox(QGridLayout *echoGrid, size_t& rowid, const std::string &format, size_t defaultValue);
QDoubleSpinBox* addNumericBox(QGridLayout *echoGrid, size_t& rowid, const std::string &format, double defaultValue, double min, double max);

QStandardItem *generateCheckboxCell(const std::string &itemFieldName, bool isChecked);

#endif //FUZZYSTRINGMATCHING2_QTBUILDER_H
