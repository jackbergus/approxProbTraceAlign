//
// Created by giacomo on 12/11/20.
//

#include "gui/WSettings.h"

WSettings::~WSettings() {
    delete grid;
}

#include <gui/QTBuilder.h>
#include <QtGui>
#include <QItemDelegate>
#include <QtWidgets/QScrollArea>
#include "../../QTSpinBoxDelegate.h"
#include "../../QTDropdownDelegate.h"

QTSpinBoxDelegate _spinbox_delegate;
QTDropdownDelegate _dropdown_delegate;

#if 0
QStandardItemModel* createModel(QTableView* parent, const std::vector<std::string>& header = {})
{
    const int numRows = 10;
    std::string itemFieldName = "Keep Low";

    QStandardItemModel* model = new QStandardItemModel();
    for (size_t i = 0, N = header.size(); i<N; i++) {
        model->setHorizontalHeaderItem(i, new QStandardItem(QString::fromStdString(header[i])));
    }
    parent->setItemDelegateForColumn(1, &_dropdown_delegate);
    parent->setItemDelegateForColumn(2, &_spinbox_delegate);
    for (int row = 0; row < numRows; ++row)
    {
        model->appendRow(generateCheckboxCell(itemFieldName));
    }
    //parent->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QComboBox * dropdown = getDropdownMenuFromEnum<LogOperations>();
    parent->setIndexWidget(model->index(0, 1), dropdown);

    /*for (int row = 0; row < numRows; ++row)
    {
        QString text = QString('A' + row) + QString::number(1);

        QStandardItem *item = generateCheckboxCell(itemFieldName);
        model->setItem(row, 0, item);

        QComboBox * dropdown = getDropdownMenuFromEnum<LogOperations>();
        parent->setIndexWidget(model->index(row, 1, QModelIndex()), new QPushButton());

    }*/

    return model;
}
#endif



WSettings::WSettings(const std::string& conf, QWidget *parent) : QWidget(parent), conf{conf} {

    grid = new QGridLayout();


    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Graph File"));
        size_t rowid = 0;
        input_file_format = addDropdownMenuFromEnum<FileFormat>(echoGrid, rowid, "Format:", this->conf.input_file_format);
        input_file = addTextField(echoGrid, rowid, "Filename:", this->conf.input_file, 0);
        is_input_compressed = addCheckBox(echoGrid, rowid, "Decompress graph", this->conf.is_input_compressed);
        ith_graph = addNumericBox(echoGrid, rowid, "Get i-th graph from file (counting from 0):", this->conf.ith_graph);
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 0, 0);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Traces File"));
        size_t rowid = 0;
        trace_file_format = addDropdownMenuFromEnum<TracesFormat>(echoGrid, rowid, "Format:", this->conf.trace_file_format);
        traces_file = addTextField(echoGrid, rowid, "Filename:", this->conf.traces_file, 0);
        are_traces_compressed = addCheckBox(echoGrid, rowid, "Decompress traces", this->conf.are_traces_compressed);
        separator_if_any = addTextField(echoGrid, rowid, "Action separator (if required):", {this->conf.separator_if_any}, 1);
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 1, 0);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Weight estimator for Petri Nets"));
        size_t rowid = 0;
        estimator_type = addDropdownMenuFromEnum<spd_we::WeightEstimatorCases>(echoGrid, rowid, "Estimator:", this->conf.estimator_type);
        use_estimator = addCheckBox(echoGrid, rowid, "Use estimator", this->conf.use_estimator);
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 2, 0);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Traces to add to the log set (from the Thompson automaton)"));
        size_t rowid = 0;
        add_traces_to_log = addCheckBox(echoGrid, rowid, "Add traces directly from the Petri-Net", this->conf.add_traces_to_log);
        max_length = addNumericBox(echoGrid, rowid, "Maximum complete trace length", this->conf.max_length);
        min_prob = addNumericBox(echoGrid, rowid, "Minimum trace probabilty:", this->conf.min_prob, 0.0, 1.0);
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 3, 0);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Traces to add to the log set (from the Thompson automaton)"));
        size_t rowid = 0;
        varepsilon = addTextField(echoGrid, rowid, "Varepsilon character", {this->conf.varepsilon}, 1);
        admissibleCharList = addTextField(echoGrid, rowid, "chars to biject the labels from the petri nets", this->conf.admissibleCharList, 0);
        seedError = addNumericBox(echoGrid, rowid, "Seed for error generator", this->conf.seedError);
        noiseThreshold = addNumericBox(echoGrid, rowid, "Maximum noise threshold", this->conf.noiseThreshold, 0.0, 1.0);
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 0, 1);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Traces to add to the log set (from the Thompson automaton)"));
        size_t rowid = 0;

        view = new QTableView();
        viewModel = new QStandardItemModel();
        viewModel->setHorizontalHeaderItem(0, new QStandardItem(QString::fromStdString("keep_low_up_otherwise")));
        viewModel->setHorizontalHeaderItem(1, new QStandardItem(QString::fromStdString("operation")));
        viewModel->setHorizontalHeaderItem(2, new QStandardItem(QString::fromStdString("factor")));
        view->setItemDelegateForColumn(1, &_dropdown_delegate);
        view->setItemDelegateForColumn(2, &_spinbox_delegate);
        if (!this->conf.operations.empty()) {
            size_t i = 0;
            for (const auto& entry : this->conf.operations) {
                QList<QStandardItem *> list;
                list.append(generateCheckboxCell(itemFieldName, entry.keep_low_up_otherwise));
                list.append(new QStandardItem(QString::fromStdString(magic_enum::enum_name(entry.operation).data())));
                list.append(new QStandardItem(QString::number(entry.factor)));
                viewModel->insertRow(i++, list);
            }

        }
        view->setModel(viewModel);
        echoGrid->addWidget(view, rowid++, 0, 1, 4);

        QPushButton* delSelect = new QPushButton();
        delSelect->setText(tr("Remove Selected Row"));
        connect(delSelect, &QPushButton::clicked, this, [=](){
            int r = view->selectionModel()->currentIndex().row();
            if (r>=0) {
                view->model()->removeRow(r);
            }
        });
        echoGrid->addWidget(delSelect, rowid, 0);

        // Move a row up
        {
            QPushButton* pushUp = new QPushButton();

            pushUp->setText(tr("Push Up"));
            connect(pushUp, &QPushButton::clicked, this, [=](){
                int r = view->selectionModel()->currentIndex().row();
                if (r>0) {                                                  // If there is a selected row which is not the first one
                    QList<QStandardItem *> list = viewModel->takeRow(r);    // Picking the current row
                    viewModel->insertRow(r-1,list);                         // Place where it has to be moved
                }
            });
            echoGrid->addWidget(pushUp, rowid, 1);
        }

        // Move a row down
        {
            QPushButton* pushDown = new QPushButton();
            pushDown->setText(tr("Push Down"));
            connect(pushDown, &QPushButton::clicked, this, [=](){
                int r = view->selectionModel()->currentIndex().row();
                if ((r>=0) && (r < viewModel->rowCount()-1)) {              // If there was a selection and if such row is not the last one
                    QList<QStandardItem *> list = viewModel->takeRow(r);    // Picking the current row
                    viewModel->insertRow(r+1,list);                         // Place it where it has to be moved, the next row
                }
            });
            echoGrid->addWidget(pushDown, rowid, 2);
        }

        // Insert a row
        {
            QPushButton* pushDown = new QPushButton();
            pushDown->setText(tr("Insert Row"));
            connect(pushDown, &QPushButton::clicked, this, [=](){
                int r = view->selectionModel()->currentIndex().row();
                if ((r < 0) || viewModel->rowCount() == 0) r = 0;
                QList<QStandardItem *> list;
                list.append(generateCheckboxCell(itemFieldName, true));
                list.append(new QStandardItem(QString::fromStdString(magic_enum::enum_name(LogOperations::LogSample).data())));
                list.append(new QStandardItem(QString::number(0.5)));
                viewModel->insertRow(r,list);                         // Place it where it has to be moved, the next row
            });
            echoGrid->addWidget(pushDown, rowid, 3);
        }


        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 0, 2);
    }

    {
        QGridLayout * echoGrid = new QGridLayout();
        QGroupBox *echoGroup = new QGroupBox(tr("Metrics for the different experiment settings"));
        size_t rowid = 0;

        constexpr auto& color_entries = magic_enum::enum_entries<UnterstuetzenStrategie>();
        constexpr std::size_t color_count = magic_enum::enum_count<UnterstuetzenStrategie>();
        for (size_t i = 0; i<color_count; i++) {
            std::string label = this->conf.fileStrategyMap[color_entries[i].first];
            fileStrategyMap.emplace_back(addTextField(echoGrid, rowid, color_entries[i].second.data(), label, 0));
            rowid++;
        }
        echoGroup->setLayout(echoGrid);
        grid->addWidget(echoGroup, 1, 1, 1, 2);

    }

    setLayout(grid);
    setWindowTitle(tr("Configuration time!"));
}



