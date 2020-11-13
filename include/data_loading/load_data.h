//
// Created by giacomo on 13/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LOAD_DATA_H
#define FUZZYSTRINGMATCHING2_LOAD_DATA_H

#include <data_loading/load_xes.h>
#include <data_loading/load_pnml.h>
#include <data_loading/load_bpmn.h>
#include <data_loading/load_petri_matrix.h>
#include <data_loading/load_matrix.h>
#include <data_loading/load_pregex.h>

enum FileFormat {
    Petri_PNML,
    Petri_BPMN,
    PetriMatrix,
    StochasticMatrix,
    ProbRegex
};

enum TracesFormat {
    XESLog,
    RawLog,
    NoLog
};

bool isFileFormatPetri(enum FileFormat format);

#endif //FUZZYSTRINGMATCHING2_LOAD_DATA_H
