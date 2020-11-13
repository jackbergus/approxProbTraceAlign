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


    QCheckBox *add_traces_to_log;
    QSpinBox *max_length;
    QDoubleSpinBox *min_prob;

    QLineEdit *varepsilon, *admissibleCharList;

    QTableView* view;
    QStandardItemModel* viewModel;

            ConfigurationFile conf;

public:
    WSettings(const std::string& conf, QWidget *parent = nullptr);


    void closeEvent (QCloseEvent *event) override;



    ~WSettings();

public slots:
    void clickedSlot()
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("MessageBox Title");
        msgBox.setText("You Clicked "+ ((QPushButton*)sender())->text());
        msgBox.exec();
    }
};


#endif //FUZZYSTRINGMATCHING2_WSETTINGS_H