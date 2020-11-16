//
// Created by giacomo on 16/11/20.
//

#ifndef FUZZYSTRINGMATCHING2_LANGUAGE_H
#define FUZZYSTRINGMATCHING2_LANGUAGE_H

#ifdef _MSC_VER
#define DEPRECATED __declspec(deprecated)
#elif defined(__GNUC__) | defined(__clang__)
#define DEPRECATED __attribute__((__deprecated__))
#else
#define DEPRECATED
#endif

#endif //FUZZYSTRINGMATCHING2_LANGUAGE_H
