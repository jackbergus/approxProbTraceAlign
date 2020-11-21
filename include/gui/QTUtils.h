/*
 * QTUtils.h
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
