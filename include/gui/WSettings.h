/*
 * WSettings.h
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

#ifndef FUZZYSTRINGMATCHING2_WSETTINGS_H
#define FUZZYSTRINGMATCHING2_WSETTINGS_H


#include <gui/QTUtils.h>
#include <QApplication>
#include <QCloseEvent>
#include <QtWidgets/QMessageBox>
#include "ConfigurationFile.h"
#include <QTableView>
#include <QStandardItemModel>

class WSettings : public QWidget {
    std::string itemFieldName = "Keep Low";
    QGridLayout *grid;

    QComboBox* input_file_format;
    QLineEdit* input_file;
    QCheckBox* is_input_compressed;
    QSpinBox* ith_graph;


    QComboBox* trace_file_format;
    QLineEdit* traces_file;
    QCheckBox* are_traces_compressed;
    QLineEdit *separator_if_any;

    QComboBox *estimator_type;
    QCheckBox *use_estimator;


    QCheckBox *add_traces_to_log, *use_tuning_factor, *use_path_lambda_factor;
    QSpinBox *max_length;
    QDoubleSpinBox *min_prob, *lambda, *tuning_factor;

    QLineEdit *varepsilon, *admissibleCharList;
    QSpinBox *seedError, *numberOfTraceAlterations;
    QLineEdit *noiseThreshold;

    QTableView* view;
    QStandardItemModel* viewModel;

    std::vector<QLineEdit*> fileStrategyMap;
            ConfigurationFile conf;

public:
    WSettings(const std::string& conf, QWidget *parent = nullptr);      // Reading the configuration file
    void closeEvent (QCloseEvent *event) override;                      // Window close: saving the configuration file
    ~WSettings();

};


#endif //FUZZYSTRINGMATCHING2_WSETTINGS_H
