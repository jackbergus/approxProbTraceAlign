//
// Created by giacomo on 30/12/19.
//

#ifndef HIERARCHY_TESTS_MULTITHREADWRAP_H
#define HIERARCHY_TESTS_MULTITHREADWRAP_H

#include <multithreaded/thread_pool.h>

/**
 * This macro sets up whether the implementation should be multithreaded or not,
 */
#define IS_MULTITHREADED        (true)

template<class T>
class MultithreadWrap {
    std::vector<std::future<T>> futures;
    std::vector<T> computation;
    thread_pool pool;
    unsigned int nthreads;
    bool isMultiThreaded;
    bool hasRun;

public:
    MultithreadWrap(unsigned int nthreads, bool isMultiThreaded) : nthreads(nthreads), pool{nthreads},
                                                                   isMultiThreaded(isMultiThreaded) {
        hasRun = false;
    }

    template <typename F, typename ...Args>
    void poolExecute(F function, Args &&...args) {
        if (isMultiThreaded) {
            futures.push_back(pool.execute(function, args...));
        } else {
            computation.push_back(function(args...));
        }
    }

    std::vector<T> foreach() {
        run();
        return computation;
    }

private:

    void run() {
        if (!hasRun) {
            if (isMultiThreaded) {
                for (auto& x : futures)
                    computation.push_back(x.get());
                futures.clear();
                hasRun = true;
            } else {
                hasRun = true;
            }
        }
    }

};

#endif //HIERARCHY_TESTS_MULTITHREADWRAP_H
