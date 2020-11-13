//
// Created by giacomo on 13/11/20.
//
#include_next <data_loading/load_data.h>

bool isFileFormatPetri(enum FileFormat format) {
    switch (format) {

        case Petri_PNML:
        case Petri_BPMN:
        case PetriMatrix:
            return true;

        case StochasticMatrix:
        case ProbRegex:
        default:
            return false;
    }
}
