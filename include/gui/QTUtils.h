//
// Created by giacomo on 12/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_QTUTILS_H
#define FUZZYSTRINGMATCHING2_QTUTILS_H

#include <QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QSpinBox>
#include <QtGui/QStandardItem>

#define TO_ENUM(x,t)      (magic_enum::enum_cast<t>((x)->currentText().toStdString()).value())
#define TO_ENUM_ITEM(x,t) (magic_enum::enum_cast<t>((x)->text().toStdString()).value())
#define TO_STRING(x)    ((x)->text().toStdString())
#define TO_BOOLEAN(x)   ((x)->checkState() == Qt::Checked)
#define TO_INT(x)       ((int)(x)->value())
#define TO_DBL(x)        ((double)(x)->value())
#define TO_DBL_ITEM(x)   ((double)(x)->text().toDouble())
#define TO_BOOLEAN_ITEM(x)  ((x)->checkState() == Qt::Checked)

#endif //FUZZYSTRINGMATCHING2_QTUTILS_H
