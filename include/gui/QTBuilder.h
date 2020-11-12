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