void WSettings::closeEvent(QCloseEvent *event) {

    conf.input_file_format = TO_ENUM(input_file_format, FileFormat);
    conf.input_file = TO_STRING(input_file);
    conf.is_input_compressed = TO_BOOLEAN(is_input_compressed);
    conf.ith_graph = TO_INT(ith_graph);

    conf.trace_file_format = TO_ENUM(trace_file_format, TracesFormat);
    conf.traces_file = TO_STRING(traces_file);
    conf.are_traces_compressed = TO_BOOLEAN(are_traces_compressed);
    conf.separator_if_any = TO_STRING(separator_if_any)[0];

    conf.use_estimator = TO_BOOLEAN(use_estimator);
    conf.estimator_type = TO_ENUM(estimator_type, spd_we::WeightEstimatorCases);

    conf.add_traces_to_log = TO_BOOLEAN(add_traces_to_log);
    conf.max_length = TO_INT(max_length);
    conf.min_prob = TO_DBL(min_prob);

    conf.varepsilon = TO_STRING(varepsilon)[0];
    conf.admissibleCharList = TO_STRING(admissibleCharList);
    conf.noiseThreshold = TO_DBL(noiseThreshold);
    conf.seedError = TO_INT(seedError);


    conf.operations.clear();
    for (size_t i = 0, N = viewModel->rowCount(); i<N; i++) {
        conf.operations.emplace_back(TO_ENUM_ITEM(viewModel->item(i, 1), LogOperations),
                                     TO_DBL_ITEM(viewModel->item(i,2)),
                                     TO_BOOLEAN_ITEM(viewModel->item(i,0)));
    }

    constexpr auto& color_entries = magic_enum::enum_entries<UnterstuetzenStrategie>();
    constexpr std::size_t color_count = magic_enum::enum_count<UnterstuetzenStrategie>();
    for (size_t i = 0; i<color_count; i++) {
        this->conf.fileStrategyMap[color_entries[i].first] =
                TO_STRING(fileStrategyMap[i]);
    }

    conf.serialize();
    event->accept();
}
